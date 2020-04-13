			/*******************************************************************		void IconInit(void)********************************************************************SYNOPSIS: This initilizes the global icon Rect locations from the 		  resource 'NORM'.CALLED BY: main()PASS: nothing	RETURN: nothingDESTROYED: nothingREVISION HISTORY:	Name				Date		Description	----				----		-----------	Jeremy C Norberg	9/01/93		Initial version*********************************************************************/void IconInit(void){	normRsrcRecord	**records;	Handle			buffer;	short			counter;		gComputerIconHandle = GetCIcon(kComputerIcon);	gComputerDownIconHandle = GetCIcon(kComputerDownIcon);	gUserIconHandle	= GetCIcon(kUserIcon);		gComputerSelected = false;		for(counter = 0; counter < 4; counter++)	{			gToolBarButtons[counter].icon = GetCIcon(kCheckInButton + counter);		gToolBarButtons[counter].buttonID = kCheckInButton + counter;		gToolBarButtons[counter].supervisorButton = false;	}		gToolBarButtons[3].supervisorButton = true;		for(counter = 0; counter < 2; counter++)	{		SetRect(&gToolBarButtons[counter].location, 283, 22 + counter * 74, 347, 86 + counter * 74);	}		for(counter = 2; counter < 4; counter++)	{		SetRect(&gToolBarButtons[counter].location, 283, 24 + counter * 74, 347, 88 + counter * 74);	}			SetRect(&gCheckInRect, 279, 22, 343, 86);	SetRect(&gCheckOutRect, 279, 96, 343, 160);			gCheckInIconHandle = GetCIcon(kCheckInButton);               /* Get CIcons from resource */	gCheckOutIconHandle = GetCIcon(kCheckOutButton);		buffer = GetResource('NORM', 130);						   /* Get icon Rects from resource 'NORM'*/	HLock(buffer);		gNumberofComputers = *(short *) *buffer;                   // set the number of computers		records = (normRsrcRecord **) NewHandle(sizeof(normRsrcRecord) * gNumberofComputers);		HLock((Handle) records);		memcpy((*records), (*buffer) + sizeof(short), sizeof(normRsrcRecord) * gNumberofComputers);		   /* Copy resource to tempRecord */		for(counter = 0; counter < gNumberofComputers; counter++)	{		gComputerRecord[counter].number = (*records)[counter].number;		gComputerRecord[counter].computerRect = (*records)[counter].computerRect;				BitSet(&gComputerRecord[counter].computerInfo.status, kComputerWorking);  /* Set computer status to working */		BitClr(&gComputerRecord[counter].computerInfo.status, kComputerInUse);    /* Set computer status to not Used */		BitClr(&gComputerRecord[counter].computerInfo.status, kTeachingSet);      /* Set computer status to teaching off */		BitClr(&gComputerRecord[counter].computerInfo.status, kIconSelected);  /* Set computer status to working */			}		HUnlock((Handle) buffer);	HUnlock((Handle) records);	DisposHandle((Handle) records);	ReleaseResource((Handle) buffer);}/*******************************************************************		void TeachingOn_Off(void)--------------------------------------------------------------------SYNOPSIS: This togles teaching status on/offCALLED BY: PASS: nothingRETURN: nothingREVISION HISTORY:	Name				Date		Description	----				----		-----------	Jeremy C Norberg	9/01/93		Initial version*********************************************************************/void TeachingOn_Off(void){	short	counter, curMark, begComputer;	MenuHandle	theMenu;		theMenu = GetMenu(mSupervisor);		GetItemMark(theMenu, iClass, &curMark);		for(counter = 0; counter < gNumberofComputers; counter ++)	{		if(gComputerRecord[counter].number == 202)		{			begComputer = counter;			break;		}	}		if(curMark == noMark)	{		for(counter = begComputer; counter < gNumberofComputers; counter ++)		{			BitSet(&gComputerRecord[counter].computerInfo.status, kTeachingSet);			SelectIcon(counter, false);		}		CheckItem(theMenu,iClass, true);		BitSet(&gFeatureFlag, kTeachingSet);	}	else	{		for(counter = begComputer; counter < gNumberofComputers; counter ++)		{			BitClr(&gComputerRecord[counter].computerInfo.status, kTeachingSet);			SelectIcon(counter, false);		}		CheckItem(theMenu,iClass, false);		BitClr(&gFeatureFlag, kTeachingSet);	}		ComputerRecordSave();		DrawStatusBox();		MakeList();}/*******************************************************************		void DatabaseChecking(void)--------------------------------------------------------------------SYNOPSIS: This togles database checking status on/offCALLED BY: PASS: nothingRETURN: nothingREVISION HISTORY:	Name				Date		Description	----				----		-----------	Jeremy C Norberg	9/01/93		Initial version*********************************************************************/void DatabaseChecking(void){	short		counter, curMark;	MenuHandle	theMenu;		theMenu = GetMenu(mSupervisor);		GetItemMark(theMenu, iDatabaseChecking, &curMark);		if(curMark == noMark)	{		CheckItem(theMenu,iDatabaseChecking, true);		BitSet(&gFeatureFlag, kDatabaseChecking);	}	else	{		CheckItem(theMenu,iDatabaseChecking, false);		BitClr(&gFeatureFlag, kDatabaseChecking);	}		ComputerRecordSave();}