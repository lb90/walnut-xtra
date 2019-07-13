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
 *	Name: beeptool.cpp
 *	
 * 	Purpose: Definitions of Tool class interface(s) and methods for the 
 *           NewBeep Tool Xtra example.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  XTRA SPECIFIC DEFINE(S)
 *  -----------------------
 *	Any #define's specific to this Xtra.  
 *****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed for 
 *  this particular class. In addition, this file may include other .h files 
 *  defining additional callback interfaces for use by a third party.   
 *****************************************************************************/ 
#include "beeptool.h"


#ifdef MACINTOSH
#include <windows.h>
#include <string.h>
#include <sound.h>
#endif /* MACINTOSH */

/*****************************************************************************
 *  FILE SPECIFIC DEFINE(S)
 *  -----------------------
 *	Any #define's specific to this file.  
 *****************************************************************************/ 
#ifndef UNUSED
#define UNUSED(x) x
#endif

/*****************************************************************************
 *	CONSTANTS
 *****************************************************************************/ 

enum 
{
	kBeep_WindowBegin = 0,
		kBeep_GroupH1Begin,
			kBeep_Label,
			kBeep_EditText,
		kBeep_GroupH1End,
		kBeep_GroupH2Begin,
			kBeep_BeepButton,
			kBeep_DoneButton,
		kBeep_GroupH2End,
	kBeep_WindowEnd,
	kBeep_NumItems
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
BEGIN_DEFINE_CLASS_INTERFACE(CBeepTool, IMoaMmXTool)
END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
BEGIN_DEFINE_CLASS_INTERFACE(CBeepTool, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
BEGIN_DEFINE_CLASS_INTERFACE(CBeepTool, IMoaNotificationClient)
END_DEFINE_CLASS_INTERFACE
#endif


/*****************************************************************************
 *  CREATE AND DESTROY METHODS
 *  --------------------------
 *  Every interface and class has an associated 'Create' and 'Destroy' pair.
 *  'Create' methods are typically used to acquire interface(s), allocate 
 *  memory, and intialize variables. 'Destroy' methods are typically used to 
 *  release interfaces and free memory.
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

/* ----------------------------------------------------------- MoaCreate_CTool */
STDMETHODIMP MoaCreate_CBeepTool(CBeepTool * This)
{
	UNUSED(This);
	X_ENTER
	
	X_RETURN(MoaError, kMoaErr_NoErr);
	X_EXIT
}


/* ---------------------------------------------------------- MoaDestroy_CTool */
STDMETHODIMP_(void) MoaDestroy_CBeepTool(CBeepTool * This)
{
	UNUSED(This);
	X_ENTER
	
	X_RETURN_VOID;
	X_EXIT
}

/* --------------------------------------- CBeepTool_IMoaMmXTool Create/Destroy */
CBeepTool_IMoaMmXTool::CBeepTool_IMoaMmXTool(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CBeepTool_IMoaMmXTool::~CBeepTool_IMoaMmXTool() {}

#ifdef USING_INIT_FROM_DICT
/* ------------------------------------ CBeepTool_IMoaInitFromDict Create/Destroy */
CBeepTool_IMoaInitFromDict::CBeepTool_IMoaInitFromDict(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CBeepTool_IMoaInitFromDict::~CBeepTool_IMoaInitFromDict() {}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* ------------------------------------ CBeepTool_IMoaNotificationClient Create/Destroy */
CBeepTool_IMoaNotificationClient::CBeepTool_IMoaNotificationClient(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CBeepTool_IMoaNotificationClient::~CBeepTool_IMoaNotificationClient() {}
#endif

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

/* -------------------------------------- CTool_IMoaInitFromDict::InitFromDict */
#ifdef USING_INIT_FROM_DICT

STDMETHODIMP CTool_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
{
	UNUSED(pRegistryDict);
	X_ENTER
	
	/* variable declarations */
	MoaError err = kMoaErr_NoErr;

	X_RETURN(MoaError, err);
	X_EXIT
}
#endif

/* ------------------------------------------------- CBeepTool_IMoaMmXTool::Invoke */
STDMETHODIMP CBeepTool_IMoaMmXTool::Invoke(void)
{
	X_ENTER

	/* variable declarations */
	MoaError err = kMoaErr_NoErr;

	PIMoaNotificationClient pNotify = NULL;
	PIMuiDialog				pMuiDialog = NULL;
	PIMoaMmValue			pMmValue = NULL;
	// setup a structure of stuff
	TMuiWindow 				myInitStruct;
	TMuiItem 				myItemArray[kBeep_NumItems];
	MoaLong					i = 0;

	// clear out structs
	memset( &myInitStruct, 0, sizeof( myInitStruct ) );
	memset( &myItemArray[0], 0, (kBeep_NumItems * sizeof( TMuiItem )) );
	
	pObj->pCallback->MoaCreateInstance(&CLSID(CMui), &IID(IMuiDialog), (PPMoaVoid)&pMuiDialog);
	if (!pMuiDialog) 
		goto exit_gracefully;

	QueryInterface(&IID_IMoaNotificationClient, (PPMoaVoid)&pNotify);
	if (!pNotify)
		goto exit_gracefully;

	pObj->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid)&pMmValue);
	if (!pMmValue) 
		goto exit_gracefully;		
	
	// --------------------------------------------------------------------------------
	// Window Params
	// --------------------------------------------------------------------------------
	
	myInitStruct.iStructSize	= sizeof( TMuiWindow );
	myInitStruct.iType  	  	= kMuiWindowType_Standard;
	myInitStruct.pTitle			= "Beep Tool";
	myInitStruct.iLayout  		= kMuiLayout_Dynamic;
	myInitStruct.iPosition		= kMuiWindowPosition_Centered;
	myInitStruct.bHasCloseBox	= TRUE;
	myInitStruct.bModal			= TRUE;
	
	// --------------------------------------------------------------------------------
	// Create Items
	// --------------------------------------------------------------------------------

	// All MuiItems are the same size
	for (i = 0; i < kBeep_NumItems; i++) 
		myItemArray[i].iStructSize = sizeof(TMuiItem);
	
	// kBeep_WindowBegin
	myItemArray[kBeep_WindowBegin].iType = kMuiWidgetType_Group_WindowBegin;

		// kBeep_GroupH1Begin
		myItemArray[kBeep_GroupH1Begin].iType = kMuiWidgetType_Group_HBegin;

			// kBeep_Label
			myItemArray[kBeep_Label].iType = kMuiWidgetType_Label_Normal;
			pMmValue->StringToValue("# of beeps: ", &myItemArray[kBeep_Label].vValue);
			myItemArray[kBeep_Label].bEnabled = TRUE;

			// kBeep_EditText
			myItemArray[kBeep_EditText].iType = kMuiWidgetType_Char_EditText;
			pMmValue->IntegerToValue(1, &myItemArray[kBeep_EditText].vValue);
			myItemArray[kBeep_EditText].bEnabled = TRUE;

		// kBeep_GroupH1End
		myItemArray[kBeep_GroupH1End].iType = kMuiWidgetType_Group_HEnd;

		// kBeep_GroupH2Begin
		myItemArray[kBeep_GroupH2Begin].iType = kMuiWidgetType_Group_HBegin;

			// kBeep_BeepButton
			myItemArray[kBeep_BeepButton].iType = kMuiWidgetType_Button_PushButton;
			myItemArray[kBeep_BeepButton].pTitle = "Beep";
			myItemArray[kBeep_BeepButton].bEnabled = TRUE;

			// kBeep_DoneButton
			myItemArray[kBeep_DoneButton].iType = kMuiWidgetType_Button_DefaultPushButton;
			myItemArray[kBeep_DoneButton].pTitle = "Done";
			myItemArray[kBeep_DoneButton].bEnabled = TRUE;

		// kBeep_GroupH2End
		myItemArray[kBeep_GroupH2End].iType = kMuiWidgetType_Group_HEnd;

	// kBeep_WindowEnd
	myItemArray[kBeep_WindowEnd].iType = kMuiWidgetType_Group_WindowEnd;
	
	// --------------------------------------------------------------------------------
	// Instantiate Window
	// --------------------------------------------------------------------------------

	if (err = pMuiDialog->Initialize(&myInitStruct, kBeep_NumItems, myItemArray))
		goto exit_gracefully;		
	
	pMuiDialog->SetCallback(pNotify, myItemArray);
	
	pMuiDialog->Run();
	

exit_gracefully:

	// ValueRelease() is safe to use with values initialized to {0,0}.
	pMmValue->ValueRelease(&myItemArray[kBeep_Label].vValue);
	pMmValue->ValueRelease(&myItemArray[kBeep_EditText].vValue);

	if(pMmValue)
		pMmValue->Release();
	if(pMuiDialog)
		pMuiDialog->Release();
	if(pNotify)
		pNotify->Release();
		
	X_RETURN(MoaError, err);
	X_EXIT
}

/* --------------------------------------------- CBeepTool_IMoaMmXTool::GetEnabled */
STDMETHODIMP CBeepTool_IMoaMmXTool::GetEnabled(PMoaDrEnabledState pEnabledState)
{
	X_ENTER

	/* This method is called by the host application to find out if
	   our tool is enabled or not.  Disabled tools will appear
	   differently in the user interface (such as a dimmed-out
	   menu item or button), and can't be selected by the user.

       In this example, we have registered a TRUE value for the
	   "alwaysEnabled" registry key.  This means that the host
	   app will assume we're always enabled, and will never
	   call this method.  This is done for performance and
	   memory considerations: if a tool is marked as 
	   "alwaysEnabled", the code for the tool doesn't have 
	   to be loaded until the tool is selected (otherwise, 
	   it must be loaded as soon as the user clicks on the menu 
	   bar, since its GetEnabled() method must be called.
		
	   So, if you're always enabled, declare "alwaysEnabled"
	   as TRUE in your IMoaRegister::Register() method; 
	   if your enable state changes dynamically, declare 
	   "alwaysEnabled" as FALSE and set *pEnabledState to the
	   appropriate value here when called. 
	
	   We're always enabled, so it doesn't matter what we do
	   here. */

	*pEnabledState = kMoaDrEnabledState_Enabled;
	
	X_RETURN(MoaError, kMoaErr_NoErr);
	X_EXIT
}

/* --------------------------------------------- CBeepTool_IMoaNotificationClient::Notify */
STDMETHODIMP CBeepTool_IMoaNotificationClient::Notify(ConstPMoaNotifyID	nid, PMoaVoid pNData, PMoaVoid pRefCon)
{
	X_ENTER
	
	if (!MoaEqualID(nid, &IID(IMuiDialog))) 
		X_STD_RETURN(kMoaErr_BadInterface);
		
	PTMuiItem recordArray = (PTMuiItem)pRefCon;
	if (!recordArray) 
		X_STD_RETURN(kMoaErr_BadParam);

	PTMuiEventRecord item = (PTMuiEventRecord)pNData;
	MoaLong numBeeps = 0L;
	PIMoaMmValue pMmValue = NULL;
	int i = 0;

	pObj->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid)&pMmValue);
	if (!pMmValue) 
		X_STD_RETURN(kMoaErr_BadInterface);
				
	if (item->event == kMuiDialogItemClicked)
	{
		switch (item->itemHit) 
		{
		case kBeep_BeepButton:
			pMmValue->ValueToInteger(&recordArray[kBeep_EditText].vValue, &numBeeps);
			for (i = 0; i < numBeeps; i++)
			{
				unsigned long goal;
#ifdef MACINTOSH
				goal = TickCount() + 12;
				SysBeep(1);
				if (numBeeps>1)
				{
					while (goal>TickCount())
						;
				}
#endif
#ifdef WINDOWS
				goal = timeGetTime() + 200;
				MessageBeep(MB_OK);
				if (numBeeps>1)
				{
					Sleep(10);
					while (goal>timeGetTime())
						;
				}
#endif
			}
			break;

		case kBeep_DoneButton:
			item->pDialog->Stop(0);
			break;
		}
	}

	X_STD_RETURN(kMoaErr_NoErr);
	X_EXIT
}
