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
 *	Name: cscript.cpp
 *	
 * 	Purpose: Definitions of scripting class interface(s) and methods for the 
 *           Scripting Xtra skeleton project.
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
 *     cscript      <this file name>
 *     CScript      <name of the class you defined>
 *
 *  3. Add and modify source code by looking for '--> insert -->' comment
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "cscript.h"

#ifdef MACINTOSH
	#include <windows.h>
	#include <string.h>
#endif

#include "../walnut.h"
#include "../process.h"

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
BEGIN_DEFINE_CLASS_INTERFACE(CScript, IMoaMmXScript)
END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
BEGIN_DEFINE_CLASS_INTERFACE(CScript, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
BEGIN_DEFINE_CLASS_INTERFACE(CScript, IMoaNotificationClient)
END_DEFINE_CLASS_INTERFACE
#endif

/*
 * --> insert additional method(s) -->
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

/* ------------------------------------------------------ MoaCreate_CScript */
STDMETHODIMP MoaCreate_CScript(CScript * This)
{
	MoaError err = kMoaErr_NoErr;
	
	walnut_initialize();

	This->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid) &This->pValueInterface);

	return(err);
}

/* ----------------------------------------------------- MoaDestroy_CScript */
STDMETHODIMP_(void) MoaDestroy_CScript(CScript * This)
{
	walnut_finalize();
	
	return;
}

/* ----------------------------------- CScript_IMoaMmXScript Create/Destroy */
CScript_IMoaMmXScript::CScript_IMoaMmXScript(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CScript_IMoaMmXScript::~CScript_IMoaMmXScript() {}

#ifdef USING_INIT_FROM_DICT
/* -------------------------------- CScript_IMoaInitFromDict Create/Destroy */
CScript_IMoaInitFromDict::CScript_IMoaInitFromDict(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CScript_IMoaInitFromDict::~CScript_IMoaInitFromDict() {}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* -------------------------- CScript_IMoaNotificationClient Create/Destroy */
CScript_IMoaNotificationClient::CScript_IMoaNotificationClient(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CScript_IMoaNotificationClient::~CScript_IMoaNotificationClient() {}
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

/* -------------------------------------------- CScript_IMoaMmXScript::Call */
STDMETHODIMP CScript_IMoaMmXScript::Call(PMoaMmCallInfo callPtr)
{
	MoaError err = kMoaErr_NoErr;
	
	switch	( callPtr->methodSelector ) 
	{
		case m_new:
			/*
			 * --> insert additional code -->
		 	 */
			break;

		case m_walnut: {
			MoaMmValue argValue;
			ConstPMoaChar   str;

			if (!pObj->pValueInterface)
				return(err);

			/* This shows how to access an argument
			/  the first argument in the list is the "me" value, so the user arguments
			/  start at the second position in the list */
			pciGetArgByIndex( callPtr, 1, &argValue );

			err = pObj->pValueInterface->ValueToStringPtr(&argValue, &str);
			if (err == kMoaErr_NoErr && str != NULL) {
				std::string res;
				process(str, res);
				pObj->pValueInterface->StringToValue(res.c_str(), &(callPtr->resultValue));
			}
			else {
				pObj->pValueInterface->StringToValue("", &(callPtr->resultValue));
			}
		}
			break;
		
		/*
		 * --> insert additional methodSelector cases -->
		 */
	}

	return(err);
}

#ifdef USING_INIT_FROM_DICT
/* --------------------------------- CScript_IMoaInitFromDict::InitFromDict */
STDMETHODIMP CScript_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 *  --> insert additional code -->
	 */

	return(err);
}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* --------------------------------- CScript_IMoaNotificationClient::Notify */
STDMETHODIMP CScript_IMoaNotificationClient::Notify(ConstPMoaNotifyID nid, PMoaVoid pNData, PMoaVoid pRefCon)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 *  --> insert additional code -->
	 */

	return(err);
}
#endif

