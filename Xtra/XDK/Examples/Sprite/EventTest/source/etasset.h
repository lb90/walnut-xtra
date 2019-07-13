/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2007 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_casset
#define _H_casset

/*****************************************************************************
 *
 *	Name: etasset.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for EventTest Xtra's asset class.
 *
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


/*****************************************************************************
 *  CONFIGURATION DEFINE(S)
 *  -----------------------
 *	Uncomment any appropriate lines below to implement the indicated Xtra
 *	interfaces within this class.
 ****************************************************************************/ 
//#define USING_INIT_FROM_DICT
//#define USING_NOTIFICATION_CLIENT

/*****************************************************************************
 *	CLASS SPECIFIC DEFINE(S)
 *	-----------------------
 *	Any #define's specific to this class 
 ****************************************************************************/ 
#ifndef UNUSED
	#define UNUSED(x) x
#endif

/* Sprite notification messages */
enum { kSprNotifyMsg_SetParentAsset = 1 };

/* Default rectangle size sprites occupy on stage */
#define RECTSIZE 			100

#ifdef WINDOWS
	/* Resource ID's for Bitmaps to be drawn */
	#define THUMBNAIL		  	107
#endif /*WIN */

/*****************************************************************************
 *	CLSID
 *	-----
 *	The CLSID is a GUID that unquely identifies your MOA class.  To generate a
 *	GUID, use the genUID.app utility (Mac) or the GUIDGEN.EXE utility (Win).
 *	The following line will produce a pre-compiler error if not replaced with
 *	a valid CLSID.
 ****************************************************************************/ 
DEFINE_GUID(CLSID(CEventTestAsset), 0x75CC1E78L, 0x97EA, 0x11D0, 0x95, 0x0C, 0x00, 0x05, 0x02, 0x70, 0xB2, 0x08);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *  ------------------------
 *  Class instance variable are variables whose scope is exclusive to the
 *  methods implemented by your MOA class.  Variables necessary for the
 *  implementation of your MOA class should be placed here.
 *****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CEventTestAsset)
	MoaLong	instData;
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
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CEventTestAsset, IMoaMmXAsset)
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

#ifdef USING_INIT_FROM_DICT
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CEventTestAsset, IMoaInitFromDict)
	EXTERN_DEFINE_METHOD(MoaError, InitFromDict, (PIMoaRegistryEntryDict))
EXTERN_END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CEventTestAsset, IMoaNotificationClient)
	EXTERN_DEFINE_METHOD(MoaError, Notify, (ConstPMoaNotifyID, PMoaVoid, PMoaVoid))
EXTERN_END_DEFINE_CLASS_INTERFACE
#endif


#endif /* _H_casset */
