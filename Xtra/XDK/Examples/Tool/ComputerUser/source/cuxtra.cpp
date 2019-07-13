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
 *	Name: cuxtra.cpp
 *	
 * 	Purpose: The Xtra definition of the ComputerUser tool Xtra.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *****************************************************************************/ 
#include "cutool.h"
#include "curegister.h"
#include "xclassver.h"


/*****************************************************************************
 *  XTRA DEFINITION
 *****************************************************************************/ 
 BEGIN_XTRA
	BEGIN_XTRA_DEFINES_CLASS(CURegister, XTRA_CLASS_VERSION)
 		CLASS_DEFINES_INTERFACE(CURegister, IMoaRegister, 	XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS
 	BEGIN_XTRA_DEFINES_CLASS(CUTool, XTRA_CLASS_VERSION)
 		CLASS_DEFINES_INTERFACE(CUTool, IMoaMmXTool, XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS
END_XTRA
