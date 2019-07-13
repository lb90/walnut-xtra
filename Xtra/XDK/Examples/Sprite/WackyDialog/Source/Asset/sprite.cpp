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
 *	Name: sprite.cpp
 *	
 * 	Purpose: Definitions of Asset class interface(s) and methods for the 
 *           Oval Sprite Xtra example.
 *
 ****************************************************************************/ 

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////

#include "Sprite.h"

// XSupport
#include "XMmValue.h"
#include "XSupport.h"
#include "XMacHelp.h"

#include "XWinHelp.h"

#ifdef MACINTOSH
#include <QuickDraw.h>
#endif

#include <String.h>

const MoaShort kLargeFontSize 		= 24;


///////////////////////////////////////////////////////////////////////////////
//
// MOA Stuff
//
///////////////////////////////////////////////////////////////////////////////
  
BEGIN_DEFINE_CLASS_INTERFACE( CWackyDlgSprite, IMoaMmXSpriteActor )
END_DEFINE_CLASS_INTERFACE

/* ------------------------------------------------------ MoaCreate_CWackyDlgSprite */
STDMETHODIMP MoaCreate_CWackyDlgSprite( CWackyDlgSprite * This )
{
	X_ENTER

	// always initialize XSupport!
	if( XSupport::Initialize( This->pCallback, kXSupportDefaultReserve, XDEBUG ) )
		X_STD_RETURN( kMoaErr_InternalError );

	This->mAsset 				= NULL;
	This->mSpriteCallback 		= NULL;

	X_STD_RETURN( kMoaErr_NoErr );	
	X_EXIT
}

/* ----------------------------------------------------- MoaDestroy_CWackyDlgSprite */
STDMETHODIMP_(void) MoaDestroy_CWackyDlgSprite( CWackyDlgSprite * This )
{
	X_ENTER
		
	if( This->mSpriteCallback )
		This->mSpriteCallback->Release();
	
	// zero it out in the asset
	This->mAsset->pObj->mSpriteCallback = NULL;
	
	if( This->mAsset )
		This->mAsset->Release();

	// don't forget to do this!
	XSupport::Shutdown();
	
	X_RETURN_VOID;
	X_EXIT
}

///////////////////////////////////////////////////////////////////////////////
//
// Interface implementation
//
///////////////////////////////////////////////////////////////////////////////

/* macros that create constructors and destructors for the following interface(s) */
STD_INTERFACE_CREATE_DESTROY( CWackyDlgSprite, IMoaMmXSpriteActor )

/* ------------------------------------ CWackyDlgSprite_IMoaMmXSpriteActor::GetProp */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::GetProp( MoaMmSymbol, PMoaMmValue )
{
	X_ENTER

	// see asset.cpp for how to implement these functions.
	
	X_STD_RETURN( kMoaMmErr_PropertyNotFound );
	X_EXIT
}

/* ------------------------------------ CWackyDlgSprite_IMoaMmXSpriteActor::SetProp */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::SetProp( MoaMmSymbol, ConstPMoaMmValue )
{
	XMOAEXCEPT_ENTER
	
	// see asset.cpp for how to implement these functions.

	X_STD_RETURN( kMoaMmErr_PropertyNotFound );
	XMOAEXCEPT_EXIT
}

/* ------------------------------- CWackyDlgSprite_IMoaMmXSpriteActor::CallFunction */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::CallFunction(MoaMmSymbol /*methodName*/, MoaLong /*nArgs*/, ConstPMoaMmValue /*pArgs*/, PMoaMmValue /*pResult*/ )
{
	X_ENTER

	// see asset.cpp for how to implement these functions.
	
	X_STD_RETURN( kMoaMmErr_FunctionNotFound );
	X_EXIT
}

/* -------------------------------- CWackyDlgSprite_IMoaMmXSpriteActor::SetCallback */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::SetCallback( PIMoaMmSpriteCallback pSpriteCallback )
{
	X_ENTER

	pObj->mSpriteCallback = pSpriteCallback;
	pObj->mSpriteCallback->AddRef();

	// um, set this into the asset so that the dialog can get to it...
	pObj->mAsset->pObj->mSpriteCallback = pSpriteCallback;
	
	X_STD_RETURN( kMoaErr_NoErr );
	X_EXIT
}

/* ------------------------------------- CWackyDlgSprite_IMoaMmXSpriteActor::Notify */
/* This method is intended for communicating between the asset and its xtras.
   The developer is free to define message codes for private use.  This may be
   called directly by an asset, or through the 
   IMoaMmAssetCallback::NotifyAllSprites() callback. */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::Notify( MoaLong msgCode, PMoaVoid refCon )
{
	X_ENTER

	/* variable declarations */
	MoaError err = kMoaErr_NoErr;

	switch( msgCode )
	{
		case kSprNotifyMsg_SetParentAsset:
			pObj->mAsset = (CWackyDlgAsset_IMoaMmXAsset *)refCon;
			pObj->mAsset->AddRef();
			break;

		case kSprNotifyMsg_Redraw:
			pObj->mSpriteCallback->InvalSpriteRect();
			break;
	}

	X_STD_RETURN( err );
	X_EXIT
}

/* ------------------------------------ CWackyDlgSprite_IMoaMmXSpriteActor::GetCaps */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::GetCaps( PMoaMmXSpriteCaps pCaps )
{
	X_ENTER
	
	// we are a static sprite.  If you are dynamic you need to pass the dynamic flag instead.
	pCaps->flags = kMoaMmXSpriteCapsFlags_IsStatic;
	
	X_STD_RETURN( kMoaErr_NoErr );
	X_EXIT
}

/* ----------------------------- CWackyDlgSprite_IMoaMmXSpriteActor::CollectChanges */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::CollectChanges( ConstPMoaMmXSpriteInfo, PIMoaMmChangeCollector )
{
	X_ENTER

	// a well behaved sprite does the right thing here when appropriate.

	X_STD_RETURN( kMoaErr_NoErr );
	X_EXIT
}

/* ------------------------------ CWackyDlgSprite_IMoaMmXSpriteActor::ContainsPoint */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::ContainsPoint( ConstPMoaMmXSpriteInfo /*pActorInfo*/, ConstPMoaPoint /*pTestPoint*/, MoaBoolParam * pTestResult )
{
	X_ENTER

	*pTestResult = TRUE;

	X_STD_RETURN( kMoaErr_NoErr );
	X_EXIT
}


/* -------------------------------------- CWackyDlgSprite_IMoaMmXSpriteActor::Image */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::Image( ConstPMoaMmXSpriteInfo pActorInfo, ConstPMoaMmPlayerStateInfo /*pPlayerStateInfo*/, PIMoaMmGC pGC )
{
	XMOAEXCEPT_ENTER	

	// gain direct access to our asset object
	CWackyDlgAsset * pAsset = pObj->mAsset->pObj;

	// we bail early, we need this!
	if( !pAsset )
		X_STD_RETURN( kMoaErr_NoErr );
	
	MoaMmNativeGCInfo moaGCInfo;
	pGC->GetNativeGCInfo( &moaGCInfo );
		
#ifdef MACINTOSH
	// erase the rectangle -- NOTE: these are stacked based classes that restore the state when they go out of scope.
	// NOTE: the port must be set, the mac versions of NativeTextOut and width both assume the port is set!@!!
	if (moaGCInfo.type==kMoaMmGCType_MacWindow)
		SetPortAndRestore port( (WindowPtr)moaGCInfo.data.mac_window.windowPtr );
	else // type==kMoaMmGCType_MacGWorld
		SetPortAndRestore port( (GWorldPtr)moaGCInfo.data.mac_gworld.gworldPtr );
	RGBColor foreColor = {0xCCCC,0xCCCC,0xCCCC};
	RGBColor backColor = {0,0,0};
	Rect 	 macRect = { (short)pActorInfo->spriteRect.top, (short)pActorInfo->spriteRect.left, (short)pActorInfo->spriteRect.bottom, (short)pActorInfo->spriteRect.right };

	// test color bits for more excitement
	if( !(pAsset->mProps.colorBits & kShowRed) )
		foreColor.red = 0;
	if( !(pAsset->mProps.colorBits & kShowGreen) )
		foreColor.green = 0;
	if( !(pAsset->mProps.colorBits & kShowBlue) )
		foreColor.blue = 0;
		
	// invert black

	if( !foreColor.red && !foreColor.green && !foreColor.blue )

		foreColor.red = foreColor.green = foreColor.blue = 0xFFFF;



	SetColorAndRestore newColors( &foreColor, &backColor );
	
	// we are setting the clip region because we sometimes draw outside of our box!	
	SetClipAndRestore clipTo( &macRect );
	::EraseRect( &macRect );

#else

	// WINDOWS
	HDC		hDC = (HDC)moaGCInfo.data.win_window.hDC;
	MoaByte fr = 0xCC, fg = 0xCC, fb = 0xCC;
	RECT	winRect = { pActorInfo->spriteRect.left, pActorInfo->spriteRect.top, pActorInfo->spriteRect.right, pActorInfo->spriteRect.bottom };

	// test color bits for more excitement
	if( !(pAsset->mProps.colorBits & kShowRed) )
		fr = 0;

	if( !(pAsset->mProps.colorBits & kShowGreen) )
		fg = 0;

	if( !(pAsset->mProps.colorBits & kShowBlue) )
		fb = 0;

	// invert black
	if( !fr && !fg && !fb )
		fr = fg = fb = 255;

	// seleset the text color
	::SetTextColor( hDC, RGB(fr, fg, fb) );
	::SetBkMode( hDC, TRANSPARENT );
	CWinClipSetAndRestore clipTo( hDC, &winRect );

	// create brush for background fill
	HBRUSH hBackBrush = ::CreateSolidBrush( RGB(0, 0, 0) );
	::FillRect( hDC, &winRect, hBackBrush );
	::DeleteObject( hBackBrush );

#endif	

	// draw some text -- don't forget to offset by the actual sprite rectangle
	MoaShort textWidth = (MoaShort)pAsset->NativeTextWidth( pGC, pAsset->mProps.outputString, pAsset->mProps.fontName, pAsset->mProps.fontSize );
	MoaLong  spriteWidth  = pActorInfo->spriteRect.right - pActorInfo->spriteRect.left;
	MoaLong  spriteHeight = pActorInfo->spriteRect.bottom - pActorInfo->spriteRect.top;
	MoaLong  xPosition = ((spriteWidth - textWidth) / 2) + pActorInfo->spriteRect.left;

#ifdef MACINTOSH
	// we take advantage of the base line
	MoaLong  yPosition = ((spriteHeight + pAsset->mProps.fontSize) / 2) + pActorInfo->spriteRect.top;
#else
	MoaLong  yPosition = ((spriteHeight - pAsset->mProps.fontSize) / 2) + pActorInfo->spriteRect.top;
#endif

	pAsset->NativeTextOut( pGC, xPosition, yPosition, pAsset->mProps.outputString, pAsset->mProps.fontName, pAsset->mProps.fontSize );

	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* -------------------------------------- CWackyDlgSprite_IMoaMmXSpriteActor::Event */
STDMETHODIMP CWackyDlgSprite_IMoaMmXSpriteActor::Event( ConstPMoaMmEventInfo /*pEventInfo*/, ConstPMoaMmPlayerStateInfo /*pPlayerStateInfo*/, MoaBoolParam * /*pHandled*/ )
{
	X_ENTER
	X_STD_RETURN( kMoaErr_NoErr );
	X_EXIT
}

// EOF
