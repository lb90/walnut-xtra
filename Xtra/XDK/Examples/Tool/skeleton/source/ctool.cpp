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
 *	Name: ctool.cpp
 *	
 * 	Purpose: Definitions of Lingo class interface(s) and methods for the 
 *           Tools skeleton Xtra example.
 *
 *  HOW TO CUSTOMIZE THIS FILE
 *  --------------------------
 *  1. Save this file under a different file name.
 *  2. Use a search and replace utility (case sensitive) to replace the
 *     following:
 *
 *     Replace         With	
 *     -------         ----
 *     ctool           <this file name>
 *     CTool           <name of the class you defined>
 *
 *  3. Add and modify source code by looking for '--> insert' comments
 ****************************************************************************/ 

/*****************************************************************************
 *  XTRA SPECIFIC DEFINE(S)
 *  -----------------------
 *	Any #define's specific to this Xtra.  
 *****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed for 
 *  this particular class. In addition, this file may include other .h files 
 *  defining additional callback interfaces for use by a third party.   
 *****************************************************************************/ 
#include "ctool.h"

#ifdef MACINTOSH
#include <windows.h>
#include <string.h>
#endif /* MACINTOSH */

/*****************************************************************************
 *  FILE SPECIFIC DEFINE(S)
 *  -----------------------
 *	Any #define's specific to this file.  
 *****************************************************************************/ 

/*****************************************************************************
 *  CLASS INTERFACE(S)
 *  ------------------
 *  The interface(s) implemented by your MOA class are specified here.  Note that
 *  at least one class in your Xtra should implement the IMoaRegister interface.
 *  NOTE: Because C++ does not use a lpVtbl to reference an interface's methods, 
 *  the actual method declaration is done in the .h file.
 *
 *  Syntax:
 *  BEGIN_DEFINE_CLASS_INTERFACE(<class-name>, <interface-name>) 
 *****************************************************************************/ 
BEGIN_DEFINE_CLASS_INTERFACE(CTool, IMoaMmXTool)
END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
BEGIN_DEFINE_CLASS_INTERFACE(CTool, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
BEGIN_DEFINE_CLASS_INTERFACE(CTool, IMoaNotificationClient)
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
 *****************************************************************************/ 

/* NOTE:  In C++, the local variable 'This' is provided implicitly within 
 * a method implementation.  Thus, there is no need explicitly declare 'This' 
 * as a function parameter. However, this implementation detail doesn't apply 
 * to the MOA class creator and destructor functions, which are standard C 
 * functions, coded exactly as in like they are in C examples. 
 *
 * Syntax:
 * STDMETHODIMP MoaCreate_<class-name>(<class-name> * This)  
 * STDMETHODIMP MoaDestroy_<class-name>(<class-name> * This)  
 */

/* ----------------------------------------------------------- MoaCreate_CTool */
STDMETHODIMP MoaCreate_CTool(CTool * This)
{	
	/* 
	 * --> insert additional code -->
	 */
	
	return kMoaErr_NoErr;
}


/* ---------------------------------------------------------- MoaDestroy_CTool */
STDMETHODIMP_(void) MoaDestroy_CTool(CTool * This)
{	
	/* 
	 * --> insert additional code -->
	 */
}

/* --------------------------------------- CTool_IMoaMmXTool Create/Destroy */
CTool_IMoaMmXTool::CTool_IMoaMmXTool(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CTool_IMoaMmXTool::~CTool_IMoaMmXTool() {}

#ifdef USING_INIT_FROM_DICT
/* ------------------------------------ CTool_IMoaInitFromDict Create/Destroy */
CTool_IMoaInitFromDict::CTool_IMoaInitFromDict(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CTool_IMoaInitFromDict::~CTool_IMoaInitFromDict() {}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* ------------------------------------ CTool_IMoaNotificationClient Create/Destroy */
CTool_IMoaNotificationClient::CTool_IMoaNotificationClient(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CTool_IMoaNotificationClient::~CTool_IMoaInitFromDict() {}
#endif

/*
 * --> insert additional create/destroy method(s) -->
 */

/*****************************************************************************
 *  METHOD IMPLEMENTATION(S)
 *  ------------------------
 *  This is where the methods to be defined by your MOA class are implemented.
 *  The bulk of the work in implementing Xtras is done here.  NOTE: 'This' is 
 *  implemented implicitly in C++, therefore it isn't used in the argument-list.
 *
 *  Syntax:
 *  STDMETHODIMP <class-name>_<interface-name>::<method-name>(<argument-list>)
 *****************************************************************************/ 

/* -------------------------------------- CTool_IMoaInitFromDict::InitFromDict */
#ifdef USING_INIT_FROM_DICT

STDMETHODIMP CTool_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
{
	MoaError err = kMoaErr_NoErr;

	/*
 	 * --> insert additional code -->
 	 */
						   	
	return err;
}
#endif

/* ------------------------------------------------- CTool_IMoaMmXTool::Invoke */
STDMETHODIMP CTool_IMoaMmXTool::Invoke()
{
	MoaError err = kMoaErr_NoErr;

	/*
 	 * --> insert additional code -->
 	 */

	return err;
}

/* --------------------------------------------- CTool_IMoaMmXTool::GetEnabled */
STDMETHODIMP CTool_IMoaMmXTool::GetEnabled(PMoaDrEnabledState pEnabledState)
{
	/* This method is called by the host application to find out if
	   our tool is enabled or not.  Disabled tools will appear
	   differently in the user interface (such as a dimmed-out
	   menu item or button), and can't be selected by the user.

       In this example, we have registered a TRUE value for the
	   "alwaysEnabled" registry key.  This means that the host
	   app will assume we're always enabled, and will never
	   call this method.  This is done for performance and
	   memory considerations: if a tool is marked as 
	   "alwaysEnabled", the code for the tool doesn't have 
	   to be loaded until the tool is selected (otherwise, 
	   it must be loaded as soon as the user clicks on the menu 
	   bar, since its GetEnabled() method must be called.
		
	   So, if you're always enabled, declare "alwaysEnabled"
	   as TRUE in your IMoaRegister::Register() method; 
	   if your enable state changes dynamically, declare 
	   "alwaysEnabled" as FALSE and set *pEnabledState to the
	   appropriate value here when called. 
	
	   We're always enabled, so it doesn't matter what we do
	   here. */

	/*
	 * --> insert additional code -->
	 */
		
	*pEnabledState = kMoaDrEnabledState_Enabled;
	
	return err;
}
