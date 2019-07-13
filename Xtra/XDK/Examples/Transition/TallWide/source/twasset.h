/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
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
 *	Name: TWAsset.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for TallWide Xtra's asset class.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .h file should automatically include all the support files needed for
 *	this particular class. In addition, this file may include other .h files
 *	defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "moaxtra.h"
#include "mmixasst.h"


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

#ifndef SHARED_DATA
#define SHARED_DATA
typedef struct {
	MoaLong			initialWidth;
	MoaLong			initialHeight;
	MoaLong			verticalSpeed;
	MoaLong			horizontalSpeed;
} SharedAssetData, * PSharedAssetData;
#endif

/*  Calculate the total number of bytes to stream in */
#define PROPERTY_BYTES (NUM_MOALONG_PROPS * sizeof(MoaLong)) + (NUM_MOADOUBLE_PROPS * sizeof(MoaDouble))

/* Number of different variable type properties for stream in/out methods */
/* Used to calculate the total number of bytes for streamOut and streamIn  */
#define NUM_MOALONG_PROPS 		4
#define NUM_MOADOUBLE_PROPS 	0

/* Default maximum string length */
#define MAX_STRING_LENGTH 		250

/* animated picture numbers */
/*******************************************************
These resources numbers should be changed to names 
(for each) if you wish to avoid conflicts when combining
Xtras into projectors on the Mac
*******************************************************/
#define FIRST_ANIM_PICT_NUM 	510
#define LAST_ANIM_PICT_NUM 		522
/* About box DLOG resource ID  */
#define ID_ABOUTBOX 			130   
/* /* Mac dialog resource ID */
#define MAC_DIALOG_ID			128

/* About box dialog items */
/* All dialogs use 1 for the OK button, if present */
#define ID_OKBUTTON 			1

#ifdef MACINTOSH
	/*Small cast icon resource number */
	#define PICON				"TallWide Xtra Transition Picon"
	#define PICON_BW			"TallWide Xtra Transition Picon_BW"
	/* Static Thumbnail resource ID */
	#define STATIC_THUMB		"\pTallWide4"  /*516 */
	/* Dialog IDs for all the items */
	#define OK					1
	#define CANCEL				2
	#define INITIAL_WIDTH	 	3
	#define INITIAL_HEIGHT	 	4
	#define VERTICAL_SPEED		5
	#define HORIZONTAL_SPEED	6
#endif  /*MAC */

#ifdef WINDOWS
	/*Small cast icon resource number */
	#define PICON				"TallWide_Xtra_Picon"
	#define PICON_BW			"TallWide_Xtra_Picon_BW"
	/* Static Thumbnail resource ID */
	#define STATIC_THUMB		516
	/* Dialog IDs for all the items */
	#define DIALOG_BOX			101

	#define INITIAL_WIDTH		1000
	#define INITIAL_HEIGHT		1001
	#define VERTICAL_SPEED		1002
	#define HORIZONTAL_SPEED	1003
#endif  /*WIN */

/*****************************************************************************
 *	CLSID
 *	-----
 *	The CLSID is a GUID that unquely identifies your MOA class.  To generate a
 *	GUID, use the genUID.app utility (Mac) or the GUIDGEN.EXE utility (Win).
 *	The following line will produce a pre-compiler error if not replaced with
 *	a valid CLSID.
 ****************************************************************************/ 
DEFINE_GUID(CLSID(CTallWideAsset), 0x70136F2CL, 0x8BD8, 0x11D0, 0x94, 0x7B, 0x00, 0x05, 0x02, 0x70, 0xB2, 0x08);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *  ------------------------
 *  Class instance variable are variables whose scope is exclusive to the
 *  methods implemented by your MOA class.  Variables necessary for the
 *  implementation of your MOA class should be placed here.
 *****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CTallWideAsset)
	MoaMmSymbol 	type;	
	MoaLong			mediaLoadStatus;
	MoaLong 		propsLoadStatus;
	SharedAssetData	sharedAssetData;
	MoaLong			pictNum;
	MoaLong 		modifyFlags;
	MoaMmSymbol		initialWidthSymbol, initialHeightSymbol, verticalSpeedSymbol, horizontalSpeedSymbol;
	MoaLong			propsDirtied;
	PIMoaMmAssetCallback pAssetCallback;
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
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CTallWideAsset, IMoaMmXAsset)
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
private:
	EXTERN_DEFINE_METHOD(short, MemXShort, (short))
	EXTERN_DEFINE_METHOD(long, MemXLong, (long))
EXTERN_END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CTallWideAsset, IMoaInitFromDict)
	EXTERN_DEFINE_METHOD(MoaError, InitFromDict, (PIMoaRegistryEntryDict))
EXTERN_END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CTallWideAsset, IMoaNotificationClient)
	EXTERN_DEFINE_METHOD(MoaError, Notify, (ConstPMoaNotifyID, PMoaVoid, PMoaVoid))
EXTERN_END_DEFINE_CLASS_INTERFACE
#endif


#endif /* _H_casset */
