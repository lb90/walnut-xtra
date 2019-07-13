/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_cutool
#define _H_cutool

/*****************************************************************************
 *
 *	Name: cutool.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for the ComputerUser tool class.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 ****************************************************************************/ 
#include "moaxtra.h"
#include "mmixtool.h"
#include "driservc.h"

#ifdef MACINTOSH
	#include <windows.h>
	#include <string.h>
#endif

/*****************************************************************************
 *  CONFIGURATION DEFINE(S)
 ****************************************************************************/ 

/*****************************************************************************
 *	CLASS SPECIFIC DEFINE(S)
 ****************************************************************************/ 

/*****************************************************************************
 *	CLSID
 ****************************************************************************/ 
DEFINE_GUID(CLSID(CUTool), 0xE9CB3F47L, 0xE2F4, 0x4C00, 0x8D, 0xE0, 0xB0, 0xE7, 0x32, 0x93, 0xE5, 0x57);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CUTool)
	PIMoaDrCastMem mDrCastMem;
	PIMoaDrScoreAccess mDrScore;
	PIMoaMmValue mMmValue;
	PIMoaDrMovie mDrMovie;
	MoaDrMemberIndex mIndex;
	MoaDrFrameIndex mFrame;
	MoaMmSymbol mSymMember;
	MoaMmSymbol mSymLoc;
	MoaMmSymbol mSymTweened;
	MoaMmSymbol mSymText;
	MoaMmSymbol mSymPathname;
	MoaMmSymbol mSymName;
	MoaMmSymbol mSymFrame;
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

/*****************************************************************************
 *  CLASS INTERFACE(S)
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CUTool, IMoaMmXTool)
	EXTERN_DEFINE_METHOD(MoaError, Invoke, ())
	EXTERN_DEFINE_METHOD(MoaError, GetEnabled, (PMoaDrEnabledState))
	EXTERN_DEFINE_METHOD(MoaError, CreateNewTextMember, (PIMoaDrCast))
	EXTERN_DEFINE_METHOD(MoaError, AddTextToMember, ())
	EXTERN_DEFINE_METHOD(MoaError, AddNewSprite, ())
	EXTERN_DEFINE_METHOD(MoaError, GetSymbols, ())
	EXTERN_DEFINE_METHOD(MoaError, ComposeStrings, (MoaMmValue *))	
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif /* _H_cutool */
