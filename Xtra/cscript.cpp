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

#include "../hazel.h"

#include <vector>

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
	
	xtra_initialize();

	This->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid) &This->pValueInterface);
	This->pCallback->QueryInterface(&IID_IMoaDrPlayer, (PPMoaVoid)& This->pDrPlayer);

	return(err);
}

/* ----------------------------------------------------- MoaDestroy_CScript */
STDMETHODIMP_(void) MoaDestroy_CScript(CScript * This)
{
	if (This->pDrPlayer)
		This->pDrPlayer->Release();
	if (This->pValueInterface)
		This->pValueInterface->Release();

	xtra_finalize();
	
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
	switch	( callPtr->methodSelector ) 
	{
		case m_new:
			/*
			 * --> insert additional code -->
		 	 */
			break;

		case m_hazpeaget: {
			MoaMmValue arg_value;
			ConstPMoaChar arg_c_string;
			HRESULT hr = kMoaErr_NoErr;

			if (!pObj->pValueInterface)
				return kMoaErr_NoErr;

			pciGetArgByIndex(callPtr, 1, &arg_value);

			hr = pObj->pValueInterface->ValueToStringPtr(&arg_value, &arg_c_string);
			if (hr != kMoaErr_NoErr || arg_c_string == NULL) {
				/* There was an error. Return an empty string. */
				pObj->pValueInterface->StringToValue("", &(callPtr->resultValue));
				return kMoaErr_NoErr;
			}

			std::vector<char> direcotor_resolved_path(4096, 0);
			if (pObj->pDrPlayer) {
				hr = pObj->pDrPlayer->ResolveFileName(arg_c_string,
				                                      direcotor_resolved_path.data(),
				                                      direcotor_resolved_path.size());
				if (hr != kMoaErr_NoErr) {
					direcotor_resolved_path.front() = 0;
				}
				/* be safe */
				direcotor_resolved_path.back() = 0;
			}

			std::string file_name_utf_8 = (direcotor_resolved_path[0] != 0) ?
			                               direcotor_resolved_path.data() : arg_c_string;
			std::string ret = "";

			xtra_hazel_get(file_name_utf_8, ret);

			pObj->pValueInterface->StringToValue(ret.c_str(), &(callPtr->resultValue));
		}
		break;
		case m_hazpeaset: {
			MoaMmValue arg_value_1;
			ConstPMoaChar arg_c_string_1 = NULL;
			MoaMmValue arg_value_2;
			ConstPMoaChar arg_c_string_2 = NULL;
			HRESULT hr = kMoaErr_NoErr;

			if (!pObj->pValueInterface)
				return kMoaErr_NoErr;

			pciGetArgByIndex(callPtr, 1, &arg_value_1);
			hr = pObj->pValueInterface->ValueToStringPtr(&arg_value_1, &arg_c_string_1);
			if (hr != kMoaErr_NoErr || arg_c_string_1 == NULL)
			{
				/* There was an error. Return an empty string. */
				pObj->pValueInterface->StringToValue("", &(callPtr->resultValue));
				return kMoaErr_NoErr;
			}

			pciGetArgByIndex(callPtr, 2, &arg_value_2);
			hr = pObj->pValueInterface->ValueToStringPtr(&arg_value_2, &arg_c_string_2);
			if (hr != kMoaErr_NoErr || arg_c_string_2 == NULL)
			{
				/* There was an error. Return an empty string. */
				pObj->pValueInterface->StringToValue("", &(callPtr->resultValue));
				return kMoaErr_NoErr;
			}

			std::vector<char> direcotor_resolved_path(4096, 0);
			if (pObj->pDrPlayer) {
				hr = pObj->pDrPlayer->ResolveFileName(arg_c_string_1,
				                                      direcotor_resolved_path.data(),
				                                      direcotor_resolved_path.size());
				if (hr != kMoaErr_NoErr) {
					direcotor_resolved_path.front() = 0;
				}
				/* be safe */
				direcotor_resolved_path.back() = 0;
			}

			std::string file_name_utf_8 = (direcotor_resolved_path[0] != 0)?
			                               direcotor_resolved_path.data() : arg_c_string_1;
			std::string mode_string = arg_c_string_2;
			int ret = 0;

			xtra_hazel_set(file_name_utf_8, mode_string, ret);

			pObj->pValueInterface->IntegerToValue(ret, &(callPtr->resultValue));
		}	break;
		
		/*
		 * --> insert additional methodSelector cases -->
		 */

		default:
			return kMoaErr_FuncNotFound;
			break;
	}

	return kMoaErr_NoErr; /* 0L, same as kMoaStatus_OK */
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

