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
 *	Name: TWTrans.cpp
 *	
 * 	Purpose: Definitions of Transition class interface(s) and methods for the 
 *           TallWide Transition Xtra.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "TWTrans.h"

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
BEGIN_DEFINE_CLASS_INTERFACE(CTallWideTransition, IMoaMmXTransitionActor)
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

/* ------------------------------------------------------ MoaCreate_CTallWideTransition */
STDMETHODIMP MoaCreate_CTallWideTransition(CTallWideTransition * This)
{
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;

	/* Nothing to do */
		
	X_RETURN(MoaError, err);
	X_EXIT
}

/* ----------------------------------------------------- MoaDestroy_CTallWideTransition */
STDMETHODIMP_(void) MoaDestroy_CTallWideTransition(CTallWideTransition * This)
{
	X_ENTER
	
	/* Nothing to do */

	X_RETURN_VOID;
	X_EXIT
}

/* ------------------------------ CTallWideTransition_IMoaMmXTransitionActor Create/Destroy */
CTallWideTransition_IMoaMmXTransitionActor::CTallWideTransition_IMoaMmXTransitionActor(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CTallWideTransition_IMoaMmXTransitionActor::~CTallWideTransition_IMoaMmXTransitionActor() {}

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

/* ---------------- CTallWideTransition_IMoaMmXTransitionActor::Notify */
STDMETHODIMP CTallWideTransition_IMoaMmXTransitionActor::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	X_ENTER

	/* This method is intended for communicating between the
		asset and its xtras.  The developer is free to define
		message codes for private use.  This may be called
		directly by an asset, or through the 
		IMoaMmAssetCallback::NotifyAllSprites() callback.
	*/

	switch (msgCode) 
	{
		case kSprNotifyMsg_SendProps:
			/*  This puts a pointer to all the CTallWideAsset instance variables  */
			/* into a local instance variable (pSharedAssetData) */
			pObj->pSharedAssetData = (PSharedAssetData) refCon;	
			break;

		default:
			break;		
	}

	X_RETURN(MoaError, kMoaErr_NoErr);
	X_EXIT
}


/* ---------------- CTallWideTransition_IMoaMmXTransitionActor::Cue */
STDMETHODIMP CTallWideTransition_IMoaMmXTransitionActor::Cue(PIMoaMmGC pDestGC,
			PIMoaMmGC 				pSrcGC,
			ConstPMoaRect			pRect,
			ConstPMoaMmTransInfo	pTransInfo)
{
	X_ENTER
	MoaError err = kMoaErr_NoErr;
	
	/* Initialize the transition: Do all the setup calculations. */
	
	short		centerX, centerY;
	
  	/* Find the center of the transition area */
	centerX = (short)((pRect->right - pRect->left)/2. + pRect->left);
	centerY = (short)((pRect->bottom - pRect->top)/2. + pRect->top);
	
	/* Setup the intial rect to begin the transition from */
	pObj->curRect.right = centerX + pObj->pSharedAssetData->initialWidth/2;
	pObj->curRect.left = centerX - pObj->pSharedAssetData->initialWidth/2;
	pObj->curRect.bottom = centerY + pObj->pSharedAssetData->initialHeight/2;
	pObj->curRect.top = centerY - pObj->pSharedAssetData->initialHeight/2;

	X_RETURN(MoaError, err);
	X_EXIT
}
	
/* ---------------- CTallWideTransition_IMoaMmXTransitionActor::Continue */
STDMETHODIMP CTallWideTransition_IMoaMmXTransitionActor::Continue(PIMoaMmGC pDestGC,
			PIMoaMmGC 				pSrcGC,
			ConstPMoaRect			pRect,
			ConstPMoaMmTransInfo	pTransInfo,
			MoaBoolParam * 			pFinished)
{
	X_ENTER

    MoaError    err = kMoaErr_NoErr;
    
	*pFinished = FALSE;  

    /* If the curRect is still not as tall as the (final) pRect, then make it taller. */
	if (pObj->curRect.bottom < pRect->bottom) 
	{
	
		/*  This expands the box vertically.  Inside this narrow band  */
		/*  is the squished image of the entire next frame. */
		pDestGC->Blit(
			&(pObj->curRect),
			pSrcGC,
			pRect,
			kMoaMmInk_Copy,
			NULL /* ink Params */,
			NULL /* clipRegion */);
			
		/* Make the curRect taller. */ 
		pObj->curRect.bottom += pObj->pSharedAssetData->verticalSpeed;
		pObj->curRect.top -= pObj->pSharedAssetData->verticalSpeed;
	}
	
	/* Since the curRect is as tall as the (final) prect, we make it wider instead. */
	else 
	{
	
		/* Make sure the top and bottom of the curRect match the prect. */
		pObj->curRect.bottom = pRect->bottom;
		pObj->curRect.top = pRect->top;
	
		/* If the curRect is still not as wide as the (final) pRect, then make it wider. */
		if (pObj->curRect.left > pRect->left) 
		{
		
			/*  This takes the narrow band and expands it to the right and left */
			/*  until the transition is finished.  The entire time this is happening,  */
			/* the expanding narrow vertical band includes the ENTIRE next frame */
			pDestGC->Blit(
				&(pObj->curRect),
				pSrcGC,
				pRect,
				kMoaMmInk_Copy,
				NULL /* ink Params */,
				NULL /* clipRegion */);
	
			/* Make the curRect wider. */
			pObj->curRect.right += pObj->pSharedAssetData->horizontalSpeed;
			pObj->curRect.left -= pObj->pSharedAssetData->horizontalSpeed;
		}
		
		else 
		{
			/* now we're done! */ 
			*pFinished = TRUE;  

		}
		

	} 
	

	X_RETURN(MoaError, err);
	X_EXIT
}
	
/* ---------------- CTallWideTransition_IMoaMmXTransitionActor::Finish */
STDMETHODIMP CTallWideTransition_IMoaMmXTransitionActor::Finish(PIMoaMmGC pDestGC,
			PIMoaMmGC 				pSrcGC,
			ConstPMoaRect 			pRect,
			ConstPMoaMmTransInfo	pTransInfo)
{
	X_ENTER
	MoaError	err = kMoaErr_NoErr;

	/* finish it off. Make sure the final image is in there */ 
			pDestGC->Blit(
				pRect,
				pSrcGC,
				pRect,
				kMoaMmInk_Copy,
				NULL /* ink Params */,
				NULL /* clipRegion */);

	X_RETURN(MoaError, err);
	X_EXIT
}

