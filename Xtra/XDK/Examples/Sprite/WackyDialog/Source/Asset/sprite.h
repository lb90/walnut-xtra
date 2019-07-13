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
 *	Name: sprite.h
 *
 ****************************************************************************/ 

#ifndef _H_Sprite
#define _H_Sprite

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////

#include "moaxtra.h"
#include "mmixasst.h"

#include "Asset.h"


///////////////////////////////////////////////////////////////////////////////
//
// Declares
//
///////////////////////////////////////////////////////////////////////////////

// {664C4543-1AD8-11d3-8635-00104B27A1A1}
DEFINE_GUID( CLSID(CWackyDlgSprite), 0x664c4543, 0x1ad8, 0x11d3, 0x86, 0x35, 0x0, 0x10, 0x4b, 0x27, 0xa1, 0xa1);

///////////////////////////////////////////////////////////////////////////////
//
// Object Instance variables
//
///////////////////////////////////////////////////////////////////////////////

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS( CWackyDlgSprite )
	// direct access to Asset that were are linked to
	CWackyDlgAsset_IMoaMmXAsset *	mAsset;				
	
	// this provides access to common sprite services
	PIMoaMmSpriteCallback	mSpriteCallback;
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

///////////////////////////////////////////////////////////////////////////////
//
// Interface method implementation declaration
//
///////////////////////////////////////////////////////////////////////////////

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE( CWackyDlgSprite, IMoaMmXSpriteActor )
	EXTERN_DEFINE_METHOD( MoaError, GetProp, 		(MoaMmSymbol, PMoaMmValue) )		
	EXTERN_DEFINE_METHOD( MoaError, SetProp, 		(MoaMmSymbol, ConstPMoaMmValue) )		
	EXTERN_DEFINE_METHOD( MoaError, CallFunction, 	(MoaMmSymbol, MoaLong, ConstPMoaMmValue,PMoaMmValue) )		
	EXTERN_DEFINE_METHOD( MoaError, SetCallback, 	(PIMoaMmSpriteCallback) )		
	EXTERN_DEFINE_METHOD( MoaError, Notify, 		(MoaLong, PMoaVoid) )		
	EXTERN_DEFINE_METHOD( MoaError, GetCaps, 		(PMoaMmXSpriteCaps) )		
	EXTERN_DEFINE_METHOD( MoaError, CollectChanges,	(ConstPMoaMmXSpriteInfo, PIMoaMmChangeCollector) )		
	EXTERN_DEFINE_METHOD( MoaError, ContainsPoint,	(ConstPMoaMmXSpriteInfo, ConstPMoaPoint,	MoaBoolParam *) )		
	EXTERN_DEFINE_METHOD( MoaError, Image, 			(ConstPMoaMmXSpriteInfo, ConstPMoaMmPlayerStateInfo, PIMoaMmGC) )		
	EXTERN_DEFINE_METHOD( MoaError, Event,			(ConstPMoaMmEventInfo, ConstPMoaMmPlayerStateInfo, MoaBoolParam *) )		
EXTERN_END_DEFINE_CLASS_INTERFACE


#endif // !_H_Sprite
