/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

/*****************************************************************************
 *
 *	Name: ETSprite.cpp
 *	
 * 	Purpose: Definitions of Sprite class interface(s) and methods for the 
 *           EventTest Sprite Xtra.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#ifndef _H_moaxtra
	#include "Moaxtra.h"
#endif

#ifndef _H_csprite
	#include "ETSprite.h"
#endif


#ifdef MACINTOSH
	#include <QuickDraw.h>
	#include <Resources.h>
#endif

#include <String.h>
  
/*****************************************************************************
 *  GLOBAL DATA
 *  ----------------------
 ****************************************************************************/

enum { kSprNotifyMsg_SetParentAsset = 1 };

#define MAX_STRING_LENGTH	32
#define MAX_MODIFIER_TOTAL	16
#define	NUM_ASCII_KEYS		128
#define ASCII_REPRESENTATION_LENGTH	3

	/* Here the names for modifiers are put in an array.  This will be used as a */
	/*look-up table when the modifier value is returned. */
#ifdef MACINTOSH
	MoaChar		gModifiers[MAX_MODIFIER_TOTAL][MAX_STRING_LENGTH] = { "None", "Shift",
		"Command", "Command-Shift ", "Option", "Option-Shift ", "Command-Option ",
		"Command-Option-Shift ", "Control", "Control-Shift", "Control-Command",
		"Command-Shift-Control", "Control-Option", "Control-Option-Shift",
		"Command-Option-Control", "Command-Option-Control-Shift"};
#endif /*MAC  */

#ifdef WINDOWS
	MoaChar		gModifiers[MAX_MODIFIER_TOTAL][MAX_STRING_LENGTH] = { "None", "Shift",
		"Not Possible", "Not Possible", "Alt", "Alt-Shift ", "Not Possible",
		"Not Possible ", "Not Possible", "Not Possible", "Control", "Shift-Control",
		"Not Possible", "Not Possible", "Control-Alt", "Control-Alt-Shift"};
#endif /*WIN */
					
	/*Similar lookup table for ascii return values.  High ascii and non-character ascii */
	/* characters will print the ascii value to the message window.  Characters will */
	/* print the actual character */
	MoaChar		gAsciiKey[NUM_ASCII_KEYS + 1][ASCII_REPRESENTATION_LENGTH + 1] = {
		"nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel", "bs" , "ht" , "nl" , "vt" ,
		"np" , "cr" , "so" , "si" , "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
		"can", "em" , "sub", "esc", "fs" , "gs" , "rs" , "us" , "sp" , "!"  , "\"" , "#"  ,
		"$"  , "%"  , "&"  , "\'" , "("  , ")"  , "*"  , "+"  , ","  , "-"  , "."  , "/"  ,
		"0"  , "1"  , "2"  , "3"  , "4"  , "5"  , "6"  , "7"  , "8"  , "9"  , ":"  , ";"  ,
		"<"  , "="  , ">"  , "?"  , "@"  , "A"  , "B"  , "C"  , "D"  , "E"  , "F"  , "G"  ,
		"H"  , "I"  , "J"  , "K"  , "L"  , "M"  , "N"  , "O"  , "P"  , "Q"  , "R"  , "S"  ,
		"T"  , "U"  , "V"  , "W"  , "X"  , "Y"  , "Z"  , "["  , "\\" , "]"  , "^"  , "_"  ,
		"`"  , "a"  , "b"  , "c"  , "d"  , "e"  , "f"  , "g"  , "h"  , "i"  , "j"  , "k"  ,
		"l"  , "m"  , "n"  , "o"  , "p"  , "q"  , "r"  , "s"  , "t"  , "u"  , "v"  , "w"  ,
		"x"  , "y"  , "z"  , "{"  , "|"  , "}"  , "~"  , "del"
	};

/*****************************************************************************
 *  INTERNAL PROTOTYPES(S)
 *  ----------------------
 *  Declarations for functions used specifically in this file.
 ****************************************************************************/

/*****************************************************************************
 *  INTERNAL ROUTINE(S)
 *  -------------------
 *  Definition of file-specific functions
 ****************************************************************************/

/*****************************************************************************
 *  CLASS INTERFACE(S)
 *  ------------------
 *  The interface(s) implemented by your MOA class are specified here.  Note
 *	that at least one class in your Xtra should implement the IMoaRegister
 *	interface.
 *  NOTE: Because C++ does not use a lpVtbl to reference an interface's
 *	methods, the actual method declaration is done in the .h file.
 *
 *  Syntax:
 *  BEGIN_DEFINE_CLASS_INTERFACE(<class-name>, <interface-name>) 
 ****************************************************************************/ 
BEGIN_DEFINE_CLASS_INTERFACE(CEventTestSprite, IMoaMmXSpriteActor)
END_DEFINE_CLASS_INTERFACE


/*****************************************************************************
 *  CREATE AND DESTROY METHODS
 *  --------------------------
 *  Every interface and class has an associated 'Create' and 'Destroy' pair.
 *  'Create' methods are typically used to acquire interface(s), allocate 
 *  memory, and intialize variables. 'Destroy' methods are typically used to 
 *  release interfaces and free memory.
 *
 * NOTE:  In C++, the local variable 'This' is provided implicitly within 
 * a method implementation.  Thus, there is no need explicitly declare 'This' 
 * as a function parameter. However, this implementation detail doesn't apply 
 * to the MOA class creator and destructor functions, which are standard C 
 * functions, coded exactly as in like they are in C examples.  
 *
 * Class Syntax:
 * STDMETHODIMP MoaCreate_<class-name>(<class-name> * This)
 * STDMETHODIMP MoaDestroy_<class-name>(<class-name> * This)
 *
 * Interface Syntax:
 * <class_name>_<if_name>::<class_name>_<if_name>(MoaError * pErr)
 * <class_name>_<if_name>::~<class_name>_<if_name>()
 ****************************************************************************/ 

/* ------------------------------------------------------ MoaCreate_CEventTestSprite */
STDMETHODIMP MoaCreate_CEventTestSprite(CEventTestSprite * This)
{
	X_ENTER

	MoaError				err = kMoaErr_NoErr;
	
	This->MessageWrite("CEventTestActor Create********\0");

	// Initialize instance variables
	This->pXAsset = NULL;
	This->state = kMyActorState_0;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------------------- MoaDestroy_CEventTestSprite */
STDMETHODIMP_(void) MoaDestroy_CEventTestSprite(CEventTestSprite * This)
{
	X_ENTER

	This->MessageWrite("CEventTestActor Destroy********\0");

	/* Release interfaces we've been hanging on to. */
	if (This->pXAsset) This->pXAsset->Release();
	
	X_RETURN_VOID;
	X_EXIT
}

/* ------------------------------ CEventTestSprite_IMoaMmXSpriteActor Create/Destroy */
CEventTestSprite_IMoaMmXSpriteActor::CEventTestSprite_IMoaMmXSpriteActor(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CEventTestSprite_IMoaMmXSpriteActor::~CEventTestSprite_IMoaMmXSpriteActor() {}


/*****************************************************************************
 *  METHOD IMPLEMENTATION(S)
 *  ------------------------
 *  This is where the methods to be defined by your MOA class are implemented.
 *  The bulk of the work in implementing Xtras is done here.  NOTE: 'This' is 
 *  implemented implicitly in C++, therefore it isn't used in the argument-
 *	list.
 *
 *  Syntax:
 *  STDMETHODIMP <class-name>_<interface-name>::<method-name>(<argument-list>)
 ****************************************************************************/ 

// -------------------------------------------- CEventTestSprite::MessageWrite
// This is a utility method scoped to the class and available to any interface
//  derived from it.  In this example, there is only one interface that uses
//  the CEventTestSprite class, but it is still necessary to scope the function
//  higher than CEventTestSprite_IMoaMmXSpriteActor because it needs to be
//  available in the class constructor/destructor methods.
MoaError CEventTestSprite::MessageWrite(char *message)
{
	X_ENTER
	
	/* variable declarations */
	MoaError				err = kMoaErr_NoErr;
	PIMoaMmUtils2   pIMoaMmUtils2 = NULL;		
	char						buf[256];

	// Get the Utils interface
	if ((err = pCallback->QueryInterface(&IID_IMoaMmUtils2, (PPMoaVoid)&pIMoaMmUtils2)))
		X_STD_RETURN(kMoaErr_BadInterface);
	
	sprintf(buf, "-- %s\n", message);
	pIMoaMmUtils2->PrintMessage(buf);

	// release Utils
	if (pIMoaMmUtils2) pIMoaMmUtils2->Release();
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CEventTestSprite_IMoaMmXSpriteActor::GetProp */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::GetProp(MoaMmSymbol symbol, PMoaMmValue pPropValue)
{
	X_ENTER
	
	/* variable declarations */
	MoaError	err;

	err = kMoaMmErr_PropertyNotFound;

	/* Print to message window that this method has been called */
	pObj->MessageWrite("GetProp\0");
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CEventTestSprite_IMoaMmXSpriteActor::SetProp */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::SetProp(MoaMmSymbol symbol,
	ConstPMoaMmValue pPropValue)
{
	X_ENTER
	
	/* variable declarations */
	MoaError	err;
	
	err = kMoaMmErr_PropertyNotFound;

	/* Print to message window that this method has been called */
	pObj->MessageWrite("SetProp\0");

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------- CEventTestSprite_IMoaMmXSpriteActor::CallFunction */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::CallFunction(MoaMmSymbol methodName, MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{
	X_ENTER
	
	/* variable declarations */
	MoaError	err;
	
	/* Print to message window that this method has been called */
	pObj->MessageWrite("CallFunction\0");

	err = kMoaMmErr_FunctionNotFound;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* -------------------------------- CEventTestSprite_IMoaMmXSpriteActor::SetCallback */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::SetCallback(PIMoaMmSpriteCallback pSpriteCallback)
{
	X_ENTER

	/* variable declarations */
	MoaError	err = kMoaErr_NoErr;

	/* Print to message window that this method has been called */
	pObj->MessageWrite("SetCallback\0");
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------- CEventTestSprite_IMoaMmXSpriteActor::Notify */
/* This method is intended for communicating between the asset and its xtras.
   The developer is free to define message codes for private use.  This may be
   called directly by an asset, or through the 
   IMoaMmAssetCallback::NotifyAllSprites() callback. */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	X_ENTER

	/* variable declarations */
	MoaError	err = kMoaErr_NoErr;

	/* Print to message window that this method has been called */
	pObj->MessageWrite("Notify\0");
	
	switch (msgCode) 
	{
		case kSprNotifyMsg_SetParentAsset:
			pObj->pXAsset = (PIMoaMmXAsset) refCon;
			pObj->pXAsset->AddRef();	
			/* up ref count since we're hanging on to this	*/
			break;
		default:
			break;		
	}

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CEventTestSprite_IMoaMmXSpriteActor::GetCaps */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::GetCaps(PMoaMmXSpriteCaps pActorCaps)
{
	X_ENTER

	/* variable declarations */
	MoaError	err = kMoaErr_NoErr;

	/* Print to message window that this method has been called */
	pObj->MessageWrite("GetCaps\0");

	/* We only want mouse and play events */
	pActorCaps->eventMask = (kMoaMmEventMask_Every);

	// We support no special actor capabilities
	//use kMoaMmXSpriteCapsFlags_AutoTabEnabled for autoTab support
	pActorCaps->flags = 	kMoaMmXSpriteCapsFlags_None;
	
	// We support either offscreen or direct-to-screen imaging,
	// but we're intended for offscreen imaging (the normal case).
	pActorCaps->requiredImageMode = kMoaMmXSpriteImageMode_Any;
	pActorCaps->preferredImageMode = kMoaMmXSpriteImageMode_Offscreen;

	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CEventTestSprite_IMoaMmXSpriteActor::CollectChanges */
/* Here we always add our entire rect as the change area, since we may be
	animating and we want Director to refresh the stage with our sprite every frame.
	
	If our actor is not changing its appearance in the coming frame, we would not
	add any change rects here.
	
	(Note: if the sprite is moving or any of its attributes have changed (such as
	color), it is always updated in its entirety automatically, and our
	CollectChanges() is not called in this case.)

	In the actorInfo, Director has supplied us with what will be our full sprite
	rect in the coming frame.  We simply add this entire rectangle to indicate that
	the entire sprite needs updating. */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::CollectChanges(ConstPMoaMmXSpriteInfo pActorInfo, PIMoaMmChangeCollector pChangeCollector)
{
	X_ENTER
	
	/* variable declarations */
	MoaError	err = kMoaErr_NoErr;

	/* Print to message window that this method has been called */
	pObj->MessageWrite("CollectChanges\0");

	pChangeCollector->AddChangeRect(&pActorInfo->spriteRect);

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------ CEventTestSprite_IMoaMmXSpriteActor::ContainsPoint */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::ContainsPoint(ConstPMoaMmXSpriteInfo pActorInfo, ConstPMoaPoint pTestPoint, MoaBoolParam * pTestResult)
{
	X_ENTER
	
	/* variable declarations */
	MoaError	err = kMoaErr_NoErr;

	/* Print to message window that this method has been called */
	pObj->MessageWrite("ContainsPoint\0");

	// Anywhere within our sprite rect is a hit.
	*pTestResult = TRUE;
	
	X_STD_RETURN(err);
	X_EXIT
}


#define PENSIZE 2

/* -------------------------------------- CEventTestSprite_IMoaMmXSpriteActor::Image */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::Image(ConstPMoaMmXSpriteInfo pActorInfo,	ConstPMoaMmPlayerStateInfo	pPlayerStateInfo, PIMoaMmGC pGC)
{
	X_ENTER	
	
	/* variable declarations */
	MoaError				err = kMoaErr_NoErr;
	PMoaRect				pImageRect = (PMoaRect) &pActorInfo->spriteRect;
	MoaMmNativeGCInfo		nativeGC;
	XtraResourceCookie 		myCookie, saveCookie;

	pGC->GetNativeGCInfo(&nativeGC);
	myCookie = pObj->pCallback->MoaBeginUsingResources(gXtraFileRef, &saveCookie);

	/* Print to message window that this method has been called */
	pObj->MessageWrite("Image\0");

#ifdef MACINTOSH

	const Str255		Hopper = "\pHopper";
	const Str255		Director_5 = "\pDirector_5";
	GrafPtr				savePort, drawPort;
	PenState			savePen;
	RGBColor			saveColor;
	RGBColor			myColor = {65535, 0, 0};	/* pure red */
	PicHandle 			hPic;
	Rect				macRect;

	MoaToMacRect(pImageRect, &macRect);
	/* Determine what port to draw into based on the */
	/* supplied Graphics Context.  The host app sets up */
	/* this context for us. */

	switch (nativeGC.type) 
	{
		case kMoaMmGCType_MacWindow:
			drawPort = (GrafPtr) nativeGC.data.mac_window.windowPtr;
			break;
		case kMoaMmGCType_MacGWorld:
			drawPort = (GrafPtr) nativeGC.data.mac_gworld.gworldPtr;
			break;
		default:
			/* we don't know about this type of graphics context; bail out. */
			goto exit_gracefully;
	}
			
	GetPort(&savePort);
	SetPort( drawPort );
	
	/* We could obey the sprite color supplied in pActorInfo here, */
	/* but instead we always draw using myColor. */
	GetForeColor(&saveColor);
	RGBForeColor(&myColor);
	GetPenState(&savePen);
	PenNormal();
	PenSize(PENSIZE, PENSIZE);
		
	/* Mouse hits on the image will cause the picture to change */
	/*  Check out CEventTestActor_IMoaMmXSpriteActor_Event for the toggle */
	switch (pObj->state) 
	{
		case kMyActorState_1:
			/* Draw Spike splash screen on stage */
			hPic = (PicHandle)GetNamedResource('PICT', Hopper);
			DrawPicture((PicHandle)hPic, &macRect);
			break;

		case kMyActorState_0:
			/* Draw director 4.0 splash screen on stage				 */
			hPic = (PicHandle)GetNamedResource('PICT', Director_5);
			DrawPicture((PicHandle)hPic, &macRect);			
			break;

		default:
		
			/* Unknown state, don't draw anything */
			break;
	}
	
	/* Restore our drawing state and port. */
	RGBForeColor(&saveColor);
	
	SetPenState(&savePen);
	SetPort(savePort);

#endif	/* MACINTOSH */

#ifdef WINDOWS

	HDC			drawDC;
	HPEN		hOldPen;
	HBRUSH		hOldBrush;
	long *		err2 = NULL;
	HDC			hdcMem1;
	BITMAP 		bm1;
	HBITMAP		hBitMap1;
	WORD		bitmapNum = THUMBNAIL;

  /* Determine what port to draw into based on the supplied Graphics Context.
  	The host app sets up this context for us; if we're Direct To Stage or
  	trails ink is applied, the context is that of the stage window; otherwise
  	it is that of the stage offscreen buffer. */

  switch (nativeGC.type) 
  {
		case kMoaMmGCType_WinDIBDC:
		case kMoaMmGCType_WinDDBDC:
		case kMoaMmGCType_WinWindow:
			drawDC = (HDC)nativeGC.data.win_window.hDC;
			break;
		default:
			/* we don't know about this type of graphics context; bail out. */
			goto exit_gracefully;
  }
 
  /* We could obey the sprite color supplied in pActorInfo here, */
  /* but instead we always draw using a fixed color. */

	hOldPen = (HPEN)SelectObject(drawDC, GetStockObject(NULL_PEN));
	hOldBrush = (HBRUSH)SelectObject(drawDC, GetStockObject(NULL_BRUSH));

  /* Mouse hits on the image will cause the picture to change */
  /*  Check out CEventTestActor_IMoaMmXSpriteActor_Event for the toggle */
  switch (pObj->state) 
  {
    case kMyActorState_0:
   		/* Load bitmap resource */
		  hBitMap1 = LoadBitmap((HINSTANCE)myCookie, MAKEINTRESOURCE(SPIKE));

			if(hBitMap1 != 0) 
			{
				// Create another graphics context equivalent to DrawGC
 				hdcMem1 = CreateCompatibleDC (drawDC);

				// Get information about hBitmap1
				GetObject(hBitMap1, sizeof(BITMAP), (LPVOID) &bm1);
			
				// Select it
				SelectObject(hdcMem1, hBitMap1);
			
				// Draw loaded bitmap into the DrawDC graphics context at a new size
				StretchBlt(drawDC, pImageRect->left, pImageRect->top,
					pImageRect->right - pImageRect->left, pImageRect->bottom - pImageRect->top,
					hdcMem1, 0, 0, bm1.bmWidth, bm1.bmHeight,  SRCCOPY);

				DeleteDC(hdcMem1);
    	}
		break;

		case kMyActorState_1:
   			// Load bitmap resource
			hBitMap1 = LoadBitmap((HINSTANCE)myCookie, MAKEINTRESOURCE(DIRECTOR_4));

			if(hBitMap1 != 0) 
			{
				// Create another graphics context equivalent to DrawGC
 				hdcMem1 = CreateCompatibleDC (drawDC);

				// Get information about hBitmap1
				GetObject(hBitMap1, sizeof(BITMAP), (LPVOID) &bm1);

				// Select it
				SelectObject(hdcMem1, hBitMap1);

				// Draw loaded bitmap into the DrawDC graphics context at a new size
				StretchBlt(drawDC, pImageRect->left, pImageRect->top, pImageRect->right - pImageRect->left, pImageRect->bottom - pImageRect->top,
							hdcMem1, 0, 0, bm1.bmWidth, bm1.bmHeight,  SRCCOPY);

				DeleteDC(hdcMem1);
    		}
      break;

    default:
      // Unknown state, don't draw anything
      break;
  }

	/*Restore brush and pen */
    SelectObject(drawDC, hOldBrush);
    SelectObject(drawDC, hOldPen);
	DeleteObject(hBitMap1);

#endif /* WINDOWS */

exit_gracefully:

	pObj->pCallback->MoaEndUsingResources(gXtraFileRef, saveCookie);

	X_STD_RETURN(err);
	X_EXIT
}


/* -------------------------------------- CEventTestSprite_IMoaMmXSpriteActor::Event */
STDMETHODIMP CEventTestSprite_IMoaMmXSpriteActor::Event(ConstPMoaMmEventInfo pEventInfo, ConstPMoaMmPlayerStateInfo pPlayerStateInfo, MoaBoolParam * pHandled)
{
	X_ENTER
	
	/* variable declarations */
	MoaError	err = kMoaErr_NoErr;
	char buf[256];

	/* This is where all the events are handled	 */
	switch (pEventInfo->type) 
	{
		case kMoaMmEvent_Step:
			/* Print to message window that this event has been registered */
			/* This also prints the mouse position */
			sprintf(buf,"Step with mouse at at v = %ld,  h = %ld", pEventInfo->where.y, pEventInfo->where.x);
	  		pObj->MessageWrite(buf);
			break;
		
		case kMoaMmEvent_MouseDown:
			/* Print to message window that this event has been registered */
			/* This also prints the position the mouse was clicked at */
			sprintf(buf,"MOUSE_DOWN at v = %ld,  h = %ld, time = %ld", 
				pEventInfo->where.y, pEventInfo->where.x, pEventInfo->when);
	  		pObj->MessageWrite(buf);

			/* Toggle our visual state */
			if (pObj->state == kMyActorState_0) pObj->state = kMyActorState_1;
			else pObj->state = kMyActorState_0;

			*pHandled =   kMoaMmSprEvent_Handled;
			break;

		case kMoaMmEvent_MouseUp:
			/* Print to message window that this event has been registered */
			/* This also prints the position the mouse was clicked up at */
			sprintf(buf,"MOUSE_UP at v = %ld,  h = %ld", pEventInfo->where.y, pEventInfo->where.x);
	  		pObj->MessageWrite(buf);
			*pHandled = kMoaMmSprEvent_Pass;
			break;
			
		case kMoaMmEvent_MouseUpOutside:
			/* Print to message window that this event has been registered */
			/* This also prints the position the mouse was clicked up at */
			/* This event happens when you click down inside the region of the Xtra */
			/* on stage then hold and drag the mouse outside the region and let up. */
			sprintf(buf,"MOUSE_UP_OUTSIDE at v = %ld,  h = %ld", pEventInfo->where.y, pEventInfo->where.x);
	  		pObj->MessageWrite(buf);

			*pHandled = kMoaMmSprEvent_Pass;
			break;
			
		case kMoaMmEvent_RightMouseDown:
			 /* Print to message window that this method has been called */
			 /* This also prints the position the mouse was clicked at */
			 sprintf(buf,"RIGHT_MOUSE_DOWN at v = %ld,  h = %ld, time = %ld", 
				pEventInfo->where.y, pEventInfo->where.x, pEventInfo->when);
			 pObj->MessageWrite(buf);

			 /* Toggle our visual state */
			 if (pObj->state == kMyActorState_0) pObj->state = kMyActorState_1;
			 else pObj->state = kMyActorState_0;

			*pHandled = kMoaMmSprEvent_Pass;
			break;

		case kMoaMmEvent_RightMouseUp:
			/* Print to message window that this method has been called */
			/* This also prints the position the mouse was clicked up at */
			sprintf(buf,"RIGHT_MOUSE_UP at v = %ld,  h = %ld", pEventInfo->where.y, pEventInfo->where.x);
			pObj->MessageWrite(buf);
		
			/* If you handle MouseUp here, set
				*pHandled = kMoaMmSprEvent_Handled. */
			*pHandled = kMoaMmSprEvent_Handled;
			break;
			
		case kMoaMmEvent_RightMouseUpOutside:
			/* Print to message window that this method has been called */
			/* This also prints the position the mouse was clicked up at */
			/* This event happens when you click down inside the region of the Xtra */
			/* on stage then hold and drag the mouse outside the region and let up. */
			sprintf(buf,"RIGHT_MOUSE_UP_OUTSIDE at v = %ld,  h = %ld",pEventInfo->where.y, pEventInfo->where.x);
			pObj->MessageWrite(buf);

			*pHandled = kMoaMmSprEvent_Pass;
			break;
            
		case kMoaMmEvent_Idle:
			*pHandled = kMoaMmSprEvent_Pass;
			break;
			
		case kMoaMmEvent_SpanBegin:
			/* Print to message window that this event has been registered */
			/* This represent the beginning of a sprite span on the stage */
	  		pObj->MessageWrite("SpanBegin\0");

			/* A new sprite span has started; init our state variables. */
			pObj->state = kMyActorState_0;
			*pHandled = kMoaMmSprEvent_Handled;
			break;
		
		case kMoaMmEvent_SpanEnd:
			/* Print to message window that this event has been registered */
			/* This represents the end of a sprite span on the stage */
	  		pObj->MessageWrite("Span End\0");

			*pHandled = kMoaMmSprEvent_Pass;
			break;
		
		case kMoaMmEvent_KeyFocusEnter:
			/* Print to message window that this event has been registered */
			/* This represent that the Xtra has received Focus */
	  		pObj->MessageWrite("FocusEnter\0");

			*pHandled = kMoaMmSprEvent_Handled;
			break;
			
		case kMoaMmEvent_KeyFocusExit:
			/* Print to message window that this event has been registered */
			/* This represent that the Xtra has lost Focus */
	  		pObj->MessageWrite("FocusExit\0\0");

			*pHandled = kMoaMmSprEvent_Handled;
			break;
			
		case kMoaMmEvent_KeyDown:
			/* Print to message window that this event has been registered */
			/* This also prints the key that was pressed and any modifiers as well */
			
			if (pEventInfo->data.keyInfo.character < 128 && pEventInfo->data.keyInfo.character > 26) 
			{
				/*Ascii characters printed to message window */
				sprintf(buf,"KEY_DOWN =  %s , Modifiers = %s", 
					gAsciiKey[pEventInfo->data.keyInfo.character], gModifiers[pEventInfo->data.keyInfo.modifiers]);
	  		} 
			else 
			{
				sprintf(buf,"KEY_DOWN ascii code = %ld, Modifiers = %s", 
					pEventInfo->data.keyInfo.character, gModifiers[pEventInfo->data.keyInfo.modifiers]);
			}
	  		pObj->MessageWrite(buf);
			
#ifdef MACINTOSH
			/*native key codes and modifiers characters printed to message window */
			sprintf(buf,"native key code =  %ld , Native modifiers = %ld", 
						pEventInfo->data.keyInfo.nativeKeyCode, pEventInfo->data.keyInfo.nativeModifiers);
  			pObj->MessageWrite(buf);
#endif  /*MAC */

			*pHandled = kMoaMmSprEvent_Pass;
			break;

		case kMoaMmEvent_KeyUp:
			/* Print to message window the appropriate key and modifier names */
			if(pEventInfo->data.keyInfo.character < 128 && pEventInfo->data.keyInfo.character > 26)
				sprintf(buf,"KEY_UP =  %s , Modifiers = %s", 
					gAsciiKey[pEventInfo->data.keyInfo.character], gModifiers[pEventInfo->data.keyInfo.modifiers]);
			else
				sprintf(buf,"KEY_UP ascii code = %ld, Modifiers = %s", 
					pEventInfo->data.keyInfo.character, gModifiers[pEventInfo->data.keyInfo.modifiers]);
	  		pObj->MessageWrite(buf);

#ifdef MACINTOSH
			/*native key codes and modifiers characters printed to message window */
			sprintf(buf,"native key code =  %ld , Native modifiers = %ld", 
				pEventInfo->data.keyInfo.nativeKeyCode, pEventInfo->data.keyInfo.nativeModifiers);
  			pObj->MessageWrite(buf);
#endif  /*MAC */

			*pHandled = kMoaMmSprEvent_Pass;
			break;

		case kMoaMmEvent_PlayBegin:
			/* Print to message window that this event has been registered */
			/* Movie has begun playing */
			pObj->MessageWrite("PlayBegin\0");

			*pHandled = kMoaMmSprEvent_Pass;
			break;

		case kMoaMmEvent_PlayEnd:
			/* Print to message window that this event has been registered */
			/* Movie has ended playing */
	  		pObj->MessageWrite("PlayEnd\0");

			*pHandled = kMoaMmSprEvent_Pass;
			break;

		case kMoaMmEvent_StageWindowDepth:
			/* Print to message window that this event has been registered */
	  		pObj->MessageWrite("StageWindowDepthChanged\0");

			*pHandled = kMoaMmSprEvent_Pass;
			break;

		case kMoaMmEvent_StageWindowOpen:
			/* Print to message window that this event has been registered */
	  		pObj->MessageWrite("StageWindowOpen\0");

			*pHandled = kMoaMmSprEvent_Pass;
			break;

		case kMoaMmEvent_StageWindowClose:
			/* Print to message window that this event has been registered */
	  		pObj->MessageWrite("StageWindowClose\0");

			*pHandled = kMoaMmSprEvent_Pass;
			break;

		case kMoaMmEvent_SoundState:
			/* Print to message window that this event has been registered */
			/* Sound volume on or off triggers this (not volume change yet) */
	  		pObj->MessageWrite("SoundStateChanged\0");

			*pHandled = kMoaMmSprEvent_Pass;
			break;

		case kMoaMmEvent_AssetModified:
			/* Print to message window that this event has been registered */
	  		pObj->MessageWrite("AssetModified\0");

			*pHandled = kMoaMmSprEvent_Pass;
			break;
			
		default:
			*pHandled = kMoaMmSprEvent_Pass;
	}

	X_STD_RETURN(err);
	X_EXIT
}

