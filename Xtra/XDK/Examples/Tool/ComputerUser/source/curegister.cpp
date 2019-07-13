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
 *	Name: curegister.cpp
 *	
 * 	Purpose: Definitions of Register class interface(s) and methods for the
 *           ComputerUser Xtra.
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

#include "curegister.h"

/*****************************************************************************
 *  INTERNAL PROTOTYPES(S)
 ****************************************************************************/

/*****************************************************************************
 *  INTERNAL ROUTINE(S)
 ****************************************************************************/

/*****************************************************************************
 *  CLASS INTERFACE(S)
 ****************************************************************************/ 
BEGIN_DEFINE_CLASS_INTERFACE(CURegister, IMoaRegister) 
END_DEFINE_CLASS_INTERFACE

/*****************************************************************************
 *  Data needed for Registering
 *****************************************************************************/ 

/*****************************************************************************
 *  CREATE AND DESTROY METHODS
 ****************************************************************************/ 

/* ---------------------------------------------------- MoaCreate_CURegister */
STDMETHODIMP MoaCreate_CURegister(CURegister * This)
{
	MoaError err = kMoaErr_NoErr;
	return err;	
}

/* --------------------------------------------------- MoaDestroy_CURegister */
STDMETHODIMP_(void) MoaDestroy_CURegister(CURegister * This)
{
}

/* ---------------------------------- CURegister_IMoaRegister Create/Destroy */
CURegister_IMoaRegister::CURegister_IMoaRegister(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CURegister_IMoaRegister::~CURegister_IMoaRegister() {}


/*****************************************************************************
 *  METHOD IMPLEMENTATION(S)
 *****************************************************************************/ 
static MoaError	_CheckVersion(PIMoaCallback pCallback)
{
	MoaChar buf[32] = {0};
	PIMoaAppInfo pAppInfo = NULL;
	MoaError err = kMoaErr_NoErr;

	err = pCallback->QueryInterface(&IID_IMoaAppInfo, (PMoaVoid*) &pAppInfo);
	if (err != kMoaErr_NoErr) goto exit_gracefully;

	// We only support Director
	err = pAppInfo->GetInfo(kMoaAppInfo_ProductName, buf, sizeof(buf));
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	if (strcmp(buf, "Director") != 0)
	{
		err = kMoaErr_NotEnoughSSW;
		goto exit_gracefully;
	}

	// Parse version; if < 11 then we alert and bail
	err = pAppInfo->GetInfo(kMoaAppInfo_ProductVersion, buf, sizeof(buf));
	if (err == kMoaErr_NoErr)
	{
		if (buf[2]=='.')	// we have a two digit version number
		{
			if (buf[0]=='1' && buf[1]=='0')
				err = kMoaErr_NotEnoughSSW; // "10" is too old, anything else is OK
		}
		else if (buf[1]=='.')
		{
			// a one digit version number, which is too old for us
			err = kMoaErr_NotEnoughSSW;
		}
	}

exit_gracefully:

	if (pAppInfo)
		pAppInfo->Release();
	return err;
}

#define kMyToolCount		1

typedef MoaLong MyData;

/* -------------------------------------- CURegister_IMoaRegister::Register */
STDMETHODIMP CURegister_IMoaRegister::Register(PIMoaCache pCache, PIMoaXtraEntryDict pXtraDict)
{
	MoaError 	err = kMoaErr_NoErr;
	PIMoaDict 	pRegDict;
	long		count;
	
	/* Standard tool entries */
	char* 				nameStr[kMyToolCount] 		= {"Computer User Tool"};
	char* 				catStr[kMyToolCount] 		= {"Tool Examples"};
	MoaBoolParam		alwaysEnabled[kMyToolCount]	= { TRUE /* alwaysEnabled */ };
	
	err = _CheckVersion(pObj->pCallback);
	if (err == kMoaErr_NoErr)
	{
		/* Register all of our tools */
		for (count = 0; count < kMyToolCount; count ++)
		{
			if ((err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CUTool, &IID_IMoaMmXTool, &pRegDict)) != kMoaErr_NoErr) 
				break;
		
			/* Register standard tool entries */
			if ((err = pRegDict->Put(kMoaMmDictType_DisplayNameString, nameStr[count], 0, kMoaMmDictKey_DisplayNameString)) != kMoaErr_NoErr) 
				break;

			if ((err = pRegDict->Put(kMoaMmDictType_DisplayCategoryString, catStr[count], 0, kMoaMmDictKey_DisplayCategoryString)) != kMoaErr_NoErr) 
				break;

			if ((err = pRegDict->Put(kMoaDrDictType_AlwaysEnabled, &alwaysEnabled[count], 0, kMoaDrDictKey_AlwaysEnabled)) != kMoaErr_NoErr) 
				break;
		}
	}

	return err;
}
