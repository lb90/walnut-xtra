/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_tileregister
#define _H_tileregister

/*****************************************************************************
 *
 *	Name: tileregister.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for a generic Xtra Register class.
 **********************************************************************************/ 

/*****************************************************************************
 *  XTRA SPECIFIC DEFINE(S)
 *****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *****************************************************************************/ 
#include "moaxtra.h"
#include "tilefilter.h"

/*****************************************************************************
 *  CONFIGURATION DEFINE(S)
 ****************************************************************************/ 

/*****************************************************************************
 *	CLASS SPECIFIC DEFINE(S)
 ****************************************************************************/ 

/*****************************************************************************
 *	CLSID
 ****************************************************************************/ 
DEFINE_GUID(CLSID(TRegister), 0x6A66E7EDL, 0x647E, 0x424D, 0x9E, 0x4E, 0x55, 0x52, 0x02, 0x93, 0x96, 0xA8);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(TRegister)
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

/*****************************************************************************
 *  CLASS INTERFACE(S)
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(TRegister, IMoaRegister) 
	EXTERN_DEFINE_METHOD(MoaError, Register, (PIMoaCache, PIMoaDict))
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif /* _H_tileregister */
