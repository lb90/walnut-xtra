/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2007 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_csprite
#define _H_csprite

/*****************************************************************************
 *
 *	Name: PlaySoundSprite.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for the PlaySound Xtra's Sprite class.
 *
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  XTRA SPECIFIC DEFINE(S)
 *  -----------------------
 *	Any #define's specific to this Xtra.  
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .h file should automatically include all the support files needed for
 *	this particular class. In addition, this file may include other .h files
 *	defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#ifndef _H_moaxtra
#include "moaxtra.h"
#endif

#ifndef _H_mmixasst
#include "mmixasst.h"
#endif

#ifndef DRISERVC_H
	#include "driservc.h"
#endif

#ifndef _H_MoaFile2_
	#include "moafile2.h"
#endif

#ifndef _H_casset
	#include "PlaySoundAsset.h"
#endif

/*****************************************************************************
 *	CLASS SPECIFIC DEFINE(S)
 *	-----------------------
 *	Any #define's specific to this class 
 ****************************************************************************/ 
#ifndef UNUSED
	#define UNUSED(x) x
#endif

/*****************************************************************************
 *  CLSID
 *  -----
 *  The CLSID is a GUID that unquely identifies your MOA class.  To generate a
 *  GUID, use the genUID.app utility (Mac) or the GUIDGEN.EXE utility (Win).
 ****************************************************************************/ 
DEFINE_GUID(CLSID(CPlaySoundSprite), 0xAED05242L, 0x0005, 0x23B5, 0x00, 0x00, 0x08, 0x00, 0x07, 0xD7, 0xF1, 0x74);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *  ------------------------
 *  Class instance variable are variables whose scope is exclusive to the
 *  methods implemented by your MOA class.  Variables necessary for the
 *  implementation of your MOA class should be placed here.
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CPlaySoundSprite)
	PIMoaStream2	pStrm;
	MoaUlong		cid;
	MoaBool			playing;
	PIMoaDrSound 	pSnd;
	PIMoaFile2		pFile;
	PIMoaMmSpriteCallback pSpriteCallback;
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

/*****************************************************************************
 *  CLASS INTERFACE(S)
 *  ------------------
 *  The interface(s) implemented by your MOA class are specified here.  Note
 *	that at least one class in your Xtra should implement the IMoaRegister
 *	interface.
 *  NOTE: Because C++ does not use a lpVtbl to reference an interface's methods, 
 *  the actual method declaration is done here.
 *
 *  Syntax:
 *  EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(<class-name>, <interface-name>) 
 *		EXTERN_DEFINE_METHOD(<return-type>, <method-name>,(<argument-list>)) 
 *	EXTERN_END_DEFINE_CLASS_INTERFACE
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CPlaySoundSprite, IMoaMmXSpriteActor)
	EXTERN_DEFINE_METHOD(MoaError, GetProp, (MoaMmSymbol, PMoaMmValue))		
	EXTERN_DEFINE_METHOD(MoaError, SetProp, (MoaMmSymbol, ConstPMoaMmValue))		
	EXTERN_DEFINE_METHOD(MoaError, CallFunction, (MoaMmSymbol, MoaLong, ConstPMoaMmValue,PMoaMmValue))		
	EXTERN_DEFINE_METHOD(MoaError, SetCallback, (PIMoaMmSpriteCallback))		
	EXTERN_DEFINE_METHOD(MoaError, Notify, (MoaLong, PMoaVoid))		
	EXTERN_DEFINE_METHOD(MoaError, GetCaps, (PMoaMmXSpriteCaps))		
	EXTERN_DEFINE_METHOD(MoaError, CollectChanges, (ConstPMoaMmXSpriteInfo, PIMoaMmChangeCollector))		
	EXTERN_DEFINE_METHOD(MoaError, ContainsPoint, (ConstPMoaMmXSpriteInfo, ConstPMoaPoint,	MoaBoolParam *))		
	EXTERN_DEFINE_METHOD(MoaError, Image, (ConstPMoaMmXSpriteInfo, ConstPMoaMmPlayerStateInfo, PIMoaMmGC))		
	EXTERN_DEFINE_METHOD(MoaError, Event, (ConstPMoaMmEventInfo, ConstPMoaMmPlayerStateInfo, MoaBoolParam *))
private:
	EXTERN_DEFINE_METHOD(MoaError, _GetStream, ())
	EXTERN_DEFINE_METHOD(MoaError, _Play, ())
	EXTERN_DEFINE_METHOD(MoaError, _Stop, ())
	
			
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif /* _H_csprite */
