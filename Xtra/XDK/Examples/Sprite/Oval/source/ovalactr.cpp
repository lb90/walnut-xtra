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
 *	Name: ovalactr.cpp
 *	
 * 	Purpose: Definitions of Actor class interface(s) and methods for the 
 *           Sprite Oval Xtra example.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed for 
 *  this particular class. In addition, this file may include other .h files 
 *  defining additional callback interfaces for use by a third party.   
 *****************************************************************************/ 
#include "ovalactr.h"
#include "ovalasst.h"

#ifdef MACINTOSH
#include <QuickDraw.h>
#endif /* /*  MACINTOSH /* */

#include <String.h>
  
/*****************************************************************************
 *  FILE SPECIFIC DEFINE(S)
 *  -----------------------
 *	Any #define's specific to this file.  
 *****************************************************************************/ 
/* These correspond to the two different visual states of our  
   actor:  an animating oval or a static "X". */
#define kMyActorState_Oval	0
#define kMyActorState_X		1

/* Sprite notification messages */ 
enum
{
	kSprNotifyMsg_SetParentAsset = 1
};

/*****************************************************************************
 *  CLASS INTERFACE(S)
 *  ------------------
 *  The interface(s) implemented by your MOA class are specified here.  Note that
 *  at least one class in your Xtra should implement the IMoaRegister interface.
 *  NOTE: Because C++ does not use a lpVtbl to reference an interface's methods, 
 *  the actual method declaration is done in the .h file.
 *
 *  Syntax:
 *  BEGIN_DEFINE_CLASS_INTERFACE(<class-name>, <interface-name>) 
 *****************************************************************************/ 
BEGIN_DEFINE_CLASS_INTERFACE(COvalSpriteActor, IMoaMmXSpriteActor)
END_DEFINE_CLASS_INTERFACE


/*****************************************************************************
 *  CREATE AND DESTROY METHODS
 *  --------------------------
 *  Every interface and class has an associated 'Create' and 'Destroy' pair.
 *  'Create' methods are typically used to acquire interface(s), allocate 
 *  memory, and intialize variables. 'Destroy' methods are typically used to 
 *  release interfaces and free memory. To create 'empty' create and destroy 
 *  methods for an interface, use the STD_INTERFACE_CREATE_DESTROY macro.
 *****************************************************************************/ 

/* NOTE:  In C++, the local variable 'This' is provided implicitly within 
 * a method implementation.  Thus, there is no need explicitly declare 'This' 
 * as a function parameter. However, this implementation detail doesn't apply 
 * to the MOA class creator and destructor functions, which are standard C 
 * functions, coded exactly as in like they are in C examples.  
 *
 * Syntax:
 * STDMETHODIMP MoaCreate_<class-name>(<class-name> * This)  
 * STDMETHODIMP MoaDestroy_<class-name>(<class-name> * This)  
 */

/* ----------------------------------------------------- MoaCreate_COvalSpriteActor */
STDMETHODIMP  		MoaCreate_COvalSpriteActor(COvalSpriteActor * This)
{
	MoaError	err = kMoaErr_NoErr;

	This->pXAsset = NULL;
	This->state = kMyActorState_Oval;
	This->stepState = 0;
	This->direction = 1;
	
	return(err);
}

/* --------------------------------------------------- MoaDestroy_COvalSpriteActor */
STDMETHODIMP_(void) MoaDestroy_COvalSpriteActor(COvalSpriteActor * This)
{		
	if (This->pXAsset)
		This->pXAsset->Release();
	
	return;
}

/* macros that create constructors and destructors for the following interface(s) */

STD_INTERFACE_CREATE_DESTROY(COvalSpriteActor, IMoaMmXSpriteActor)

/*****************************************************************************
 *  METHOD IMPLEMENTATION(S)
 *  ------------------------
 *  This is where the methods to be defined by your MOA class are implemented.
 *  The bulk of the work in implementing Xtras is done here.  NOTE: 'This' is 
 *  implemented implicitly in C++, therefore it isn't used in the argument-list.
 *
 *  Syntax:
 *  STDMETHODIMP <class-name>_<interface-name>::<method-name>(<argument-list>)
 *****************************************************************************/ 

/* ---------------------------------- COvalSpriteActor_IMoaMmXSpriteActor::GetProp */
STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::GetProp(MoaMmSymbol symbol,
	PMoaMmValue pPropValue)
{
	/* We have no actor properties we wish export */
	return(kMoaMmErr_PropertyNotFound);
}

/* ---------------------------------- COvalSpriteActor_IMoaMmXSpriteActor::SetProp */
STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::SetProp(MoaMmSymbol symbol,
	ConstPMoaMmValue pPropValue)
{
	/* We have no actor properties we wish export */
	return(kMoaMmErr_PropertyNotFound);
}

/* ----------------------------- COvalSpriteActor_IMoaMmXSpriteActor::CallFunction */
STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::CallFunction(MoaMmSymbol methodName, 
	MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{
	/* We have no actor functions we wish export */
	return(kMoaMmErr_FunctionNotFound);
}

/* ------------------------------ COvalSpriteActor_IMoaMmXSpriteActor::SetCallback */
STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::SetCallback(PIMoaMmSpriteCallback pSpriteCallback)
{
	return(kMoaErr_NoErr);
}

/* ----------------------------------- COvalSpriteActor_IMoaMmXSpriteActor::Notify */
STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	/* This method is intended for communicating between the
	   asset and its xtras.  The developer is free to define
	   message codes for private use.  This may be called
	   directly by an asset, or through the 
	   IMoaMmAssetCallback::NotifyAllSprites() callback. */

	switch (msgCode)
	{
		case kSprNotifyMsg_SetParentAsset:
			pObj->pXAsset = (PIMoaMmXAsset) refCon;
			pObj->pXAsset->AddRef();	
			 
			/* up ref count since we're hanging on to this */
			break;

		default:
			break;
	}	

	return(kMoaErr_NoErr);
}

/* ---------------------------------- COvalSpriteActor_IMoaMmXSpriteActor::GetCaps */
STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::GetCaps(PMoaMmXSpriteCaps pActorCaps)
{
	MoaError	err = kMoaErr_NoErr;
		
	/* We only want mouse, play, span, and step events 
	   Optionally get key/focus and environment events too */
	
	pActorCaps->eventMask = (kMoaMmEventMask_Mouse 
						| kMoaMmEventMask_Play
						| kMoaMmEventMask_Span
						| kMoaMmEventMask_Step
#ifdef HANDLE_KEY_EVENTS
						| kMoaMmEventMask_Key
#endif /* HANDLE_KEY_EVENTS */
#ifdef HANDLE_ENV_EVENTS
						| kMoaMmEventMask_Environment
#endif /* HANDLE_ENV_EVENTS */
	);

	/* We support no special actor capabilities
	   use kMoaMmXSpriteCapsFlags_AutoTabEnabled for autoTab support */
	
	pActorCaps->flags = 	kMoaMmXSpriteCapsFlags_None
						| kMoaMmXSpriteCapsFlags_AutoTabEnabled
	 					;
	
	/* We support either offscreen or direct-to-screen imaging,
	   but we're intended for offscreen imaging (the normal case). */
	
	pActorCaps->requiredImageMode = kMoaMmXSpriteImageMode_Any;
	pActorCaps->preferredImageMode = kMoaMmXSpriteImageMode_Offscreen;
	
	return(err);
}

/* --------------------------- COvalSpriteActor_IMoaMmXSpriteActor::CollectChanges */
STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::CollectChanges(ConstPMoaMmXSpriteInfo pActorInfo,
	PIMoaMmChangeCollector pChangeCollector)
{
	MoaError	err = kMoaErr_NoErr;
	
	/* Here we always add our entire rect as the change area, since 
	   we may be animating and we want Director to refresh 
	   the stage with our sprite every frame.  
	
	   If our actor is not changing its appearance in the  
	   coming frame, we would not add any change rects here.  
	
	   (Note: if the sprite is moving or any of its attributes 
	   have changed (such as color), it is always updated in 
	   its entirety automatically, and our CollectChanges() 
	   is not called in this case.) 

	 
	   In the actorInfo, Director has supplied us with what will be our 
	   full sprite rect in the coming frame.  We simply 
	   add this entire rectangle to indicate that the 
	   entire sprite needs updating. */

	pChangeCollector->AddChangeRect(&pActorInfo->spriteRect);

	return(err);
}

/* ---------------------------- COvalSpriteActor_IMoaMmXSpriteActor::ContainsPoint */
STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::ContainsPoint(ConstPMoaMmXSpriteInfo pActorInfo,
	ConstPMoaPoint pTestPoint, MoaBoolParam * pTestResult)
{
	MoaError	err = kMoaErr_NoErr;
	
	/* anywhere within our spriteRect is a hit. 
	   If we wanted to support non-rectangular hit-testing, we'd 
	   be less lazy here and perform the test based upon, say, 
	   the current oval size (if our state == kMyActorState_Oval). */
	
	*pTestResult = TRUE;
		
	return(err);
}


/* ------------------------------------ COvalSpriteActor_IMoaMmXSpriteActor::Image */
#define PENSIZE 2
#define MAXSTEP	10

STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::Image(ConstPMoaMmXSpriteInfo 	pActorInfo,
	ConstPMoaMmPlayerStateInfo	pPlayerStateInfo, PIMoaMmGC pGC)
{
	MoaError			err = kMoaErr_NoErr;
	PMoaRect			pImageRect = (PMoaRect) &pActorInfo->spriteRect;
	MoaMmNativeGCInfo	nativeInfo;
	MoaLong		r, g, b;

	pGC->GetNativeGCInfo(&nativeInfo);

	COvalSpriteAsset_IMoaMmXAsset* ptr = ((COvalSpriteAsset_IMoaMmXAsset*)pObj->pXAsset);
	
	r = ptr->pObj->redValue;
	g = ptr->pObj->greenValue;
	b = ptr->pObj->blueValue;

#ifdef MACINTOSH
	{
	GrafPtr				savePort, drawPort;
	PenState			savePen;
	RGBColor			saveColor;
	RGBColor			myColor = { r * r, g * g, b * b };

	/* Determine what port to draw into based on the 
	   supplied Graphics Context.  The host app sets up 
	   this context for us. */

	switch (nativeInfo.type)
	{
		case kMoaMmGCType_MacWindow:
			drawPort = (GrafPtr) nativeInfo.data.mac_window.windowPtr;
			break;
		case kMoaMmGCType_MacGWorld:
			drawPort = (GrafPtr) nativeInfo.data.mac_gworld.gworldPtr;
			break;
	
		default:
			/* we don't know about this type of graphics context; bail out. */
			goto done;
	}
			
	GetPort(&savePort);
	SetPort( drawPort );
	
	/* We could obey the sprite color supplied in pActorInfo here, 
	   but instead we always draw using myColor. */
	
	GetForeColor(&saveColor);
	RGBForeColor(&myColor);
	GetPenState(&savePen);
	PenNormal();
	PenSize(PENSIZE, PENSIZE);
		
	switch (pObj->state)
	{
		case kMyActorState_X:
		{
			/* Draw an "X" if our state is kMyActorState_X */
			
			MoveTo(pImageRect->left+PENSIZE, pImageRect->top+PENSIZE);
			LineTo(pImageRect->right-PENSIZE, pImageRect->bottom-PENSIZE);
			MoveTo(pImageRect->left+PENSIZE, pImageRect->bottom-PENSIZE);
			LineTo(pImageRect->right-PENSIZE, pImageRect->top+PENSIZE);
			
			break;
		}
		case kMyActorState_Oval:
		{
			/* Draw an oval if our state is kMyActorState_Oval */
		
			MoaLong ovalSize = pObj->stepState;
			Rect ovalRect;
			
			MoaToMacRect(pImageRect, &ovalRect);
			ovalSize = (ovalSize * (pImageRect->right - pImageRect->left)/2) / MAXSTEP;
			InsetRect(&ovalRect, ovalSize, ovalSize);
			FrameOval(&ovalRect);
			break;
		}
			
		default:
			/* Unknown state, don't draw anything */
			break;
	}
	
	/* Restore our drawing state and port. */
	
	RGBForeColor(&saveColor);
	
	SetPenState(&savePen);
	SetPort(savePort);
	}
#endif	/*  MACINTOSH */

#ifdef WINDOWS
	{
	HDC			drawDC;
	HPEN		hPen, hOldPen;
	HBRUSH		hOldBrush;
	int			oldRop;

    /* Determine what DC to draw into based on the 
       supplied Graphics Context.  The host app sets up 
       this context for us. */

    switch (nativeInfo.type)
    {
		case kMoaMmGCType_WinDIBDC:
		case kMoaMmGCType_WinDDBDC:
		case kMoaMmGCType_WinWindow:
			drawDC = (HDC)(MoaLong) nativeInfo.data.win_window.hDC;
			break;

        default:
            /*  we don't know about this type of graphics context; bail out. */
            goto done;
    }

	hPen = CreatePen(PS_SOLID, PENSIZE, PALETTERGB(r, g, b));
	hOldPen = (HPEN)SelectObject(drawDC, hPen);
	hOldBrush = (HBRUSH)SelectObject(drawDC, GetStockObject(NULL_BRUSH));

	/* Ensure ROP is set correctly, since this might have been 
	   changed by host application. */

	oldRop = SetROP2(drawDC, R2_COPYPEN);

    if( pObj->state == kMyActorState_X )
    {
            /* Draw an "X" if our state is kMyActorState_X */

            MoveToEx(drawDC, (short) pImageRect->left+PENSIZE, (short) pImageRect->top+PENSIZE, 0);
            LineTo(drawDC, (short) pImageRect->right-PENSIZE, (short) pImageRect->bottom-PENSIZE);
            MoveToEx(drawDC, (short) pImageRect->left+PENSIZE, (short) pImageRect->bottom-PENSIZE, 0);
            LineTo(drawDC, (short) pImageRect->right-PENSIZE, (short) pImageRect->top+PENSIZE);
	}
    else if( pObj->state == kMyActorState_Oval ) 
	{
            /* Draw an oval if our state is kMyActorState_Oval */
        
            MoaLong ovalSize = pObj->stepState;
            RECT ovalRect;
            
            ovalRect.top = (short) pImageRect->top;
            ovalRect.left = (short) pImageRect->left;
            ovalRect.bottom = (short) pImageRect->bottom;
            ovalRect.right  = (short) pImageRect->right;
            
			ovalSize = (ovalSize * (pImageRect->right - pImageRect->left)/2) / MAXSTEP;
            InflateRect(&ovalRect, (short) -ovalSize, (short) -ovalSize);
            Ellipse(drawDC, ovalRect.left, ovalRect.top, ovalRect.right, ovalRect.bottom);
	}

    /* Restore our drawing state and port. */
    
    if (oldRop)
    	SetROP2(drawDC, oldRop);
    	
    SelectObject(drawDC, hOldBrush);
    SelectObject(drawDC, hOldPen);
	DeleteObject(hPen);
	}
#endif /*  WINDOWS */
	
done:
	return(err);
}

/* ------------------------------------ COvalSpriteActor_IMoaMmXSpriteActor::Event */
STDMETHODIMP COvalSpriteActor_IMoaMmXSpriteActor::Event(ConstPMoaMmEventInfo pEventInfo,
	ConstPMoaMmPlayerStateInfo pPlayerStateInfo, MoaBoolParam * pHandled)
{
	MoaError	err = kMoaErr_NoErr;
	COvalSpriteAsset_IMoaMmXAsset* ptr = ((COvalSpriteAsset_IMoaMmXAsset*)pObj->pXAsset);

	switch (pEventInfo->type)
	{
		case kMoaMmEvent_Step:
			/* Change our step state (this corresponds to 
			   the size of the oval when state == kMyActorState_Oval) */
			pObj->stepState += pObj->direction;
			if (pObj->stepState < 0 || pObj->stepState > MAXSTEP)
			{
				pObj->direction = - pObj->direction;
				pObj->stepState += pObj->direction;
			}
			*pHandled = kMoaMmSprEvent_Handled;
			break;
		
		case kMoaMmEvent_MouseDown:
		case kMoaMmEvent_MouseDblClick:
			/* Toggle our visual state */
			if( ptr->pObj->clickValue )
				if( pObj->state == kMyActorState_Oval)
					pObj->state = kMyActorState_X;
				else
					pObj->state = kMyActorState_Oval;
			*pHandled = kMoaMmSprEvent_Handled;
			break;

		case kMoaMmEvent_MouseUp:
			/* If you handle MouseUp here, set
				*pHandled = TRUE. */
			*pHandled = kMoaMmSprEvent_Pass;
			break;
			
		case kMoaMmEvent_Idle:
			*pHandled = kMoaMmSprEvent_Pass;
			break;
			
		case kMoaMmEvent_SpanBegin:
			/* A new sprite span has started; init our state variables. */
			pObj->state = kMyActorState_Oval;
			pObj->stepState = 0;
			pObj->direction = 1;
			*pHandled = kMoaMmSprEvent_Handled;
			break;
		
		case kMoaMmEvent_SpanEnd:
			*pHandled = kMoaMmSprEvent_Pass;
			break;

#ifdef HANDLE_KEY_EVENTS
		case kMoaMmEvent_KeyFocusEnter:
			/* We've entered keyboard focus. */
			pObj->state = kMyActorState_X;
			pObj->stepState = 0;
			pObj->direction = 1;
			*pHandled = kMoaMmSprEvent_Handled;
			break;
		
		case kMoaMmEvent_KeyFocusExit:
			/* We're exiting keyboard focus. */
			pObj->state = kMyActorState_Oval;
			pObj->stepState = 0;
			pObj->direction = 1;
			*pHandled = kMoaMmSprEvent_Handled;
			break;

		case kMoaMmEvent_KeyDown:
#ifdef MACINTOSH
			SysBeep(1);
#endif /*  MACINTOSH */
			*pHandled = kMoaMmSprEvent_Handled;
			break;

#endif /*  HANDLE_KEY_EVENTS */

#ifdef HANDLE_ENV_EVENTS
		case kMoaMmEvent_AssetModified:
			*pHandled = kMoaMmSprEvent_Handled;
			break;
#endif /* HANDLE_ENV_EVENTS */

		default:
			*pHandled = kMoaMmSprEvent_Pass;
			break;
	}

	return(err);
}

