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
 *	Name: asset.h
 *	
 * 	Purpose: Definitions of Asset class interface(s) and methods for the 
 *           Oval Sprite Xtra example.
 *
 ****************************************************************************/ 

#ifndef _H_Asset
#define _H_Asset
	
///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////

#include "moaxtra.h"
#include "mmixasst.h"
#include "driservc.h"


///////////////////////////////////////////////////////////////////////////////
//
// Private file save data record
//
///////////////////////////////////////////////////////////////////////////////

enum
{
	kShowRed 	= 1,
	kShowGreen	= 1 << 1,
	kShowBlue	= 1 << 2
}; 
typedef short TColorBits;


typedef struct
{
	MoaUlong		version;			
	MoaUlong		size;				// set to total size of structure
	MoaShort		fontSize;			// text size of output string
	TColorBits		colorBits;			// these are just for fun
	MoaChar			fontName[63];		// small string to hold name of font
	MoaChar			outputString[256];	// small string to output
	MoaChar			editString[256];	// small string for middle edit box
	MoaChar			watchString[256];	// small string to hold watch string
	MoaLong			radioGroupButton;	// which button is checked in Radio radio group
	MoaLong			checkboxGroupButton;// which button is checked in Checkbox radio group
	MoaChar			popupItemString[32];// which popup item is selected
	MoaLong			checkbox1State;		// state of checkbox one in Radio radio group
	MoaLong			checkbox2State;		// state of checkbox two in Radio radio group
} TWackyDlgProperties, * PTWackyDlgProperties;


///////////////////////////////////////////////////////////////////////////////
//
// Class Data
//
///////////////////////////////////////////////////////////////////////////////

// you can define your own messages in the same fashion as below
const MoaLong	kSprNotifyMsg_SetParentAsset 	= 1;
const MoaLong	kSprNotifyMsg_Redraw		 	= 2;

const MoaUlong	kAssetPropertiesVersion		 	= 1;

// {664C4541-1AD8-11d3-8635-00104B27A1A1}
DEFINE_GUID( CLSID(CWackyDlgRegister), 0x664c4541, 0x1ad8, 0x11d3, 0x86, 0x35, 0x0, 0x10, 0x4b, 0x27, 0xa1, 0xa1 );

// {664C4542-1AD8-11d3-8635-00104B27A1A1}
DEFINE_GUID( CLSID(CWackyDlgAsset), 0x664c4542, 0x1ad8, 0x11d3, 0x86, 0x35, 0x0, 0x10, 0x4b, 0x27, 0xa1, 0xa1 );

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS( CWackyDlgRegister )
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

// these are class instance variables (accessed from the interface class through pObj)
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS( CWackyDlgAsset )
	// Callback interfaces
	PIMoaMmAssetCallback2	mAssetCallback;
	PIMoaMmSpriteCallback	mSpriteCallback; // this is only valid when the sprite is around
	
	// these are here for easier quicker access	
	PIMoaHandle				mIMoaHandle;
	PIMoaDrMovie			mIMoaDrMovie;
	PIMoaMmUtils			mIMoaMmUtils;
	PIMoaMmList				mIMoaMmList;

	// these are the current properties for this asset.
	TWackyDlgProperties		mProps;

	void SwapProperties( TWackyDlgProperties& saveProps );

	// you would really expect these to be in the sprite but we use it in both places.
	void 	NativeTextOut( PIMoaMmGC pGC, MoaLong x, MoaLong y, PMoaChar outString, ConstPMoaChar fontName, MoaShort size );
	MoaLong NativeTextWidth( PIMoaMmGC pGC, PMoaChar outString, ConstPMoaChar fontName, MoaShort size );
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

///////////////////////////////////////////////////////////////////////////////
//
// Class implementation definition
//
///////////////////////////////////////////////////////////////////////////////

// this is the interface object implementation
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE( CWackyDlgRegister, IMoaRegister ) 
	EXTERN_DEFINE_METHOD(MoaError, Register, (PIMoaCache, PIMoaDict))
	
private:
	EXTERN_DEFINE_METHOD(MoaError, RegisterIcon, (PIMoaDict pDict, ConstPMoaChar pResName, MoaBoolParam bColor))
EXTERN_END_DEFINE_CLASS_INTERFACE

// these are the standard asset interfaces that need to be implemented...
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE( CWackyDlgAsset, IMoaMmXAsset )
	EXTERN_DEFINE_METHOD(MoaError, GetProp, (MoaMmSymbol, PMoaMmValue))		
	EXTERN_DEFINE_METHOD(MoaError, SetProp, (MoaMmSymbol, ConstPMoaMmValue))	
	EXTERN_DEFINE_METHOD(MoaError, CallFunction, (MoaMmSymbol, MoaLong, ConstPMoaMmValue, PMoaMmValue))		
	EXTERN_DEFINE_METHOD(MoaError, SetCallback, (PIMoaMmAssetCallback))		
	EXTERN_DEFINE_METHOD(MoaError, Notify, (MoaLong, PMoaVoid))		
	EXTERN_DEFINE_METHOD(MoaError, GetDefaultRect, (PMoaRect))		
	EXTERN_DEFINE_METHOD(MoaError, PrepareNewContent, (MoaMmNewFlags))		
	EXTERN_DEFINE_METHOD(MoaError, StreamOutMedia, (PIMoaStream))		
	EXTERN_DEFINE_METHOD(MoaError, StreamInMedia, (PIMoaStream))		
	EXTERN_DEFINE_METHOD(MoaError, FreeMedia, ())		
	EXTERN_DEFINE_METHOD(MoaError, GetInMemMediaSize, (MoaUlong *))		
	EXTERN_DEFINE_METHOD(MoaError, GetStreamOutMediaSize, (MoaUlong *))		
	EXTERN_DEFINE_METHOD(MoaError, StreamOutProps, (PIMoaStream))		
	EXTERN_DEFINE_METHOD(MoaError, StreamInProps, (PIMoaStream))		
	EXTERN_DEFINE_METHOD(MoaError, FreeProps, ())		
	EXTERN_DEFINE_METHOD(MoaError, GetInMemPropsSize, (MoaUlong *))		
	EXTERN_DEFINE_METHOD(MoaError, GetStreamOutPropsSize, (MoaUlong *))		
	EXTERN_DEFINE_METHOD(MoaError, NewXActor, (PMoaVoid *))  
	EXTERN_DEFINE_METHOD(MoaError, InvokeMediaEditor, (PMoaMmModifyFlags))
	EXTERN_DEFINE_METHOD(MoaError, InvokePropsEditor, ())
	EXTERN_DEFINE_METHOD(MoaError, FinishPropsEditor, (MoaBoolParam, PMoaMmModifyFlags))
	EXTERN_DEFINE_METHOD(MoaError, InvokeAboutBox, ())
	EXTERN_DEFINE_METHOD(MoaError, ImageStaticThumb, (PIMoaMmGC, ConstPMoaRect))		
	EXTERN_DEFINE_METHOD(MoaError, BeginImageAnimThumb, (PIMoaMmGC, ConstPMoaRect))		
	EXTERN_DEFINE_METHOD(MoaError, ServiceImageAnimThumb, (PIMoaMmGC, ConstPMoaRect))		
	EXTERN_DEFINE_METHOD(MoaError, EndImageAnimThumb, (PIMoaMmGC, ConstPMoaRect))		
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif // !_H_Asset

// EOF
