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
 *	Name: ovalasst.cpp
 *	
 * 	Purpose: Definitions of Asset class interface(s) and methods for the 
 *           Oval Sprite Xtra example.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed for 
 *  this particular class. In addition, this file may include other .h files 
 *  defining additional callback interfaces for use by a third party.   
 *****************************************************************************/ 
#include "ovalasst.h"

#ifdef MACINTOSH
#include <QuickDraw.h>
#endif /* /*  MACINTOSH /* */

#include <String.h> 

/*****************************************************************************
 *  FILE SPECIFIC DEFINE(S)
 *  -----------------------
 *	Any #define's specific to this file.  
 *****************************************************************************/ 
/* Sprite notification messages */ 
enum
{
	kSprNotifyMsg_SetParentAsset = 1
};

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
BEGIN_DEFINE_CLASS_INTERFACE(COvalSpriteAsset, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE

BEGIN_DEFINE_CLASS_INTERFACE(COvalSpriteAsset, IMoaMmXAsset)
END_DEFINE_CLASS_INTERFACE

/*****************************************************************************
 *  CREATE AND DESTROY METHODS
 *  --------------------------
 *  Every interface and class has an associated 'Create' and 'Destroy' pair.
 *  'Create' methods are typically used to acquire interface(s), allocate 
 *  memory, and intialize variables. 'Destroy' methods are typically used to 
 *  release interfaces and free memory. To create 'empty' create and destroy 
 *  methods for an interface, use the STD_INTERFACE_CREATE_DESTROY macro.
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

/* ---------------------------------------------------- MoaCreate_COvalSpriteAsset */
STDMETHODIMP  		MoaCreate_COvalSpriteAsset(COvalSpriteAsset * This)
{
	MoaError	err = kMoaErr_NoErr;
	
	This->pCallback->MoaCreateInstance(&CLSID_CMoaFile2, &IID_IMoaFile2, (PPMoaVoid)&This->pFile);
	
	This->pCallback->QueryInterface(&IID_IMoaMmUtils,(PPMoaVoid)&This->pUtils);
	
	This->pCallback->QueryInterface(&IID_IMoaMmImage,(PPMoaVoid)&This->pImage);

	This->pUtils->StringToSymbol("red", &This->red);
	This->pUtils->StringToSymbol("green", &This->green);
	This->pUtils->StringToSymbol("blue", &This->blue);
	This->pUtils->StringToSymbol("clickable", &This->click);
	This->pUtils->StringToSymbol("image", &This->image);

	return(err);
}

/* --------------------------------------------------- MoaDestroy_COvalSpriteAsset */
STDMETHODIMP_(void) MoaDestroy_COvalSpriteAsset(COvalSpriteAsset * This)
{
	This->pUtils->Release();
	This->pFile->Release();
	if (This->pImage)
		This->pImage->Release();
	
	return;
}

/* macros that create constructors and destructors for the following interface(s) */

STD_INTERFACE_CREATE_DESTROY(COvalSpriteAsset, IMoaInitFromDict)
STD_INTERFACE_CREATE_DESTROY(COvalSpriteAsset, IMoaMmXAsset)


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

#ifdef USING_INIT_FROM_DICT
typedef MoaLong MyData;

/* ------------------------------- COvalSpriteAsset_IMoaInitFromDict::InitFromDict */
STDMETHODIMP_(MoaError) COvalSpriteAsset_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
{
	MoaError	err = kMoaErr_NoErr;
	MyData		myData;
	
	/* This method lets us grab any information in our dictionary 
	   entry that might be useful for initialization.   

	   This particular cast member doesn't need anything from the dict. 

	   This code shows how you'd grab something from the dict. 
	   Normally you'd want to use the resulting value to initialize 
	   yourself in a certain way (perhaps save it off in an instance 
	   var...) */
	
	if (pRegistryDict)
	{
		if (pRegistryDict->Get(kMoaDictType_Bytes, 
				&myData, sizeof(MyData), "myData") == kMoaErr_NoErr)
		{
			/* The data is now in local variable myData */
		}
	}
	
	return(err);
}
#endif

/* ---------------------------------------- COvalSpriteAsset_IMoaMmXAsset::GetProp */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::GetProp(MoaMmSymbol symbol, 
    PMoaMmValue	pPropValue)
{
	MoaError	err = kMoaErr_NoErr;

	// Check to see what symbol is sent, convert it to a moa value 
	// and get the appropriate value
	if( symbol == pObj->red )
		pObj->pUtils->IntegerToValue(pObj->redValue, pPropValue);
	
	else if ( symbol == pObj->green )
		pObj->pUtils->IntegerToValue(pObj->greenValue, pPropValue);
	
	else if( symbol == pObj->blue )
		pObj->pUtils->IntegerToValue(pObj->blueValue, pPropValue);

	else if( symbol == pObj->click )
		pObj->pUtils->IntegerToValue(pObj->clickValue, pPropValue); 

	else if( symbol == pObj->image && pObj->pImage )
	{
		// with D7 and D8 there isn't any image interface
		// with D8.5 the image interface is available and functional
		err = GetImage(pPropValue);
	}

	else
		err = kMoaMmErr_PropertyNotFound;	// be sure to flag so caller doesn't process non-existant value

	return( err );
}

/* ---------------------------------------- COvalSpriteAsset_IMoaMmXAsset::SetProp */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::SetProp(MoaMmSymbol symbol,
	ConstPMoaMmValue pPropValue)
{
	MoaError	err = kMoaErr_NoErr;

	// Sees which symbol Director wants to set, converts it into an integer, 
	// and sets the value in an instance variable
	if( symbol == pObj->red )
		pObj->pUtils->ValueToInteger(pPropValue, &(pObj->redValue));

	else if ( symbol == pObj->green )
		pObj->pUtils->ValueToInteger(pPropValue, &(pObj->greenValue));

	else if( symbol == pObj->blue )
		pObj->pUtils->ValueToInteger(pPropValue, &(pObj->blueValue));

	else if( symbol == pObj->click )
		pObj->pUtils->ValueToInteger(pPropValue, &(pObj->clickValue));

	return( err );
}

/* ----------------------------------- COvalSpriteAsset_IMoaMmXAsset::CallFunction */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::CallFunction(MoaMmSymbol methodName, 
	MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{	
	MoaError	err;
	
	/* We have no asset functions we wish export */

	err = kMoaMmErr_FunctionNotFound;

	return(err);
}

/* ------------------------------------ COvalSpriteAsset_IMoaMmXAsset::SetCallback */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::SetCallback(PIMoaMmAssetCallback pAssetCallback)
{

	/* We don't use any of the callbacks, so do nothing.
	   If we did want to use them, we'd AddRef() on the
	   supplied interface and save it off in an instance var. */

	return(kMoaErr_NoErr);
}

/* ----------------------------------------- COvalSpriteAsset_IMoaMmXAsset::Notify */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	/* This method is intended for communicating between the
	   asset and its xtras.  The developer is free to define
	   message codes for private use.  This may be called
	   directly by an actor, or through the 
	   IMoaMmSpriteCallback::NotifyAsset() callback. */

	return(kMoaErr_NoErr);
}

#define DEFAULT_OVAL_SIZE 32

/* --------------------------------- COvalSpriteAsset_IMoaMmXAsset::GetDefaultRect */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::GetDefaultRect(PMoaRect pRect)
{
	MoaError	err = kMoaErr_NoErr;
	
	/* We return our default rectangle centered around (0,0). 
	   (0,0) here is corresponds to the "loc" of the sprite, 
	   This is the point of reference for the sprite. 
	   Dragging the sprite out to the stage will position the sprite 
	   such that the loc is at the mouse drop position. 
	
	   32 x 32 centered about (0,0) */
	
	pRect->top = -(DEFAULT_OVAL_SIZE / 2);
	pRect->left = -(DEFAULT_OVAL_SIZE / 2);
	pRect->bottom = (DEFAULT_OVAL_SIZE / 2);
	pRect->right = (DEFAULT_OVAL_SIZE / 2);
	
	return(err);
}

/* ------------------------------ COvalSpriteAsset_IMoaMmXAsset::PrepareNewContent */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::PrepareNewContent(MoaMmNewFlags newFlags)
{
	MoaError	err = kMoaErr_NoErr;
	
	/* This method is called to have the class set-up its default state 
	   when a new cast member is created (note that this is different 
	   from when a new instance of COvalSpriteAsset is created; instances of 
	   the XAsset are created at both movie-open time (for  
	   pre-existing cast members being read-in) and new cast member 
	   creation time.  PrepareNewContent() is only called at new cast member 
	   creation time, just after the XAsset instance is created. 
	
	   A new cast member is being created in memory.  Set up 
	   default media and props. 
	
	   Dispose of any media and props we may have currently in memory */
	
	COvalSpriteAsset_IMoaMmXAsset::FreeMedia();
	COvalSpriteAsset_IMoaMmXAsset::FreeProps();
	
	/* Initialize props / media to default values here. 
	   
	   The host app assumes that both props and media are "dirty" 
	   when a new cast member is created. 
	
	   We are now "loaded" in the sense that we have everything we need from 
	   disk (there isn't anything on disk yet, so we exist only in-memory 
	   at this point). */
	
	return err;
}


/* --------------------------------- COvalSpriteAsset_IMoaMmXAsset::StreamOutMedia */
#define	MEDIA_BYTES	100

STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::StreamOutMedia(PIMoaStream pOutStream)
{
	MoaError			err = kMoaErr_NoErr;
	MoaChar				foo[MEDIA_BYTES];
	MoaShort			i;

	for (i=0; i < MEDIA_BYTES; i++)
		foo[i] = (MoaChar) i;

	/* This is test code.  Write out our "media" data. */

	/* Write some test data */
	err = pOutStream->Open(kMoaStreamOpenAccess_WriteOnly, 
						   kMoaStreamSetPositionType_None);
	if (err != kMoaErr_NoErr) goto done;					

	/* Write in 5 chunks */
	for (i=0; i < 5; i++)
	{
		err = pOutStream->Write(&foo[(MEDIA_BYTES / 5) * i], 
							    (MEDIA_BYTES / 5),
							    NULL);

		if (err != kMoaErr_NoErr) goto done;					
	}
	
	err = pOutStream->Close();

done:
	return(err);
}

/* ---------------------------------- COvalSpriteAsset_IMoaMmXAsset::StreamInMedia */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::StreamInMedia(PIMoaStream pInStream)
{
	MoaError		   err = kMoaErr_NoErr;
	MoaChar			   foo[MEDIA_BYTES];
	MoaShort		   i;
	MoaLong			   bufReadBytes = MEDIA_BYTES / 5;
	
	/* This is test code.  Read in our "media" data. */
	
	/* Zero out our test read buffer */
	for (i=0; i < MEDIA_BYTES; i++)
		foo[i] = 0;

	/* Read the test data */
	err = pInStream->Open(kMoaStreamOpenAccess_ReadOnly,
	                      kMoaStreamSetPositionType_None);
	if (err != kMoaErr_NoErr) goto done;					

	/* Read in 5 chunks */
	for (i=0; i < 5; i++)
	{
		err = pInStream->Read(&foo[(MEDIA_BYTES / 5) * i], 
							  bufReadBytes,
							  NULL);

		if (err != kMoaErr_NoErr) goto done;
	}
	
	err = pInStream->Close();

	/* Returning kMoaErr_NoErr to host app indicates that 
	   we have successfully loaded. */

done:
	return(err);
}

/* -------------------------------------- COvalSpriteAsset_IMoaMmXAsset::FreeMedia */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::FreeMedia()
{
	/* Nothing to do */
	return(kMoaErr_NoErr);
}

/* ------------------------------ COvalSpriteAsset_IMoaMmXAsset::GetInMemMediaSize */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::GetInMemMediaSize(MoaUlong * pSizeBytes)
{
	MoaError	err = kMoaErr_NoErr;
	*pSizeBytes = 0;
	return(err);
}

/* -------------------------- COvalSpriteAsset_IMoaMmXAsset::GetStreamOutMediaSize */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::GetStreamOutMediaSize(MoaUlong * pSizeBytes)
{
	MoaError	err = kMoaErr_NoErr;
	*pSizeBytes = MEDIA_BYTES;
	return(err);
}

/* --------------------------------- COvalSpriteAsset_IMoaMmXAsset::StreamOutProps */
#define PROPS_BYTES 50

STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::StreamOutProps(PIMoaStream pOutStream)
{
	MoaError			err = kMoaErr_NoErr;
	MoaChar				foo[PROPS_BYTES];
	MoaShort			i;

	for (i=0; i < PROPS_BYTES; i++)
		foo[i] = (MoaChar) i;

	/* This is test code.  Write out our "media" data. */

	/* Write some test data */
	err = pOutStream->Open(kMoaStreamOpenAccess_WriteOnly, 
						   kMoaStreamSetPositionType_None);
	if (err != kMoaErr_NoErr) goto done;					

	/* Write in 5 chunks */
	for (i=0; i < 5; i++)
	{
		err = pOutStream->Write(&foo[(PROPS_BYTES / 5) * i], 
							    (PROPS_BYTES / 5),
							    NULL);

		if (err != kMoaErr_NoErr) goto done;					
	}
	
	err = pOutStream->Close();

done:
	return(err);
}

/* ---------------------------------- COvalSpriteAsset_IMoaMmXAsset::StreamInProps */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::StreamInProps(PIMoaStream pInStream)
{
	MoaError		   err = kMoaErr_NoErr;
	MoaChar			   foo[PROPS_BYTES];
	MoaShort		   i;
	MoaLong			   bufReadBytes = PROPS_BYTES / 5;
	
	/* This is test code.  Read in our "media" data. */
	
	/* Zero out our test read buffer */
	for (i=0; i < PROPS_BYTES; i++)
		foo[i] = 0;

	/* Read the test data */
	err = pInStream->Open(kMoaStreamOpenAccess_ReadOnly,
						  kMoaStreamSetPositionType_None);
	if (err != kMoaErr_NoErr) goto done;					

	/* Read in 5 chunks */
	for (i=0; i < 5; i++)
	{
		err = pInStream->Read(&foo[(PROPS_BYTES / 5) * i], 
							  bufReadBytes,
							  NULL);

		if (err != kMoaErr_NoErr) goto done;
	}
	
	err = pInStream->Close();

	/* Returning kMoaErr_NoErr to host app indicates that 
	   we have successfully loaded. */
		
done:
	return(err);
}

/* -------------------------------------- COvalSpriteAsset_IMoaMmXAsset::FreeProps */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::FreeProps()
{
	return(kMoaErr_NoErr);
}

/* ------------------------------ COvalSpriteAsset_IMoaMmXAsset::GetInMemPropsSize */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::GetInMemPropsSize(MoaUlong * pSizeBytes)
{
	MoaError	err = kMoaErr_NoErr;
	*pSizeBytes = 0;
	return(err);
}

/* -------------------------- COvalSpriteAsset_IMoaMmXAsset::GetStreamOutPropsSize */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::GetStreamOutPropsSize(MoaUlong * pSizeBytes)
{
	MoaError	err = kMoaErr_NoErr;	
	*pSizeBytes = PROPS_BYTES;
	return(err);
}

/* -------------------------------------- COvalSpriteAsset_IMoaMmXAsset::NewXActor */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::NewXActor(PMoaVoid * ppXActor) 
{
	MoaError err = kMoaErr_NoErr;
	PIMoaMmXAsset pSelf = NULL;	
	PIMoaMmXSpriteActor pNewActor = NULL;

	/* Do a createinstance using IMoaMmXSpriteActor for return to caller... */
	if ((err = pObj->pCallback->MoaCreateInstance(&CLSID_COvalSpriteActor, 
	                                              &IID_IMoaMmXSpriteActor, 
	                                              (PMoaVoid*) &pNewActor)
		) == kMoaErr_NoErr)
	{
		pSelf = (PIMoaMmXAsset) this; /* Because the is C++ have to use 'this'
		                                 instead of 'This' */
			
		/* Tell actor who its parent is, in case it's interested. 
			It should AddRef() on the interface if it wants to
			hold onto it.
		*/
		
		pNewActor->Notify(kSprNotifyMsg_SetParentAsset,pSelf);
				
		*ppXActor = pNewActor;
	}

	return(err);
}

/* ------------------------------ COvalSpriteAsset_IMoaMmXAsset::InvokeMediaEditor */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::InvokeMediaEditor(PMoaMmModifyFlags	pModifyFlags)
{
	/* We don't have a properties editor */
	return(kMoaErr_NoErr);
}

/* ------------------------------ COvalSpriteAsset_IMoaMmXAsset::InvokePropsEditor */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::InvokePropsEditor()
{
	/* We don't have a properties editor */	
	return(kMoaErr_NoErr);
}

/* ------------------------------ COvalSpriteAsset_IMoaMmXAsset::FinishPropsEditor */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::FinishPropsEditor(MoaBoolParam commitChanges,					
	PMoaMmModifyFlags pModifyFlags)
{
	/* We don't have a properties editor */	
	return(kMoaErr_NoErr);
}

/* ---------------------------------- COvalSpriteAsset_IMoaMmXAsset_InvokeAboutBox */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::InvokeAboutBox()
{
	/* We don't have an about box */
	return(kMoaErr_NoErr);
}

/* ------------------------------- COvalSpriteAsset_IMoaMmXAsset::ImageStaticThumb */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::ImageStaticThumb(PIMoaMmGC pGC, 
	ConstPMoaRect pImageRect)
{
	/* We don't support this capability; Director should never call this, 
	   because it wasn't specified in the assetCaps (above). */
	
	return(kMoaMmErr_NotImplemented);
}

/* ---------------------------- COvalSpriteAsset_IMoaMmXAsset::BeginImageAnimThumb */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::BeginImageAnimThumb(PIMoaMmGC pGC, 
	ConstPMoaRect pImageRect)
{
	/* We don't support this capability; Director should never call this, 
	   because it wasn't specified in the assetCaps (above). */
	
	return(kMoaMmErr_NotImplemented);
}

/* -------------------------- COvalSpriteAsset_IMoaMmXAsset::ServiceImageAnimThumb */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::ServiceImageAnimThumb(PIMoaMmGC pGC, 
	ConstPMoaRect pImageRect)
{
	/* We don't support this capability; Director should never call this, 
	   because it wasn't specified in the assetCaps (above). */
	
	return(kMoaMmErr_NotImplemented);
}

/* ------------------------------ COvalSpriteAsset_IMoaMmXAsset::EndImageAnimThumb */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::EndImageAnimThumb(PIMoaMmGC pGC, 
	ConstPMoaRect pImageRect)
{
	/* We don't support this capability; Director should never call this, 
	   because it wasn't specified in the assetCaps (above). */
	
	return(kMoaMmErr_NotImplemented);
}

static void mystrncpy(PMoaChar pDest, PMoaChar pSrc, MoaLong maxLen)
/* Copy at most maxLen char from source to dest until end (null) of dest is reached.  Does not pad src with nulls. 
   Differs from strncpy in that it doesn't require an int length arg (which may be 2 or 4 bytes) and doesn't pad
   the source with nulls since the maxLen may be big and this would be inefficient. 
*/
{
	register MoaLong i; 
	
	for (i = 0; (i < maxLen) && *pSrc; i++)
	{
		*pDest++ = *pSrc++;
	} 
	if (i < maxLen)
	{
		*pDest = 0;
	}
}

/* ---------------------------------------- COvalSpriteAsset_IMoaMmXAsset::Getimage */
STDMETHODIMP COvalSpriteAsset_IMoaMmXAsset::GetImage(PMoaMmValue pPropValue)
{
	MoaError	err = kMoaErr_NoErr;
	MoaMmSymbol shapetypesym, ovalsym, colorsym, linesizesym;
	MoaRect aRect = {0, 0, DEFAULT_OVAL_SIZE, DEFAULT_OVAL_SIZE};
	MoaMmValue propvalue = {0,0};
	MoaMmValue rectvalue = {0,0};
	MoaMmValue tempvalue = {0,0};
	MoaMmValue proplistvalue = {0,0};
	PIMoaMmList pListInterface = NULL;
	PIMoaMmValue pValueInterface = NULL;
	PIMoaMmColor pColorInterface = NULL;
	MoaMmColorSpec colorspec = {{(MoaMmColorIndex)0xff}, kMoaMmColorSpec_Index} ;
	
	colorspec.type = kMoaMmColorSpec_RGB;
	colorspec.color.rgb.red = 0;
	colorspec.color.rgb.green = 0;
	colorspec.color.rgb.blue = 0;
	
	pObj->pUtils->StringToSymbol("shapetype", &shapetypesym);
	pObj->pUtils->StringToSymbol("oval", &ovalsym);
	pObj->pUtils->StringToSymbol("color", &colorsym);
	pObj->pUtils->StringToSymbol("linesize", &linesizesym);

	err = pObj->pCallback->QueryInterface(&IID_IMoaMmList, (PPMoaVoid) &pListInterface);

	if (err == kMoaErr_NoErr)
		err = pObj->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid) &pValueInterface);

	if (err == kMoaErr_NoErr)
		err = pObj->pCallback->QueryInterface(&IID_IMoaMmColor, (PPMoaVoid) &pColorInterface);

	if (err == kMoaErr_NoErr)
		err = pObj->pUtils->RectToValue(&aRect, &rectvalue);

	if (err == kMoaErr_NoErr)
		err = pListInterface->NewPropListValue( &proplistvalue );

	pValueInterface->SymbolToValue( shapetypesym, &propvalue);
	pValueInterface->SymbolToValue( ovalsym, &tempvalue);
	if (err == kMoaErr_NoErr)
		err = pListInterface->AppendValueToPropList( &proplistvalue , &propvalue, &tempvalue);
	pValueInterface->ValueRelease( &tempvalue);
	pValueInterface->ValueRelease( &propvalue);

	pValueInterface->SymbolToValue( colorsym, &propvalue);
	pColorInterface->ColorToValue( &colorspec, &tempvalue);
	if (err == kMoaErr_NoErr)
		err = pListInterface->AppendValueToPropList( &proplistvalue , &propvalue, &tempvalue);
	pValueInterface->ValueRelease( &tempvalue);
	pValueInterface->ValueRelease( &propvalue);

	pValueInterface->SymbolToValue( linesizesym, &propvalue);
	pValueInterface->IntegerToValue( 2, &tempvalue);
	if (err == kMoaErr_NoErr)
		err = pListInterface->AppendValueToPropList( &proplistvalue , &propvalue, &tempvalue);
	pValueInterface->ValueRelease( &tempvalue);
	pValueInterface->ValueRelease( &propvalue);

	if (err == kMoaErr_NoErr)
		err = pObj->pImage->NewImage(DEFAULT_OVAL_SIZE, DEFAULT_OVAL_SIZE, 32, 0, NULL, pPropValue);

	if (err == kMoaErr_NoErr)
		err = pObj->pImage->Draw(pPropValue, &rectvalue, &proplistvalue);

	pValueInterface->ValueRelease( &rectvalue);
	pValueInterface->ValueRelease( &proplistvalue);

	if (pListInterface)
		pListInterface->Release();
	if (pValueInterface)
		pValueInterface->Release();
	if (pColorInterface)
		pColorInterface->Release();

	return( err );
}
