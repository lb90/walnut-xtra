/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_Script
#define _H_Script

#include "moastdif.h"
#include "xmmvalue.h"
#include "mmiutil.h"


/* --------------------------------------------------- */


#error PLEASE DEFINE A NEW CLSID
//DEFINE_GUID(CLSID_TStdXtra, 0x00000000L, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(TStdXtra)

	PIMoaMmUtils2 	pMoaUtils;
	PIMoaMmValue	pValueInterface;
	/*
	 * ---> insert additional variable(s) -->
	 */ 

EXTERN_END_DEFINE_CLASS_INSTANCE_VARS



/*****************************************************************************
 * Xtra Moa Registration Interface - Class definition
 ****************************************************************************/

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaRegister)

	EXTERN_DEFINE_METHOD(MoaError, Register, (PIMoaCache, PIMoaXtraEntryDict))

EXTERN_END_DEFINE_CLASS_INTERFACE



/*****************************************************************************
 * Xtra Moa Scripting Interface - Class definition
 ****************************************************************************/

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaMmXScript)

	EXTERN_DEFINE_METHOD(MoaError, Call, (PMoaDrCallInfo))
private:
	// Methods that implement the Lingo commands.
	// These are dispatched from "Call"

	EXTERN_DEFINE_METHOD(MoaError, XScrpGlobalHandler, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XScrpParentHandler, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XScrpChildHandler, (PMoaDrCallInfo))
	/*
	 * ---> insert additional method(s) -->
	 */ 
	 
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif /* _H_Script */
