#include "Lab Assistant.h"#include "database.h"#include "user.h"#include "error.h"#include "machine.h"#include "interface.h"#include "teachingOnOff.h"#include "Menus.h"#include "computerIcons.h"#define kRm332FirstComputer		202L#define kRm332LastComputer		226Lvoid TeachingOnOff(void){	computerRecPtr		theComputer;	long 				machineArrayNumber;	long				lastMachineArrayNumber;	MenuHandle 			theMenu;	extern char			gFeatureFlag;		LockMachineRecordHdl();	theMenu = GetMenu(mMachine);		SetItem(theMenu, iTeaching, "\pTurn Teaching On");		FindMachineNumber(kRm332FirstComputer, &machineArrayNumber);	FindMachineNumber(kRm332LastComputer, &lastMachineArrayNumber);		for(;machineArrayNumber <= lastMachineArrayNumber; machineArrayNumber++)	{		theComputer = GetMachineRecord(machineArrayNumber);				if ( BitTst(&theComputer->status, kTeachingSet) )		{			BitClr(&theComputer->status, kTeachingSet);			SetItem(theMenu, iTeaching, "\pTurn Teaching On");		}		else if ( !BitTst(&theComputer->status, kTeachingSet) )		{			BitSet(&theComputer->status, kTeachingSet);			SetItem(theMenu, iTeaching, "\pTurn Teaching Off");		}				DrawMachineIcon(theComputer);		}	if(BitTst(&gFeatureFlag, kTeachingFlag))		BitClr(&gFeatureFlag, kTeachingFlag);	else		BitSet(&gFeatureFlag, kTeachingFlag);		UpdateMachines(theComputer);	UnLockMachineRecordHdl();	DisposePtr((Ptr) theComputer);}