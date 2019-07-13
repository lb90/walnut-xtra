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
 *	Name: ETReg.cpp
 *	
 * 	Purpose: Definitions of Register class interface(s) and methods for the
 *           EventTest Xtra.
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
	#include "ETReg.h"
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
BEGIN_DEFINE_CLASS_INTERFACE(CEventTestRegister, IMoaRegister) 
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
STDMETHODIMP MoaCreate_CEventTestRegister(CEventTestRegister * This)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;

	X_RETURN(MoaError, err);
	X_EXIT
}

/* --------------------------------------------------- MoaDestroy_CRegister */
STDMETHODIMP_(void) MoaDestroy_CEventTestRegister(CEventTestRegister * This)
{
}

/* ---------------------------------- CRegister_IMoaRegister Create/Destroy */
CEventTestRegister_IMoaRegister::CEventTestRegister_IMoaRegister(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CEventTestRegister_IMoaRegister::~CEventTestRegister_IMoaRegister() {}


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

#define kMyAssetTypeCount	1
/* ------------------------------------ CRegister_IMoaRegister::Register */
STDMETHODIMP_(MoaError) CEventTestRegister_IMoaRegister::Register(PIMoaCache pCache,
    PIMoaDict pXtraDict)
{
	X_ENTER
	
	/* variable declaration */
	MoaError 	err = kMoaErr_NoErr;
	PIMoaDict 	pRegDict;
	long		count;
	
	/* Standard asset entries */
	char* 				symStr[kMyAssetTypeCount] 		= {"EventTest"};
	char* 				nameStr[kMyAssetTypeCount] 		= {"Event and Actor Tester"};
	char* 				catStr[kMyAssetTypeCount] 		= {"Sprite Examples"};
	MoaMmActorType 		actorType[kMyAssetTypeCount] 	= { kMoaMmActorType_Sprite };
	MoaMmAssetCaps		assetCaps[kMyAssetTypeCount] 	= { kMoaMmAssetCaps_CanImageStaticThumb };
	
	#ifdef USING_LOAD_CONTROL
	MoaLong		loadOptions = (kMoaXlc_PreloadAtLaunch | kMoaXlc_NeverUnload);
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CInterrogate, &IID_IMoaXtraLoadControl, &pRegDict);
	err = pRegDict->Put(kMoaRegType_XtraLoadControlOptions, &loadOptions, 0, kMoaRegKey_XtraLoadControlOptions);
	#endif
	
	/* Register all of our asset types */
	/* NOTE:  Eventhough we are only registering one Xtra we are using a loop so that 
	          it may be modified to allow the registration of multiple types of Xtras
	          in the future. */
	for (count = 0; count < kMyAssetTypeCount; count ++)
	{
		if ((err = pCache->AddRegistryEntry(pXtraDict,
			&CLSID_CEventTestAsset,
			&IID_IMoaMmXAsset,
			&pRegDict))
			!= kMoaErr_NoErr) break;
	
		/* Register standard asset entries */
		
		if ((err = pRegDict->Put(kMoaMmDictType_SymbolString, 
			symStr[count], 0, kMoaMmDictKey_SymbolString)) 
			!= kMoaErr_NoErr) break;
			
		if ((err = pRegDict->Put(kMoaMmDictType_DisplayNameString, 
			nameStr[count], 0, kMoaMmDictKey_DisplayNameString)) 
			!= kMoaErr_NoErr) break;

		if ((err = pRegDict->Put(kMoaMmDictType_DisplayCategoryString, 
			catStr[count], 0, kMoaMmDictKey_DisplayCategoryString)) 
			!= kMoaErr_NoErr) break;
			
		if ((err = pRegDict->Put(kMoaMmDictType_ActorType, 
			&actorType[count], 0, kMoaMmDictKey_ActorType)) 
			!= kMoaErr_NoErr) break;
			
		if ((err = pRegDict->Put(kMoaMmDictType_AssetCaps, 
			&assetCaps[count], sizeof(MoaMmAssetCaps), kMoaMmDictKey_AssetCaps)) 
			!= kMoaErr_NoErr) break;
								
		/* Mark this Xtra "Safe for Shockwave" */
		MoaBool bItsSafe = TRUE;
		if ((err = pRegDict->Put(kMoaMmDictType_SafeForShockwave,
			&bItsSafe, 	sizeof( bItsSafe ), kMoaMmDictKey_SafeForShockwave))
			!= kMoaErr_NoErr) break;
			
	}

	
	X_STD_RETURN(err);
	X_EXIT
}

