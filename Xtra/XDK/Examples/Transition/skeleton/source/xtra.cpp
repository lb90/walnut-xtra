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
 * 	Purpose: The Xtra definition of a Transition Skeleton Xtra example.
 *
 *  HOW TO CUSTOMIZE THIS FILE
 *  --------------------------
 *  1. Save this file under a different file name.
 *  2. Use a search and replace utility (case sensitive) to replace the
 *     following:
 *
 *     Replace         With	
 *     -------			----
 *     casset		   <its file name>
 *     CAsset          <name of the class you defined>
 *
 *     cActor          <its file name>
 *     CActor          <name of the class you defined>
 *
 *     cregster        <its file name>
 *     CRegister       <name of the class you defined>
 *
 *  3. Add and modify source code by looking for '--> insert' comment
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed for this 
 *  particular class. In addition, this file may include other .h files defining 
 *  additional callback interfaces for use by a third party.   
 *****************************************************************************/ 
#include "ctrans.h"
#include "casset.h"
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
		 * --> insert additional interface(s)
		 */
	END_XTRA_DEFINES_CLASS
	BEGIN_XTRA_DEFINES_CLASS(CAsset, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(CAsset, IMoaMmXAsset, XTRA_CLASS_VERSION)
		#ifdef USING_INIT_FROM_DICT
		CLASS_DEFINES_INTERFACE(CAsset, IMoaInitFromDict, XTRA_CLASS_VERSION)
		#endif
		#ifdef USING_NOTIFICATION_CLIENT
		CLASS_DEFINES_INTERFACE(CAsset, IMoaNotificationClient, XTRA_CLASS_VERSION)	
		#endif
		/*
		 * --> insert additional interface(s)
		 */
	END_XTRA_DEFINES_CLASS
	BEGIN_XTRA_DEFINES_CLASS(CTransition, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(CTransition, IMoaMmXTransitionActor, XTRA_CLASS_VERSION)
		/*
		 * --> insert additional interface(s)
		 */
	END_XTRA_DEFINES_CLASS
	/*
	 * ---> insert additional class(es) -->
	 */ 
END_XTRA

