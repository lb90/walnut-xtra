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
 *	Name: ETAsset.cpp
 *	
 * 	Purpose: Definitions of Asset class interface(s) and methods for the 
 *           EventTest Xtra.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "ETAsset.h"
#include "ETSprite.h"

#ifdef MACINTOSH
	#include <QuickDraw.h>
	#include <Resources.h>
	#include <String.h>
#endif

#include <String.h> 

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
BEGIN_DEFINE_CLASS_INTERFACE(CEventTestAsset, IMoaMmXAsset)
END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
BEGIN_DEFINE_CLASS_INTERFACE(CEventTestAsset, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
BEGIN_DEFINE_CLASS_INTERFACE(CEventTestAsset, IMoaNotificationClient)
END_DEFINE_CLASS_INTERFACE
#endif

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

/* ------------------------------------------------------- MoaCreate_CEventTestAsset */
STDMETHODIMP  		MoaCreate_CEventTestAsset(CEventTestAsset * This)
{
	UNUSED(This);
	X_ENTER
	
	/* variable declarations */
	MoaError	err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------------------------ MoaDestroy_CEventTestAsset */
STDMETHODIMP_(void) MoaDestroy_CEventTestAsset(CEventTestAsset * This)
{
	UNUSED(This);
	X_ENTER
	
	X_RETURN_VOID;
	X_EXIT
}

/* ------------------------------------- CEventTestAsset_IMoaMmXAsset Create/Destroy */
CEventTestAsset_IMoaMmXAsset::CEventTestAsset_IMoaMmXAsset(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CEventTestAsset_IMoaMmXAsset::~CEventTestAsset_IMoaMmXAsset() {}

#ifdef USING_INIT_FROM_DICT
/* ------------------------------------ CEventTestAsset_IMoaInitFromDict Create/Destroy */
CEventTestAsset_IMoaInitFromDict::CEventTestAsset_IMoaInitFromDict(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CEventTestAsset_IMoaInitFromDict::~CEventTestAsset_IMoaInitFromDict() {}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* --------------------------- CEventTestAsset_IMoaNotificationClient Create/Destroy */
CEventTestAsset_IMoaNotificationClient::CEventTestAsset_IMoaNotificationClient(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CEventTestAsset_IMoaNotificationClient::~CEventTestAsset_IMoaNotificationClient() {}
#endif

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

/* ------------------------------------------- CEventTestAsset_IMoaMmXAsset::GetProp */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::GetProp(MoaMmSymbol symbol, PMoaMmValue	pPropValue)
{
	UNUSED(symbol);
	UNUSED(pPropValue);
	X_ENTER

	MoaError		err = kMoaMmErr_PropertyNotFound;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------------- CEventTestAsset_IMoaMmXAsset::SetProp */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::SetProp(MoaMmSymbol symbol, ConstPMoaMmValue pPropValue)
{
	UNUSED(symbol);
	UNUSED(pPropValue);
	X_ENTER

	MoaError		err = kMoaMmErr_PropertyNotFound;

	X_STD_RETURN(err);
	X_EXIT
}

/* -------------------------------------- CEventTestAsset_IMoaMmXAsset::CallFunction */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::CallFunction(MoaMmSymbol methodName, MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{
	UNUSED(methodName);
	UNUSED(nArgs);
	UNUSED(pArgs);
	UNUSED(pResult);
	X_ENTER
	
	MoaError		err = kMoaMmErr_FunctionNotFound;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------------- CEventTestAsset_IMoaMmXAsset::SetCallback */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::SetCallback(PIMoaMmAssetCallback pAssetCallback)
{
	UNUSED(pAssetCallback);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* -------------------------------------------- CEventTestAsset_IMoaMmXAsset::Notify */
/* This method is intended for communicating between the asset and its xtras.
   The developer is free to define message codes for private use.  This may be
   called directly by an actor, or through the
   IMoaMmSpriteCallback::NotifyAsset() callback.
 */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	UNUSED(msgCode);
	UNUSED(refCon);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CEventTestAsset_IMoaMmXAsset::GetDefaultRect */
/* We return our default rectangle centered around (0,0).  (0,0) here
   corresponds to the "loc" of the sprite, whichis the point of reference for
   the sprite.  Dragging the sprite out to the stage will position the sprite
   such that the loc is at the mouse drop position.
 */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::GetDefaultRect(PMoaRect pRect)
{
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;
	
	/* RECTSIZE x RECTSIZE centered about (0,0) */
	
	pRect->top = -(RECTSIZE/2);
	pRect->left = -(RECTSIZE/2);
	pRect->bottom = RECTSIZE/2;
	pRect->right = RECTSIZE/2;
		
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CEventTestAsset_IMoaMmXAsset::PrepareNewContent */
/* This method is called to have the class set-up its default state when a new
   cast member is created (note that this is different from when a new
   instance of CEventTestAsset is created; instances of the XAsset are created at both
   movie-open time (for pre-existing cast members being read-in) and new cast
   member creation time.  PrepareNewContent() is only called at new cast
   member creation time, just after the XAsset instance is created. 
 */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::PrepareNewContent(MoaMmNewFlags newFlags)
{
	UNUSED(newFlags);
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;
	
	/* A new cast member is being created in memory.  Set up default media and
	   props. */

	/* Dispose of any media and props we may have currently in memory */
	CEventTestAsset_IMoaMmXAsset::FreeMedia();
	CEventTestAsset_IMoaMmXAsset::FreeProps();
	
	/* Initialize props / media to default values here. 
	   The host app assumes that both props and media are "dirty" when a new
	   cast member is created. 
	
	   We are now "loaded" in the sense that we have everything we need from 
	   disk (there isn't anything on disk yet, so we exist only in-memory 
	   at this point). */

	X_STD_RETURN(err);
	X_EXIT
}


/* ------------------------------------ CEventTestAsset_IMoaMmXAsset::StreamOutMedia */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::StreamOutMedia(PIMoaStream pOutStream)
{
	UNUSED(pOutStream);
	X_ENTER
	
	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------- CEventTestAsset_IMoaMmXAsset::StreamInMedia */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::StreamInMedia(PIMoaStream pInStream)
{
	UNUSED(pInStream);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------- CEventTestAsset_IMoaMmXAsset::FreeMedia */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::FreeMedia()
{
	X_ENTER

	MoaError	err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CEventTestAsset_IMoaMmXAsset::GetInMemMediaSize */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::GetInMemMediaSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CEventTestAsset_IMoaMmXAsset::GetStreamOutMediaSize */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::GetStreamOutMediaSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CEventTestAsset_IMoaMmXAsset::StreamOutProps */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::StreamOutProps(PIMoaStream pOutStream)
{
	UNUSED(pOutStream);
	X_ENTER
	
	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------- CEventTestAsset_IMoaMmXAsset::StreamInProps */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::StreamInProps(PIMoaStream pInStream)
{
	UNUSED(pInStream);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------- CEventTestAsset_IMoaMmXAsset::FreeProps */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::FreeProps()
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CEventTestAsset_IMoaMmXAsset::GetInMemPropsSize */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::GetInMemPropsSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CEventTestAsset_IMoaMmXAsset::GetStreamOutPropsSize */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::GetStreamOutPropsSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------- CEventTestAsset_IMoaMmXAsset::NewXActor */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::NewXActor(PMoaVoid * ppXActor) 
{
	X_ENTER

	MoaError						err = kMoaErr_NoErr;
	PIMoaMmXAsset 			pSelf = NULL;	
	PIMoaMmXSpriteActor pNewActor = NULL;
	
	/* Do a createinstance using IMoaMmXSpriteActor for return to caller... */
	if ((err = pObj->pCallback->MoaCreateInstance(
		&CLSID_CEventTestSprite, &IID_IMoaMmXSpriteActor, (PPMoaVoid)&pNewActor)) != kMoaErr_NoErr)
			goto exit_gracefully;

	pSelf = (PIMoaMmXAsset) this;
			
	/* Tell actor who its parent is, in case it's interested. 
		It should AddRef() on the interface if it wants to hold onto it.
	*/
		
	(pNewActor)->Notify(kSprNotifyMsg_SetParentAsset, pSelf);
				
	*ppXActor = pNewActor;

exit_gracefully:
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CEventTestAsset_IMoaMmXAsset::InvokeMediaEditor */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::InvokeMediaEditor(PMoaMmModifyFlags pModifyFlags)
{
	UNUSED(pModifyFlags);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CEventTestAsset_IMoaMmXAsset::InvokePropsEditor */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::InvokePropsEditor()
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CEventTestAsset_IMoaMmXAsset::FinishPropsEditor */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::FinishPropsEditor(MoaBoolParam commitChanges,					
	PMoaMmModifyFlags pModifyFlags)
{
	UNUSED(commitChanges);
	UNUSED(pModifyFlags);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------- CEventTestAsset_IMoaMmXAsset_InvokeAboutBox */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::InvokeAboutBox()
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ---------------------------------- CEventTestAsset_IMoaMmXAsset::ImageStaticThumb */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::ImageStaticThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	X_ENTER

	MoaError						err = kMoaErr_NoErr;
	MoaMmNativeGCInfo		nativeGC;
	XtraResourceCookie 	myCookie, saveCookie;

	pGC->GetNativeGCInfo(&nativeGC);
	myCookie = pObj->pCallback->MoaBeginUsingResources(gXtraFileRef, &saveCookie);
	
#ifdef MACINTOSH

	GrafPtr				savePort, drawPort;
	Rect 				macRect;
	RGBColor			saveColor;
	Handle				myHandle;
	const Str255		Dying_Star = "\pDying_Star";
		
	MoaToMacRect(pImageRect, &macRect);
	/* Determine what port to draw into based on the */
	/* supplied Graphics Context.  The host app sets up */
	/* this context for us; if we're Direct To Stage or */
	/* trails ink is applied, the context is that of */
	/* the stage window; otherwise it is that of the stage */
	/* offscreen buffer. */
	
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
			goto exit_gracefully;;
	}

	/* Save port and color */
	GetForeColor(&saveColor);
	GetPort(&savePort);
	SetPort(drawPort);
	

	/* Draw Static Thumbnail Icon */
	if ((myHandle = GetNamedResource('PICT', Dying_Star))) DrawPicture((PicHandle)myHandle, &macRect);

	SetPort(savePort);
	RGBForeColor(&saveColor	);	

#endif	/* MACINTOSH */
	
#ifdef WINDOWS

	/*LPCSTR		Dying_Star = "Dying_Star"; */
	HDC				drawDC;
	HPEN			hOldPen;
	HBRUSH		hOldBrush;
	long *		err2 = NULL;
	HDC				hdcMem1;
	BITMAP 		bm1;
	HBITMAP		hBitMap1;
	WORD			bitmapNum = THUMBNAIL;

	/* Determine what port to draw into based on the */
	/* supplied Graphics Context.  The host app sets up */
	/* this context for us; if we're Direct To Stage or */
	/* trails ink is applied, the context is that of */
	/* the stage window; otherwise it is that of the stage */
	/* offscreen buffer. */

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

	/* Load bitmap resource */
    
	hBitMap1 = LoadBitmap((HINSTANCE)myCookie, MAKEINTRESOURCE(bitmapNum));

	if(hBitMap1 != 0) 
	{
		/* Create another graphics context equivalent to DrawGC */
 		hdcMem1 = CreateCompatibleDC (drawDC);

		/*Get information about hBitmap1 */
		GetObject(hBitMap1, sizeof(BITMAP), (LPVOID) &bm1);
	
		/* Do I need to do this? */
		SelectObject(hdcMem1, hBitMap1);
	
		/*Draw loaded bitmap into the DrawDC graphics context at a new size			 	 */
		StretchBlt(drawDC, pImageRect->left, pImageRect->top, 
			pImageRect->right - pImageRect->left,
			pImageRect->bottom - pImageRect->top,
			hdcMem1, 0, 0, bm1.bmWidth, bm1.bmHeight,  SRCCOPY);
 
 		DeleteDC(hdcMem1);
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

/* ------------------------------- CEventTestAsset_IMoaMmXAsset::BeginImageAnimThumb */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::BeginImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	UNUSED(pGC);
	UNUSED(pImageRect);
	X_ENTER

	MoaError		err = kMoaMmErr_NotImplemented;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CEventTestAsset_IMoaMmXAsset::ServiceImageAnimThumb */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::ServiceImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	UNUSED(pGC);
	UNUSED(pImageRect);
	X_ENTER

	MoaError		err = kMoaMmErr_NotImplemented;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CEventTestAsset_IMoaMmXAsset::EndImageAnimThumb */
STDMETHODIMP CEventTestAsset_IMoaMmXAsset::EndImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	UNUSED(pGC);
	UNUSED(pImageRect);
	X_ENTER

	MoaError		err = kMoaMmErr_NotImplemented;
	
	X_STD_RETURN(err);
	X_EXIT
}

#ifdef USING_INIT_FROM_DICT
/* ---------------------------------- CEventTestAsset_IMoaInitFromDict::InitFromDict */
STDMETHODIMP CEventTestAsset_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
{
	UNUSED(pRegistryDict);
	X_ENTER
	
	/* variable declarations */
	MoaError err = kMoaErr_NoErr;

	X_STD_RETURN(err);
	X_EXIT
}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* ---------------------------------- CEventTestAsset_IMoaNotificationClient::Notify */
STDMETHODIMP CEventTestAsset_IMoaNotificationClient::Notify(ConstPMoaNotifyID nid, PMoaVoid pNData, PMoaVoid pRefCon)
{
	UNUSED(nid);
	UNUSED(pNData);
	UNUSED(pRefCon);
	X_ENTER
	
	/* variable declarations */
	MoaError err = kMoaErr_NoErr;

	X_STD_RETURN(err);
	X_EXIT
}
#endif

