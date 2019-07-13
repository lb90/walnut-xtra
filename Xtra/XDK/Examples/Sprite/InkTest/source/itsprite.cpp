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
 *	Name: itsprite.cpp
 *	
 * 	Purpose: Definitions of Sprite class interface(s) and methods for the 
 *           InkTest Sprite Xtra.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "ITSprite.h"

#ifdef MACINTOSH
	#include <QuickDraw.h>
	#include <Resources.h>
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
BEGIN_DEFINE_CLASS_INTERFACE(CInkTestSprite, IMoaMmXSpriteActor)
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

/* ------------------------------------------------------ MoaCreate_CInkTestSprite */
STDMETHODIMP MoaCreate_CInkTestSprite(CInkTestSprite * This)
{
	X_ENTER

	MoaError err = kMoaErr_NoErr;
	
	// Initialize instance variables
	This->pXAsset = NULL;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------------------- MoaDestroy_CInkTestSprite */
STDMETHODIMP_(void) MoaDestroy_CInkTestSprite(CInkTestSprite * This)
{
	X_ENTER

	/* Release interfaces we've been hanging on to. */
	if (This->pXAsset) This->pXAsset->Release();
	
	X_RETURN_VOID;
	X_EXIT
}

/* ------------------------------ CInkTestSprite_IMoaMmXSpriteActor Create/Destroy */
CInkTestSprite_IMoaMmXSpriteActor::CInkTestSprite_IMoaMmXSpriteActor(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CInkTestSprite_IMoaMmXSpriteActor::~CInkTestSprite_IMoaMmXSpriteActor() {}

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

/* ------------------------------------ CInkTestSprite_IMoaMmXSpriteActor::GetProp */
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::GetProp(MoaMmSymbol symbol, PMoaMmValue pPropValue)
{
	X_ENTER
	
	MoaError	err;

	err = kMoaMmErr_PropertyNotFound;

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CInkTestSprite_IMoaMmXSpriteActor::SetProp */
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::SetProp(MoaMmSymbol symbol,
	ConstPMoaMmValue pPropValue)
{
	X_ENTER
	
	MoaError	err;
	
	err = kMoaMmErr_PropertyNotFound;

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------- CInkTestSprite_IMoaMmXSpriteActor::CallFunction */
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::CallFunction(MoaMmSymbol methodName, MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{
	X_ENTER
	
	MoaError	err;
	
	err = kMoaMmErr_FunctionNotFound;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* -------------------------------- CInkTestSprite_IMoaMmXSpriteActor::SetCallback */
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::SetCallback(PIMoaMmSpriteCallback pSpriteCallback)
{
	X_ENTER

	MoaError	err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------- CInkTestSprite_IMoaMmXSpriteActor::Notify */
/* This method is intended for communicating between the asset and its xtras.
   The developer is free to define message codes for private use.  This may be
   called directly by an asset, or through the 
   IMoaMmAssetCallback::NotifyAllSprites() callback. */
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	X_ENTER

	MoaError	err = kMoaErr_NoErr;

	switch (msgCode) 
	{
		case kSprNotifyMsg_SetParentAsset:
			pObj->pXAsset = (PIMoaMmXAsset) refCon;
			pObj->pXAsset->AddRef();	
			/* up ref count since we're hanging on to this	*/
			break;
		case kSprNotifyMsg_SetImage:
			pObj->pImage = (PImageData) refCon;
			break;
		default:
			break;		
	}

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CInkTestSprite_IMoaMmXSpriteActor::GetCaps */
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::GetCaps(PMoaMmXSpriteCaps pActorCaps)
{
	X_ENTER

	MoaError	err = kMoaErr_NoErr;

	/* We don't want any events */
	pActorCaps->eventMask = (kMoaMmEventMask_None);

	// We support no special actor capabilities
	//use kMoaMmXSpriteCapsFlags_AutoTabEnabled for autoTab support
	pActorCaps->flags = 	kMoaMmXSpriteCapsFlags_None;
	
	// We support either offscreen or direct-to-screen imaging,
	// but we're intended for offscreen imaging (the normal case).
	pActorCaps->requiredImageMode = kMoaMmXSpriteImageMode_Any;
	pActorCaps->preferredImageMode = kMoaMmXSpriteImageMode_Offscreen;
	
	// Add these to support all the new Director goodies...
	pActorCaps->flags |= 
			kMoaMmXSpriteCapsFlags_SupportsRotation 
			| kMoaMmXSpriteCapsFlags_SupportsSkew	
			| kMoaMmXSpriteCapsFlags_SupportsFlipH 
			| kMoaMmXSpriteCapsFlags_SupportsFlipV
			| kMoaMmXSpriteCapsFlags_SupportsQuad 
			| kMoaMmXSpriteCapsFlags_SupportsAutoBounds
			;

	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CInkTestSprite_IMoaMmXSpriteActor::CollectChanges */
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
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::CollectChanges(ConstPMoaMmXSpriteInfo pActorInfo, PIMoaMmChangeCollector pChangeCollector)
{
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;

	pChangeCollector->AddChangeRect(&pActorInfo->spriteRect);

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------ CInkTestSprite_IMoaMmXSpriteActor::ContainsPoint */
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::ContainsPoint(ConstPMoaMmXSpriteInfo pActorInfo, ConstPMoaPoint pTestPoint, MoaBoolParam * pTestResult)
{
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;

	// Anywhere within our sprite rect is a hit.
	*pTestResult = TRUE;
	
	X_STD_RETURN(err);
	X_EXIT
}


#define PENSIZE 2

/* -------------------------------------- CInkTestSprite_IMoaMmXSpriteActor::Image */
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::Image(ConstPMoaMmXSpriteInfo pActorInfo, ConstPMoaMmPlayerStateInfo pPlayerStateInfo, PIMoaMmGC pGC)
{
	X_ENTER	
	
	MoaError				err = kMoaErr_NoErr;
	MoaRect 				sourceRect;
	MoaMmNativeGCInfo		nativeInfo;

	sourceRect.top = 0;
	sourceRect.left = 0;
	sourceRect.right = 0;
	sourceRect.bottom = 0;

#ifdef MACINTOSH
	pObj->pImage->pGC->GetContentRect(&sourceRect);
	
#endif
#ifdef WINDOWS
	RECT winRect;
	
	/* Get source rect for blit */
	if (pObj->pImage->pbitmap) 
	{
		LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER) pObj->pImage->pbitmap;
		
		SetRect(&winRect, 0, 0, (short) lpbmi->biWidth, (short) lpbmi->biHeight);
		if (winRect.bottom < 0) winRect.bottom = -winRect.bottom;
		
		/* Center rect about (0,0) */
		OffsetRect(&winRect, -(winRect.right / 2), -(winRect.bottom / 2));
	} 
	else 
		SetRect(&winRect, sourceRect.left, sourceRect.top, sourceRect.right, sourceRect.bottom);
	
	OffsetRect(&winRect, -winRect.left, -winRect.top);

	WinToMoaRect(&winRect, &sourceRect);
#endif
	
	// Use new Blit2 interface if its available...
	{
		PIMoaMmGC2	pGC2;
		err = pGC->QueryInterface(&IID_IMoaMmGC2,(PPMoaVoid)&pGC2);

		/* If we find our new Director interface, use it,
		/  otherwise, use the D6 one. */
		if (err == kMoaErr_NoErr)
		{
			err = pGC2->Blit2((PMoaRect) &pActorInfo->spriteRect,
								pObj->pImage->pGC,
								&sourceRect, 
								pActorInfo->ink, 
								(PMoaMmInkParams) &pActorInfo->inkParams,   
								(PMoaMmBlitParams) &pActorInfo->blitParams,
								NULL,
								NULL
								);	// nativeClip

			pGC2->Release();
		}
		else
		{	
			err = pGC->Blit((PMoaRect) &pActorInfo->spriteRect, 
							pObj->pImage->pGC,
							&sourceRect, 
							pActorInfo->ink,
							(PMoaMmInkParams) &pActorInfo->inkParams, 
							NULL /* nativeClip */);
		}				
	}

	pGC->GetNativeGCInfo(&nativeInfo);
	
	X_STD_RETURN(err);
	X_EXIT
}

/* -------------------------------------- CInkTestSprite_IMoaMmXSpriteActor::Event */
STDMETHODIMP CInkTestSprite_IMoaMmXSpriteActor::Event(ConstPMoaMmEventInfo pEventInfo, ConstPMoaMmPlayerStateInfo pPlayerStateInfo, MoaBoolParam * pHandled)
{
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;

	*pHandled = FALSE;

	X_STD_RETURN(err);
	X_EXIT
}

