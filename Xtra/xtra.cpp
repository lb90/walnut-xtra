#define USING_INIT_FROM_DICT

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
 *	Name: xtra.cpp
 *	
 * 	Purpose: The Xtra definition of the Script Xtra example.
 *
 *  HOW TO CUSTOMIZE THIS FILE
 *  --------------------------
 *  1. Save this file under a different file name.
 *  2. Use a search and replace utility (case sensitive) to replace the
 *     following:
 *
 *     Replace         With	
 *     -------			----
 *     clingo		   <its file name>
 *     CLingo          <name of the class you defined>
 *
 *     cregster        <its file name>
 *     CRegister       <name of the class you defined>
 *
 *  3. Add and modify source code by looking for '--> insert' comment
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "cscript.h"
#include "cregster.h"
#include "xclassver.h"

/*****************************************************************************
 *  XTRA DEFINITION
 *  ---------------
 *  The Xtra definition specfies the classes and interfaces defined by your
 *  MOA Xtra.
 *
 *  Syntax:
 *	BEGIN_XTRA_DEFINES_CLASS(<class-name>,<version>)
 *	CLASS_DEFINES_INTERFACE(<class-name>, <interface-name>,<version>) 
 ****************************************************************************/ 
BEGIN_XTRA
	BEGIN_XTRA_DEFINES_CLASS(CRegister, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(CRegister, IMoaRegister, XTRA_CLASS_VERSION)
		/*
		 * ---> insert additional interface(s) -->
		 */ 
	END_XTRA_DEFINES_CLASS
	BEGIN_XTRA_DEFINES_CLASS(CScript, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(CScript, IMoaMmXScript, XTRA_CLASS_VERSION)
		#ifdef USING_INIT_FROM_DICT
		CLASS_DEFINES_INTERFACE(CScript, IMoaInitFromDict, XTRA_CLASS_VERSION)	
		#endif
		#ifdef USING_NOTIFICATION_CLIENT
		CLASS_DEFINES_INTERFACE(CScript, IMoaNotificationClient, XTRA_CLASS_VERSION)	
		#endif
		/*
		 * ---> insert additional interface(s) -->
		 */ 
	END_XTRA_DEFINES_CLASS
	/*
	 * ---> insert additional class(es) -->
	 */ 
END_XTRA
