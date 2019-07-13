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
 *	Name: beepreg.cpp
 *	
 * 	Purpose: Definitions of Register class interface(s) and methods for the 
 *           NewBeep Tool Xtra.
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

#include "beepreg.h"


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
BEGIN_DEFINE_CLASS_INTERFACE(CBeepReg, IMoaRegister) 
END_DEFINE_CLASS_INTERFACE

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

/* ---------------------------------------------------- MoaCreate_CRegister */
STDMETHODIMP MoaCreate_CBeepReg(CBeepReg * This)
{
	UNUSED(This);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;

	X_RETURN(MoaError, err);
	X_EXIT
}

/* --------------------------------------------------- MoaDestroy_CRegister */
STDMETHODIMP_(void) MoaDestroy_CBeepReg(CBeepReg * This)
{
	UNUSED(This);
	X_ENTER

	X_RETURN_VOID;
	X_EXIT
}

/* ---------------------------------- CRegister_IMoaRegister Create/Destroy */
CBeepReg_IMoaRegister::CBeepReg_IMoaRegister(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CBeepReg_IMoaRegister::~CBeepReg_IMoaRegister() {}

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
#define kMyToolCount		1

typedef MoaLong MyData;

/* -------------------------------------- CRegister_IMoaRegister::Register */
STDMETHODIMP CBeepReg_IMoaRegister::Register(PIMoaCache pCache, PIMoaXtraEntryDict pXtraDict)
{
	X_ENTER
	
	/* variable declaration */
	MoaError 				err = kMoaErr_NoErr;
	PIMoaDict 				pRegDict;
	PIMoaNotificationClient pNotify = NULL;
	PIMuiDialog				pMuiDialog = NULL;
	PIMoaMmValue			pMmValue = NULL;
	MoaLong					count = 0;
#ifdef USING_LOAD_CONTROL
	MoaLong					loadOptions = (kMoaXlc_PreloadAtLaunch | kMoaXlc_NeverUnload);
#endif
	/* Standard tool entries */
	char* 					nameStr[kMyToolCount] 		= {"Beep Tool"};
	char* 					catStr[kMyToolCount] 		= {"Tool Examples"};
	MoaBoolParam			alwaysEnabled[kMyToolCount]	= { TRUE /* alwaysEnabled */ };
	
	// Safety check for required interfaces
	pObj->pCallback->MoaCreateInstance(&CLSID(CMui), &IID(IMuiDialog), (PPMoaVoid)&pMuiDialog);
	if (!pMuiDialog) 
		goto exit_gracefully;

	pObj->pCallback->MoaCreateInstance(&CLSID(CBeepTool), &IID(IMoaNotificationClient), (PPMoaVoid)&pNotify);
	if (!pNotify) 
		goto exit_gracefully;

	pObj->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid)&pMmValue);
	if (!pMmValue) 
		goto exit_gracefully;


#ifdef USING_LOAD_CONTROL
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CInterrogate, &IID_IMoaXtraLoadControl, &pRegDict);
	err = pRegDict->Put(kMoaRegType_XtraLoadControlOptions, &loadOptions, 0, kMoaRegKey_XtraLoadControlOptions);
#endif


	/* Register all of our tools */
	/* NOTE:  Eventhough we are only registering one Xtra we are using a loop so that 
	          it may be modified to allow the registration of multiple types of Xtras
	          in the future. */
	for (count = 0; count < kMyToolCount; count ++)
	{
		if ((err = pCache->AddRegistryEntry(
			pXtraDict,
			&CLSID_CBeepTool,
			&IID_IMoaMmXTool,
			&pRegDict))
			!= kMoaErr_NoErr) break;
	
		/* Register standard tool entries */
		if ((err = pRegDict->Put(kMoaMmDictType_DisplayNameString, 
			nameStr[count], 0, kMoaMmDictKey_DisplayNameString)) 
			!= kMoaErr_NoErr) break;

		if ((err = pRegDict->Put(kMoaMmDictType_DisplayCategoryString, 
			catStr[count], 0, kMoaMmDictKey_DisplayCategoryString)) 
			!= kMoaErr_NoErr) break;

		if ((err = pRegDict->Put(kMoaDrDictType_AlwaysEnabled, 
			&alwaysEnabled[count], 0, kMoaDrDictKey_AlwaysEnabled)) 
			!= kMoaErr_NoErr) break;
											
	}

exit_gracefully:
	if(pMuiDialog)
		pMuiDialog->Release();
	if(pNotify)
		pNotify->Release();
	if(pMmValue)
		pMmValue->Release();

	X_STD_RETURN(err);
	X_EXIT
}

