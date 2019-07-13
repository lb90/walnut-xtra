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
 *	Name: GReg.cpp
 *	
 * 	Purpose: Definitions of Register class interface(s) and methods for the
 *           Greeting Xtra.
 *	
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#ifndef INITGUID        /* INITGUID causes the actual GUIDs to be declared, */ 
	#define INITGUID      /* not just references to them. Must be declared in */
#endif                  /* only one file. */          

#ifndef _H_cregster
	#include "GReg.h"
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
BEGIN_DEFINE_CLASS_INTERFACE(CRegister, IMoaRegister) 
END_DEFINE_CLASS_INTERFACE


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

/* ---------------------------------------------------- MoaCreate_CRegister */
STDMETHODIMP MoaCreate_CRegister(CRegister * This)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;

	X_RETURN(MoaError, err);
	X_EXIT
}

/* --------------------------------------------------- MoaDestroy_CRegister */
STDMETHODIMP_(void) MoaDestroy_CRegister(CRegister * This)
{
}

/* ---------------------------------- CRegister_IMoaRegister Create/Destroy */
CRegister_IMoaRegister::CRegister_IMoaRegister(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CRegister_IMoaRegister::~CRegister_IMoaRegister() {}


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
 *****************************************************************************/ 

/* ------------------------------------------- CRegister_IMoaRegister::Register */
STDMETHODIMP_(MoaError) CRegister_IMoaRegister::Register(PIMoaCache pCache, 
	PIMoaDict pXtraDict)
{
	X_ENTER

	MoaError 			err = kMoaErr_NoErr;
	PIMoaDict			pRegDict;
	PIMuiAlert			pMuiAlert = NULL;
	PIMoaAppInfo		pAppInfo = NULL;
		
	// Get all interfaces and bail if they're not available
	// This is done so we don't register when these interfaces we will
	// need later are not available.
	pObj->pCallback->MoaCreateInstance(&CLSID(CMuiAlert), &IID(IMuiAlert), (PPMoaVoid)&pMuiAlert);
	if (!pMuiAlert) 
	{
		err = kMoaErr_BadInterface;
		goto exit_gracefully;
	}
	pObj->pCallback->QueryInterface(&IID_IMoaAppInfo, (PPMoaVoid)&pAppInfo);
	if (!pAppInfo) 
	{
		err = kMoaErr_BadInterface;
		goto exit_gracefully;
	}

	
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CInterrogator, &IID_IMoaMmInterrogator, &pRegDict);
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CInterrogator, &IID_IMoaMmTerminator, &pRegDict);

	#ifdef USING_INIT_FROM_DICT
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CChangeMe, &IID_IMoaInitFromDict, &pRegDict);
	#endif
	
	#ifdef USING_NOTIFICATION_CLIENT
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CInterrogator, &IID_IMoaNotificationClient, &pRegDict);
	#endif
	
	#ifdef USING_LOAD_CONTROL
	MoaLong		loadOptions = (kMoaXlc_PreloadAtLaunch | kMoaXlc_NeverUnload);
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CChangeMe, &IID_IMoaXtraLoadControl, &pRegDict);
	err = pRegDict->Put(kMoaRegType_XtraLoadControlOptions, &loadOptions, 0, kMoaRegKey_XtraLoadControlOptions);
	#endif

exit_gracefully:
	
	pMuiAlert->Release();
	pAppInfo->Release();

	X_STD_RETURN(err);
	X_EXIT
}

