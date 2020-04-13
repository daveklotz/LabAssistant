#include "lab assistant.h"#include "createDatabase.h"#include "Norm_Files.h"#include "dialog.h"void CreateDatabaseFiles(void){	dialogRecordHdl		dialogRecord;	StandardFileReply	*dbEngrFileReply, *dbOtherFileReply;		dbEngrFileReply = (StandardFileReply *) NewPtr(sizeof(StandardFileReply));	dbOtherFileReply = (StandardFileReply *) NewPtr(sizeof(StandardFileReply));		dialogRecord = CreateDialogRec(DatabaseSetupCallBack, kEventCheckIn, (long) dbEngrFileReply, nil, (long) dbOtherFileReply);	DatabaseSetupBoxSetup(dialogRecord);}void DatabaseSetupBoxSetup(dialogRecordHdl dialogRecord){	GrafPtr			oldPort;	DialogPtr		theDialog;	winRefConRecHdl	refCon;	short			itemType;	Handle			itemHandle;	Rect			itemRect;		GetPort(&oldPort);		theDialog = GetNewDialog(kDatabaseSetupBoxID, nil, (WindowPtr)-1L);		SetPort(theDialog);	ShowWindow(theDialog);		refCon = (winRefConRecHdl) NewHandle(sizeof(winRefConRec));        // Create Ptr to refCon record		HLock((Handle) refCon);	(*refCon)->windowID = kDatabaseSetupBoxID;                          // Assign the WindowID	(*refCon)->oldPort = (long) oldPort;                         	    // Assign the misc field to the oldPort. This required when closeing dialog box	(*refCon)->dialogRecord = dialogRecord;	HUnlock((Handle) refCon);		SetWRefCon(theDialog, (long) refCon);	GetDItem(theDialog, 11, &itemType, &itemHandle, &itemRect);	SetDItem(theDialog, 11, itemType, (Handle) DrawOKOutline, &itemRect);		GetDItem(theDialog, ok, &itemType, &itemHandle, &itemRect);	HiliteControl((ControlHandle) itemHandle, 255);}void DatabaseSetupBoxEvent(EventRecord *theEvent){	WindowPtr			theDialog, oldPort;	winRefConRecHdl		refCon;	Handle				idNumberHandle, okButtonHandle;	short				itemHit = 0;	short				itemType;	Handle				itemHandle;	Rect				itemRect;	Boolean				done = false;	dialogProcPtr		callBack;	dialogRecordHdl		dialogRecord;	Boolean				dialogDone = false, engrPicked = false, otherPicked = false;	StandardFileReply	*dbEngrFileReply, *dbOtherFileReply;	SFTypeList			dbTypeList;	Str255				itemText;	char				theKey;		theDialog = FrontWindow();		dbTypeList[0] = 'TEXT';		refCon = (winRefConRecHdl) GetWRefCon(theDialog);        // Get the Window Ref Record from the Windows Ref Con	HLock((Handle) refCon);	oldPort = (WindowPtr) (*refCon)->oldPort;	dialogRecord = (dialogRecordHdl) (*refCon)->dialogRecord;	HUnlock((Handle) refCon);		HLock((Handle) dialogRecord);	callBack = (*dialogRecord)->callBack;	dbEngrFileReply = (StandardFileReply *) (*dialogRecord)->theRecord;	dbOtherFileReply = (StandardFileReply *) (*dialogRecord)->misc;	HUnlock((Handle) dialogRecord);			GetDItem(theDialog, 4, &itemType, &idNumberHandle, &itemRect);	GetDItem(theDialog, ok, &itemType, &okButtonHandle, &itemRect);			switch(theEvent->what)          							 //Handle Keys before passing off to Dialog Select	{		case keyDown:		case autoKey:			theKey = theEvent->message & charCodeMask;       			 // Mask out the char from the event message						switch (theKey)			{				case '\r':			// Return				case '\003':		// Enter					itemHit = ok;					done = true;					break;				case '\033':		// ESC					itemHit = cancel;					done = true;					break;			}	}		if(!done)	{			if(!DialogSelect(theEvent, (DialogPtr *) &theDialog, &itemHit))			return;                // Nothing to Handle, just return.	}		GetDItem(theDialog, 6, &itemType, &itemHandle, &itemRect);	GetIText(itemHandle, itemText);	if(EqualString(itemText, dbOtherFileReply->sfFile.name, false, false))	{			HiliteControl((ControlHandle) okButtonHandle, 0);		otherPicked = true;	}	GetDItem(theDialog, 5, &itemType, &itemHandle, &itemRect);	GetIText(itemHandle, itemText);	if(EqualString(itemText, dbEngrFileReply->sfFile.name, false, false))	{			HiliteControl((ControlHandle) okButtonHandle, 0);		engrPicked = true;	}		switch (itemHit)	{		case 4:			StandardGetFile(nil, 1, dbTypeList, dbEngrFileReply);			if(dbEngrFileReply->sfGood)			{				GetDItem(theDialog, 5, &itemType, &itemHandle, &itemRect);				SetIText(itemHandle, dbEngrFileReply->sfFile.name);				(*dialogRecord)->theRecord = (long) dbEngrFileReply;								GetDItem(theDialog, 6, &itemType, &itemHandle, &itemRect);				GetIText(itemHandle, itemText);				if(EqualString(itemText, dbOtherFileReply->sfFile.name, false, false))					HiliteControl((ControlHandle) okButtonHandle, 0);			}			break;		case 3:			StandardGetFile(nil, 1, dbTypeList, dbOtherFileReply);			if(dbOtherFileReply->sfGood)			{				GetDItem(theDialog, 6, &itemType, &itemHandle, &itemRect);				SetIText(itemHandle, dbOtherFileReply->sfFile.name);				(*dialogRecord)->misc = (long) dbOtherFileReply;								GetDItem(theDialog, 5, &itemType, &itemHandle, &itemRect);				GetIText(itemHandle, itemText);				if(EqualString(itemText, dbEngrFileReply->sfFile.name, false, false))					HiliteControl((ControlHandle) okButtonHandle, 0);			}			break;		case ok:			if(!otherPicked || !engrPicked)				break;			case cancel:			DisposeDialog(theDialog);			DisposeHandle((Handle) refCon);			SetPort(oldPort);						HLock((Handle) dialogRecord);			(*dialogRecord)->itemHit = itemHit;			HUnlock((Handle) dialogRecord);						callBack(dialogRecord);						break;	}}void DatabaseSetupCallBack(dialogRecordHdl dialogRecord){	StandardFileReply	*dbEngrFileReply, *dbOtherFileReply;	short 				itemHit;		HLock((Handle) dialogRecord);	itemHit = (*dialogRecord)->itemHit;		if(itemHit == ok)	{		dbEngrFileReply = (StandardFileReply *) (*dialogRecord)->misc;		dbOtherFileReply = (StandardFileReply *) (*dialogRecord)->theRecord;				Merge(dbEngrFileReply, dbOtherFileReply);	}		DisposePtr((Ptr) (*dialogRecord)->misc);	DisposePtr((Ptr) (*dialogRecord)->theRecord);	HUnlock((Handle) dialogRecord);	DisposeHandle((Handle) dialogRecord);}