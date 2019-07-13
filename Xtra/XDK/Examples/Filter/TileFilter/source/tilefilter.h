/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_tilefilter
#define _H_tilefilter

/*****************************************************************************
 *
 *	Name: tilefilter.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for a generic Xtra's filter class.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  XTRA SPECIFIC DEFINE(S)
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 ****************************************************************************/ 
#include "moaxtra.h"
#include "bmfilters.h"

/*****************************************************************************
 *	CLASS SPECIFIC DEFINE(S)
 ****************************************************************************/ 

/*****************************************************************************
 *  CLSID
 ****************************************************************************/ 
DEFINE_GUID(CLSID(TileFilter), 0x9A7C7BD5L, 0x35F1, 0x4925, 0xAA, 0x64, 0x81, 0xF3, 0x6A, 0x6A, 0xAF, 0x87);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(TileFilter)
	PIMoaMmUtils mUtils;
	MoaMmSymbol mSymStampSize;
	MoaMmSymbol mSymStyle;
	MoaMmSymbol mSymDbgPropList;
	MoaLong mSize;	// size of tile
	MoaLong mStyle; //
	MoaLong mParam1;
	MoaLong mParam2;
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

/*****************************************************************************
 *  CLASS INTERFACE(S)
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(TileFilter, IMoaBitmapFilter)
	EXTERN_DEFINE_METHOD(MoaError, GetProp, (THIS_ MoaMmSymbol, PMoaMmValue))
	EXTERN_DEFINE_METHOD(MoaError, SetProp, (THIS_ MoaMmSymbol, ConstPMoaMmValue))
	EXTERN_DEFINE_METHOD(MoaError, RunFilter, (THIS_ PIMoaMmGC, const PMoaFilterInfo))
	EXTERN_DEFINE_METHOD(MoaError, StreamOutProps, (THIS_ PIMoaStream))
	EXTERN_DEFINE_METHOD(MoaError, StreamInProps, (THIS_ PIMoaStream))
	EXTERN_DEFINE_METHOD(MoaError, GetStreamOutPropsSize, (THIS_ PMoaLong))
	EXTERN_DEFINE_METHOD(MoaError, InvokePropsEditor, (THIS_ ))
	EXTERN_DEFINE_METHOD(MoaError, FinishPropsEditor, (THIS_ MoaBoolParam, PMoaMmModifyFlags))
	EXTERN_DEFINE_METHOD(MoaError, GetOutputRect, (THIS_ const PMoaFilterInfo, PMoaFilterInfo))
	EXTERN_DEFINE_METHOD(MoaError, CallFunction, (THIS_ MoaMmSymbol, MoaLong, ConstPMoaMmValue, PMoaMmValue))
	EXTERN_DEFINE_METHOD(MoaError, GetFilterCaps, (THIS_ PMoaBMFilterCaps))
	EXTERN_DEFINE_METHOD(MoaError, SetCallback, (THIS_ PIMoaBitmapFilterCallback)) 
	EXTERN_DEFINE_METHOD(MoaError, Notify, (THIS_ MoaLong, PMoaVoid))
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif // _H_tilefilter
