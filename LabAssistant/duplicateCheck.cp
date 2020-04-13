#include <string.h>#include <stdio.h>#include <stdlib.h>#include "lab assistant.h"#include "user.h"#include "machine.h"#include "database.h"#include "interface.h"#include "duplicateCheck.h"#include "mainMachineList.h"#include "error.h"#include "machineRecord.h"short CheckForDuplicates(userNode *theUser){	computerRecPtr	theComputer;	short 			number = 0;	short			numberofComputers;		numberofComputers = GetNumberofMachines();		for(short counter = 0; counter < numberofComputers; counter++)	{		LockMachineRecordHdl();		theComputer = GetMachineRecord(counter);				if(BitTst(&theComputer->status, kComputerInUse))		{				if (strncmp(theComputer->userInfo.name, theUser->name, 29) == 0)			{					number++;			}			}				UnLockMachineRecordHdl();	}		return number;}short CheckForRestrictions(userNode *theUser){		if (!BitTst(&theUser->status, RestrictedUserBit))		return kNoRestricition;	else if (BitTst(&theUser->status, LevelOfRestrictionBit))		return kFullRestriction;	else return kPartialRestriction;}