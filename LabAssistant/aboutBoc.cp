#include <Movies.h>#include <GestaltEqu.h>//#include <StandardFile.h>#include "Lab Assistant.h"#include "aboutBoc.h"#include "error.h"#include "Files.h"void PlayAboutBox(void){	long						qtVers;	StandardFileReply			reply;	OSType						movieType = MovieFileType;	Rect						bounds;	WindowPtr					theWindow;	OSErr						theErr;	GrafPtr						oldPort;				MaxApplZone();		if (Gestalt(gestaltQuickTime, &qtVers) != noErr)	{		SysBeep(0);		NewAlert("\pQuickTime must be installed first!", false, ok, 2);		return;	}		GetPort(&oldPort);		EnterMovies();				//inits the movie toolbox				theErr = FSMakeFSSpec(nil, nil, "\pMiddle Management:Development:Lab Assistant �:About Box", &reply.sfFile);			//StandardGetFilePreview(nil, 1, &movieType, &reply);	//if(reply.sfGood == false) return;		SetRect(&bounds, 75, 75, 75+240, 75+200);	theWindow = NewCWindow(nil, &bounds, reply.sfFile.name, true, 0, (WindowPtr)-1, false, 0);		SetPort(theWindow);	playMovieInWindow(theWindow, &reply.sfFile, &theWindow->portRect);		DisposeWindow(theWindow);		SetPort(oldPort);}OSErr playMovieInWindow(WindowPtr theWindow, const FSSpec *theFile, const Rect *movieBox){	OSErr			err;	Movie			theMovie;	CGrafPtr		savePort;	GDHandle		saveGD;	//GrafPtr			oldPort;			//GetPort(&oldPort);	SetPort(theWindow);		//GetGWorld(&savePort, &saveGD);	//SetGWorld((CGrafPtr)theWindow, nil);		err = loadMovie(theFile, &theMovie);	if (err) goto bail;		SetMovieBox(theMovie, movieBox);		GoToBeginningOfMovie(theMovie);	MoviesTask(theMovie, 0);	StartMovie(theMovie);	while ((IsMovieDone(theMovie) == false) && (Button() == false))		MoviesTask(theMovie, 0);			bail:		DisposeMovie(theMovie);		//SetGWorld(savePort, saveGD);		//SetPort(oldPort);		return err;}OSErr loadMovie(const FSSpec *theFile, Movie *theMovie){	OSErr		err;	short 		movieResRef;		*theMovie = nil;		err = OpenMovieFile(theFile, &movieResRef, fsRdPerm);		if (err == noErr)	{		err = NewMovieFromFile(theMovie, movieResRef, nil, nil, newMovieActive, nil);		CloseMovieFile(movieResRef);	}		return err;}