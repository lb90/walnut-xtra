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
 *	Name: PlaySoundAsset.cpp
 *	
 * 	Purpose: Definitions of Asset class interface(s) and methods for the 
 *           PlaySound Xtra project.
 *
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "PlaySoundAsset.h"

#ifdef MACINTOSH
	#include <QuickDraw.h>
#endif

#include "MoaTry.h"

#include <String.h>

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
BEGIN_DEFINE_CLASS_INTERFACE(CPlaySoundAsset, IMoaMmXAsset)
END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
BEGIN_DEFINE_CLASS_INTERFACE(CPlaySoundAsset, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
BEGIN_DEFINE_CLASS_INTERFACE(CPlaySoundAsset, IMoaNotificationClient)
END_DEFINE_CLASS_INTERFACE
#endif

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

/* ------------------------------------------------------- MoaCreate_CPlaySoundAsset */
STDMETHODIMP  		MoaCreate_CPlaySoundAsset(CPlaySoundAsset * This)
{
moa_try	
	ThrowErr(This->pCallback->MoaCreateInstance(&CLSID_CMoaFile2, &IID_IMoaFile2, (PPMoaVoid)&This->pFile));
	ThrowNull(This->pFile);
	
	ThrowErr(This->pCallback->QueryInterface(&IID_IMoaMmUtils,(PPMoaVoid)&This->pUtils));
	ThrowNull(This->pUtils);
	
	ThrowErr(This->pUtils->StringToSymbol("fname", &This->symFileName));
	
moa_catch
moa_catch_end		

moa_try_end
}

/* ------------------------------------------------------ MoaDestroy_CPlayoundAsset */
STDMETHODIMP_(void) MoaDestroy_CPlaySoundAsset(CPlaySoundAsset * This)
{
	if (This->pFile) This->pFile->Release();
	This->pFile = NULL;
	
	if (This->pUtils) This->pUtils->Release();
	This->pUtils = NULL;
	
	return;
}

/* ------------------------------------- CPlaySoundAsset_IMoaMmXAsset Create/Destroy */
CPlaySoundAsset_IMoaMmXAsset::CPlaySoundAsset_IMoaMmXAsset(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CPlaySoundAsset_IMoaMmXAsset::~CPlaySoundAsset_IMoaMmXAsset() {}

#ifdef USING_INIT_FROM_DICT
/* ------------------------------------ CPlaySoundAsset_IMoaInitFromDict Create/Destroy */
CPlaySoundAsset_IMoaInitFromDict::CPlaySoundAsset_IMoaInitFromDict(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CPlaySoundAsset_IMoaInitFromDict::~CPlaySoundAsset_IMoaInitFromDict() {}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* --------------------------- CPlaySoundAsset_IMoaNotificationClient Create/Destroy */
CPlaySoundAsset_IMoaNotificationClient::CPlaySoundAsset_IMoaNotificationClient(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CPlaySoundAsset_IMoaNotificationClient::~CPlaySoundAsset_IMoaNotificationClient() {}
#endif


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

/* ------------------------------------------- CPlaySoundAsset_IMoaMmXAsset::GetProp */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::GetProp(MoaMmSymbol symbol, PMoaMmValue	pPropValue)
{
moa_try
	PIMoaPathName pPathName = NULL;
	char buf[MOA_MAX_PATHNAME];

	// Check to see if the symbol is correct
	if (symbol != pObj->symFileName)
		Throw(kMoaMmErr_PropertyNotFound);
	
	ThrowErr(pObj->pCallback->MoaCreateInstance(&CLSID_CMoaPath, &IID_IMoaPathName,(PPMoaVoid)&pPathName));
	ThrowNull(pPathName);
		 
	//Return the filename in pPropValue...
	ThrowErr(pObj->pFile->GetPathName(&pPathName));
	ThrowErr(pPathName->GetPath(buf, MOA_MAX_PATHNAME));
	ThrowErr(pObj->pUtils->StringToValue(buf, pPropValue));

moa_catch
moa_catch_end

	if (pPathName) pPathName->Release();
	pPathName = NULL;
	
moa_try_end
}

/* ------------------------------------------- CPlaySoundAsset_IMoaMmXAsset::SetProp */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::SetProp(MoaMmSymbol symbol, ConstPMoaMmValue pPropValue)
{
moa_try
	PIMoaPathName pPathName = NULL;
	char buf[MOA_MAX_PATHNAME];
	
	// Check to see if the symbol is correct
	if (symbol != pObj->symFileName) 
		Throw(kMoaMmErr_PropertyNotFound);
	
	ThrowErr(pObj->pCallback->MoaCreateInstance(&CLSID_CMoaPath, &IID_IMoaPathName,(PPMoaVoid)&pPathName));
	ThrowNull(pPathName);
		
	// put the incoming value into instance var...
	ThrowErr(pObj->pUtils->ValueToString(pPropValue,buf,MOA_MAX_PATHNAME));
	ThrowErr(pPathName->InitFromString(buf, kMoaPathDialect_LOCAL, FALSE, FALSE));
	ThrowErr(pObj->pFile->SetPathName(pPathName));
	
moa_catch
moa_catch_end

	if (pPathName) pPathName->Release();
	pPathName = NULL;
		
moa_try_end
}

/* -------------------------------------- CPlaySoundAsset_IMoaMmXAsset::CallFunction */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::CallFunction(MoaMmSymbol methodName, MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{
	MoaError		err = kMoaMmErr_FunctionNotFound;	
	return(err);
}

/* --------------------------------------- CPlaySoundAsset_IMoaMmXAsset::SetCallback */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::SetCallback(PIMoaMmAssetCallback pAssetCallback)
{
	MoaError		err = kMoaErr_NoErr;
	return(err);
}

/* -------------------------------------------- CPlaySoundAsset_IMoaMmXAsset::Notify */
/* This method is intended for communicating between the asset and its xtras.
   The developer is free to define message codes for private use.  This may be
   called directly by an actor, or through the
   IMoaMmSpriteCallback::NotifyAsset() callback.
 */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	MoaError		err = kMoaErr_NoErr;	
	return(err);
}

/* ------------------------------------ CPlaySoundAsset_IMoaMmXAsset::GetDefaultRect */
/* We return our default rectangle centered around (0,0).  (0,0) here
   corresponds to the "loc" of the sprite, whichis the point of reference for
   the sprite.  Dragging the sprite out to the stage will position the sprite
   such that the loc is at the mouse drop position.
 */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::GetDefaultRect(PMoaRect pRect)
{	
	MoaError	err = kMoaErr_NoErr;
	
	/* 32 x 32 centered about (0,0) */
	pRect->top = -16;
	pRect->left = -16;
	pRect->bottom = 16;
	pRect->right = 16;
	
	return(err);
}

/* --------------------------------- CPlaySoundAsset_IMoaMmXAsset::PrepareNewContent */
/* This method is called to have the class set-up its default state when a new
   cast member is created (note that this is different from when a new
   instance of SoundAsset is created; instances of the XAsset are created at both
   movie-open time (for pre-existing cast members being read-in) and new cast
   member creation time.  PrepareNewContent() is only called at new cast
   member creation time, just after the XAsset instance is created. 
 */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::PrepareNewContent(MoaMmNewFlags newFlags)
{
	MoaError	err = kMoaErr_NoErr;
	
	// A new cast member is being created in memory.  Set up default media and props...

	// Dispose of any media and props we may have currently in memory
	FreeMedia();
	FreeProps();
	
	// Get the name of the sound file...
	InvokePropsEditor();
	
	/* Initialize props / media to default values here. 
	   The host app assumes that both props and media are "dirty" when a new
	   cast member is created. 
	
	   We are now "loaded" in the sense that we have everything we need from 
	   disk (there isn't anything on disk yet, so we exist only in-memory 
	   at this point).
    */

	
	return(err);
}


/* ------------------------------------ CPlaySoundAsset_IMoaMmXAsset::StreamOutMedia */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::StreamOutMedia(PIMoaStream pOutStream)
{
	UNUSED(pOutStream);
	
	MoaError		err = kMoaErr_NoErr;
	
	return(err);
}

/* ------------------------------------- CPlaySoundAsset_IMoaMmXAsset::StreamInMedia */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::StreamInMedia(PIMoaStream pInStream)
{
	MoaError		err = kMoaErr_NoErr;
	return(err);
}

/* ----------------------------------------- CPlaySoundAsset_IMoaMmXAsset::FreeMedia */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::FreeMedia()
{
	MoaError	err = kMoaErr_NoErr;
	return(err);
}

/* --------------------------------- CPlaySoundAsset_IMoaMmXAsset::GetInMemMediaSize */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::GetInMemMediaSize(MoaUlong * pSizeBytes)
{
	*pSizeBytes = 0L;
	MoaError		err = kMoaErr_NoErr;
	return(err);
}

/* ----------------------------- CPlaySoundAsset_IMoaMmXAsset::GetStreamOutMediaSize */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::GetStreamOutMediaSize(MoaUlong * pSizeBytes)
{
	MoaError		err = kMoaErr_NoErr;
	*pSizeBytes = 0L;
	return(err);
}

/* ------------------------------------ CPlaySoundAsset_IMoaMmXAsset::StreamOutProps */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::StreamOutProps(PIMoaStream pOutStream)
{
	MoaError		err = kMoaErr_NoErr;	
	return(err);
}

/* ------------------------------------- CPlaySoundAsset_IMoaMmXAsset::StreamInProps */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::StreamInProps(PIMoaStream pInStream)
{
	MoaError		err = kMoaErr_NoErr;	
	return(err);
}

/* ----------------------------------------- CPlaySoundAsset_IMoaMmXAsset::FreeProps */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::FreeProps()
{
	MoaError		err = kMoaErr_NoErr;	
	return(err);
}

/* --------------------------------- CPlaySoundAsset_IMoaMmXAsset::GetInMemPropsSize */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::GetInMemPropsSize(MoaUlong * pSizeBytes)
{
	MoaError		err = kMoaErr_NoErr;	
	*pSizeBytes = 0L;
	return(err);
}

/* ----------------------------- CPlaySoundAsset_IMoaMmXAsset::GetStreamOutPropsSize */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::GetStreamOutPropsSize(MoaUlong * pSizeBytes)
{
	MoaError		err = kMoaErr_NoErr;	
	*pSizeBytes = 0L;
	return(err);
}

/* ----------------------------------------- CPlaySoundAsset_IMoaMmXAsset::NewXActor */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::NewXActor(PMoaVoid * ppXActor) 
{
moa_try
	PIMoaMmXSpriteActor pNewActor = NULL;	  

	ThrowErr(pObj->pCallback->MoaCreateInstance(
				&CLSID_CPlaySoundSprite,
				&IID_IMoaMmXSpriteActor,
				(PPMoaVoid)&pNewActor));

	*ppXActor = pNewActor;
	pNewActor->Notify(kSpriteMsg_FileName,pObj->pFile);

moa_catch
moa_catch_end

moa_try_end
}

/* --------------------------------- CPlaySoundAsset_IMoaMmXAsset::InvokeMediaEditor */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::InvokeMediaEditor(PMoaMmModifyFlags pModifyFlags)
{
	MoaError		err = kMoaErr_NoErr;	
	return(err);
}

/* --------------------------------- CPlaySoundAsset_IMoaMmXAsset::InvokePropsEditor */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::InvokePropsEditor()
{
moa_try
	PIMoaPathName pPath = NULL;
	PIMuiFile pFileDlg = NULL;
	
	ThrowErr(pObj->pCallback->MoaCreateInstance(&CLSID_CMuiFile, &IID_IMuiFile, (PMoaVoid*)&pFileDlg));
	ThrowNull(pFileDlg);
	
	ThrowErr(pFileDlg->SimpleFileOpen(&pPath, "Select an AIFF, SWA, or WAV File:"));
	
	pObj->pFile->SetPathName(pPath);

moa_catch
moa_catch_end		
	if (pPath) pPath->Release();
	pPath = NULL;
	
	if (pFileDlg) pFileDlg->Release();
	pFileDlg = NULL;	
moa_try_end
}

/* --------------------------------- CPlaySoundAsset_IMoaMmXAsset::FinishPropsEditor */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::FinishPropsEditor(MoaBoolParam commitChanges,					
	PMoaMmModifyFlags pModifyFlags)
{
	MoaError		err = kMoaErr_NoErr;
	return(err);
}

/* ------------------------------------- CPlaySoundAsset_IMoaMmXAsset_InvokeAboutBox */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::InvokeAboutBox()
{
	MoaError		err = kMoaErr_NotImplemented;
	return(err);
}

/* ---------------------------------- CPlaySoundAsset_IMoaMmXAsset::ImageStaticThumb */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::ImageStaticThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	MoaError		err = kMoaMmErr_NotImplemented;
	return(err);
}

/* ------------------------------- CPlaySoundAsset_IMoaMmXAsset::BeginImageAnimThumb */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::BeginImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	MoaError		err = kMoaMmErr_NotImplemented;
	return(err);
}

/* ----------------------------- CPlaySoundAsset_IMoaMmXAsset::ServiceImageAnimThumb */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::ServiceImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	MoaError		err = kMoaMmErr_NotImplemented;
	return(err);
}

/* --------------------------------- CPlaySoundAsset_IMoaMmXAsset::EndImageAnimThumb */
STDMETHODIMP CPlaySoundAsset_IMoaMmXAsset::EndImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	MoaError		err = kMoaMmErr_NotImplemented;	
	return(err);
}

#ifdef USING_INIT_FROM_DICT
/* ---------------------------------- CPlaySoundAsset_IMoaInitFromDict::InitFromDict */
STDMETHODIMP CPlaySoundAsset_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
{
	MoaError err = kMoaErr_NoErr;
	return(err);
}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* ---------------------------------- CPlaySoundAsset_IMoaNotificationClient::Notify */
STDMETHODIMP CPlaySoundAsset_IMoaNotificationClient::Notify(ConstPMoaNotifyID nid, PMoaVoid pNData, PMoaVoid pRefCon)
{
	MoaError err = kMoaErr_NoErr;
	return(err);
}
#endif
