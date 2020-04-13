#include "lab assistant.h"#include "database.h"#include "user.h"#include "machineUp.h"#include "machine.h"#include "error.h"#include "interface.h"#include "waitList.h"void HandleMachineUp(short computerNumber){	dialogRecordHdl		dialogRecord;	computerRecPtr		theComputer;	extern char			gFeatureFlag;		if(!BitTst(&gFeatureFlag, 3L))	{		NewAlert("\pA Supervisor Must Be Logged In First!", false, ok, 3);		return;	}		dialogRecord = (dialogRecordHdl) NewHandle(sizeof(dialogRec));		if(computerNumber != -99)	{			if(computerNumber != -99)	{			LockMachineRecordHdl();		theComputer = GetMachineRecord(computerNumber);				if(BitTst(&theComputer->status, kComputerWorking))		{			 DoError("\pThe computer is already marked working.");			 return;		}		computerNumber = (*theComputer).number;		UnLockMachineRecordHdl();	}	}		HLock((Handle) dialogRecord);	(*dialogRecord)->theEvent = kEventMachineUp;	(*dialogRecord)->callBack = MachineUpCallBack;	(*dialogRecord)->theRecord = computerNumber;	HUnlock((Handle) dialogRecord);		if(computerNumber == -99)		MachineEnterBoxSetup(dialogRecord);	else		MachineUpCallBack(dialogRecord);;}void MachineUpCallBack(dialogRecordHdl dialogRecord){	long 			machineArrayNumber;	computerRecPtr	theComputer;	short			itemHit;	Boolean			foundIt;	extern char		gFeatureFlag;		HLock((Handle) dialogRecord);	itemHit = (*dialogRecord)->itemHit;		if(itemHit == cancel)	{		HUnlock((Handle) dialogRecord);		DisposeHandle((Handle) dialogRecord);		return;	}		foundIt = FindMachineNumber((*dialogRecord)->theRecord, &machineArrayNumber);		if(!foundIt)	{		NewAlert("\pThe number you typed was not found.", false, ok, 2);		HUnlock((Handle) dialogRecord);		DisposeHandle((Handle) dialogRecord);		HandleMachineUp();	}	else	{		LockMachineRecordHdl();		theComputer = GetMachineRecord(machineArrayNumber);				if(BitTst(&theComputer->status, kComputerWorking))		{			NewAlert("\pMachine already working.", false, ok, 2);			// Handle already working		}		else		{			BitSet(&theComputer->status, kComputerWorking);						UpdateMachines(theComputer);	// record saving						if(BitTst(&gFeatureFlag, 5L))			{				WaitList		theWaitingList;						theWaitingList.CheckInFromWaitList();			}		}				HUnlock((Handle) dialogRecord);		UnLockMachineRecordHdl();		DisposeHandle((Handle) dialogRecord);	}	}