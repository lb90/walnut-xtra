/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_cfilter
#define _H_cfilter

/*****************************************************************************
 *
 *	Name: cfilter.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for a generic Xtra's filter class.
 *
 *
 *  HOW TO CUSTOMIZE THIS FILE
 *  --------------------------
 *  1. Save this file under a different file name.
 *  2. Use a search and replace utility (case sensitive) to replace the
 *     following:
 *
 *     Replace         With	
 *     -------         ----
 *     cfilter		   <this file name>
 *     CFilter         <name of the class you defined>
 *
 *  3. Add and modify source code by looking for '--> insert' comment
 *  4. Create a Xtra.cpp file that contains the Xtra definition.
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
#include "moaxtra.h"
#include "bmfilters.h"

/*****************************************************************************
 *	CLASS SPECIFIC DEFINE(S)
 *	-----------------------
 *	Any #define's specific to this class 
 ****************************************************************************/ 

/*****************************************************************************
 *  CLSID
 *  -----
 *  The CLSID is a GUID that unquely identifies your MOA class.  To generate a
 *  GUID, use the uuidgen utility (Mac) or the GUIDGEN.EXE utility (Win).
 ****************************************************************************/ 
#error PLEASE DEFINE A NEW CLSID
//DEFINE_GUID(CLSID(CFilter), 0x00000000L, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *  ------------------------
 *  Class instance variable are variables whose scope is exclusive to the
 *  methods implemented by your MOA class.  Variables necessary for the
 *  implementation of your MOA class should be placed here.
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CFilter)
	/*
	 * --> insert additional variable(s) -->
	 */
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
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CFilter, IMoaBitmapFilter)
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

/*
 * --> insert additional interface(s) -->
 */

#endif // _H_cfilter
