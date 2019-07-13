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
 *	Name: tileregister.cpp
 *	
 * 	Purpose: Definitions of Register class interface(s) and methods for a 
 *           generic Xtra.
 *	
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 ****************************************************************************/ 
#ifndef INITGUID        /* INITGUID causes the actual GUIDs to be declared, */ 
	#define INITGUID      /* not just references to them. Must be declared in */
#endif                  /* only one file. */          

#include "tileregister.h"
#include "tilefilter.h"


/*****************************************************************************
 *  INTERNAL PROTOTYPES(S)
 ****************************************************************************/

/*****************************************************************************
 *  INTERNAL ROUTINE(S)
 ****************************************************************************/

#define kMyFilterCount	1

#define kMyFilterCaps	(kMoaMmXFilterCapsFlags_AffectsAlpha)		

/*****************************************************************************
 *  CLASS INTERFACE(S)
 ****************************************************************************/ 
BEGIN_DEFINE_CLASS_INTERFACE(TRegister, IMoaRegister) 
END_DEFINE_CLASS_INTERFACE

/*****************************************************************************
 *  CREATE AND DESTROY METHODS
 ****************************************************************************/ 

/* ---------------------------------------------------- MoaCreate_TRegister */
STDMETHODIMP MoaCreate_TRegister(TRegister * This)
{
	MoaError err = kMoaErr_NoErr;
	return err;
}

/* --------------------------------------------------- MoaDestroy_TRegister */
STDMETHODIMP_(void) MoaDestroy_TRegister(TRegister * This)
{
}

/* ---------------------------------- TRegister_IMoaRegister Create/Destroy */
TRegister_IMoaRegister::TRegister_IMoaRegister(MoaError * pErr) { *pErr = (kMoaErr_NoErr); }
	
TRegister_IMoaRegister::~TRegister_IMoaRegister() {}

/*****************************************************************************
 *  METHOD IMPLEMENTATION(S)
 *****************************************************************************/ 

/* ---------------------------------------------- TRegister_IMoaRegister::Register */
STDMETHODIMP TRegister_IMoaRegister::Register(PIMoaCache pCache, PIMoaDict pXtraDict)
{
	MoaError err = kMoaErr_NoErr;
	PIMoaDict pRegDict;
	long count;
	MoaBool bItsSafe = TRUE;
	/* Standard filter entries */
	PMoaChar 		symStr[kMyFilterCount]			= {"tilefilter"};
	PMoaChar 		nameStr[kMyFilterCount]			= {"Test Tile Filter"};
	MoaClassID		classID[kMyFilterCount]			= { CLSID_TileFilter };
	MoaMmXFilterCapsFlags filtCaps[kMyFilterCount]	= { kMyFilterCaps };
	
	/* Register all of our filter types */
	/* NOTE:  Eventhough we are only registering one Filter we are using a loop so that 
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

		// Mark this Xtra "Safe for Shockwave"
		if (err == kMoaErr_NoErr)
			err = pRegDict->Put(kMoaMmDictType_SafeForShockwave, &bItsSafe, sizeof( bItsSafe ), kMoaMmDictKey_SafeForShockwave);
	}

	return err;
}
