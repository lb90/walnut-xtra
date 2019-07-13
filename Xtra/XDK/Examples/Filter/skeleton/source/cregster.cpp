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
 *	Name: cregster.cpp
 *	
 * 	Purpose: Definitions of Register class interface(s) and methods for a 
 *           generic Xtra.
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
 *     cregster        <this file name>
 *     CRegister       <name of the class you defined>
 *     cfilter		   <name of header file for filter>
 *	   CFilter		   <name of class for your filter>
 *     
 *  3. Add and modify source code by looking for '--> insert' comment
 *  4. Create a Xtra.cpp file that contains the Xtra definition.
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

#include "cregster.h"
#include "cfilter.h"


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

#define kMyFilterCount	1

#define kMyFilterCaps	(kMoaMmXFilterCapsFlags_AffectsAlpha)		

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

/*
 * --> insert additional interface(s) -->
 */

/*****************************************************************************
 *  Data needed for Registering
 *	---------------------------
 *	Specific code needed to register different types of Xtras.  The skeleton
 *	source files include minimal implementations appropriate for each Xtra
 *	type.  Current necessary actions:
 *
 *	Scripting Xtra:				Add the Scripting Xtra Message Table
 *	Sprite Asset Xtra:			Nothing
 *	Tool Xtra:					Nothing
 *	Transition Asset Xtra		Nothing
 *  Filter Xtra					Add IID_IMoaBitmapFilter interface
 *****************************************************************************/ 

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
	MoaError err = kMoaErr_NoErr;

	/*
	 * --> insert additional code -->
	 */

	return err;
}

/* --------------------------------------------------- MoaDestroy_CRegister */
STDMETHODIMP_(void) MoaDestroy_CRegister(CRegister * This)
{
	/*
	 * --> insert additional code -->
	 */
}

/* ---------------------------------- CRegister_IMoaRegister Create/Destroy */
CRegister_IMoaRegister::CRegister_IMoaRegister(MoaError * pErr) { *pErr = (kMoaErr_NoErr); }
	
CRegister_IMoaRegister::~CRegister_IMoaRegister() {}

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
 *****************************************************************************/ 

/* ---------------------------------------------- CRegister_IMoaRegister::Register */
STDMETHODIMP CRegister_IMoaRegister::Register(PIMoaCache pCache, PIMoaDict pXtraDict)
{
	MoaError 	err = kMoaErr_NoErr;
	PIMoaDict 	pRegDict;
	long		count;
	/* Standard filter entries */
	/*
	 * --> insert appropriate names for the filter -->
	 */
	PMoaChar 		symStr[kMyFilterCount]			= {"testfilter"};
	PMoaChar 		nameStr[kMyFilterCount]			= {"Test Filter"};
	MoaClassID		classID[kMyFilterCount]			= { CLSID_CFilter };
	MoaMmXFilterCapsFlags filtCaps[kMyFilterCount]	= { kMyFilterCaps };
	
	#ifdef USING_LOAD_CONTROL
	MoaLong		loadOptions = (kMoaXlc_PreloadAtLaunch | kMoaXlc_NeverUnload);
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CInterrogate, &IID_IMoaXtraLoadControl, &pRegDict);
	err = pRegDict->Put(kMoaRegType_XtraLoadControlOptions, &loadOptions, 0, kMoaRegKey_XtraLoadControlOptions);
	#endif
	
	// I am NOT sure what this is for
	// err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CBitmapFilterRegister, &IID_IMoaRegister, &pRegDict);
	//if (err != kMoaErr_NoErr)
	//	return err;
		
	/* Register all of our filter types */
	/* NOTE:  Eventhough we are only registering one Filters we are using a loop so that 
	          it may be modified to allow the registration of multiple types of Filters
	          in the future. */
	for (count = 0; count < kMyFilterCount; count ++)
	{
		/* Register standard filter entries */
		err = pCache->AddRegistryEntry(pXtraDict, &classID[count], &IID_IMoaBitmapFilter, &pRegDict);
	
		if (err == kMoaErr_NoErr)
			err = pRegDict->Put(kMoaMmDictType_SymbolString, symStr[count], 0, kMoaMmDictKey_SymbolString);
					
		if (err == kMoaErr_NoErr)
			err = pRegDict->Put(kMoaMmDictType_DisplayNameString, nameStr[count], 0, kMoaMmDictKey_DisplayNameString);

		if (err == kMoaErr_NoErr)
			err = pRegDict->Put(kMoaMmDictType_AssetCaps, &filtCaps[count], sizeof(MoaMmAssetCaps), kMoaMmDictKey_AssetCaps);
	}

	/*
	 * --> insert additional code -->
	 */

	return err;
}
