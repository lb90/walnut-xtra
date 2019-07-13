/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_ovalactr
#define _H_ovalactr

/*****************************************************************************
 *
 *	Name: ovalactr.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for the Sprite Oval Xtra example Actor class.
 *
 *
 **********************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .h file should automatically include all the support files needed for this 
 *  particular class. In addition, this file may include other .h files defining 
 *  additional callback interfaces for use by a third party.   
 *****************************************************************************/ 
#include "moaxtra.h"
#include "mmixasst.h"

/*****************************************************************************
 *  FILE SPECIFIC DEFINE(S)
 *  -----------------------
 *	Any #define's specific to this file  
 *****************************************************************************/ 
#undef USING_INIT_FROM_DICT

/*****************************************************************************
 *  CLSID
 *  -----
 *  The CLSID is a GUID that unquely identifies your MOA class.  To generate a
 *  GUID, use the genUID.app utility (Mac) or the GUIDGEN.EXE utility (Win).
 *****************************************************************************/ 
DEFINE_GUID(CLSID(COvalSpriteActor), 0xAC404242L, 0x000A, 0x17DC, 0x00, 0x00, 0x08, 0x00, 0x07, 0x2C, 0x63, 0x26);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *  ------------------------
 *  Class instance variable are variables whose scope is exclusive to the
 *  methods implemented by your MOA class.  Variables necessary for the
 *  implementation of your MOA class should be placed here.
 *****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(COvalSpriteActor)
	PIMoaMmXAsset 	 	 pXAsset;
	MoaLong 			 state;	
	MoaLong				 stepState;
	MoaLong				 direction;
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

/*****************************************************************************
 *  CLASS INTERFACE(S)
 *  ------------------
 *  The interface(s) implemented by your MOA class are specified here.  Note that
 *  at least one class in your Xtra should implement the IMoaRegister interface.
 *  NOTE: Because C++ does not use a lpVtbl to reference an interface's methods, 
 *  the actual method declaration is done here.
 *
 *  Syntax:
 *  EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(<class-name>, <interface-name>) 
 *	EXTERN_DEFINE_METHOD(<return-type>, <method-name>,(<argument-list>)) 
 *****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(COvalSpriteActor, IMoaMmXSpriteActor)
	/* IMoaMmPropOwner methods */
		EXTERN_DEFINE_METHOD(MoaError, GetProp, (MoaMmSymbol, PMoaMmValue))		
		EXTERN_DEFINE_METHOD(MoaError, SetProp, (MoaMmSymbol, ConstPMoaMmValue))		
	
	/* Sprite Actor methods */
		EXTERN_DEFINE_METHOD(MoaError, CallFunction, (MoaMmSymbol, MoaLong, 
				ConstPMoaMmValue,PMoaMmValue))		
		EXTERN_DEFINE_METHOD(MoaError, SetCallback, (PIMoaMmSpriteCallback))		
		EXTERN_DEFINE_METHOD(MoaError, Notify, (MoaLong, PMoaVoid))		
		EXTERN_DEFINE_METHOD(MoaError, GetCaps, (PMoaMmXSpriteCaps))		
		EXTERN_DEFINE_METHOD(MoaError, CollectChanges, (ConstPMoaMmXSpriteInfo,
			PIMoaMmChangeCollector))		
		EXTERN_DEFINE_METHOD(MoaError, ContainsPoint, (ConstPMoaMmXSpriteInfo, 
			ConstPMoaPoint,	MoaBoolParam *))		
		EXTERN_DEFINE_METHOD(MoaError, Image, (ConstPMoaMmXSpriteInfo, 
			ConstPMoaMmPlayerStateInfo, PIMoaMmGC))		
		EXTERN_DEFINE_METHOD(MoaError, Event, (ConstPMoaMmEventInfo, 
			ConstPMoaMmPlayerStateInfo, MoaBoolParam *))		
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif /* _H_ovalactr */
