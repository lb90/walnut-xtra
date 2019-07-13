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
 *	Name: itasset.cpp
 *	
 * 	Purpose: Definitions of Asset class interface(s) and methods for the 
 *           InkTest Xtra.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "ITAsset.h"
#include "ITSprite.h"


#ifdef MACINTOSH
	#include <QuickDraw.h>
	#include <QDOffscreen.h>
	#include <Resources.h>
	#include <String.h>
#endif

#ifdef WINDOWS
	#include <windows.h>
	#include <windowsx.h>
	#include <string.h>
#endif

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
BEGIN_DEFINE_CLASS_INTERFACE(CInkTestAsset, IMoaMmXAsset)
END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
BEGIN_DEFINE_CLASS_INTERFACE(CInkTestAsset, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
BEGIN_DEFINE_CLASS_INTERFACE(CInkTestAsset, IMoaNotificationClient)
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

/* ------------------------------------------------------- MoaCreate_CInkTestAsset */
STDMETHODIMP  		MoaCreate_CInkTestAsset(CInkTestAsset * This)
{
	UNUSED(This);
	X_ENTER
	
	/* variable declarations */
	MoaError	err = kMoaErr_NoErr;
	
	/* Initialize image data */
	This->ImageCreate(&This->image);
		
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------------------------ MoaDestroy_CInkTestAsset */
STDMETHODIMP_(void) MoaDestroy_CInkTestAsset(CInkTestAsset * This)
{
	UNUSED(This);
	X_ENTER
	
	/* Dispose image data */
	This->ImageDestroy(&This->image);
	
	X_RETURN_VOID;
	X_EXIT
}

/* ------------------------------------- CInkTestAsset_IMoaMmXAsset Create/Destroy */
CInkTestAsset_IMoaMmXAsset::CInkTestAsset_IMoaMmXAsset(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CInkTestAsset_IMoaMmXAsset::~CInkTestAsset_IMoaMmXAsset() {}

#ifdef USING_INIT_FROM_DICT
/* ------------------------------------ CInkTestAsset_IMoaInitFromDict Create/Destroy */
CInkTestAsset_IMoaInitFromDict::CInkTestAsset_IMoaInitFromDict(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CInkTestAsset_IMoaInitFromDict::~CInkTestAsset_IMoaInitFromDict() {}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* --------------------------- CInkTestAsset_IMoaNotificationClient Create/Destroy */
CInkTestAsset_IMoaNotificationClient::CInkTestAsset_IMoaNotificationClient(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CInkTestAsset_IMoaNotificationClient::~CInkTestAsset_IMoaNotificationClient() {}
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

/* ------------------------------------------- CInkTestAsset::ImageCreate */
MoaError CInkTestAsset::ImageCreate(PImageData pImage) {
	MoaError			err = kMoaErr_NoErr;
	XtraResourceCookie 	myCookie, saveCookie;
	PIMoaMmUtils 		pUtils = NULL;
	MoaMmNativeGCInfo 	gcInfo;

	myCookie = pCallback->MoaBeginUsingResources(gXtraFileRef, &saveCookie);

#ifdef MACINTOSH
	PicHandle 					hPic = NULL;
	GWorldPtr					pGWorld = NULL;
	OSErr						oserr;
	Rect						bounds;
	
	hPic = (PicHandle) GetNamedResource('PICT', "\pInkTest_Image");
	if (hPic) 
	{
		LoadResource((Handle) hPic);
		
#if (defined(__APPLE__) && defined(__MACH__)) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_3)
	    QDGetPictureBounds(hPic, &bounds);
#else
		bounds = (**hPic).picFrame;
#endif
		oserr = NewGWorld(&pGWorld, 8, &bounds, NULL, NULL, kNativeEndianPixMap);
		if (oserr == noErr) 
		{
			CGrafPtr savePort;
			GDHandle saveDev;
			
			GetGWorld(&savePort, &saveDev);
			SetGWorld(pGWorld, NULL);
			
			DrawPicture(hPic, &bounds);
			
			SetGWorld(savePort, saveDev);
		}
		ReleaseResource((Handle) hPic);
		
		/* Make a new GC based on this GWorld */
		if ( (pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&pUtils)) == kMoaErr_NoErr) 
		{
			gcInfo.type = kMoaMmGCType_MacGWorld;
			gcInfo.data.mac_gworld.gworldPtr = pGWorld;
			if ((pUtils->NewGC(&gcInfo, &pImage->pGC)) != kMoaErr_NoErr) pImage->pGC = NULL;
				
			/* Release the Utils interface */
			pUtils->Release();
		}
		
		/* Save off our GWorld too */
		pImage->bitmap = (PMoaVoid) pGWorld;
		
#endif
#ifdef WINDOWS
	HGLOBAL 			hBitmap;
	LPVOID				pBitmap;
	DWORD				size;
	HRSRC 				hRsrc;
	MoaHandle			hSaveBitmap;
	PMoaVoid			pSaveBitmap;
		
	hRsrc = FindResource((HINSTANCE) myCookie, "InkTest_Image", MAKEINTRESOURCE(RT_BITMAP));
	if (hRsrc)
	{
		hBitmap = LoadResource((HINSTANCE) myCookie, hRsrc);
		if (hBitmap)
		{
			PIMoaHandle			pHandle;

			pCallback->QueryInterface(&IID_IMoaHandle, (PPMoaVoid)&pHandle);

			pBitmap = LockResource(hBitmap);
			size = SizeofResource((HINSTANCE) myCookie, hRsrc);

			hSaveBitmap = pHandle->Alloc(size);
			if (hSaveBitmap)
			{ 	
				pSaveBitmap = pHandle->Lock(hSaveBitmap);
	       		_fmemcpy(pSaveBitmap, pBitmap, (size_t) size);
			}

			pHandle->Release();

			UnlockResource(hBitmap);
			FreeResource(hBitmap);
		}
		
		/* Make a new GC based on this DIB */
		if ( (pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&pUtils)) == kMoaErr_NoErr) 
		{
			LPBITMAPINFOHEADER	lpbmi = (LPBITMAPINFOHEADER) pSaveBitmap;
			PMoaChar	 		lpbits;
			WORD    			TableSize, TableUsed;
			DWORD   			header_size;

			gcInfo.type = kMoaMmGCType_WinDIBDC;
			gcInfo.data.win_dibdc.hDC = NULL;	/* App doesn't need DC for DIB blit */
			gcInfo.data.win_dibdc.lpBitMapInfo = lpbmi;

			switch (lpbmi->biBitCount) {
				case 1: TableSize = 2; break;
				case 4: TableSize = 16; break;
				case 8: TableSize = 256; break;
				default: TableSize = 0; break;
			}
			
			if (lpbmi->biClrUsed) TableUsed = (WORD) lpbmi->biClrUsed;
			else TableUsed = TableSize;

			header_size = sizeof(BITMAPINFOHEADER) + TableUsed * sizeof(RGBQUAD);
			lpbits = (char *)lpbmi + header_size;

			gcInfo.data.win_dibdc.lpBits = lpbits;

			if ((pUtils->NewGC(&gcInfo, &pImage->pGC)) != kMoaErr_NoErr) pImage->pGC = NULL;

			/* Release Utils interface */
			pUtils->Release();
		}
		
		/* Save off our bitmap too */
		pImage->bitmap = (PMoaVoid) hSaveBitmap;
		pImage->pbitmap = (PMoaVoid) pSaveBitmap;
		pImage->lpbits = gcInfo.data.win_dibdc.lpBits;
		
#endif
	}
	else
		err = kMoaErr_BadParam;

	pCallback->MoaEndUsingResources(gXtraFileRef, saveCookie);
	
	return err;
}

/* ------------------------------------------- CInkTestAsset::ImageDestroy */
MoaError CInkTestAsset::ImageDestroy(PImageData pImage) {
	/* Wipe out out GC and our GWorld or bitmap */
	if (pImage->pGC) pImage->pGC->Release();

#ifdef MACINTOSH

	if (pImage->bitmap) DisposeGWorld((GWorldPtr) pImage->bitmap);

#endif
#ifdef WINDOWS

	if (pImage->bitmap) 
	{
		PIMoaHandle			pHandle;

		pCallback->QueryInterface(&IID_IMoaHandle, (PPMoaVoid)&pHandle);

		pHandle->Unlock((MoaHandle) pImage->bitmap);
		pHandle->Free((MoaHandle) pImage->bitmap);

		pHandle->Release();
	}

#endif
		
	return kMoaErr_NoErr;
}

/* ------------------------------------------- CInkTestAsset::ImageGetRect */
MoaError CInkTestAsset::ImageGetRect(PImageData pImage, PMoaRect pRect) {
#ifdef MACINTOSH
	Rect bounds;
	
	if (pImage->bitmap) 
	{
		PixMapHandle hPixMap = GetGWorldPixMap((GWorldPtr) pImage->bitmap);
		
		bounds = (**hPixMap).bounds;
		
		/* Center rect about (0,0) */
		OffsetRect(&bounds, -bounds.left, -bounds.top);
		OffsetRect(&bounds, -(bounds.right / 2), -(bounds.bottom / 2));
	} 
	else 
		SetRect(&bounds, 0, 0, 0, 0);

	MacToMoaRect(&bounds, pRect);

#endif
#ifdef WINDOWS
	RECT	bounds;

	if (pImage->pbitmap) 
	{
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER) pImage->pbitmap;
		
		SetRect(&bounds, 0, 0, (short) lpbmi->biWidth, (short) lpbmi->biHeight);
		if (bounds.bottom < 0) bounds.bottom = -bounds.bottom;
		
		/* Center rect about (0,0) */
		OffsetRect(&bounds, -(bounds.right / 2), -(bounds.bottom / 2));
	} 
	else 
		SetRect(&bounds, 0, 0, 0, 0);

	WinToMoaRect(&bounds, pRect);

#endif		
	return kMoaErr_NoErr;
}

/* ------------------------------------------- CInkTestAsset_IMoaMmXAsset::GetProp */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::GetProp(MoaMmSymbol symbol, PMoaMmValue	pPropValue)
{
	UNUSED(symbol);
	UNUSED(pPropValue);
	X_ENTER

	MoaError		err = kMoaMmErr_PropertyNotFound;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------------- CInkTestAsset_IMoaMmXAsset::SetProp */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::SetProp(MoaMmSymbol symbol, ConstPMoaMmValue pPropValue)
{
	UNUSED(symbol);
	UNUSED(pPropValue);
	X_ENTER

	MoaError		err = kMoaMmErr_PropertyNotFound;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* -------------------------------------- CInkTestAsset_IMoaMmXAsset::CallFunction */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::CallFunction(MoaMmSymbol methodName, MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
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

/* --------------------------------------- CInkTestAsset_IMoaMmXAsset::SetCallback */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::SetCallback(PIMoaMmAssetCallback pAssetCallback)
{
	UNUSED(pAssetCallback);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* -------------------------------------------- CInkTestAsset_IMoaMmXAsset::Notify */
/* This method is intended for communicating between the asset and its xtras.
   The developer is free to define message codes for private use.  This may be
   called directly by an actor, or through the
   IMoaMmSpriteCallback::NotifyAsset() callback.
 */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	UNUSED(msgCode);
	UNUSED(refCon);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CInkTestAsset_IMoaMmXAsset::GetDefaultRect */
/* We return our default rectangle centered around (0,0).  (0,0) here
   corresponds to the "loc" of the sprite, whichis the point of reference for
   the sprite.  Dragging the sprite out to the stage will position the sprite
   such that the loc is at the mouse drop position.
 */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::GetDefaultRect(PMoaRect pRect)
{
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;
	
	pObj->ImageGetRect(&pObj->image, pRect);
		
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CInkTestAsset_IMoaMmXAsset::PrepareNewContent */
/* This method is called to have the class set-up its default state when a new
   cast member is created (note that this is different from when a new
   instance of CInkTestAsset is created; instances of the XAsset are created at both
   movie-open time (for pre-existing cast members being read-in) and new cast
   member creation time.  PrepareNewContent() is only called at new cast
   member creation time, just after the XAsset instance is created. 
 */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::PrepareNewContent(MoaMmNewFlags newFlags)
{
	UNUSED(newFlags);
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;
	
	/* A new cast member is being created in memory.  Set up default media and
	   props. */

	/* Dispose of any media and props we may have currently in memory */
	CInkTestAsset_IMoaMmXAsset::FreeMedia();
	CInkTestAsset_IMoaMmXAsset::FreeProps();
	
	/* Initialize props / media to default values here. 
	   The host app assumes that both props and media are "dirty" when a new
	   cast member is created. 
	
	   We are now "loaded" in the sense that we have everything we need from 
	   disk (there isn't anything on disk yet, so we exist only in-memory 
	   at this point). */

	X_STD_RETURN(err);
	X_EXIT
}


/* ------------------------------------ CInkTestAsset_IMoaMmXAsset::StreamOutMedia */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::StreamOutMedia(PIMoaStream pOutStream)
{
	UNUSED(pOutStream);
	X_ENTER
	
	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------- CInkTestAsset_IMoaMmXAsset::StreamInMedia */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::StreamInMedia(PIMoaStream pInStream)
{
	UNUSED(pInStream);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------- CInkTestAsset_IMoaMmXAsset::FreeMedia */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::FreeMedia()
{
	X_ENTER

	MoaError	err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CInkTestAsset_IMoaMmXAsset::GetInMemMediaSize */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::GetInMemMediaSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CInkTestAsset_IMoaMmXAsset::GetStreamOutMediaSize */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::GetStreamOutMediaSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CInkTestAsset_IMoaMmXAsset::StreamOutProps */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::StreamOutProps(PIMoaStream pOutStream)
{
	UNUSED(pOutStream);
	X_ENTER
	
	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------- CInkTestAsset_IMoaMmXAsset::StreamInProps */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::StreamInProps(PIMoaStream pInStream)
{
	UNUSED(pInStream);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------- CInkTestAsset_IMoaMmXAsset::FreeProps */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::FreeProps()
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CInkTestAsset_IMoaMmXAsset::GetInMemPropsSize */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::GetInMemPropsSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CInkTestAsset_IMoaMmXAsset::GetStreamOutPropsSize */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::GetStreamOutPropsSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------- CInkTestAsset_IMoaMmXAsset::NewXActor */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::NewXActor(PMoaVoid * ppXActor) 
{
	X_ENTER

	MoaError				err = kMoaErr_NoErr;
	PIMoaMmXAsset 			pSelf = NULL;	
	PIMoaMmXSpriteActor		pNewActor = NULL;
	
	/* Do a createinstance using IMoaMmXSpriteActor for return to caller... */
	if ((err = pObj->pCallback->MoaCreateInstance(
		&CLSID_CInkTestSprite, &IID_IMoaMmXSpriteActor, (PPMoaVoid)&pNewActor)) != kMoaErr_NoErr)
		goto exit_gracefully;;

	pSelf = (PIMoaMmXAsset) this;
			
	/* Tell actor who its parent is, in case it's interested. 
		It should AddRef() on the interface if it wants to hold onto it.
	*/
		
	(pNewActor)->Notify(kSprNotifyMsg_SetParentAsset, pSelf);
				
	/* Give actor ptr to our image data */
	(pNewActor)->Notify(kSprNotifyMsg_SetImage, &pObj->image);

	*ppXActor = pNewActor;
	
exit_gracefully:

	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CInkTestAsset_IMoaMmXAsset::InvokeMediaEditor */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::InvokeMediaEditor(PMoaMmModifyFlags pModifyFlags)
{
	UNUSED(pModifyFlags);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CInkTestAsset_IMoaMmXAsset::InvokePropsEditor */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::InvokePropsEditor()
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CInkTestAsset_IMoaMmXAsset::FinishPropsEditor */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::FinishPropsEditor(MoaBoolParam commitChanges,					
	PMoaMmModifyFlags pModifyFlags)
{
	UNUSED(commitChanges);
	UNUSED(pModifyFlags);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------- CInkTestAsset_IMoaMmXAsset_InvokeAboutBox */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::InvokeAboutBox()
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ---------------------------------- CInkTestAsset_IMoaMmXAsset::ImageStaticThumb */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::ImageStaticThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	X_ENTER

	MoaError err = kMoaMmErr_NotImplemented;

	/*  We don't support this capability; Director should never call this, */
	/*  because it wasn't specified in the assetCaps (above). */
	
	X_RETURN(MoaError, err);
	X_EXIT
}

/* ------------------------------- CInkTestAsset_IMoaMmXAsset::BeginImageAnimThumb */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::BeginImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	UNUSED(pGC);
	UNUSED(pImageRect);
	X_ENTER

	MoaError		err = kMoaMmErr_NotImplemented;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CInkTestAsset_IMoaMmXAsset::ServiceImageAnimThumb */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::ServiceImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	UNUSED(pGC);
	UNUSED(pImageRect);
	X_ENTER

	MoaError		err = kMoaMmErr_NotImplemented;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CInkTestAsset_IMoaMmXAsset::EndImageAnimThumb */
STDMETHODIMP CInkTestAsset_IMoaMmXAsset::EndImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	UNUSED(pGC);
	UNUSED(pImageRect);
	X_ENTER

	MoaError		err = kMoaMmErr_NotImplemented;
	
	X_STD_RETURN(err);
	X_EXIT
}

#ifdef USING_INIT_FROM_DICT
/* ---------------------------------- CInkTestAsset_IMoaInitFromDict::InitFromDict */
STDMETHODIMP CInkTestAsset_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
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
/* ---------------------------------- CInkTestAsset_IMoaNotificationClient::Notify */
STDMETHODIMP CInkTestAsset_IMoaNotificationClient::Notify(ConstPMoaNotifyID nid, PMoaVoid pNData, PMoaVoid pRefCon)
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

