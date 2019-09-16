/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_cscript
#define _H_cscript

/*****************************************************************************
 *
 *	Name: cscript.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for an Xtra's scripting class.
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
 *     cscript         <this file name>
 *     CScript         <name of the class you defined>
 *
 *  3. Add and modify source code by looking for '--> insert -->' comment
 *  4. Create a Xtra.cpp file that contains the Xtra definition.
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .h file should automatically include all the support files needed for
 *	this particular class. In addition, this file may include other .h files
 *	defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "moaxtra.h"
#include "mmiservc.h"
#include "mmixscrp.h"

#ifdef MACINTOSH
	#include <windows.h>
	#include <string.h>
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

/*****************************************************************************
 *	CLSID
 *	-----
 *	The CLSID is a GUID that unquely identifies your MOA class.  To generate a
 *	GUID, use the genUID.app utility (Mac) or the GUIDGEN.EXE utility (Win).
 *	The following line will produce a pre-compiler error if not replaced with
 *	a valid CLSID.
 ****************************************************************************/ 
 // {1898B2DF-BCDC-4EB3-B085-0A68926C8C21}
DEFINE_GUID(CLSID(CScript), 0x1898b2df, 0xbcdc, 0x4eb3, 0xb0, 0x85, 0xa, 0x68, 0x92, 0x6c, 0x8c, 0x21);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *  ------------------------
 *  Class instance variable are variables whose scope is exclusive to the
 *  methods implemented by your MOA class.  Variables necessary for the
 *  implementation of your MOA class should be placed here.
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CScript)
	/*
	 * ---> insert additional variable(s) -->
	 */ 
	PIMoaMmValue	pValueInterface;
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
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CScript, IMoaMmXScript)
	EXTERN_DEFINE_METHOD(MoaError, Call, (PMoaMmCallInfo))
   // definition of private member functions
   private:
   // PMoaDrCallInfo param is optional, include () if lingo call is to have no params...
	/*EXTERN_DEFINE_METHOD(MoaError, XScrpGlobalHandler, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XScrpParentHandler, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XScrpChildHandler, (PMoaDrCallInfo))*/
EXTERN_END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CScript, IMoaInitFromDict)
	EXTERN_DEFINE_METHOD(MoaError, InitFromDict, (PIMoaRegistryEntryDict))
EXTERN_END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CScript, IMoaNotificationClient)
	EXTERN_DEFINE_METHOD(MoaError, Notify, (ConstPMoaNotifyID, PMoaVoid, PMoaVoid))
EXTERN_END_DEFINE_CLASS_INTERFACE
#endif

/*
 * --> insert additional interface(s) -->
 */ 

/* 	MODIFY: This is the enumerated scripting method list. This list should
 *	directly correspond to the msgTable defined in an xxx.CPP file. It is used
 *	to dispatch method calls via the methodSelector. The 'm_XXXX' method must
 *	be last.
 */

enum 
{
m_new = 0,						/* standard */
m_hazpeaget,
m_hazpeaset,
m_XXXX
};

#endif /* _H_cscript */
