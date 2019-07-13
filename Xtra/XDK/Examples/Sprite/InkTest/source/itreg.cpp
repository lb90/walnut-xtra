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
 *	Name: itreg.cpp
 *	
 * 	Purpose: Definitions of Register class interface(s) and methods for the
 *           InkTest Xtra.
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
	#include "ITReg.h"
#endif

#ifdef MACINTOSH
	#include <Resources.h>
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
BEGIN_DEFINE_CLASS_INTERFACE(CInkTestRegister, IMoaRegister) 
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
STDMETHODIMP MoaCreate_CInkTestRegister(CInkTestRegister * This)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;

	X_RETURN(MoaError, err);
	X_EXIT
}

/* --------------------------------------------------- MoaDestroy_CRegister */
STDMETHODIMP_(void) MoaDestroy_CInkTestRegister(CInkTestRegister * This)
{
}

/* ---------------------------------- CRegister_IMoaRegister Create/Destroy */
CInkTestRegister_IMoaRegister::CInkTestRegister_IMoaRegister(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CInkTestRegister_IMoaRegister::~CInkTestRegister_IMoaRegister() {}

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
STDMETHODIMP_(MoaError) CInkTestRegister_IMoaRegister::Register(PIMoaCache pCache,
    PIMoaDict pXtraDict)
{
	X_ENTER
	
	MoaError 	err = kMoaErr_NoErr;
	PIMoaDict 	pRegDict;
	long		count;
	
	/* Standard asset entries */
	char* 				symStr[kMyAssetTypeCount] 		= {"InkTest"};
	char* 				nameStr[kMyAssetTypeCount] 		= {"Ink Tester"};
	char* 				catStr[kMyAssetTypeCount] 		= {"Sprite Examples"};
	MoaMmActorType 		actorType[kMyAssetTypeCount] 	= { kMoaMmActorType_Sprite };
	MoaMmAssetCaps		assetCaps[kMyAssetTypeCount] 	= { kMoaMmAssetCaps_None };
	
	/* Optional entries - custom 16x16 icons.  The numbers
						  here refer to the resource IDs
						  for the PICT (Mac) or BITMAP (Win)
						  resources for the color & BW
						  versions of the icons.
	 */
	char*				colorIconResName[kMyAssetTypeCount]	= { "InkTest_ColorIcon" };
	char*				bwIconResName[kMyAssetTypeCount]	= { "InkTest_BWIcon" };

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
			&CLSID_CInkTestAsset,
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
			
		/* Register custom icons */

		RegisterIcon(pRegDict, colorIconResName[count], TRUE /* bColor */);
		RegisterIcon(pRegDict, bwIconResName[count], FALSE /* bColor */);
	}

	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CRegister_IMoaRegister::Register */
/* Convert C to Pascal string (for Mac code) */

STDMETHODIMP_(void) CInkTestRegister_IMoaRegister::CToPas(ConstPMoaChar pCStr, PMoaChar pPasStr)
{	
	PMoaChar pOrigPasStr = pPasStr;
	while (*pCStr)
		*(++pPasStr) = *(pCStr++);
	pOrigPasStr[0] = pPasStr - pOrigPasStr;
}

/* -------------------------------- CRegister_IMoaRegister::RegisterIcon */
/* Pulls an icon out of a Mac PICT or Windows BITMAP resource
   and registers it as a custom asset type icon.  This scheme
   can be used for both sprite and transition xtras.
*/

STDMETHODIMP_(MoaError) CInkTestRegister_IMoaRegister::RegisterIcon(PIMoaDict pDict, ConstPMoaChar pResName, MoaBoolParam bColor)
{
	XtraResourceCookie 	myCookie, saveCookie;
	MoaError	err = kMoaErr_NoErr;
	
	myCookie = pObj->pCallback->MoaBeginUsingResources(gXtraFileRef, &saveCookie);

#ifdef MACINTOSH
	/**** Macintosh Implementation ****/
	Handle hPic;
	Str255 pasResName;
		
	CToPas(pResName, (PMoaChar) pasResName);
	hPic = GetNamedResource('PICT', pasResName);
	if (hPic) 
	{
		LoadResource(hPic);
		HLock(hPic);
		
		/* Stuff the icon picture data into the registry */
		err = pDict->Put(
				bColor ? kMoaMmDictType_ColorIcon : kMoaMmDictType_BWIcon, 
				*hPic, 
				GetHandleSize(hPic), 
				bColor ? kMoaMmDictKey_ColorIcon : kMoaMmDictKey_BWIcon);

		HUnlock(hPic);
		ReleaseResource(hPic);
	} 
	else 
		err = kMoaErr_BadParam;
#endif /* MACINTOSH */
#ifdef WINDOWS
	/**** Windows Implementation ****/
	HGLOBAL hBitmap;
	LPVOID	pBitmap;
	DWORD	size;
	HRSRC hRsrc = FindResource((HINSTANCE) myCookie, pResName, MAKEINTRESOURCE(RT_BITMAP));
	if (hRsrc) 
	{
		hBitmap = LoadResource((HINSTANCE) myCookie, hRsrc);
		if (hBitmap) 
		{
			pBitmap = LockResource(hBitmap);
			size = SizeofResource((HINSTANCE) myCookie, hRsrc);
			err = pDict->Put(
					bColor ? kMoaMmDictType_ColorIcon : kMoaMmDictType_BWIcon, 
					pBitmap, 
					size, 
					bColor ? kMoaMmDictKey_ColorIcon : kMoaMmDictKey_BWIcon);
			UnlockResource(hBitmap);
			FreeResource(hBitmap);
		}
	} 
	else 
		err = kMoaErr_BadParam;
#endif /* WINDOWS */

	pObj->pCallback->MoaEndUsingResources(gXtraFileRef, saveCookie);

	return err;
}

