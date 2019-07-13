/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_cregister
#define _H_cregister

/*****************************************************************************
 *
 *	Name: cregister.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for the ComputerUser Xtra Register class.
 *
 **********************************************************************************/ 

/*****************************************************************************
 *  XTRA SPECIFIC DEFINE(S)
 *****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *****************************************************************************/ 
#include "moaxtra.h"
#include "cutool.h"

/* NOTE: #include any .h file that contains the CLSID of the class(es) that you
   you will be registering */

/*****************************************************************************
 *  CONFIGURATION DEFINE(S)
 ****************************************************************************/ 

/*****************************************************************************
 *	CLASS SPECIFIC DEFINE(S)
 ****************************************************************************/ 

/*****************************************************************************
 *	CLSID
 ****************************************************************************/ 
DEFINE_GUID(CLSID(CURegister), 0x0409BEAA8L, 0xF176, 0x43C2, 0xA5, 0xD7, 0xAF, 0x38, 0xEE, 0xA7, 0x98, 0x59);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CURegister) 
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

/*****************************************************************************
 *  CLASS INTERFACE(S)
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CURegister, IMoaRegister) 
	EXTERN_DEFINE_METHOD(MoaError, Register, (PIMoaCache, PIMoaDict))
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif /* _H_cregister */
