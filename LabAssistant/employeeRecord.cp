#include "lab assistant.h"#include "employeeRecord.h"#include "Norm_Files.h"#include <string.h>#include <stdio.h>#include "error.h"employeeNode *gEmployeeLog;void CreateEmployeeLog(void){	gEmployeeLog = CreateEmployeeList();}long GetActiveEmployee(void){	MenuHandle			theMenu;	short				counter, currentMark, menuCount;	Str255				itemString, name;	employeeNode		*theEmployee;		theMenu = GetMenu(mEmployee);		theEmployee = gEmployeeLog->next;		menuCount = CountMItems(theMenu);	for(counter = menuCount; counter > 3; counter --)	{		GetItemMark(theMenu, counter, &currentMark);		if(currentMark == checkMark)		{			GetItem(theMenu, counter, itemString);			while(theEmployee->next != theEmployee)			{				strcpy((char *) name, theEmployee->name);				CtoPstr((char *) name);								if(EqualString(name, itemString, false, false))					return theEmployee->studentNumber;				else					theEmployee = theEmployee->next;			}		}	}		return 0;}void MakeActiveEmployee(short menuItem){	MenuHandle			theMenu;	short				counter, currentMark, menuCount;		theMenu = GetMenu(mEmployee);	menuCount = CountMItems(theMenu);		for(counter = menuCount; counter > 3; counter --)	{		GetItemMark(theMenu, counter, &currentMark);		if(currentMark != noMark)		{			SetItemMark(theMenu, counter, noMark);		}	}		SetItemMark(theMenu, menuItem, checkMark);}employeeNode * CreateEmployeeList(void){	employeeNode *endNode, *employeeHead;		employeeHead = (employeeNode *) NewPtr(sizeof(employeeNode));		endNode = (employeeNode *) NewPtr(sizeof(employeeNode));		employeeHead->next = endNode;	endNode->next = endNode;		return employeeHead;}void DeleteEmployeeList(employeeNode *employeeHead){	employeeNode *nextEmployee;		nextEmployee = employeeHead->next;		while(employeeHead->next != employeeHead)	{				DisposePtr((Ptr) employeeHead);				employeeHead = nextEmployee;				nextEmployee = employeeHead->next;	}		DisposePtr((Ptr) employeeHead);}void CreateDefaultEmployee(employeeNode *employeeHead){	employeeNode	theEmployee;		sprintf(theEmployee.name, "Default Supervisor");	sprintf(theEmployee.password, "ynot");	theEmployee.supervisor = true;	theEmployee.studentNumber = 11111111;		AddEmployeeRecord(employeeHead, &theEmployee);		sprintf(theEmployee.name, "Temporary Employee");	sprintf(theEmployee.password, "");	theEmployee.supervisor = false;		theEmployee.studentNumber = 11111112;	AddEmployeeRecord(employeeHead, &theEmployee);}OSErr ReadEmployeeInfo(employeeNode *employeeHead){	short			fRefNum;	FSSpec 			fileSpec;	OSErr			theError;	long			inOutCount;	employeeNode	theEmployee;		theError = FSMakeFSSpec(0, 0, kEmployeeDBFileName, &fileSpec);		if(theError)		return theError;		theError = FSpOpenDF(&fileSpec, fsCurPerm, &fRefNum);		if(theError)		return theError;		while(theError != -39)	{			inOutCount = sizeof(employeeNode);				theError = FSRead(fRefNum, &inOutCount, (Ptr) &theEmployee);				if(inOutCount == sizeof(employeeNode))			AddEmployeeRecord(employeeHead, &theEmployee);	}		theError = FSClose(fRefNum);		return theError;}void AddEmployeeRecord(employeeNode *employeeHead, employeeNode *theEmployee){	employeeNode	*newEmployee;	employeeNode	*empPtr, *prevPtr;	if (!CheckForOtherEmployee(employeeHead, theEmployee))	{			newEmployee = (employeeNode *) NewPtr(sizeof(employeeNode));		empPtr = employeeHead->next;		prevPtr = employeeHead;				while (empPtr != empPtr->next)		{			if(strcmp(theEmployee->name, empPtr->name) < 0)				break;						empPtr = empPtr->next;			prevPtr = prevPtr->next;		}			memcpy(newEmployee, theEmployee, sizeof(employeeNode));		prevPtr->next = newEmployee;		newEmployee->next = empPtr;			//newEmployee->next = employeeHead->next;			//employeeHead->next = newEmployee;	}	else	{		NewAlert("\pThat Employee Already Exists.", false, ok, 2);	}}void ReplaceEmployeeRecord(employeeNode *employeeHead, employeeNode *theEmployee){	employeeNode	*replaceEmployee;		replaceEmployee = employeeHead->next;		while(replaceEmployee->next != theEmployee && replaceEmployee->next != replaceEmployee)	{		replaceEmployee = replaceEmployee->next;	}		theEmployee->next = replaceEmployee->next->next;	DisposePtr((Ptr) replaceEmployee->next);	replaceEmployee->next = theEmployee;	}void DeleteEmployeeRecord(employeeNode *employeeHead, employeeNode *theEmployee){	employeeNode	*deleteEmployee;		deleteEmployee = employeeHead;		while((deleteEmployee->next != theEmployee))	{		deleteEmployee = deleteEmployee->next;	}		deleteEmployee->next = theEmployee->next;		DisposePtr((Ptr) theEmployee);}void WriteEmployeeInfo(employeeNode *employeeHead){	short		fRefNum;	FSSpec 		fileSpec, tempFileSpec;	OSErr		theError;	long		inOutCount;	employeeNode	*theEmployee;	Str255		tempFileName;		CreateTempFileName(tempFileName);	theError = FSMakeFSSpec(0, 0, kEmployeeDBFileName, &fileSpec);	if(theError)	{		theError = FSpCreate(&fileSpec, kCreatorType, 'NMDB', (ScriptCode) -1);	}		theError = FSMakeFSSpec(0, 0, tempFileName, &tempFileSpec);		if(theError)	{		theError = FSpCreate(&tempFileSpec, kCreatorType, 'TEMP', (ScriptCode) -1);	}		theError = FSpOpenDF(&tempFileSpec, fsCurPerm, &fRefNum);	theEmployee = employeeHead->next;		while(theEmployee->next != theEmployee)	{		inOutCount = sizeof(employeeNode);				theError = FSWrite(fRefNum, &inOutCount, theEmployee);				theEmployee = theEmployee->next;	}		theError = FSpExchangeFiles(&fileSpec, &tempFileSpec);	theError = FSClose(fRefNum);		theError = FSpDelete(&tempFileSpec);}Boolean CheckForOtherEmployee(employeeNode *employeeHead, employeeNode *theEmp){	Boolean 		foundIt = false;	employeeNode	*searchEmp;	searchEmp = employeeHead->next;	while(searchEmp != searchEmp->next)	{		if( (theEmp->studentNumber == searchEmp->studentNumber) ||				(!strcmp(searchEmp->name, theEmp->name) ) )		{			foundIt = true;			break;		}		searchEmp = searchEmp->next;	}	return foundIt;}	