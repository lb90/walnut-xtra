/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2007 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_ValueChecker
#define _H_ValueChecker

#include "moastdif.h"
#include "mivect3d.h"
#include "xmmvalue.h"


/* --------------------------------------------------- */


DEFINE_GUID(CLSID_TStdXtra, 0x1819C16AL, 0xBC37, 0x11D5, 0xAF, 0x93, 0x00, 0x30, 0x65, 0x09, 0xC1, 0xBB);

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(TStdXtra)

	PIMoaHandle 	pMoaHandle;
	PIMoaMmValue	pValueInterface;
	PIMoaMmList		pListInterface;
	PIMoa3dVectorValueUtils pVectorInterface;

EXTERN_END_DEFINE_CLASS_INSTANCE_VARS



/*****************************************************************************
 * Xtra Moa Registration Interface - Class definition
 ****************************************************************************/

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaRegister)

	EXTERN_DEFINE_METHOD(MoaError, Register, (PIMoaCache, PIMoaXtraEntryDict))

EXTERN_END_DEFINE_CLASS_INTERFACE



EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaMmXScript)
	// -----------------
	// Public methods 
	// -----------------

	EXTERN_DEFINE_METHOD(MoaError, Call, (PMoaDrCallInfo))
	
	// -----------------
	// Private methods 
	// -----------------

	// Methods that implement the Lingo commands.
	// These are dispatched from "Call"

	EXTERN_DEFINE_METHOD(MoaError, TestValue, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, TestValue3D, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, TestValue3DTransform, (PMoaDrCallInfo))

	// -----------------
	// Private data
	// -----------------


EXTERN_END_DEFINE_CLASS_INTERFACE



#endif /* #ifdef _H_ValueChecker */
