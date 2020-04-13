#include "roomStatus.h"#include "lab assistant.h"#include "database.h"#include "interface.h"#define kMeterSize    7void DrawRoomStatus(void){	short			room324 = 0, room332 = 0;	float			stat324, stat332;	long			roomNum;		Rect			startingPoint, boxRect;		Str255			theString;	Str255			room;		GetRoomStatistics(&room324, &room332);		stat324 =   (int)( ( (float)room324 / 50.0 ) * 100 );	stat332 =   (int)( ( (float)room332 / 25.0 ) * 100 );		room324 = 50 - room324;	room332 = 25 - room332;		SetRect(&boxRect, 13, 8, 245, 26);	Add3dRaised(&boxRect, true, true, true);		SetRect(&startingPoint, 13, 10, 26, 20);	roomNum = 324;		MoveTo(startingPoint.left + 4, 21);		NumToString(roomNum, room);	DrawString("\pRoom ");	DrawString(room);	DrawString("\p Status:");	DrawString("\p     Free: ");		NumToString(room324, theString);	DrawString(theString);		DrawString("\p       ");		DrawString("\pFilled: ");	NumToString(stat324, theString);	DrawString(theString);	DrawString("\p%");		SetRect(&boxRect, 383, 8, 615, 26);	Add3dRaised(&boxRect, true, true, true);		SetRect(&startingPoint, 383, 10, 396, 20);	roomNum = 332;		MoveTo(startingPoint.left + 4, 21);	NumToString(roomNum, room);	DrawString("\pRoom ");	DrawString(room);	DrawString("\p Status:");	DrawString("\p     Free: ");		NumToString(room332, theString);	DrawString(theString);		DrawString("\p       ");		DrawString("\pFilled: ");	NumToString(stat332, theString);	DrawString(theString);	DrawString("\p%");}short ComputerStatus(Byte computerStatus){	short returnValue;		if(BitTst(&computerStatus, kComputerWorking))	{		if(BitTst(&computerStatus, kComputerInUse))		{			returnValue = kComputerInUse;		}		else			returnValue = kComputerWorking;	}	else if(!BitTst(&computerStatus, kComputerWorking))	{		returnValue = kComputerNotWorking;	}		return returnValue;}void GetRoomStatistics(short *room324, short *room332){	long			counter, numberofMachines;	computerRecPtr	theComputer;	short			computerStatus;		numberofMachines = GetNumberofMachines();		LockMachineRecordHdl();	for(counter = 0; counter < 50; counter ++)	{		theComputer = GetMachineRecord(counter);		computerStatus = ComputerStatus(theComputer->status);				switch (computerStatus)		{			case kComputerNotWorking:			case kComputerInUse:				(*room324) ++;				break;		}	}		for(counter = 50; counter < numberofMachines; counter ++)	{		theComputer = GetMachineRecord(counter);		computerStatus = ComputerStatus(theComputer->status);		if(BitTst(&theComputer->status, kTeachingSet))			computerStatus = kTeachingSet;				switch (computerStatus)		{			case kComputerNotWorking:			case kComputerInUse:			case kTeachingSet:				(*room332) ++;				break;		}	}		UnLockMachineRecordHdl();}