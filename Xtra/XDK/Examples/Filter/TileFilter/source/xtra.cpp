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
 *	Name: xtra.cpp
 *	
 * 	Purpose: The Xtra definition of a Filter Skeleton Xtra example.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *****************************************************************************/ 
#include "tilefilter.h"
#include "tileregister.h"
#include "xclassver.h"

/*****************************************************************************
 *  XTRA DEFINITION
 ****************************************************************************/ 
BEGIN_XTRA
	BEGIN_XTRA_DEFINES_CLASS(TRegister, XTRA_CLASS_VERSION) 
		CLASS_DEFINES_INTERFACE(TRegister, IMoaRegister, XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS

	BEGIN_XTRA_DEFINES_CLASS(TileFilter, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(TileFilter, IMoaBitmapFilter, XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS
END_XTRA
