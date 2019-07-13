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
 *	Name: PlaySoundSprite.cpp
 *	
 * 	Purpose: Definitions of Sprite class interface(s) and methods for the 
 *           PlaySound Xtra project.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "PlaySoundSprite.h"

#ifdef MACINTOSH
	#include <QuickDraw.h>
	#include <Resources.h>
#endif

#include <String.h>
#include <ctype.h>

#ifdef WINDOWS
	#include "resource.h"
#endif

#include "MoaTry.h"
 
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
BEGIN_DEFINE_CLASS_INTERFACE(CPlaySoundSprite, IMoaMmXSpriteActor)

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

/* ------------------------------------------------------ MoaCreate_CPlaySoundSprite */
STDMETHODIMP MoaCreate_CPlaySoundSprite(CPlaySoundSprite * This)
{
moa_try
	This->pStrm = NULL;	
	This->cid = 0;
	This->playing = false;
	This->pSpriteCallback = NULL;
	This->pFile = NULL;
	
	//Save off the sound interface
	ThrowErr(This->pCallback->QueryInterface(&IID_IMoaDrSound,(PPMoaVoid)&This->pSnd));
	ThrowNull(This->pSnd);
	
moa_catch
moa_catch_end

moa_try_end
}

/* ----------------------------------------------------- MoaDestroy_CPlaySoundSprite */
STDMETHODIMP_(void) MoaDestroy_CPlaySoundSprite(CPlaySoundSprite * This)
{	
	// Clean up...
	
	if (This->pSnd) This->pSnd->Release();
	This->pSnd = NULL;
	
	if (This->pSpriteCallback) This->pSpriteCallback->Release();
	This->pSpriteCallback = NULL;
	
	This->pFile = NULL;
	
	return;
}

/* ------------------------------ SoundSprite_IMoaMmXSpriteActor Create/Destroy */
CPlaySoundSprite_IMoaMmXSpriteActor::CPlaySoundSprite_IMoaMmXSpriteActor(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CPlaySoundSprite_IMoaMmXSpriteActor::~CPlaySoundSprite_IMoaMmXSpriteActor() 
{
	//Stop a sound if it's still playing
	_Stop();
}


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

/* ------------------------------------ CPlaySoundSprite_IMoaMmXSpriteActor::GetProp */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::GetProp(MoaMmSymbol symbol, PMoaMmValue pPropValue)
{
	MoaError err;
	
	err = kMoaMmErr_PropertyNotFound;
	
	return(err);
}

/* ------------------------------------ CPlaySoundSprite_IMoaMmXSpriteActor::SetProp */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::SetProp(MoaMmSymbol symbol,
	ConstPMoaMmValue pPropValue)
{
	MoaError	err;
	
	err = kMoaMmErr_PropertyNotFound;

	return(err);
}

/* ------------------------------- CPlaySoundSprite_IMoaMmXSpriteActor::CallFunction */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::CallFunction(MoaMmSymbol methodName, MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{
	
	/* variable declarations */
	MoaError	err;

	err = kMoaMmErr_FunctionNotFound;
	
	return(err);
}

/* -------------------------------- CPlaySoundSprite_IMoaMmXSpriteActor::SetCallback */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::SetCallback(PIMoaMmSpriteCallback pSpriteCallback)
{
moa_try
	
	pObj->pSpriteCallback = pSpriteCallback;
	ThrowErr(pObj->pSpriteCallback->AddRef());
	
moa_catch
moa_catch_end

moa_try_end
}

/* ------------------------------------- CPlaySoundSprite_IMoaMmXSpriteActor::Notify */
/* This method is intended for communicating between the asset and its xtras.
   The developer is free to define message codes for private use.  This may be
   called directly by an asset, or through the 
   IMoaMmAssetCallback::NotifyAllSprites() callback. */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	MoaError	err = kMoaErr_NoErr;

	if (msgCode == kSpriteMsg_FileName) 
		pObj->pFile = (PIMoaFile2)refCon;
		
	return(err);
}

/* ------------------------------------ CPlaySoundSprite_IMoaMmXSpriteActor::GetCaps */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::GetCaps(PMoaMmXSpriteCaps pActorCaps)
{
	MoaError	err = kMoaErr_NoErr;

//  We only want play, span events
	pActorCaps->eventMask = (kMoaMmEventMask_Step | 
							kMoaMmEventMask_Play |
							kMoaMmEventMask_Span );

//  We support no special actor capabilities
	
	pActorCaps->flags = 	kMoaMmXSpriteCapsFlags_None;
	
//  We support either offscreen or direct-to-screen imaging,
//		but we're intended for offscreen imaging (the normal case).
	
	pActorCaps->requiredImageMode = kMoaMmXSpriteImageMode_Any;
	pActorCaps->preferredImageMode = kMoaMmXSpriteImageMode_Offscreen;
	
	return(err);
}

/* ----------------------------- CPlaySoundSprite_IMoaMmXSpriteActor::CollectChanges */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::CollectChanges(ConstPMoaMmXSpriteInfo pActorInfo, PIMoaMmChangeCollector pChangeCollector)
{
	MoaError	err = kMoaErr_NoErr;
	
	return(err);
}

/* ------------------------------ CPlaySoundSprite_IMoaMmXSpriteActor::ContainsPoint */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::ContainsPoint(ConstPMoaMmXSpriteInfo pActorInfo, ConstPMoaPoint pTestPoint, MoaBoolParam * pTestResult)
{
	MoaError	err = kMoaErr_NoErr;

	*pTestResult = TRUE;
	
	return(err);
}


/* -------------------------------------- CPlaySoundSprite_IMoaMmXSpriteActor::Image */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::Image(ConstPMoaMmXSpriteInfo pActorInfo,	ConstPMoaMmPlayerStateInfo	pPlayerStateInfo, PIMoaMmGC pGC)
{	
moa_try

	PMoaRect			pImageRect = (PMoaRect) &pActorInfo->spriteRect;
	MoaMmNativeGCInfo	nativeGC;
	XtraResourceCookie 	myCookie, saveCookie;

	myCookie = pObj->pCallback->MoaBeginUsingResources(gXtraFileRef, &saveCookie);
	ThrowErr(pGC->GetNativeGCInfo(&nativeGC));

#ifdef MACINTOSH
	{
		const Str255		Image = "\pImage";
		GrafPtr				savePort, drawPort;
		PenState			savePen;
		RGBColor			saveColor;
		RGBColor			myColor = {65535, 0, 0};	/* pure red */
		PicHandle 			hPic;
		Rect				macRect;

		MoaToMacRect(pImageRect, &macRect);
		// Determine what port to draw into based on the
		// supplied Graphics Context.  The host app sets up
		// this context for us.

		switch (nativeGC.type)
		{
			case kMoaMmGCType_MacWindow:
				drawPort = (GrafPtr) nativeGC.data.mac_window.windowPtr;
				break;
			case kMoaMmGCType_MacGWorld:
				drawPort = (GrafPtr) nativeGC.data.mac_gworld.gworldPtr;
				break;
			default:
				// we don't know about this type of graphics context; bail out...
				Throw(kMoaMmErr_GCNotAvailable);
		}
			
		GetPort(&savePort);
		SetPort( drawPort );
	
		// We could obey the sprite color supplied in pActorInfo here,
		// but instead we always draw using myColor.
	
		GetForeColor(&saveColor);
		RGBForeColor(&myColor);
		GetPenState(&savePen);
		PenNormal();
		
		hPic = (PicHandle)GetNamedResource('PICT',Image);
		DrawPicture((PicHandle)hPic, &macRect);
	
	
		// Restore our drawing state and port. */
	
		RGBForeColor(&saveColor);
	
		SetPenState(&savePen);
		SetPort(savePort);
	}
#endif	/* MACINTOSH */

#ifdef WINDOWS
	{
		HDC			drawDC;
		HPEN		hOldPen;
		HBRUSH		hOldBrush;
		HDC			hdcMem1;
		BITMAP 		bm1;
		HBITMAP		hBitMap1;
        
		/*
		   Determine what port to draw into based on the
		   supplied Graphics Context.  The host app sets up
		   this context for us; if we're Direct To Stage or
		   trails ink is applied, the context is that of
		   the stage window; otherwise it is that of the stage
		   offscreen buffer.
		*/

    switch (nativeGC.type)
    {
		case kMoaMmGCType_WinDIBDC:
		case kMoaMmGCType_WinDDBDC:
		case kMoaMmGCType_WinWindow:
			drawDC = (HDC)nativeGC.data.win_window.hDC;
			break;
        default:
            /* we don't know about this type of graphics context; bail out. */
            Throw(kMoaMmErr_GCNotAvailable);
    }
                
 
    //We could obey the sprite color supplied in pActorInfo here,
    // but instead we always draw using a fixed color.

	hOldPen = (HPEN) SelectObject(drawDC, GetStockObject(NULL_PEN));
	hOldBrush = (HBRUSH) SelectObject(drawDC, GetStockObject(NULL_BRUSH));

	/* Load bitmap resource */
	hBitMap1 = LoadBitmap((HINSTANCE)myCookie, MAKEINTRESOURCE(REHEAR));

	if(hBitMap1 != 0)
	{
		/* Create another graphics context equivalent to DrawGC */
 		hdcMem1 = CreateCompatibleDC (drawDC);

		/*Get information about hBitmap1 */
		GetObject(hBitMap1, sizeof(BITMAP), (LPVOID) &bm1);
	
		/* Select it */
		SelectObject(hdcMem1, hBitMap1);
	
		/*Draw loaded bitmap into the DrawDC graphics context at a new size			 	 */
		StretchBlt(drawDC, pImageRect->left, pImageRect->top, pImageRect->right - pImageRect->left, pImageRect->bottom - pImageRect->top,
					hdcMem1, 0, 0, bm1.bmWidth, bm1.bmHeight,  SRCCOPY);

		DeleteDC(hdcMem1);
    }
            

	/*Restore brush and pen */
    SelectObject(drawDC, hOldBrush);
    SelectObject(drawDC, hOldPen);
	DeleteObject(hBitMap1);

	}
#endif /* WINDOWS */

moa_catch
moa_catch_end
	pObj->pCallback->MoaEndUsingResources(gXtraFileRef, saveCookie);
moa_try_end
}

/* -------------------------------------- CPlaySoundSprite_IMoaMmXSpriteActor::Event */
STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::Event(ConstPMoaMmEventInfo pEventInfo, ConstPMoaMmPlayerStateInfo pPlayerStateInfo, MoaBoolParam * pHandled)
{
moa_try

	switch (pEventInfo->type)
	{
		case kMoaMmEvent_SpanBegin:
			// if it's off, turn it on at the begin_span
			if (pPlayerStateInfo->playState == kMoaMmPlayState_Playing)
				_Play();
			*pHandled = kMoaMmSprEvent_Handled;
		break;		
		case kMoaMmEvent_SpanEnd:
			// if it's on, turn it off at the end_span
			if (pPlayerStateInfo->playState == kMoaMmPlayState_Playing)
				_Stop();
			*pHandled = kMoaMmSprEvent_Handled;
		break;
		case kMoaMmEvent_PlayBegin:
			// if movie begins and there is a sprite in that frame, need to play the sound here, 
			// due to a lack of spanbegin at the movie start
			_Play();
			*pHandled = kMoaMmSprEvent_Handled;
		break;
		case kMoaMmEvent_PlayEnd:
		//Same as above, only switched	
			_Stop();
			*pHandled = kMoaMmSprEvent_Handled;
		break;
		default:
			*pHandled = kMoaMmSprEvent_Pass;
		break;			
	}

moa_catch
	*pHandled = kMoaMmSprEvent_Handled;
moa_catch_end
	
moa_try_end
}

STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::_GetStream()
{
	MoaError 	err = kMoaErr_NoErr;
	
	//Bail if stream exists
	if (pObj->pStrm)
		return(kMoaErr_NoErr);

	// Get a stream to the file
	err = pObj->pFile->GetStream(1024, &pObj->pStrm);

	//stream is returned in the pObj pStrm member
	return (err);
}

STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::_Play()
{
	PIMoaDrSpriteCallback 	pDrSpriteCbk = NULL;
	MoaDrSpriteChanIndex 	spriteChanIndex = 1;
	PIMoaDrValue    		pIMoaDrValue = NULL;
	PIMoaMmValue			pIMoaMmValue = NULL;
	MoaMmValue				proxy = {0};		
	MoaDrSpriteRef			spriteRef;
	PIMoaPathName			pPathName;
	char buf[12];			// hold the file extension
	char filetype[30];		// hold the symbol for the file type
	char *p;
moa_try
	
	// If it's already playing, bail
	if (pObj->playing) Throw(kMoaErr_NoErr);

	//Gets the stream into pObj->pStrm
	ThrowErr(_GetStream());
	
	//Check for a sound channel
	if (pObj->cid == 0) 
	{
		// get a new sound channel, starting at the top
		ThrowErr(pObj->pSnd->GetFreeSoundChannel(&pObj->cid));
	}
	
	// Use the Sprite callback to get a DrSpriteCallback -- which is used to get the channel the sprite is in...
	ThrowErr(pObj->pSpriteCallback->QueryInterface(&IID_IMoaDrSpriteCallback, (PPMoaVoid)&pDrSpriteCbk));
	ThrowNull(pDrSpriteCbk);
	
	ThrowErr(pDrSpriteCbk->GetSpriteChanIndex(&spriteChanIndex));
	SpriteRef_Make(&spriteRef, (MoaShort)spriteChanIndex);
	
	ThrowErr(pObj->pCallback->QueryInterface(&IID_IMoaDrValue, (PPMoaVoid)&pIMoaDrValue));
	ThrowNull(pIMoaDrValue);
	
	// this will be used later to release the value obtained below...
	ThrowErr(pObj->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid)&pIMoaMmValue));
	ThrowNull(pIMoaMmValue);
	
	ThrowErr(pIMoaDrValue->SpriteRefToValue(&spriteRef, &proxy));
	
	// very primitive file type extractor (use the file extension to determine if its a wav, aiff or swa)...
	pObj->pFile->GetPathName(&pPathName);	
	pPathName->GetExtension( buf, sizeof(buf));
	if (strlen(buf) <= 0)
		Throw(kMoaErr_BadParam);
	
	p = buf;
	while (*p)
	{
		*p = (char)tolower(*p);
		p++;
	}
	
	if (strstr(buf, "aif"))
		strcpy(filetype, "kMoaCfFormat_AIFF");
	else if (strstr(buf, "wav"))
		strcpy(filetype, "kMoaCfFormat_WAVE");
	else if (strstr(buf, "swa"))
		strcpy(filetype, "kMoaCfFormat_SWA");
	else
		return (kMoaErr_BadParam); 
		
	//play with your stream, in the cid channel as an AIFF format
	ThrowErr(pObj->pSnd->PlaySoundFormat(pObj->cid, (PIMoaStream)pObj->pStrm, filetype, &proxy, NULL));
	
	// if no errors, set playing...
	pObj->playing = true;

moa_catch
	pObj->playing = false;
moa_catch_end

	if (pIMoaMmValue)
	{
		pIMoaMmValue->ValueRelease(&proxy);
		pIMoaMmValue->Release();
	}
	pIMoaMmValue = NULL;
	
	if (pIMoaDrValue)
		pIMoaDrValue->Release();
	pIMoaDrValue = NULL;

	if (pPathName)
		pPathName->Release();
	pPathName = NULL;
	
	if (pDrSpriteCbk)
		pDrSpriteCbk->Release();
	pDrSpriteCbk = NULL;
		
moa_try_end
}

STDMETHODIMP CPlaySoundSprite_IMoaMmXSpriteActor::_Stop()
{
moa_try
	//If it's not already playing, return
	if (!pObj->playing)
		Throw(kMoaErr_NoErr);
	
	//set it to not playing
	pObj->playing = false;
	
	// check for bogus channel and bail
	if (pObj->cid == 0)
		Throw(kMoaErr_BadParam);
	
	//Kill the sound in the cid channel
	ThrowErr(pObj->pSnd->StopSoundChannel(pObj->cid, NULL));
	
moa_catch
moa_catch_end	
	if (pObj->pStrm)
	{
		pObj->pStrm->Release();
		pObj->pStrm = NULL;
	}
	
moa_try_end
}