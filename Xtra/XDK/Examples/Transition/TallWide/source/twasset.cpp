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
 *	Name: TWAsset.cpp
 *	
 * 	Purpose: Definitions of Asset class interface(s) and methods for the 
 *           TallWide Xtra.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "TWAsset.h"
#include "TWTrans.h"

#ifdef MACINTOSH
	#include <QuickDraw.h>
	#include <QDOffscreen.h>
	#include <Resources.h>
	#include <String.h>
	#include <Dialogs.h>
#endif

#ifdef WINDOWS
	#include <windows.h>
	#include <windowsx.h>
	#include <string.h>
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
BEGIN_DEFINE_CLASS_INTERFACE(CTallWideAsset, IMoaMmXAsset)
END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
BEGIN_DEFINE_CLASS_INTERFACE(CTallWideAsset, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
BEGIN_DEFINE_CLASS_INTERFACE(CTallWideAsset, IMoaNotificationClient)
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

/* ------------------------------------------------------- MoaCreate_CTallWideAsset */
STDMETHODIMP MoaCreate_CTallWideAsset(CTallWideAsset * This)
{
	X_ENTER
	
	MoaError err = kMoaErr_NoErr;
	PIMoaMmUtils pIMoaMmUtils;

	/* get the callback interface for utilities routines*/
	if ((err = This->pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&pIMoaMmUtils))
		!= kMoaErr_NoErr) X_STD_RETURN(kMoaErr_BadInterface);
	
	/*Create the symbols symbol for all of our custom props, store away in instance vars*/
	pIMoaMmUtils->StringToSymbol("initialWidth", &(This->initialWidthSymbol));
	pIMoaMmUtils->StringToSymbol("initialHeight", &(This->initialHeightSymbol));
	pIMoaMmUtils->StringToSymbol("verticalSpeed", &(This->verticalSpeedSymbol));
	pIMoaMmUtils->StringToSymbol("horizontalSpeed", &(This->horizontalSpeedSymbol));

	/* release Utils interface*/
	pIMoaMmUtils->Release();

	X_RETURN(MoaError, err);
	X_EXIT
}

/* ------------------------------------------------------ MoaDestroy_CTallWideAsset */
STDMETHODIMP_(void) MoaDestroy_CTallWideAsset(CTallWideAsset * This)
{
	X_ENTER
	
	if(This->pAssetCallback) This->pAssetCallback->Release();	

	X_RETURN_VOID;
	X_EXIT
}

/* ------------------------------------- CTallWideAsset_IMoaMmXAsset Create/Destroy */
CTallWideAsset_IMoaMmXAsset::CTallWideAsset_IMoaMmXAsset(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CTallWideAsset_IMoaMmXAsset::~CTallWideAsset_IMoaMmXAsset() {}

#ifdef USING_INIT_FROM_DICT
/* ------------------------------------ CTallWideAsset_IMoaInitFromDict Create/Destroy */
CTallWideAsset_IMoaInitFromDict::CTallWideAsset_IMoaInitFromDict(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CTallWideAsset_IMoaInitFromDict::~CTallWideAsset_IMoaInitFromDict() {}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* --------------------------- CTallWideAsset_IMoaNotificationClient Create/Destroy */
CTallWideAsset_IMoaNotificationClient::CTallWideAsset_IMoaNotificationClient(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CTallWideAsset_IMoaNotificationClient::~CTallWideAsset_IMoaNotificationClient() {}
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

/* ------------------------------------------- CTallWideAsset_IMoaMmXAsset::GetProp */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::GetProp(MoaMmSymbol symbol, PMoaMmValue pPropValue)
{
	X_ENTER

	/*  Here's where the user can access the properties from lingo*/
	MoaError				err = kMoaErr_NoErr;
	PIMoaMmUtils 		pIMoaMmUtils;

	/* get the callback interface for utilities routines*/
	if ((err = pObj->pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&pIMoaMmUtils))
		!= kMoaErr_NoErr) X_STD_RETURN(kMoaErr_BadInterface);

	/* if we got the symbol AND it is the symbol we are being asked about
	 we return the value for this property for this transition cast type*/
	if (symbol == pObj->initialWidthSymbol)
		pIMoaMmUtils->IntegerToValue(pObj->sharedAssetData.initialWidth, pPropValue);
	else if (symbol == pObj->initialHeightSymbol)
		pIMoaMmUtils->IntegerToValue(pObj->sharedAssetData.initialHeight, pPropValue);
	else if (symbol == pObj->verticalSpeedSymbol)
		pIMoaMmUtils->IntegerToValue(pObj->sharedAssetData.verticalSpeed, pPropValue);
	else if (symbol == pObj->horizontalSpeedSymbol)
		pIMoaMmUtils->IntegerToValue(pObj->sharedAssetData.horizontalSpeed, pPropValue);
	else err = kMoaMmErr_PropertyNotFound;
	
	/* release Utils interface*/
	pIMoaMmUtils->Release();

	X_RETURN(MoaError, err);
	X_EXIT
}

/* ------------------------------------------- CTallWideAsset_IMoaMmXAsset::SetProp */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::SetProp(MoaMmSymbol symbol, ConstPMoaMmValue pPropValue)
{
	X_ENTER

	MoaError					err = kMoaErr_NoErr;
	MoaMmModifyFlags	propsDirtied;
	PIMoaMmUtils 		pIMoaMmUtils;

	/**************************************************
	  Here's where the user can access the properties from lingo
	***************************************************/

	/* get the callback interface for utilities routines*/
	if ((err = pObj->pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&pIMoaMmUtils))
		!= kMoaErr_NoErr) X_STD_RETURN(kMoaErr_BadInterface);
	
	/* if we got the symbol AND it is the symbol we are being asked about
	 we set the instance variable value for this property and dirty the props flag*/
	pObj->propsDirtied = kMoaMmModifyFlags_Props;
	if (symbol == pObj->initialWidthSymbol)
		pIMoaMmUtils->ValueToInteger(pPropValue, &(pObj->sharedAssetData.initialWidth));
	else if (symbol == pObj->initialHeightSymbol)
		pIMoaMmUtils->ValueToInteger(pPropValue, &(pObj->sharedAssetData.initialHeight));
	else if (symbol == pObj->verticalSpeedSymbol)
		pIMoaMmUtils->ValueToInteger(pPropValue, &(pObj->sharedAssetData.verticalSpeed));
	else if (symbol == pObj->horizontalSpeedSymbol)
		pIMoaMmUtils->ValueToInteger(pPropValue, &(pObj->sharedAssetData.horizontalSpeed));
	else pObj->propsDirtied = FALSE;
	
	/* Tell Director we have been dirtied*/
	if (pObj->propsDirtied == kMoaMmModifyFlags_Props)
		pObj->pAssetCallback->SetModified(pObj->propsDirtied);
			
	/* Get the modified state (just to check it works)*/
	pObj->pAssetCallback->GetModified(&propsDirtied);
	pIMoaMmUtils->PrintMessage2("Set modified to %ld\n Get modified is %ld\n", pObj->propsDirtied, propsDirtied); 
		
	/* release Utils interface*/
	pIMoaMmUtils->Release();
	
	X_RETURN(MoaError, err);
	X_EXIT
}

/* -------------------------------------- CTallWideAsset_IMoaMmXAsset::CallFunction */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::CallFunction(MoaMmSymbol methodName, MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{
	UNUSED(methodName);
	UNUSED(nArgs);
	UNUSED(pArgs);
	UNUSED(pResult);
	X_ENTER
	
	MoaError		err = kMoaMmErr_FunctionNotFound;
		
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------------- CTallWideAsset_IMoaMmXAsset::SetCallback */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::SetCallback(PIMoaMmAssetCallback pAssetCallback)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	// Save off this interface to use in SetProp to dirty yourself(oh my!)
	pObj->pAssetCallback = pAssetCallback;
	pObj->pAssetCallback->AddRef();	

	X_STD_RETURN(err);
	X_EXIT
}

/* -------------------------------------------- CTallWideAsset_IMoaMmXAsset::Notify */
/* This method is intended for communicating between the asset and its xtras.
   The developer is free to define message codes for private use.  This may be
   called directly by an actor, or through the
   IMoaMmTransitionCallback::NotifyAsset() callback.
 */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	UNUSED(msgCode);
	UNUSED(refCon);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/*
	 * --> insert additional code -->
	 */
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CTallWideAsset_IMoaMmXAsset::GetDefaultRect */
/* We return our default rectangle centered around (0,0).  (0,0) here
   corresponds to the "loc" of the sprite, whichis the point of reference for
   the sprite.  Dragging the sprite out to the stage will position the sprite
   such that the loc is at the mouse drop position.
 */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::GetDefaultRect(PMoaRect pRect)
{
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;
	
	/* 32 x 32 centered about (0,0) */
	
	pRect->top = -16;
	pRect->left = -16;
	pRect->bottom = 16;
	pRect->right = 16;
		
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CTallWideAsset_IMoaMmXAsset::PrepareNewContent */
/* This method is called to have the class set-up its default state when a new
   cast member is created (note that this is different from when a new
   instance of CTallWideAsset is created; instances of the XAsset are created at both
   movie-open time (for pre-existing cast members being read-in) and new cast
   member creation time.  PrepareNewContent() is only called at new cast
   member creation time, just after the XAsset instance is created. 
 */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::PrepareNewContent(MoaMmNewFlags newFlags)
{
	UNUSED(newFlags);
	X_ENTER
	
	MoaError	err = kMoaErr_NoErr;
	
	/* A new cast member is being created in memory.  Set up default media and
	   props. */

	/* Dispose of any media and props we may have currently in memory */
	CTallWideAsset_IMoaMmXAsset::FreeMedia();
	CTallWideAsset_IMoaMmXAsset::FreeProps();
	
	/* Initialize the props to default values here. */
	pObj->sharedAssetData.initialWidth = 2/*40*/;
	pObj->sharedAssetData.initialHeight = 3/*20*/;
	pObj->sharedAssetData.verticalSpeed = 4 /*3*/;
	pObj->sharedAssetData.horizontalSpeed = 5 /*7*/;	

	/* Xtras may invoke a "new cast member" dialog (or props or media */
	/* editor) here to have the user specify content for the new */
	/* cast member.  However, this method is called when creating new */
	/* cast members via Lingo as well, so you may want to check the */
	/* authoringMode player property before posting dialogs. */
	
	/* Put up props editor to customize your transition */
	if (newFlags) InvokePropsEditor();
	
	/* The host app assumes that both props and media are "dirty" */
	/* when a new cast member is created, so we don't care */
	/* about the modifyFlags. */

	/* We are now "loaded" in the sense that we have everything we need from */
	/* disk (there isn't anything on disk yet, so we exist only in-memory */
	/* at this point). */
	
	X_STD_RETURN(err);
	X_EXIT
}


/* ------------------------------------ CTallWideAsset_IMoaMmXAsset::StreamOutMedia */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::StreamOutMedia(PIMoaStream pOutStream)
{
	UNUSED(pOutStream);
	X_ENTER
	
	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------- CTallWideAsset_IMoaMmXAsset::StreamInMedia */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::StreamInMedia(PIMoaStream pInStream)
{
	UNUSED(pInStream);
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------- CTallWideAsset_IMoaMmXAsset::FreeMedia */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::FreeMedia()
{
	X_ENTER

	MoaError	err = kMoaErr_NoErr;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CTallWideAsset_IMoaMmXAsset::GetInMemMediaSize */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::GetInMemMediaSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CTallWideAsset_IMoaMmXAsset::GetStreamOutMediaSize */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::GetStreamOutMediaSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	X_STD_RETURN(err);
	X_EXIT
}

/* ------------------------------------ CTallWideAsset_IMoaMmXAsset::StreamOutProps */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::StreamOutProps(PIMoaStream pOutStream)
{
	X_ENTER
	
	MoaError err = kMoaErr_NoErr;
	PIMoaMmUtils pIMoaMmUtils = NULL;
	MoaLong temphorizontalSpeed, tempinitialWidth, tempinitialHeight, tempverticalSpeed;

	/* get the callback interface for utilities routines*/
	if ((err = pObj->pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&pIMoaMmUtils)) != kMoaErr_NoErr) 
		goto exit_gracefully;

	pIMoaMmUtils->PrintMessage1("Stream Out, Num bytes = %ld\n", PROPERTY_BYTES);

	/* Open the stream*/
	if ((err = pOutStream->Open(kMoaStreamOpenAccess_WriteOnly, kMoaStreamSetPositionType_None)) != kMoaErr_NoErr) 
		goto exit_gracefully;

#if defined(WINDOWS) || (defined(__APPLE__) && defined(__MACH__) && defined(__LITTLE_ENDIAN__))
	/* Do byte swap here*/
	tempinitialWidth = MemXLong(pObj->sharedAssetData.initialWidth);
	tempinitialHeight = MemXLong(pObj->sharedAssetData.initialHeight);
	temphorizontalSpeed = MemXLong(pObj->sharedAssetData.horizontalSpeed);
	tempverticalSpeed = MemXLong(pObj->sharedAssetData.verticalSpeed);
#else
	tempinitialWidth = pObj->sharedAssetData.initialWidth;
	tempinitialHeight = pObj->sharedAssetData.initialHeight;
	temphorizontalSpeed = pObj->sharedAssetData.horizontalSpeed;
	tempverticalSpeed = pObj->sharedAssetData.verticalSpeed;
#endif

	/* Stream out the current values for our properties (usually done at directors save time)
	 This data will be retrieved at the next start up by StreamIn Props, below*/
	err = pOutStream->Write(&(tempinitialWidth), sizeof(tempinitialWidth), NULL);
	pIMoaMmUtils->PrintMessage1("Init Width = %ld\n", tempinitialWidth);
	err = pOutStream->Write(&(tempinitialHeight), sizeof(tempinitialHeight), NULL);
	pIMoaMmUtils->PrintMessage1("Init Height = %ld\n", tempinitialHeight);
	err = pOutStream->Write(&(temphorizontalSpeed), sizeof(temphorizontalSpeed), NULL);
	pIMoaMmUtils->PrintMessage1("Horizontal Speed = %ld\n", temphorizontalSpeed);
	err = pOutStream->Write(&(tempverticalSpeed), sizeof(tempverticalSpeed), NULL);
	pIMoaMmUtils->PrintMessage1("Vertical Speed = %ld\n", tempverticalSpeed);
	
	pOutStream->Close();

exit_gracefully:

	/* release Utils interface*/
	if(pIMoaMmUtils) 
		pIMoaMmUtils->Release();

	X_RETURN(MoaError, err);
	X_EXIT
}

/* ------------------------------------- CTallWideAsset_IMoaMmXAsset::StreamInProps */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::StreamInProps(PIMoaStream pInStream)
{
	X_ENTER

	MoaError err = kMoaErr_NoErr;
	PIMoaMmUtils pIMoaMmUtils = NULL;	
	MoaStreamPosition getEnd;
	MoaLong temphorizontalSpeed, tempinitialWidth, tempinitialHeight, tempverticalSpeed;

	/* get the callback interface for utilities routines*/
	if ((err = pObj->pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&pIMoaMmUtils)) != kMoaErr_NoErr) 
		goto exit_gracefully;

	/* Open the stream*/
	if ((err = pInStream->Open(kMoaStreamOpenAccess_ReadOnly, kMoaStreamSetPositionType_None)) != kMoaErr_NoErr) 
		goto exit_gracefully;	

	/* Find the last position in the stream, print it to the message window*/				
	err = pInStream->GetEnd(&getEnd);

	pIMoaMmUtils->PrintMessage1("End of Stream In = %ld\n", getEnd);

	if(getEnd == PROPERTY_BYTES) {
		/*  Here we get the last saved values for our custom properties streamed in from Director*/
		err = pInStream->Read(&(tempinitialWidth), sizeof(tempinitialWidth), NULL);
		err = pInStream->Read(&(tempinitialHeight), sizeof(tempinitialHeight), NULL);
		err = pInStream->Read(&(temphorizontalSpeed), sizeof(temphorizontalSpeed), NULL);
		err = pInStream->Read(&(tempverticalSpeed), sizeof(tempverticalSpeed), NULL);
		
#if defined(WINDOWS) || (defined(__APPLE__) && defined(__MACH__) && defined(__LITTLE_ENDIAN__))
		tempinitialWidth = MemXLong(tempinitialWidth);
		tempinitialHeight = MemXLong(tempinitialHeight);
		temphorizontalSpeed = MemXLong(temphorizontalSpeed);
		tempverticalSpeed = MemXLong(tempverticalSpeed);
#endif
		pObj->sharedAssetData.initialWidth = tempinitialWidth;
		pObj->sharedAssetData.initialHeight = tempinitialHeight;
		pObj->sharedAssetData.horizontalSpeed = temphorizontalSpeed;
		pObj->sharedAssetData.verticalSpeed = tempverticalSpeed;

		pIMoaMmUtils->PrintMessage1("Init Width = %ld\n", pObj->sharedAssetData.initialWidth);
		pIMoaMmUtils->PrintMessage1("Init Height = %ld\n", pObj->sharedAssetData.initialHeight);
		pIMoaMmUtils->PrintMessage1("Horizontal Speed = %ld\n", pObj->sharedAssetData.horizontalSpeed);
		pIMoaMmUtils->PrintMessage1("Vertical Speed = %ld\n", pObj->sharedAssetData.verticalSpeed);
	}

	pInStream->Close();
	
exit_gracefully:

	/* release Utils interface*/
	if (pIMoaMmUtils) 
		pIMoaMmUtils->Release();

	X_RETURN(MoaError, err);
	X_EXIT
}

/* ----------------------------------------- CTallWideAsset_IMoaMmXAsset::FreeProps */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::FreeProps()
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CTallWideAsset_IMoaMmXAsset::GetInMemPropsSize */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::GetInMemPropsSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* !!NOTIMP */
	*pSizeBytes = 0;

	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------- CTallWideAsset_IMoaMmXAsset::GetStreamOutPropsSize */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::GetStreamOutPropsSize(MoaUlong * pSizeBytes)
{
	X_ENTER

	MoaError		err = kMoaErr_NoErr;
	
	/* the number of bytes that we will stream out */
	*pSizeBytes = PROPERTY_BYTES;
	
	X_STD_RETURN(err);
	X_EXIT
}

/* ----------------------------------------- CTallWideAsset_IMoaMmXAsset::NewXActor */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::NewXActor(PMoaVoid * ppXActor) 
{
	X_ENTER

	MoaError err = kMoaErr_NoErr;
	PIMoaMmXTransitionActor pNewActor = NULL;

	/* Do a createinstance using IMoaMmXSpriteActor for return to caller... */
	if ((err = pObj->pCallback->MoaCreateInstance(&CLSID_CTallWideTransition,
		&IID_IMoaMmXTransitionActor, (PPMoaVoid)&pNewActor)) != kMoaErr_NoErr) X_STD_RETURN(kMoaErr_BadInterface);

	/* Pass transition class (CTallWideActor) the props from here (CTallWideAsset)*/
	(pNewActor)->Notify(kSprNotifyMsg_SendProps,
				(PMoaVoid) &pObj->sharedAssetData /* send pointer to instance vars */);

	*ppXActor = pNewActor;
		
	X_STD_RETURN(err);
	X_EXIT
}

/* --------------------------------- CTallWideAsset_IMoaMmXAsset::InvokeMediaEditor */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::InvokeMediaEditor(PMoaMmModifyFlags pModifyFlags)
{
	X_ENTER

	MoaError	err = kMoaErr_NoErr;
	
	/* We don't have a media editor */
	/* !!NOTIMP */
	
	X_RETURN(MoaError, err);
	X_EXIT
}

#ifdef WINDOWS
	
#define WIN_CALLBACK CALLBACK

/*  Windows dialog Proc */
 BOOL WIN_CALLBACK propertiesDlgProc(HWND hDlg, UINT iMsg, WPARAM wParm, LPARAM lParm)
{

	MoaError 	err = kMoaErr_NoErr;
	static 		CTallWideAsset_IMoaMmXAsset * This;

	switch (iMsg)
	{
		case WM_INITDIALOG:		/* initialize dialog */

			This = (CTallWideAsset_IMoaMmXAsset *)lParm;

			/* initialize the props dirtied instance variable to zero  */
			This->pObj->modifyFlags = 0;

			/* Put current values of the properties into the editable fields of the dialog */
			SetDlgItemInt(hDlg, INITIAL_WIDTH, (UINT)(This->pObj->sharedAssetData.initialWidth), TRUE);
			SetDlgItemInt(hDlg, INITIAL_HEIGHT, (UINT)(This->pObj->sharedAssetData.initialHeight), TRUE);
			SetDlgItemInt(hDlg, VERTICAL_SPEED, (UINT)(This->pObj->sharedAssetData.verticalSpeed), TRUE);
			SetDlgItemInt(hDlg, HORIZONTAL_SPEED, (UINT)(This->pObj->sharedAssetData.horizontalSpeed), TRUE);

			break;

		case WM_COMMAND:	/* a control was used */
		
			switch (wParm)
			{
				case IDCANCEL:

					/* bail */
					EndDialog(hDlg, FALSE);
					return TRUE;

				case IDOK:
				/* Get the values in each editable field in the dialog and store them off in instance variables */
					This->pObj->sharedAssetData.initialWidth = GetDlgItemInt(hDlg, INITIAL_WIDTH, NULL, TRUE);
					This->pObj->sharedAssetData.initialHeight = GetDlgItemInt(hDlg, INITIAL_HEIGHT, NULL, TRUE);
					This->pObj->sharedAssetData.verticalSpeed = GetDlgItemInt(hDlg, VERTICAL_SPEED, NULL, TRUE);
					This->pObj->sharedAssetData.horizontalSpeed = GetDlgItemInt(hDlg, HORIZONTAL_SPEED, NULL, TRUE);
					
					/*Set property to dirtied */
					This->pObj->modifyFlags = kMoaMmModifyFlags_Props;

					EndDialog(hDlg, TRUE);

					return TRUE;
			}
			break; 
			 
		default:
			return FALSE;
	}
	return FALSE;
	
}
#endif  /*WIN */

#ifdef MACINTOSH
#if !TARGET_API_MAC_CARBON
void p2cstrcpy (char * dst, ConstStr255Param src)
{
	int len = *src;
	BlockMove(((Ptr)src)+1, (Ptr)dst, len);
	dst[len] = 0;
}

void c2pstrcpy (Str255 dst, char * src);
void c2pstrcpy (Str255 dst, char * src)
{
	int len = strlen(src);
	BlockMove((Ptr)src, ((Ptr)dst)+1, len);
	dst[0] = len;
}

/*Comment this section out for Apple Universal Header 3.4 or later....
/*void SetPortDialogPort (DialogPtr dialog);
void SetPortDialogPort (DialogPtr dialog)
{
	SetPort(dialog);
}*/


#endif
#endif

/* --------------------------------- CTallWideAsset_IMoaMmXAsset::InvokePropsEditor */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::InvokePropsEditor()
{
	X_ENTER

	MoaError			err = kMoaErr_NoErr;
	XtraResourceCookie 	myCookie, saveCookie;
	PIMoaMmUtils 		pUtils;

#ifdef MACINTOSH
	
	GrafPtr 		savePort;
	DialogRef		myDialog;
	RGBColor		saveColor;
	short			itemHit;
	short			iType;
	Handle	 		iHandle;
	Rect 			iRect;
	Str255			tempString255;
	MoaChar			tempString[MAX_STRING_LENGTH];
	MoaMmDialogCookie	dialogCookie;
#endif /*MAC */

#ifdef WINDOWS
	WORD				bitmapNum = DIALOG_BOX;
	int 				result;
    static FARPROC 		lpfnDialogProc;
#endif /*WIN */

	myCookie = pObj->pCallback->MoaBeginUsingResources(gXtraFileRef, &saveCookie);
	pObj->pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&pUtils);

#ifdef MACINTOSH
	/* Save port and color */
	GetForeColor(&saveColor);
	GetPort(&savePort);

	/* Get dialog from DLOG resource */
	myDialog = GetNewDialog(MAC_DIALOG_ID, NULL, (WindowPtr) -1);
	/*InitCursor(); */

	if(myDialog)
	{
		SetPortDialogPort(myDialog);
		/* For window layering to work properly, the MOA calls MacPrepareModalDialog(), followed
		by the MacModalDialog() and finally the MacUnprepareModalDialog(), should be used*/			
		pUtils->MacPrepareModalDialog(&dialogCookie);	
		ShowWindow(GetDialogWindow(myDialog));
		
		/*  Set the dialog text fields to the current values for each property */
		/* initialWidth */
		GetDialogItem(myDialog, (short)INITIAL_WIDTH, &iType, &iHandle, &iRect);
		sprintf(tempString, "%ld", pObj->sharedAssetData.initialWidth);
		c2pstrcpy(tempString255, tempString);
		SetDialogItemText(iHandle, tempString255);
		/* initial height */
		GetDialogItem(myDialog, (short)INITIAL_HEIGHT, &iType, &iHandle, &iRect);
		sprintf(tempString, "%ld", pObj->sharedAssetData.initialHeight);
		c2pstrcpy(tempString255, tempString);
		SetDialogItemText(iHandle, tempString255);
		/* vertical speed */
		GetDialogItem(myDialog, (short)VERTICAL_SPEED, &iType, &iHandle, &iRect);
		sprintf(tempString, "%ld", pObj->sharedAssetData.verticalSpeed);
		c2pstrcpy(tempString255, tempString);
		SetDialogItemText(iHandle, tempString255);
		/* horizontal speed */
		GetDialogItem(myDialog, (short)HORIZONTAL_SPEED, &iType, &iHandle, &iRect);
		sprintf(tempString, "%ld", pObj->sharedAssetData.horizontalSpeed);
		c2pstrcpy(tempString255, tempString);
		SetDialogItemText(iHandle, tempString255);

		do 
		{
			pUtils->MacModalDialog(dialogCookie, NULL, &itemHit);	
		
			switch (itemHit) 
			{
					
				case OK:	

					/* Get the Dialog values and store them off in instance variables */
					/* initialWidth */
					GetDialogItem(myDialog, (short)INITIAL_WIDTH, &iType, &iHandle, &iRect);
					GetDialogItemText( iHandle, tempString255);	
					p2cstrcpy(tempString, tempString255);
					sscanf(tempString,"%ld",&pObj->sharedAssetData.initialWidth);

					/* initial height */
					GetDialogItem(myDialog, (short)INITIAL_HEIGHT, &iType, &iHandle, &iRect);
					GetDialogItemText( iHandle, tempString255);	
					p2cstrcpy(tempString, tempString255);
					sscanf(tempString,"%ld",&pObj->sharedAssetData.initialHeight);

					/* vertical speed */
					GetDialogItem(myDialog, (short)VERTICAL_SPEED, &iType, &iHandle, &iRect);
					GetDialogItemText( iHandle, tempString255);	
					p2cstrcpy(tempString, tempString255);
					sscanf(tempString,"%ld",&pObj->sharedAssetData.verticalSpeed);

					/* horizontal speed */
					GetDialogItem(myDialog, (short)HORIZONTAL_SPEED, &iType, &iHandle, &iRect);
					GetDialogItemText( iHandle, tempString255);	
					p2cstrcpy(tempString, tempString255);
					sscanf(tempString,"%ld",&pObj->sharedAssetData.horizontalSpeed);

					/*Set property to dirtied */
					pObj->modifyFlags = kMoaMmModifyFlags_Props;
					break;				
			}
			
		} while (itemHit != OK && itemHit != CANCEL);	
		
		DisposeDialog(myDialog);
		pUtils->MacUnprepareModalDialog(dialogCookie);	
	}		
	RGBForeColor(&saveColor);	
	/*SetPenState(&savePen); */
	SetPort(savePort); 
#endif  /*MAC */

#ifdef WINDOWS
	/* set up dialog using WinDialogBoxParam() call*/

	lpfnDialogProc = MakeProcInstance((FARPROC)propertiesDlgProc, (HINSTANCE)myCookie);
	result = pUtils->WinDialogBoxParam(myCookie, MAKEINTRESOURCE(bitmapNum), (PMoaVoid)lpfnDialogProc,
									(MoaLong)this);
	FreeProcInstance(lpfnDialogProc);
#endif  /*WIN			 */

	/*Release the utils interface*/
	if(pUtils)	
		pUtils->Release();
	pObj->pCallback->MoaEndUsingResources(gXtraFileRef, saveCookie);

	X_RETURN(MoaError, err);
	X_EXIT
}

/* --------------------------------- CTallWideAsset_IMoaMmXAsset::FinishPropsEditor */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::FinishPropsEditor(MoaBoolParam commitChanges,					
	PMoaMmModifyFlags pModifyFlags)
{
	X_ENTER

	MoaError	err = kMoaErr_NoErr;
	
	if (pObj->modifyFlags == kMoaMmModifyFlags_Props && commitChanges == TRUE) {
		*pModifyFlags = kMoaMmModifyFlags_Props;
	}
	
	X_RETURN(MoaError, err);
	X_EXIT
}

#ifdef WINDOWS
	
#define WIN_CALLBACK CALLBACK	
	
BOOL WIN_CALLBACK aboutBoxDlgProc(HWND hDlg, UINT iMsg, WPARAM wParm, LPARAM lParm)
{

	MoaError 	err = kMoaErr_NoErr;

	switch (iMsg)
	{
		case WM_INITDIALOG:		/* nothing to do */

			break;

		case WM_COMMAND:	/* a control was used */
		
			switch (wParm)
			{
	
				case IDOK:
					/* Nothing to do, just kill the dialog*/
					EndDialog(hDlg, TRUE);
					return TRUE;
			}
			break; 
			 
		default:
			return FALSE;
	}
	return FALSE;
	
}
#endif  /*WIN */

/* ------------------------------------- CTallWideAsset_IMoaMmXAsset_InvokeAboutBox */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::InvokeAboutBox()
{
	X_ENTER

	MoaError			err = kMoaErr_NoErr;
	XtraResourceCookie 	myCookie, saveCookie;
	PIMoaMmUtils 		pUtils;

#ifdef WINDOWS
	WORD				bitmapNum = ID_ABOUTBOX;
	int 				result;
    static FARPROC 		lpfnDialogProc;
#endif /*WIN */

	myCookie = pObj->pCallback->MoaBeginUsingResources(gXtraFileRef, &saveCookie);
	pObj->pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&pUtils);

#ifdef MACINTOSH  
	{
	GrafPtr 		savePort;
	DialogPtr		myDialog;
	RGBColor		saveColor;
	short			itemHit;
	Rect			myRect; 
	MoaMmDialogCookie	dialogCookie;

	/* Save port and color */
	GetForeColor(&saveColor);
	GetPort(&savePort);
			
	SetRect(&myRect, 75, 114, 447, 300);	/* rectangle for dialog box */
	myDialog = GetNewDialog(ID_ABOUTBOX,NULL,(WindowPtr)-1);	
	
	/* For window layering to work properly, the MOA calls MacPrepareModalDialog(), followed
	by the MacModalDialog() and finally the MacUnprepareModalDialog(), should be used*/			
	pUtils->MacPrepareModalDialog(&dialogCookie);	
	ShowWindow(GetDialogWindow(myDialog));
	InitCursor();
	SetPortDialogPort(myDialog);
							
	do {
		pUtils->MacModalDialog(dialogCookie, NULL, &itemHit);	
	} while (itemHit != ID_OKBUTTON);
	
	DisposeDialog(myDialog);
	pUtils->MacUnprepareModalDialog(dialogCookie);	
	
	done:
	
	RGBForeColor(&saveColor);	
	SetPort(savePort);
	}	
#endif  /*MAC	 */
#ifdef WINDOWS
	
	/* set up dialog */

	lpfnDialogProc = MakeProcInstance((FARPROC)aboutBoxDlgProc, (HINSTANCE)myCookie);
	result = pUtils->WinDialogBox(myCookie, MAKEINTRESOURCE(bitmapNum), (PMoaVoid)lpfnDialogProc);
	FreeProcInstance(lpfnDialogProc);

#endif  /*WIN			 */

	/* Release Utils interface*/
	if (pUtils) pUtils->Release();

	pObj->pCallback->MoaEndUsingResources(gXtraFileRef, saveCookie);
	
	X_RETURN(MoaError, err);
	X_EXIT
}

/* ---------------------------------- CTallWideAsset_IMoaMmXAsset::ImageStaticThumb */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::ImageStaticThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	X_ENTER

	MoaError 			err = kMoaErr_NoErr;
	MoaMmNativeGCInfo	nativeGC;
	XtraResourceCookie 	myCookie, saveCookie;

	pGC->GetNativeGCInfo(&nativeGC);
	myCookie = pObj->pCallback->MoaBeginUsingResources(gXtraFileRef, &saveCookie);

#ifdef MACINTOSH
	{

	GrafPtr				savePort, drawPort;
	Rect 				macRect;
	RGBColor			saveColor;
	PicHandle			hPic;
		
	MoaToMacRect(pImageRect, &macRect);
	/* Determine what port to draw into based on the */
	/* supplied Graphics Context.  The host app sets up */
	/* this context for us; if we're Direct To Stage or */
	/* trails ink is applied, the context is that of */
	/* the stage window; otherwise it is that of the stage */
	/* offscreen buffer. */
	
	switch (nativeGC.type)
	{
		case kMoaMmGCType_MacWindow:
			drawPort = (GrafPtr) nativeGC.data.mac_window.windowPtr;
			break;
		case kMoaMmGCType_MacGWorld:
			drawPort = (GrafPtr) nativeGC.data.mac_gworld.gworldPtr;
			break;
		default:
			/* we don't know about this type of graphics context; bail out. */
			goto done;
	}
	/* Save port and color */
	GetForeColor(&saveColor);
	GetPort(&savePort);
	SetPort(drawPort);
	

	/* Draw Static Thumbnail Icon	 */
	if ((hPic = (PicHandle)GetNamedResource('PICT', STATIC_THUMB)) != NULL)
		DrawPicture((PicHandle)hPic, &macRect);

	SetPort(savePort);
	RGBForeColor(&saveColor	);	
	}
#endif	/* MACINTOSH */
	
#ifdef WINDOWS
	{
	HDC			drawDC;
	HPEN		hOldPen;
	HBRUSH		hOldBrush;
    long *		err2 = NULL;
	HDC			hdcMem1;
	BITMAP 		bm1;
	HBITMAP		hBitMap1;
	WORD		bitmapNum = (WORD)STATIC_THUMB;

    /* Determine what port to draw into based on the */
    /* supplied Graphics Context.  The host app sets up */
    /* this context for us; if we're Direct To Stage or */
    /* trails ink is applied, the context is that of */
    /* the stage window; otherwise it is that of the stage */
    /* offscreen buffer. */

    switch (nativeGC.type)
    {
		case kMoaMmGCType_WinDIBDC:
		case kMoaMmGCType_WinDDBDC:
		case kMoaMmGCType_WinWindow:
			drawDC = (HDC)nativeGC.data.win_window.hDC;
			break;
        default:
            /* we don't know about this type of graphics context; bail out. */
            goto done;
    }
                
 
    /* We could obey the sprite color supplied in pActorInfo here, */
    /* but instead we always draw using a fixed color. */

	hOldPen = (HPEN)SelectObject(drawDC, GetStockObject(NULL_PEN));
	hOldBrush = (HBRUSH)SelectObject(drawDC, GetStockObject(NULL_BRUSH));

   	/* Load bitmap resource */
    hBitMap1 = LoadBitmap((HINSTANCE)myCookie, MAKEINTRESOURCE(bitmapNum));

	if(hBitMap1 != 0)
	{
		/* Create another graphics context equivalent to DrawGC */
 		hdcMem1 = CreateCompatibleDC (drawDC);

		/*Get information about hBitmap1 */
		GetObject(hBitMap1, sizeof(BITMAP), (LPVOID) &bm1);
	
		/* Do I need to do this? */
		SelectObject(hdcMem1, hBitMap1);
	
		/*Draw loaded bitmap into the DrawDC graphics context at a new size			 	 */
		StretchBlt(drawDC, pImageRect->left, pImageRect->top, pImageRect->right - pImageRect->left, pImageRect->bottom - pImageRect->top,
					hdcMem1, 0, 0, bm1.bmWidth, bm1.bmHeight,  SRCCOPY);

		DeleteDC(hdcMem1);
    }


	/*Restore brush and pen */
    SelectObject(drawDC, hOldBrush);
    SelectObject(drawDC, hOldPen);
	DeleteObject(hBitMap1);


	}
#endif /* WINDOWS */

done:

	pObj->pCallback->MoaEndUsingResources(gXtraFileRef, saveCookie);

	X_RETURN(MoaError, err);
	X_EXIT
}

/* ------------------------------- CTallWideAsset_IMoaMmXAsset::BeginImageAnimThumb */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::BeginImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	X_ENTER

	MoaError 	err = kMoaErr_NoErr;
	
	/*Set the image number instance variable to the first one in the sequence */
	pObj->pictNum = FIRST_ANIM_PICT_NUM;	
	
	X_RETURN(MoaError, err);
	X_EXIT
}

/* ----------------------------- CTallWideAsset_IMoaMmXAsset::ServiceImageAnimThumb */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::ServiceImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	X_ENTER

	MoaError 			err = kMoaErr_NoErr;
	MoaMmNativeGCInfo	nativeGC;
	XtraResourceCookie 	myCookie, saveCookie;

	pGC->GetNativeGCInfo(&nativeGC);
	myCookie = pObj->pCallback->MoaBeginUsingResources(gXtraFileRef, &saveCookie);

#ifdef MACINTOSH
	{

	GrafPtr				savePort, drawPort;
	Rect 				macRect;
	RGBColor			saveColor;
	PicHandle			hPic;
		
	MoaToMacRect(pImageRect, &macRect);
	/* Determine what port to draw into based on the */
	/* supplied Graphics Context.  The host app sets up */
	/* this context for us; if we're Direct To Stage or */
	/* trails ink is applied, the context is that of */
	/* the stage window; otherwise it is that of the stage */
	/* offscreen buffer. */
	
	switch (nativeGC.type)
	{
		case kMoaMmGCType_MacWindow:
			drawPort = (GrafPtr) nativeGC.data.mac_window.windowPtr;
			break;
		case kMoaMmGCType_MacGWorld:
			drawPort = (GrafPtr) nativeGC.data.mac_gworld.gworldPtr;
			break;
		default:
			/* we don't know about this type of graphics context; bail out. */
			goto done;
	}
	/* Save port and color */
	GetForeColor(&saveColor);
	GetPort(&savePort);
	SetPort(drawPort);
	

	/* Draw Anim Thumbnail Icon	 */
	if ((hPic = (PicHandle)GetResource('PICT', pObj->pictNum)) != NULL)
		DrawPicture((PicHandle)hPic, &macRect);

	SetPort(savePort);
	RGBForeColor(&saveColor	);	
	}
#endif	/* MACINTOSH */
	
#ifdef WINDOWS
	{
	HDC			drawDC;
	HPEN		hOldPen;
	HBRUSH		hOldBrush;
    long *		err2 = NULL;
	HDC			hdcMem1;
	BITMAP 		bm1;
	HBITMAP		hBitMap1;
	WORD		bitmapNum = (WORD)pObj->pictNum;

    /* Determine what port to draw into based on the */
    /* supplied Graphics Context.  The host app sets up */
    /* this context for us; if we're Direct To Stage or */
    /* trails ink is applied, the context is that of */
    /* the stage window; otherwise it is that of the stage */
    /* offscreen buffer. */

    switch (nativeGC.type)
    {
		case kMoaMmGCType_WinDIBDC:
		case kMoaMmGCType_WinDDBDC:
		case kMoaMmGCType_WinWindow:
			drawDC = (HDC)nativeGC.data.win_window.hDC;
			break;
        default:
            /* we don't know about this type of graphics context; bail out. */
            goto done;
    }
                
 
    /* We could obey the sprite color supplied in pActorInfo here, */
    /* but instead we always draw using a fixed color. */

	hOldPen = (HPEN)SelectObject(drawDC, GetStockObject(NULL_PEN));
	hOldBrush = (HBRUSH)SelectObject(drawDC, GetStockObject(NULL_BRUSH));

   	/* Load bitmap resource */
    hBitMap1 = LoadBitmap((HINSTANCE)myCookie, MAKEINTRESOURCE(bitmapNum));

	if(hBitMap1 != 0)
	{
		/* Create another graphics context equivalent to DrawGC */
 		hdcMem1 = CreateCompatibleDC (drawDC);

		/*Get information about hBitmap1 */
		GetObject(hBitMap1, sizeof(BITMAP), (LPVOID) &bm1);
	
		/* Do I need to do this? */
		SelectObject(hdcMem1, hBitMap1);
	
		/*Draw loaded bitmap into the DrawDC graphics context at a new size			 	 */
		StretchBlt(drawDC, pImageRect->left, pImageRect->top, pImageRect->right - pImageRect->left, pImageRect->bottom - pImageRect->top,
					hdcMem1, 0, 0, bm1.bmWidth, bm1.bmHeight,  SRCCOPY);

		DeleteDC(hdcMem1);
    }


	/*Restore brush and pen */
    SelectObject(drawDC, hOldBrush);
    SelectObject(drawDC, hOldPen);
	DeleteObject(hBitMap1);


	}
#endif /* WINDOWS */

done:

	pObj->pCallback->MoaEndUsingResources(gXtraFileRef, saveCookie);

	pObj->pictNum += 1;

	if (pObj->pictNum == LAST_ANIM_PICT_NUM) pObj->pictNum = FIRST_ANIM_PICT_NUM;	
	
	X_RETURN(MoaError, err);
	X_EXIT
}

/* --------------------------------- CTallWideAsset_IMoaMmXAsset::EndImageAnimThumb */
STDMETHODIMP CTallWideAsset_IMoaMmXAsset::EndImageAnimThumb(PIMoaMmGC pGC, ConstPMoaRect pImageRect)
{
	X_ENTER

	MoaError err = kMoaErr_NoErr;

	/* Nothing to do */
	
	X_RETURN(MoaError, err);
	X_EXIT
}

/* ----------------------------- CTallWideAsset_IMoaMmXAsset::MemXShort */
STDMETHODIMP_(short) CTallWideAsset_IMoaMmXAsset::MemXShort(short x)
{
	MoaByte temp;
	union
	{
		short oneWord;
		MoaByte twoBytes[2];
	} Moto;
	Moto.oneWord = x;
	/* swap two end bytes */
	temp = Moto.twoBytes[1];
	Moto.twoBytes[1] = Moto.twoBytes[0];
	Moto.twoBytes[0] = temp;
	return (Moto.oneWord);
}

/* ----------------------------- CTallWideAsset_IMoaMmXAsset::MemXLong */
STDMETHODIMP_(long) CTallWideAsset_IMoaMmXAsset::MemXLong(long x)
{
	MoaByte temp;
	union
	{
		long oneDWord;
		MoaByte fourBytes[4];
	} Moto;
	Moto.oneDWord = x;
	/* swap two end bytes */
	temp = Moto.fourBytes[3];
	Moto.fourBytes[3] = Moto.fourBytes[0];
	Moto.fourBytes[0] = temp;
	/* now swap the middle bytes */
	temp = Moto.fourBytes[2];
	Moto.fourBytes[2] = Moto.fourBytes[1];
	Moto.fourBytes[1] = temp;
	return (Moto.oneDWord);
}

#ifdef USING_INIT_FROM_DICT
/* ---------------------------------- CTallWideAsset_IMoaInitFromDict::InitFromDict */
STDMETHODIMP CTallWideAsset_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
{
	X_ENTER
	
	MoaError err = kMoaErr_NoErr;

	X_STD_RETURN(err);
	X_EXIT
}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* ---------------------------------- CTallWideAsset_IMoaNotificationClient::Notify */
STDMETHODIMP CTallWideAsset_IMoaNotificationClient::Notify(ConstPMoaNotifyID nid, PMoaVoid pNData, PMoaVoid pRefCon)
{
	X_ENTER
	
	MoaError err = kMoaErr_NoErr;

	X_STD_RETURN(err);
	X_EXIT
}
#endif

