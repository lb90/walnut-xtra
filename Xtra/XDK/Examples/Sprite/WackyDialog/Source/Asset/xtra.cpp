/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2007 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

/*****************************************************************************
 *
 *	Name: xtra.cpp
 *
 ****************************************************************************/ 

///////////////////////////////////////////////////////////////////////////////
//
// Notes on project settings:
//
// - Turn on Enable C++ exceptions.
// * Make sure to set the CFM initialization entry point to __initialize
// * Make sure to set the CFM termination entry point to __terminate
//
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////

// Important!  This define controls the GUID definitions.  Turning this define 
// on causes the GUID to be declared.  Without this define turned on in this
// file you will get link errors against the GUID for the interfaces you are using.
// You need to add the headers to this file to get those linker errors to go away.
#define INITGUID

#include "Asset.h"
#include "Sprite.h"
#include "Dialog.h"
#undef INITGUID
// see that wasn't so hard now was it...

// this is for the version macros below.
#include "xclassver.h"

///////////////////////////////////////////////////////////////////////////////
//
// Xtra definition -- any classes you bundle in this Xtra are declared here!
//
///////////////////////////////////////////////////////////////////////////////

 BEGIN_XTRA
 	BEGIN_XTRA_DEFINES_CLASS(CWackyDlgAsset, XTRA_CLASS_VERSION)
 		CLASS_DEFINES_INTERFACE(CWackyDlgAsset, IMoaMmXAsset, XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS

	BEGIN_XTRA_DEFINES_CLASS(CWackyDlgSprite, XTRA_CLASS_VERSION)
 		CLASS_DEFINES_INTERFACE(CWackyDlgSprite, IMoaMmXSpriteActor, XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS

	BEGIN_XTRA_DEFINES_CLASS(CWackyDlgRegister, XTRA_CLASS_VERSION)
 		CLASS_DEFINES_INTERFACE(CWackyDlgRegister, IMoaRegister, XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS
 END_XTRA

// EOF
