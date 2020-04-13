#include "lab assistant.h"#include "duplicateCheck.h"#include "database.h"#include "user.h"#include "checkIn.h"#include "error.h"#include <string.h>#include "interface.h"#include "dialog.h"#include "checkStatus.h"#include "menu.h"#include "employeeRecord.h"#include "otherUser.h"#include "bumpUser.h"#include "waitList.h"#include "loginEmployee.h"#include "checkInOther.h"#include "loginTemp.h"void HandleCheckIn(short computerNumber){	dialogRecordHdl		dialogRecord;	computerRecPtr		theComputer;	extern char			gFeatureFlag;		if ( ( !BitTst(&gFeatureFlag, 3L) ) && ( !BitTst(&gFeatureFlag, 4L) ) )	{		NewAlert("\pAn Employee Must Be Logged In First!", false, ok, 2);		LoginEmployee();		return;	}	LockMachineRecordHdl();	theComputer = GetMachineRecord(computerNumber);		if(computerNumber != -99)	{		if(!BitTst(&theComputer->status, kComputerWorking))		{			 DoError("\pThe computer is marked down.");			 return;		}		else if(BitTst(&theComputer->status, kTeachingSet))		{			DoError("\pThe computer is being used for a class.");			return;		}		else if(BitTst(&theComputer->status, kComputerInUse))		{			DoError("\pThe computer is being used.");			return;		}	}		UnLockMachineRecordHdl();		dialogRecord = CreateDialogRec(CheckInCallBack, kEventCheckIn, nil, nil,computerNumber);		DisableMenus(true);			UserEnterBoxSetup(dialogRecord);}void CheckInCallBack(dialogRecordHdl dialogRecord){	long 			machineArrayNumber;	computerRecPtr	theComputer;	short 			machineNumber, itemHit;	userNode		theUser;	short			numberofDuplicates;	unsigned long	theTime;	Boolean			truthFlag = 1;	Str255 			badIDString;	extern char		gFeatureFlag;		HLock((Handle) dialogRecord);	itemHit = (*dialogRecord)->itemHit;	machineArrayNumber = (*dialogRecord)->misc;		if(itemHit == cancel)	{		HUnlock((Handle) dialogRecord);		DisposeHandle((Handle) dialogRecord);		DisableMenus(false);		return;	}		/*	if (itemHit == 3)  		// 3 is the number for other user	{		CheckInOtherUser(dialogRecord);		CheckInOtherUserCallBack(dialogRecord);		truthFlag = 0;	}	*/	if (!BitTst(&gFeatureFlag, kDatabaseChecking))	{		LoginTemp(dialogRecord);		return;	}					if( (!Search(&(*dialogRecord)->theRecord, &theUser)) && truthFlag)	{		NumToString((*dialogRecord)->theRecord,badIDString);		HUnlock((Handle) dialogRecord);		PtoCString(badIDString);		strcat((char *)badIDString, " was not found.");		CtoPString((char *)badIDString);		//NewAlert("\pThe number you typed was not found.", false, ok, 2);		NewAlert(badIDString, false, ok, 2);		// Alert that is wrong number entered and to reenter number		DisposeHandle((Handle) dialogRecord);		DisableMenus(false);		HandleCheckIn();	}	else	{			theUser.name[29] = '\0';				numberofDuplicates = CheckForDuplicates(&theUser);		WaitList	theWaitingList;					if(numberofDuplicates > 1)			{				DoError("\pUser already has 2 machines checked out. The limit is 2");			HUnlock((Handle) dialogRecord);			DisposeHandle((Handle) dialogRecord);			DisableMenus(false);			return;		}		if( (numberofDuplicates == 1) && (theWaitingList.IsThereAWaitList()) &&				!FindEmptyMachine(&machineNumber) )		{			NewAlert("\pThe User Already Has A Machine And Cant Be Added To The Waiting List.", 							false, ok, 2);			HUnlock((Handle) dialogRecord);			DisposeHandle((Handle) dialogRecord);			DisableMenus(false);			return;		}		if(machineArrayNumber != -99)		{			SysBeep(0);		}		else if(!FindEmptyMachine(&machineNumber))		{			//NewAlert("\pNo Empty Machines.", false, ok, 2);			// Handle no empty machines	 		// WaitListBoxSetup(dialogRecord);	 		Bump(&theUser, dialogRecord);						HUnlock((Handle) dialogRecord);			DisposeHandle((Handle) dialogRecord);			DisableMenus(false);			return;		}		else			FindMachineNumber((long) machineNumber, &machineArrayNumber);				if(numberofDuplicates == 1)			if(NewAlert("\pThe user already has a machine checked out. Check out another?", true, cancel) == cancel) 			{				HUnlock((Handle) dialogRecord);				DisposeHandle((Handle) dialogRecord);				DisableMenus(false);				return;			}				if(BitTst(&theUser.status, LevelOfRestrictionBit) && BitTst(&theUser.status, RestrictedUserBit))		{			theComputer = (computerRecPtr) NewPtr(sizeof(computerRecord));			memcpy(&theComputer->userInfo, &theUser, sizeof(userNode));						GetDateTime(&theTime);			theComputer->timeOn = theTime;						(*dialogRecord)->misc = (long) theComputer;			(*dialogRecord)->theEvent = kEventCheckUserStatus;			(*dialogRecord)->callBack = CheckUserStatus;					HUnlock((Handle) dialogRecord);						UserInfoBoxSetup(dialogRecord);						DisableMenus(true);			return;		}				LockMachineRecordHdl();		theComputer = GetMachineRecord(machineArrayNumber);		memcpy(&theComputer->userInfo, &theUser, sizeof(userNode));				GetDateTime(&theTime);		theComputer->timeOn = theTime;		theComputer->employeeNumber = GetActiveEmployee();				(*dialogRecord)->theRecord = machineArrayNumber;		(*dialogRecord)->theEvent = kEventCheckIn;		(*dialogRecord)->callBack = CheckInUser;				HUnlock((Handle) dialogRecord);		UnLockMachineRecordHdl();				DisableMenus(true);							UserInfoBoxSetup(dialogRecord);	}}void CheckInUser(dialogRecordHdl dialogRecord){	computerRecPtr		theComputer;	long				machineArrayNumber;	short				itemHit;	unsigned long		theTime;	MenuHandle			theMenu;		HLock((Handle) dialogRecord);	itemHit = (*dialogRecord)->itemHit;	machineArrayNumber = (*dialogRecord)->theRecord;	HUnlock((Handle) dialogRecord);		if(itemHit == ok)	{			LockMachineRecordHdl();		theComputer = GetMachineRecord(machineArrayNumber);				GetDateTime(&theTime);		theComputer->timeOn = theTime;				theMenu = GetMenu(mEmployee);		theComputer->employeeNumber = GetActiveEmployee();				BitSet(&theComputer->status, kComputerInUse);				UpdateMachines(theComputer);		UnLockMachineRecordHdl();	}		WaitList		theWaitingList;	if ( (itemHit == cancel) && (theWaitingList.IsThereAWaitList()) )	{		theWaitingList.CheckInFromWaitList();	}		DisposeHandle((Handle) dialogRecord);		DisableMenus(false);}