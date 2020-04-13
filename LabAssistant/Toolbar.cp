#include "norm.h"#include "Toolbar.h"#include "Computer.h"#include "User.h"#include "Norm_Dialogs.h"#include "Employee.h"extern computerRecord	gComputerRecord[100];extern short			gNumberofComputers;extern consultant		gTheConsultant;extern Boolean			gConsultantFlag, gSupervisorFlag;extern consultant		gTheSupervisor, gActiveEmployee;extern toolBarIcon		gToolBarButtons[4];/*******************************************************************		void DrawToolBar(Boolean Supervisor)--------------------------------------------------------------------SYNOPSIS: Check if mouse was clicked on button in toolbar.CALLED BY: PASS: Point mouseLocation          : point of click      short *buttonPressed       RETURN: Boolean                 : button clicked on?REVISION HISTORY:	Name				Date		Description	----				----		-----------	Jeremy C Norberg	9/01/93		Initial version*********************************************************************/void DrawToolBar(Boolean Supervisor){	short 			counter;		for(counter = 0; counter < 4; counter ++)	{		if((gToolBarButtons[counter].supervisorButton && gSupervisorFlag) || !gToolBarButtons[counter].supervisorButton)			PlotCIconHandle(&gToolBarButtons[counter].location, atNone, ttNone, gToolBarButtons[counter].icon);		else			EraseRect(&gToolBarButtons[counter].location);			}}/*******************************************************************		Boolean CheckForToolbar(Point mouseLocation, short *buttonPressed)--------------------------------------------------------------------SYNOPSIS: Check if mouse was clicked on button in toolbar.CALLED BY: PASS: Point mouseLocation          : point of click      short *buttonPressed       RETURN: Boolean                 : button clicked on?REVISION HISTORY:	Name				Date		Description	----				----		-----------	Jeremy C Norberg	9/01/93		Initial version*********************************************************************/Boolean CheckForToolbar(Point mouseLocation, short *buttonPressed){	short counter;		for(counter = 0; counter < 4; counter ++)	{		if(PtInRect(mouseLocation, &gToolBarButtons[counter].location))		{							if(gToolBarButtons[counter].supervisorButton && !gSupervisorFlag);			else			{				*buttonPressed = gToolBarButtons[counter].buttonID;				return true;			}		}	}		return false;}/*******************************************************************		Boolean ToolBarButtonPressed(short buttonPressed, short draggedComputer, Boolean autoPick)--------------------------------------------------------------------SYNOPSIS: Check if mouse was clicked on button in toolbar.CALLED BY: PASS: Point mouseLocation          : point of click      short *buttonPressed       RETURN: Boolean                 : button clicked on?REVISION HISTORY:	Name				Date		Description	----				----		-----------	Jeremy C Norberg	9/01/93		Initial version*********************************************************************/Boolean ToolBarButtonPressed(short buttonPressed, short draggedComputer, Boolean autoPick){	Point mousePoint;	Boolean onButton, selected = false;	short	counter, theButton;	DrawToolBar(false);	theButton = buttonPressed - kCheckInButton;		if(autoPick)	{		GetMouse(&mousePoint);				PlotCIconHandle(&gToolBarButtons[theButton].location, atNone, ttSelected, gToolBarButtons[theButton].icon);					while(StillDown())		{			GetMouse(&mousePoint);						onButton = PtInRect(mousePoint, &gToolBarButtons[theButton].location);						if(!onButton && selected)			{				selected = false;				PlotCIconHandle(&gToolBarButtons[theButton].location, atNone, ttNone, gToolBarButtons[theButton].icon);				}			else if(onButton && !selected)			{				selected = true;				PlotCIconHandle(&gToolBarButtons[theButton].location, atNone, ttSelected, gToolBarButtons[theButton].icon);				}		}					PlotCIconHandle(&gToolBarButtons[theButton].location, atNone, ttNone, gToolBarButtons[theButton].icon);						onButton = PtInRect(mousePoint, &gToolBarButtons[theButton].location);	}	else	{		onButton = true;	}		if(onButton)		{			if(!CheckforEmployeeLogin("perform that operation. Would you like to log in as current consultant."))			return onButton;				switch(buttonPressed)		{			case kCheckInButton:				CheckInUser(draggedComputer, autoPick);				break;			case kCheckOutButton:				DoCheckOut(draggedComputer, autoPick);				break;			case kMarkDownButton:				MarkMachineDown(draggedComputer, !autoPick);				break;			case kMarkUpButton:				MarkMachineUp(draggedComputer, !autoPick);				break;		}	}			return onButton;}/*******************************************************************		void ToolBarDraw(grayRgnRefCon *theRefCon, short buttonPressed)--------------------------------------------------------------------SYNOPSIS: Check if mouse was clicked on button in toolbar.CALLED BY: PASS: Point mouseLocation          : point of click      short *buttonPressed       RETURN: Boolean                 : button clicked on?REVISION HISTORY:	Name				Date		Description	----				----		-----------	Jeremy C Norberg	9/01/93		Initial version*********************************************************************/void ToolBarDraw(grayRgnRefCon *theRefCon, short buttonPressed){	short 			counter = 0;		if((theRefCon->isButtonHighlited == true) && (buttonPressed == theRefCon->highlitedButton))	{			return;	}		theRefCon->isButtonHighlited = true;		DrawToolBar(false);		for(counter = 0; counter < 4; counter ++)	{		if(gToolBarButtons[counter].buttonID == buttonPressed)		{			PlotCIconHandle(&gToolBarButtons[counter].location, atNone, ttSelected, gToolBarButtons[counter].icon);			theRefCon->highlitedButton = buttonPressed;			break;		}	}}