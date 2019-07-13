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
 *	Name: cinterro.cpp
 *	
 * 	Purpose: Definitions of Interrogator class interface(s) and methods for
 *           the Interrogator Xtra skeleton project.
 *
 *  HOW TO CUSTOMIZE THIS FILE
 *  --------------------------
 *  1. Save this file under a different file name.
 *  2. Use a search and replace utility (case sensitive) to replace the
 *     following:
 *
 *     Replace         With	
 *     -------         ----
 *     cinterro        <this file name>
 *     CInterrogator   <name of the class you defined>
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
#ifndef _H_cinterro
	#include "cinterro.h"
#endif


/*****************************************************************************
 *  INTERNAL PROTOTYPES(S)
 *  ----------------------
 *  Declarations for functions used specifically in this file.
 ****************************************************************************/

/*****************************************************************************
 *  INTERNAL ROUTINE(S)
 *  -------------------
 *  Definition of file-specific functions
 ****************************************************************************/

/*****************************************************************************
 *  CLASS INTERFACE(S)
 *  ------------------
 *  The interface(s) implemented by your MOA class are specified here.  Note
 *	that at least one class in your Xtra should implement the IMoaRegister
 *	interface.
 *  NOTE: Because C++ does not use a lpVtbl to reference an interface's
 *	methods, the actual method declaration is done in the .h file.
 *
 *  Syntax:
 *  BEGIN_DEFINE_CLASS_INTERFACE(<class-name>, <interface-name>) 
 ****************************************************************************/ 
BEGIN_DEFINE_CLASS_INTERFACE(CInterrogator, IMoaMmInterrogator)
END_DEFINE_CLASS_INTERFACE

BEGIN_DEFINE_CLASS_INTERFACE(CInterrogator, IMoaMmTerminator)
END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
BEGIN_DEFINE_CLASS_INTERFACE(CInterrogator, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
BEGIN_DEFINE_CLASS_INTERFACE(CInterrogator, IMoaNotificationClient)
END_DEFINE_CLASS_INTERFACE
#endif

/*
 * --> insert additional interface(s) -->
 */ 

/*****************************************************************************
 *  CREATE AND DESTROY METHODS
 *  --------------------------
 *  Every interface and class has an associated 'Create' and 'Destroy' pair.
 *  'Create' methods are typically used to acquire interface(s), allocate 
 *  memory, and intialize variables. 'Destroy' methods are typically used to 
 *  release interfaces and free memory.
 *
 * NOTE:  In C++, the local variable 'This' is provided implicitly within 
 * a method implementation.  Thus, there is no need explicitly declare 'This' 
 * as a function parameter. However, this implementation detail doesn't apply 
 * to the MOA class creator and destructor functions, which are standard C 
 * functions, coded exactly as in like they are in C examples.  
 *
 * Class Syntax:
 * STDMETHODIMP MoaCreate_<class-name>(<class-name> * This)
 * STDMETHODIMP MoaDestroy_<class-name>(<class-name> * This)
 *
 * Interface Syntax:
 * <class_name>_<if_name>::<class_name>_<if_name>(MoaError * pErr)
 * <class_name>_<if_name>::~<class_name>_<if_name>()
 ****************************************************************************/ 

/* ------------------------------------------------ MoaCreate_CInterrogator */
STDMETHODIMP MoaCreate_CInterrogator(CInterrogator * This)
{
	MoaError err = kMoaErr_NoErr;
	
	/*
	 * --> insert additional code -->
	 */

	return err;
}

/* ----------------------------------------------- MoaDestroy_CInterrogator */
STDMETHODIMP_(void) MoaDestroy_CInterrogator(CInterrogator * This)
{
	/* 
	 * --> insert additional code -->
	 */
}

/* ------------------------ CInterrogator_IMoaMmInterrogator Create/Destroy */
CInterrogator_IMoaMmInterrogator::CInterrogator_IMoaMmInterrogator(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CInterrogator_IMoaMmInterrogator::~CInterrogator_IMoaMmInterrogator() {}

/* -------------------------- CInterrogator_IMoaMmTerminator Create/Destroy */
CInterrogator_IMoaMmTerminator::CInterrogator_IMoaMmTerminator(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CInterrogator_IMoaMmTerminator::~CInterrogator_IMoaMmTerminator() {}

#ifdef USING_INIT_FROM_DICT
/* -------------------------- CInterrogator_IMoaInitFromDict Create/Destroy */
CInterrogator_IMoaInitFromDict::CInterrogator_IMoaInitFromDict(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CInterrogator_IMoaInitFromDict::~CInterrogator_IMoaInitFromDict() {}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* -------------------- CInterrogator_IMoaNotificationClient Create/Destroy */
CInterrogator_IMoaNotificationClient::CInterrogator_IMoaNotificationClient(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CInterrogator_IMoaNotificationClient::~CInterrogator_IMoaNotificationClient() {}
#endif

/*
 * --> insert additional create/destroy method(s) -->
 */

/*****************************************************************************
 *  METHOD IMPLEMENTATION(S)
 *  ------------------------
 *  This is where the methods to be defined by your MOA class are implemented.
 *  The bulk of the work in implementing Xtras is done here.  NOTE: 'This' is 
 *  implemented implicitly in C++, therefore it isn't used in the argument-
 *	list.
 *
 *  Syntax:
 *  STDMETHODIMP <class-name>_<interface-name>::<method-name>(<argument-list>)
 ****************************************************************************/ 

/* -------------------------- CInterrogator_IMoaMmInterrogator::Interrogate */
STDMETHODIMP_(void) CInterrogator_IMoaMmInterrogator::Interrogate()
{
	MoaError err = kMoaErr_NoErr;

	/*
	 *  --> insert additional code -->
	 */
}

/* ------------------------------ CInterrogator_IMoaMmTerminator::Terminate */
STDMETHODIMP_(void) CInterrogator_IMoaMmTerminator::Terminate()
{
	/* variable declarations */
	MoaError err = kMoaErr_NoErr;

	/*
	 *  --> insert additional code -->
	 */
}

#ifdef USING_INIT_FROM_DICT
/* --------------------------- CInterrogator_IMoaInitFromDict::InitFromDict */
STDMETHODIMP CInterrogator_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
{
	MoaError err = kMoaErr_NoErr;

	/*
 	 * --> insert additional code -->
 	 */
						   	
	return err;
}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* --------------------------- CInterrogator_IMoaNotificationClient::Notify */
STDMETHODIMP CInterrogator_IMoaNotificationClient::Notify(ConstPMoaNotifyID nid, PMoaVoid pNData, PMoaVoid pRefCon)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 *  --> insert additional code -->
	 */
	return err;
}
#endif
