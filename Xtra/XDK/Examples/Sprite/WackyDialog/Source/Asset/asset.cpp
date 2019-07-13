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
 *	Name: ovalasst.cpp
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

#include "Asset.h"
#include "Sprite.h"
#include "Dialog.h"

// XSupport
#include "XSupport.h"
#include "MuiHelpr.h"
#include "XMmList.h"
#include "XMoaHelp.h"
#include "XMacHelp.h"

#include "XWinHelp.h"
#include "XThrwErr.h"

#ifdef WINDOWS
#include "ByteSwap.h"
#endif

#ifdef MACINTOSH
#include <QuickDraw.h>
#include <Fonts.h>
#include <Resources.h>
#endif

// ANSI
#include <String.h>

///////////////////////////////////////////////////////////////////////////////
//
// Defines / Constants
//
///////////////////////////////////////////////////////////////////////////////

typedef struct tagWackyDlgData
{
	MmListHelper* 		pList;
	bool				collectOutlineOnly;
} TWackyDlgData, * PTWackyDlgData;


const MoaLong kDefaultRectBiSection = 50;
const MoaLong kStaticThumbFontSize  = 20;

#ifdef MACINTOSH
const MoaLong kStaticThumbXPos		= 4;
const MoaLong kStaticThumbYPos		= 24;
#else
const MoaLong kStaticThumbXPos		= 3;
const MoaLong kStaticThumbYPos		= 4;
#endif

// hey this is handy for debugging, turn this on to see what is going in
// with stream/props in/out...  this spews data to the message window
#define SPEW_STREAM_MESSAGES

///////////////////////////////////////////////////////////////////////////////
//
// Class construction
//
///////////////////////////////////////////////////////////////////////////////

BEGIN_DEFINE_CLASS_INTERFACE( CWackyDlgRegister, IMoaRegister )
END_DEFINE_CLASS_INTERFACE

STDMETHODIMP MoaCreate_CWackyDlgRegister( CWackyDlgRegister * This )
{
	// Very important -- Always initialize XSupport, this is ref counted so it's okay to do more than
	// once as long as the calls are always matched up with a shutdown. p.s. XDEBUG should always be defined,
	// a few headers depend on it for giving you back better debugging info when things go haywire.
	X_STD_RETURN( XSupport::Initialize( This->pCallback, kXSupportDefaultReserve, XDEBUG ) );
}

STDMETHODIMP_(void) MoaDestroy_CWackyDlgRegister( CWackyDlgRegister * )
{
	// see my note above.
	XSupport::Shutdown();
}

/* macros that create constructors and destructors for the following interface(s) */
STD_INTERFACE_CREATE_DESTROY( CWackyDlgRegister, IMoaRegister )


/* ------------------------------------ CWackyDlgAsset_IMoaRegister::Register */
STDMETHODIMP CWackyDlgRegister_IMoaRegister::Register( PIMoaCache pCache, PIMoaDict pXtraDict )
{
	XMOAEXCEPT_ENTER
	
	// variable declaration
	MoaError 	err = kMoaErr_NoErr;
	PIMoaDict 	pRegDict;
	
	// Standard asset entries
	MoaMmActorType 		actorType = kMoaMmActorType_Sprite;
	MoaMmAssetCaps		assetCaps = kMoaMmAssetCaps_HasPropsEditor 
									| kMoaMmAssetCaps_CanImageStaticThumb
									| kMoaMmAssetCaps_StaticThumbNeedsMedia;
	MoaBool				bItsSafe = TRUE;
	
	err = pCache->AddRegistryEntry( pXtraDict, &CLSID(CWackyDlgAsset), &IID(IMoaMmXAsset), &pRegDict );
	if( err != kMoaErr_NoErr ) 
		X_STD_RETURN( err );

	// Register our "Shockwave-Safe" flag
	err = pRegDict->Put( kMoaMmDictType_SafeForShockwave, &bItsSafe, sizeof(bItsSafe), kMoaMmDictKey_SafeForShockwave );
	if( err != kMoaErr_NoErr ) 
		X_STD_RETURN( err );
	
	// Register standard asset entries
	err = pRegDict->Put( kMoaMmDictType_SymbolString, "Wacky Dialog", 0, kMoaMmDictKey_SymbolString );
	if( err != kMoaErr_NoErr ) 
		X_STD_RETURN( err );
		
	err = pRegDict->Put( kMoaMmDictType_DisplayNameString, "Wacky Dialog", 0, kMoaMmDictKey_DisplayNameString );
	if( err	!= kMoaErr_NoErr ) 
		X_STD_RETURN( err );

	err = pRegDict->Put(kMoaMmDictType_DisplayCategoryString, "Sprite Examples", 0, kMoaMmDictKey_DisplayCategoryString );
	if( err != kMoaErr_NoErr )
		X_STD_RETURN( err );

	err = pRegDict->Put(kMoaMmDictType_ActorType, &actorType, 0, kMoaMmDictKey_ActorType );
	if( err != kMoaErr_NoErr ) 
		X_STD_RETURN( err );
	
	err = pRegDict->Put(kMoaMmDictType_AssetCaps, &assetCaps, sizeof(MoaMmAssetCaps), kMoaMmDictKey_AssetCaps );
	if( err != kMoaErr_NoErr)
		X_STD_RETURN( err );

	// do this before we switch registries.
	RegisterIcon( pRegDict, "colorpicon", TRUE );
	RegisterIcon( pRegDict, "bwpicon", FALSE );

	X_STD_RETURN( err );
	XMOAEXCEPT_EXIT
}

// private routine for registering a unique icon to go with our asset...
STDMETHODIMP CWackyDlgRegister_IMoaRegister::RegisterIcon(PIMoaDict pDict, ConstPMoaChar pResName, MoaBoolParam bColor)
{
	MoaError	err = kMoaErr_NoErr;

	// important we need temporary access to our resources, this is a stacked based object.	
	MoaRezHelper openThemResources;

#ifdef MACINTOSH
	// Macintosh Implementation
	Handle 	 hPic;
	Str255   pascalStringName;
	
	int slen = ::strlen( pResName );
	if (slen>255)
		slen = 255;
	pascalStringName[0] = slen;
		
	::strncpy( (char *)&pascalStringName[1], pResName, ::strlen( pResName ) );
	
	hPic = ::GetNamedResource( 'PICT', pascalStringName );

	if( hPic )
	{
		::HLock( hPic );
				
		// Stuff the icon picture data into the registry
		err = pDict->Put( bColor ? kMoaMmDictType_ColorIcon : kMoaMmDictType_BWIcon, 
				*hPic, ::GetHandleSize( hPic ), bColor ? kMoaMmDictKey_ColorIcon : kMoaMmDictKey_BWIcon );

		::HUnlock( hPic );
		::ReleaseResource( hPic );
	} 
	else 
		err = kMoaErr_BadParam;
#endif 
#ifdef WINDOWS
	// Windows Implementation
	HGLOBAL hBitmap;
	LPVOID	pBitmap;
	DWORD	size;
	HRSRC 	hRsrc = ::FindResourceA( (HINSTANCE)openThemResources.GetFileRef(), pResName, MAKEINTRESOURCEA(RT_BITMAP) );
	if( hRsrc )
	{
		// some of these are macros so using :: is bad!
		hBitmap = LoadResource( (HINSTANCE)openThemResources.GetFileRef(), hRsrc );
		if( hBitmap ) 
		{
			pBitmap = LockResource( hBitmap );
			size = SizeofResource( (HINSTANCE)openThemResources.GetFileRef(), hRsrc );
			err = pDict->Put( bColor ? kMoaMmDictType_ColorIcon : kMoaMmDictType_BWIcon, 
					pBitmap, size, bColor ? kMoaMmDictKey_ColorIcon : kMoaMmDictKey_BWIcon );
			UnlockResource( hBitmap );
			FreeResource( hBitmap );
		}
	}
	else 
		err = kMoaErr_BadParam;
#endif

	return err;
}

#ifdef MACINTOSH
#pragma mark -
#endif

BEGIN_DEFINE_CLASS_INTERFACE( CWackyDlgAsset, IMoaMmXAsset )
END_DEFINE_CLASS_INTERFACE

/* ------------------------------------------------------- MoaCreate_CWackyDlgAsset */
STDMETHODIMP MoaCreate_CWackyDlgAsset( CWackyDlgAsset * This )
{
	XMOAEXCEPT_ENTER

	// variable declarations
	MoaError		err = kMoaErr_NoErr;
	PIMoaDrPlayer	pIMoaDrPlayer;
	
	// again, always initialize XSupport it won't hurt you.	
	err = XSupport::Initialize( This->pCallback, kXSupportDefaultReserve, XDEBUG );
	if( err )
		X_STD_RETURN( err );
	
	This->mAssetCallback 		= NULL;
	This->mSpriteCallback 		= NULL;
	This->mIMoaHandle 			= NULL;
	This->mIMoaDrMovie 			= NULL;
	This->mIMoaMmUtils 			= NULL;
	This->mIMoaMmList 			= NULL;

	// Obtain callback interfaces.
	This->pCallback->QueryInterface( &IID(IMoaHandle), (PPMoaVoid)&This->mIMoaHandle );
	This->pCallback->QueryInterface( &IID(IMoaMmUtils), (PPMoaVoid)&This->mIMoaMmUtils );
	This->pCallback->QueryInterface( &IID(IMoaMmList), (PPMoaVoid)&This->mIMoaMmList );
	This->pCallback->QueryInterface( &IID(IMoaMmSpriteCallback), (PPMoaVoid)&This->mSpriteCallback );
	
	// deal with player
	This->pCallback->QueryInterface( &IID(IMoaDrPlayer), (PPMoaVoid)&pIMoaDrPlayer );
	if(pIMoaDrPlayer)
	{
		pIMoaDrPlayer->GetActiveMovie( &This->mIMoaDrMovie );
		pIMoaDrPlayer->Release();
	}

	if( !This->mIMoaHandle || !pIMoaDrPlayer || !This->mIMoaDrMovie || !This->mIMoaMmUtils || !This->mIMoaMmList )
		return kMoaErr_BadInterface;
		
	// stuff some default data in our properties.
	This->mProps.version   = kAssetPropertiesVersion;
	This->mProps.size	   = sizeof( TWackyDlgProperties );
	This->mProps.fontSize  = 24;
	This->mProps.colorBits = kShowRed | kShowGreen | kShowBlue;
	// no font name for now.
	This->mProps.fontName[0] = 0;
	This->mProps.checkbox1State = 0;
	This->mProps.checkbox2State = 0;
	This->mProps.radioGroupButton = 0;
	This->mProps.checkboxGroupButton = 1;

	::strcpy( This->mProps.outputString, "Hello!" );
	::strcpy( This->mProps.watchString, "Type Here" );
	::strcpy( This->mProps.popupItemString, "One" );

#ifdef SPEW_STREAM_MESSAGES
	IMmUtilsHelper().Use()->PrintMessage1( "MoaCreate-> WackyDlg: %x\n", (MoaLong)This );
#endif
	
	X_STD_RETURN( err );
	XMOAEXCEPT_EXIT
}

/* ------------------------------------------------------ MoaDestroy_CWackyDlgAsset */
STDMETHODIMP_(void) MoaDestroy_CWackyDlgAsset( CWackyDlgAsset * This )
{
	XMOAEXCEPT_ENTER
	
	if( This->mAssetCallback )
		This->mAssetCallback->Release();

	if( This->mIMoaHandle )
		This->mIMoaHandle->Release();

	if( This->mIMoaDrMovie )
		This->mIMoaDrMovie->Release();

	if( This->mIMoaMmUtils )
		This->mIMoaMmUtils->Release();

	if( This->mIMoaMmList )
		This->mIMoaMmList->Release();

	if( This->mSpriteCallback )
		This->mSpriteCallback->Release();

#ifdef SPEW_STREAM_MESSAGES
	IMmUtilsHelper().Use()->PrintMessage1( "MoaDestroy-> WackyDlg: %x\n", (MoaLong)This );
#endif
	
	XSupport::Shutdown();
	
	X_RETURN_VOID;
	XMOAEXCEPT_EXIT_VOID
}


// properties in the file are stored in network order, aka big endian order
void CWackyDlgAsset::SwapProperties( TWackyDlgProperties& saveProps )
{
	// byte swap it gently but not too gently
#ifdef WINDOWS
	::SwapLong( saveProps.version );
	::SwapLong( saveProps.size );
	::SwapShort( saveProps.fontSize );
	::SwapShort( saveProps.colorBits );
	::SwapLong( saveProps.radioGroupButton );
	::SwapLong( saveProps.checkboxGroupButton );
	::SwapLong( saveProps.checkbox1State );
	::SwapLong( saveProps.checkbox2State );
#endif
#if (defined(__APPLE__) && defined(__MACH__) && defined(__LITTLE_ENDIAN__))
	saveProps.version = Endian32_Swap( saveProps.version );
	saveProps.size = Endian32_Swap( saveProps.size );
	saveProps.fontSize = Endian16_Swap( saveProps.fontSize );
	saveProps.colorBits = Endian16_Swap( saveProps.colorBits );
	saveProps.radioGroupButton = Endian32_Swap( saveProps.radioGroupButton );
	saveProps.checkboxGroupButton = Endian32_Swap( saveProps.checkboxGroupButton );
	saveProps.checkbox1State = Endian32_Swap( saveProps.checkbox1State );
	saveProps.checkbox2State = Endian32_Swap( saveProps.checkbox2State );
#endif
	//  strings are byte arrays, so they don't need swapping.
}


MoaLong CWackyDlgAsset::NativeTextWidth( PIMoaMmGC pGC, PMoaChar outString, ConstPMoaChar fontName, MoaShort size )
{
	if( !pGC )
		return 0;

	MoaMmNativeGCInfo nativeGC;
	pGC->GetNativeGCInfo( &nativeGC );

#ifdef MACINTOSH
	// on Mac we assume the port is already set up!

	// translate style - on mac these match
	short fontID = applFont;
	Str255 mmFontName;

	if( fontName && fontName[0] )
	{
		// convert to pascal again
		int slen = ::strlen( fontName );
		if (slen>255)
			slen = 255;
		mmFontName[0] = slen;
		::strncpy( (char*)&mmFontName[1], fontName, mmFontName[0] );
		::GetFNum( mmFontName, &fontID );
	}

	FontHelper 	setSize( fontID, size );
	
	return ::TextWidth( outString, 0, ::strlen( outString ) );
#else
	// WINDOWS

	HDC	 hDC = (HDC)nativeGC.data.win_window.hDC;
	SIZE textDimensions = {0,0};
	HFONT	newFace = NULL;

	if( !fontName )
		newFace = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	else
		newFace = ::CreateFontA(  -size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
								  OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
								  DEFAULT_PITCH, fontName );

	if( newFace )
	{
		HGDIOBJ oldObj = ::SelectObject( hDC, newFace );
		::GetTextExtentPoint32A( hDC, outString, ::strlen( outString ), &textDimensions );
		::SelectObject( hDC, oldObj );
		::DeleteObject( newFace );
	}

	// return only the width in this case
	return textDimensions.cx;

#endif
}


void CWackyDlgAsset::NativeTextOut( PIMoaMmGC pGC, MoaLong x, MoaLong y, PMoaChar outString, ConstPMoaChar fontName, MoaShort size )
{
	if( !pGC )
		return;

	MoaMmNativeGCInfo nativeGC;
	pGC->GetNativeGCInfo( &nativeGC );

#ifdef MACINTOSH
	// on Mac we assume the port is set up!

	// translate style - on mac these match
	short fontID = applFont;
	Str255 mmFontName;

	if( fontName && fontName[0] )
	{
		// convert to pascal again
		int slen = ::strlen( fontName );
		if (slen>255)
			slen = 255;
		mmFontName[0] = slen;
		::strncpy( (char*)&mmFontName[1], fontName, mmFontName[0] );
		::GetFNum( mmFontName, &fontID );
	}

	FontHelper 	setSize( fontID, size );
	
	::MoveTo( x, y );
	::DrawText( outString, 0, ::strlen( outString ) );
#else
	// WINDOWS
	HDC		hDC		= (HDC)nativeGC.data.win_window.hDC;
	HFONT	newFace = NULL;

	if( !fontName )
		newFace = (HFONT)::GetStockObject( DEFAULT_GUI_FONT );
	else
		newFace = ::CreateFontA(  -size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
								  OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
								  DEFAULT_PITCH, fontName );

	if( newFace )
	{
		HGDIOBJ oldObj = ::SelectObject( hDC, newFace );

		::TextOutA( hDC, x, y, outString, ::strlen( outString ) );
		::SelectObject( hDC, oldObj );
		::DeleteObject( newFace );
	}
#endif
}



///////////////////////////////////////////////////////////////////////////////
//
// Interface construction
//
///////////////////////////////////////////////////////////////////////////////

/* macros that create constructors and destructors for the following interface(s) */
STD_INTERFACE_CREATE_DESTROY( CWackyDlgAsset, IMoaMmXAsset )

#ifdef MACINTOSH
#pragma mark -
#endif


/* ------------------------------------------- CWackyDlgAsset_IMoaMmXAsset::GetProp */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::GetProp( MoaMmSymbol /*symbol*/, PMoaMmValue /*pValue*/ )
{
	XMOAEXCEPT_ENTER
	X_STD_RETURN( kMoaMmErr_PropertyNotFound );
	XMOAEXCEPT_EXIT
}

/* ------------------------------------------- CWackyDlgAsset_IMoaMmXAsset::SetProp */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::SetProp( MoaMmSymbol /*symbol*/, ConstPMoaMmValue /*pPropValue*/ )
{
	XMOAEXCEPT_ENTER
	X_STD_RETURN( kMoaMmErr_PropertyNotFound );
	XMOAEXCEPT_EXIT
}

/* -------------------------------------- CWackyDlgAsset_IMoaMmXAsset::CallFunction */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::CallFunction( MoaMmSymbol /*method*/, MoaLong /*nArgs*/, ConstPMoaMmValue /*pArgs*/, PMoaMmValue /*pResult*/ )
{
	XMOAEXCEPT_ENTER
	X_STD_RETURN( kMoaMmErr_FunctionNotFound );
	XMOAEXCEPT_EXIT
}


/* --------------------------------------- CWackyDlgAsset_IMoaMmXAsset::SetCallback */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::SetCallback( PIMoaMmAssetCallback pAssetCallback )
{
	XMOAEXCEPT_ENTER

	// be safe, query for the newer one -- note this gets disposed in MoaDestroy
	pAssetCallback->QueryInterface( &IID(IMoaMmAssetCallback2), (PPMoaVoid)&pObj->mAssetCallback );

	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

// This method is intended for communicating between the asset and its xtras.
//   The developer is free to define message codes for private use.  This may be
//   called directly by an actor, or through the IMoaMmSpriteCallback::NotifyAsset() callback.
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::Notify( MoaLong /*msgCode*/, PMoaVoid /*refCon*/ )
{
	XMOAEXCEPT_ENTER
	MoaError err = kMoaErr_NoErr;
	X_STD_RETURN( err );
	XMOAEXCEPT_EXIT
}

/* ------------------------------------ CWackyDlgAsset_IMoaMmXAsset::GetDefaultRect */
// We return our default rectangle centered around (0,0).  (0,0) here
//   corresponds to the "loc" of the sprite, whichis the point of reference for
//   the sprite.  Dragging the sprite out to the stage will position the sprite
//   such that the loc is at the mouse drop position.
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::GetDefaultRect( PMoaRect pRect )
{
	XMOAEXCEPT_ENTER
	
	// really small
	pRect->top 		= -kDefaultRectBiSection;
	pRect->left		= -kDefaultRectBiSection;
	pRect->bottom	= kDefaultRectBiSection;
	pRect->right	= kDefaultRectBiSection;
	
	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* --------------------------------- CWackyDlgAsset_IMoaMmXAsset::PrepareNewContent */
// This method is called to have the class set-up its default state when a new
//   cast member is created (note that this is different from when a new
//   instance of CWackyDlgAsset is created; instances of the XAsset are created at both
//   movie-open time (for pre-existing cast members being read-in) and new cast
//   member creation time.  PrepareNewContent() is only called at new cast
//   member creation time, just after the XAsset instance is created. 
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::PrepareNewContent( MoaMmNewFlags newFlags )
{
	XMOAEXCEPT_ENTER

#ifdef SPEW_STREAM_MESSAGES
	IMmUtilsHelper().Use()->PrintMessage( "WackyDlg: prepare new content\n" );
#endif
	
	// A new cast member is being created in memory.  Set up default media and props
	MoaError err = kMoaErr_NoErr;
	
	// Initialize props / media to default values here. 
	//   The host app assumes that both props and media are "dirty" when a new cast member is created. 
	if( (newFlags & kMoaMmNewFlags_UiPermitted) == kMoaMmNewFlags_UiPermitted )
	{
		// put up our Dialog, if successful (they didn't press cancel) return normally
		err = InvokePropsEditor();
		
		MoaMmModifyFlags flags = kMoaMmModifyFlags_Props;
		FinishPropsEditor( err == kMoaErr_NoErr, &flags );
	}
	
	//   We are now "loaded" in the sense that we have everything we need from 
	//   disk (there isn't anything on disk yet, so we exist only in-memory 
	//   at this point)

	X_STD_RETURN( err );
	XMOAEXCEPT_EXIT
}


/* ------------------------------------ CWackyDlgAsset_IMoaMmXAsset::StreamOutMedia */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::StreamOutMedia( PIMoaStream /*pOutStream*/ )
{
	XMOAEXCEPT_ENTER
	
#ifdef SPEW_STREAM_MESSAGES
	IMmUtilsHelper().Use()->PrintMessage( "WackyDlg: stream out media\n" );
#endif

	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* ------------------------------------- CWackyDlgAsset_IMoaMmXAsset::StreamInMedia */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::StreamInMedia( PIMoaStream /*pInputStream*/ )
{
	XMOAEXCEPT_ENTER

#ifdef SPEW_STREAM_MESSAGES
	IMmUtilsHelper().Use()->PrintMessage( "WackyDlg: stream in media\n" );
#endif

	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* ----------------------------------------- CWackyDlgAsset_IMoaMmXAsset::FreeMedia */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::FreeMedia()
{
	XMOAEXCEPT_ENTER
	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* --------------------------------- CWackyDlgAsset_IMoaMmXAsset::GetInMemMediaSize */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::GetInMemMediaSize( MoaUlong * pSizeBytes )
{
	XMOAEXCEPT_ENTER

	*pSizeBytes = 0;

	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* ----------------------------- CWackyDlgAsset_IMoaMmXAsset::GetStreamOutMediaSize */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::GetStreamOutMediaSize( MoaUlong * pSizeBytes )
{
	XMOAEXCEPT_ENTER

	*pSizeBytes = 0;
	
	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* ------------------------------------ CWackyDlgAsset_IMoaMmXAsset::StreamOutProps */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::StreamOutProps( PIMoaStream pOutStream )
{
	XMOAEXCEPT_ENTER

#ifdef SPEW_STREAM_MESSAGES
	IMmUtilsHelper().Use()->PrintMessage( "WackyDlg: stream out props\n" );
#endif

	TWackyDlgProperties  tempProps = pObj->mProps;
	TMoaError err;

	try
	{
		// open that puppy
		err = pOutStream->Open( kMoaStreamOpenAccess_WriteOnly, kMoaStreamSetPositionType_None );

		MoaUlong writeSize = sizeof( TWackyDlgProperties );
		pObj->SwapProperties( tempProps );
		
		// stream out the header
		err = pOutStream->Write( &tempProps, writeSize, &writeSize );
		if( writeSize != sizeof( TWackyDlgProperties ) )
			kThrowMoaException( CMoaLengthError );		

	}
	catch( CMoaException& ex )
	{
		// close the stream and bail out of here.
		pOutStream->Close();
			
		throw ex;
	}

	pOutStream->Close();

	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* ------------------------------------- CWackyDlgAsset_IMoaMmXAsset::StreamInProps */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::StreamInProps( PIMoaStream pInStream )
{
	XMOAEXCEPT_ENTER
	
	TWackyDlgProperties 	saveProps;
	TMoaError 			err;			// this is a throwing error class

#ifdef SPEW_STREAM_MESSAGES
	IMmUtilsHelper().Use()->PrintMessage( "WackyDlg: stream in props\n" );
#endif

	try
	{
		// open that puppy
		err = pInStream->Open( kMoaStreamOpenAccess_ReadOnly, kMoaStreamSetPositionType_None );

		// to stream in the properties, we basically read everything using the TWackyDlgProperties struct
		MoaUlong 				readSize = 0;
		
		// stream in the header
		err = pInStream->Read( &saveProps, sizeof( TWackyDlgProperties ), &readSize );
		if( readSize != sizeof( TWackyDlgProperties ) )
			kThrowMoaException( CMoaLengthError );		
		pObj->SwapProperties( saveProps );

		// here we need to check to see if the version is correct
		if( saveProps.version != kAssetPropertiesVersion )
			kThrowMoaExceptionReason( CMoaRuntimeError, "The properties for this asset are not of the current version!" );

	}
	catch( CMoaException& ex )
	{
		// close the stream
		pInStream->Close();
		
		// keep on going.		
		throw ex;
	}

	// finally overwrite the options with the ones we loaded.
	pObj->mProps = saveProps;

	pInStream->Close();

	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* ----------------------------------------- CWackyDlgAsset_IMoaMmXAsset::FreeProps */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::FreeProps()
{
	XMOAEXCEPT_ENTER
	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* --------------------------------- CWackyDlgAsset_IMoaMmXAsset::GetInMemPropsSize */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::GetInMemPropsSize( MoaUlong * pSizeBytes )
{
	XMOAEXCEPT_ENTER

	*pSizeBytes = 0;
	*pSizeBytes += sizeof( TWackyDlgProperties );
	
	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* ----------------------------- CWackyDlgAsset_IMoaMmXAsset::GetStreamOutPropsSize */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::GetStreamOutPropsSize( MoaUlong * pSizeBytes )
{
	XMOAEXCEPT_ENTER

	*pSizeBytes = 0;
	*pSizeBytes += sizeof( TWackyDlgProperties );
	
	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* ----------------------------------------- CWackyDlgAsset_IMoaMmXAsset::NewXActor */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::NewXActor( PPMoaVoid ppXActor ) 
{
	XMOAEXCEPT_ENTER

	// don't freak out if we don't have a sprite, this is okay -- You may decide this is not okay, in that case return an error.
	// do no error handling here on purpose, we are still safe however
	// in general the sprite is only for authoring (to help identify the fonts)
	PIMoaMmXSpriteActor pNewActor = NULL;

	// Do a createinstance using IMoaMmXSpriteActor for return to caller...
	XSupport::GetCallback()->MoaCreateInstance( &CLSID(CWackyDlgSprite), &IID(IMoaMmXSpriteActor), (PPMoaVoid)&pNewActor );

	// Tell actor who its parent is, in case it's interested. 
	//	It should AddRef() on the interface if it wants to hold onto it.
	if( pNewActor )
		pNewActor->Notify( kSprNotifyMsg_SetParentAsset, this );
	
	// stuff result				
	*ppXActor = pNewActor;

	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* --------------------------------- CWackyDlgAsset_IMoaMmXAsset::InvokeMediaEditor */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::InvokeMediaEditor( PMoaMmModifyFlags )
{
	XMOAEXCEPT_ENTER

	// see other samples for how to implement a media editor in Director

	X_STD_RETURN(kMoaErr_NoErr);
	XMOAEXCEPT_EXIT
}

/* --------------------------------- CWackyDlgAsset_IMoaMmXAsset::InvokePropsEditor */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::InvokePropsEditor()
{
	XMOAEXCEPT_ENTER
	MoaError 			err 	= kMoaErr_NoErr;
	PIMoaPropsDialog 	pDialog = NULL;

	// construct our props editor
	XSupport::GetCallback()->MoaCreateInstance( &CLSID(CMoaPropsDialog), &IID(IMoaPropsDialog), (PPMoaVoid)&pDialog );
	
	if( !pDialog )
		kThrowMoaExceptionReason( CMoaBadInterface, "Hey we can't find the WackyDlgDialog Xtra!" );
		
	PIMoaDrAssetCallback pDcbk = NULL;
	pObj->mAssetCallback->QueryInterface( &IID(IMoaDrAssetCallback), (PPMoaVoid)&pDcbk );

	// it's okay for the director callback to be NULL - we could be in Authorware although very unlikely!@
	// NOTE: this interface is defined by you, you can change it in anyway you want and add as many function as necessary.
	err = pDialog->InvokePropsDialog( this, pDcbk, pObj->mAssetCallback );
	pDialog->Release();
	
	if( pDcbk )
		pDcbk->Release();
	
	if( !err )
	{
		// yeah, we are happy. Do something interesting, only if you want to.
	}
	
	X_STD_RETURN( err );
	XMOAEXCEPT_EXIT
}

/* --------------------------------- CWackyDlgAsset_IMoaMmXAsset::FinishPropsEditor */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::FinishPropsEditor( MoaBoolParam /*commitChanges*/, PMoaMmModifyFlags /*pModifyFlags*/ )
{
	XMOAEXCEPT_ENTER
	X_STD_RETURN(kMoaErr_NoErr);
	XMOAEXCEPT_EXIT
}

/* ------------------------------------- CWackyDlgAsset_IMoaMmXAsset::InvokeAboutBox */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::InvokeAboutBox()
{
	XMOAEXCEPT_ENTER

#ifdef SPEW_STREAM_MESSAGES
	pObj->mIMoaMmUtils->PrintMessage( "invoke about\n" );
#endif

	X_STD_RETURN(kMoaMmErr_NotImplemented);
	XMOAEXCEPT_EXIT
}

/* ---------------------------------- CWackyDlgAsset_IMoaMmXAsset::ImageStaticThumb */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::ImageStaticThumb( PIMoaMmGC pGC, ConstPMoaRect pImageRect )
{
	XMOAEXCEPT_ENTER

	if( !pGC )
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

	// test color bits for more excitement
	if( !(pObj->mProps.colorBits & kShowRed) )
		foreColor.red = 0;
	if( !(pObj->mProps.colorBits & kShowGreen) )
		foreColor.green = 0;
	if( !(pObj->mProps.colorBits & kShowBlue) )
		foreColor.blue = 0;

	// invert black
	if( !foreColor.red && !foreColor.green && !foreColor.blue )
		foreColor.red = foreColor.green = foreColor.blue = 0xFFFF;

	SetColorAndRestore newColors( &foreColor, &backColor );
	
	// use current background color...
	Rect macRect = { (short)pImageRect->top, (short)pImageRect->left, (short)pImageRect->bottom, (short)pImageRect->right };
	::EraseRect( &macRect );
#else

	// WINDOWS
	HDC		hDC = (HDC)moaGCInfo.data.win_window.hDC;
	MoaByte fr = 0xCC, fg = 0xCC, fb = 0xCC;
	RECT	winRect = { pImageRect->left, pImageRect->top, pImageRect->right, pImageRect->bottom };

	// test color bits for more excitement
	if( !(pObj->mProps.colorBits & kShowRed) )
		fr = 0;
	if( !(pObj->mProps.colorBits & kShowGreen) )
		fg = 0;
	if( !(pObj->mProps.colorBits & kShowBlue) )
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

	// take some temporaries to screw with.
	MoaShort fontSize = pObj->mProps.fontSize;
	MoaLong  spriteWidth  = pImageRect->right - pImageRect->left;
	MoaLong  spriteHeight = pImageRect->bottom - pImageRect->top;

	MoaShort textWidth = (MoaShort)pObj->NativeTextWidth( pGC, pObj->mProps.outputString, pObj->mProps.fontName, fontSize );
	
	// check to see if the text is too big
	if( textWidth > spriteWidth || fontSize > spriteHeight )
	{
		fontSize = 9;
		textWidth = (MoaShort)pObj->NativeTextWidth( pGC, pObj->mProps.outputString, pObj->mProps.fontName, fontSize );
	}

	MoaLong  xPosition = ((spriteWidth - textWidth) / 2) + pImageRect->left;

#ifdef MACINTOSH
	// we take advantage of the base line
	MoaLong  yPosition = ((spriteHeight + fontSize) / 2) + pImageRect->top;
#else
	MoaLong  yPosition = ((spriteHeight - fontSize) / 2) + pImageRect->top;
#endif

	pObj->NativeTextOut( pGC, xPosition, yPosition, pObj->mProps.outputString, pObj->mProps.fontName, fontSize );

	X_STD_RETURN( kMoaErr_NoErr );
	XMOAEXCEPT_EXIT
}

/* ------------------------------- CWackyDlgAsset_IMoaMmXAsset::BeginImageAnimThumb */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::BeginImageAnimThumb( PIMoaMmGC /*pGC*/, ConstPMoaRect /*pImageRect*/ )
{
	X_ENTER
	X_STD_RETURN(kMoaMmErr_NotImplemented);
	X_EXIT
}

/* ----------------------------- CWackyDlgAsset_IMoaMmXAsset::ServiceImageAnimThumb */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::ServiceImageAnimThumb( PIMoaMmGC /*pGC*/, ConstPMoaRect /*pImageRect*/ )
{
	X_ENTER
	X_STD_RETURN(kMoaMmErr_NotImplemented);
	X_EXIT
}

/* --------------------------------- CWackyDlgAsset_IMoaMmXAsset::EndImageAnimThumb */
STDMETHODIMP CWackyDlgAsset_IMoaMmXAsset::EndImageAnimThumb( PIMoaMmGC /*pGC*/, ConstPMoaRect /*pImageRect*/ )
{
	X_ENTER
	X_STD_RETURN(kMoaMmErr_NotImplemented);
	X_EXIT
}

// EOF
