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
 *	Name: cfilter.cpp
 *	
 * 	Purpose: Definitions of class interface(s) and methods for the 
 *           Filter Xtra skeleton project.
 *
 *  HOW TO CUSTOMIZE THIS FILE
 *  --------------------------
 *  1. Save this file under a different file name.
 *  2. Use a search and replace utility (case sensitive) to replace the
 *     following:
 *
 *     Replace         With	
 *     -------         ----
 *     cfilter         <this file name>
 *     CFilter		   <name of the class you defined>
 *
 *  3. Add and modify source code by looking for '--> insert' comment
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "cfilter.h"

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
BEGIN_DEFINE_CLASS_INTERFACE(CFilter, IMoaBitmapFilter)
END_DEFINE_CLASS_INTERFACE

/*
 * --> insert additional interface(s) -->
 */ 

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

/* -------------------------------------------------- MoaCreate_CFilter */
STDMETHODIMP MoaCreate_CFilter(CFilter * This)
{
	MoaError err = kMoaErr_NoErr;
	/*
	 * --> insert additional code -->
	 */
	return err;
}

/* ------------------------------------------------- MoaDestroy_CFilter */
STDMETHODIMP_(void) MoaDestroy_CFilter(CFilter * This)
{
	/*
	 * --> insert additional code -->
	 */
}

/* ---------------------- CFilter_IMoaBitmapFilter Create/Destroy */
CFilter_IMoaBitmapFilter::CFilter_IMoaBitmapFilter(MoaError * pErr) { *pErr = (kMoaErr_NoErr); }

CFilter_IMoaBitmapFilter::~CFilter_IMoaBitmapFilter() {}

/*
 * --> insert additional create/destroy method(s) -->
 */

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

/* ----------------------------- CFilter_IMoaBitmapFilter::CallFunction */
STDMETHODIMP CFilter_IMoaBitmapFilter::CallFunction(MoaMmSymbol symbol, MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{
	MoaError err = kMoaMmErr_FunctionNotFound;
	
	/*
	 * --> insert additional code -->
	 */
	
	return err;
}

/* -------------------------------- CFilter_IMoaBitmapFilter::GetProp */
STDMETHODIMP CFilter_IMoaBitmapFilter::GetProp(MoaMmSymbol symbol, PMoaMmValue pPropValue)
{
	MoaError err = kMoaMmErr_PropertyNotFound;
	
	/*
	 * --> insert additional code -->
	 */
	
	return err;
}
	
/* --------------------------- CFilter_IMoaBitmapFilter::SetProp */
STDMETHODIMP CFilter_IMoaBitmapFilter::SetProp(MoaMmSymbol symbol, ConstPMoaMmValue pPropValue)
{
  MoaError err = kMoaMmErr_PropertyNotFound;
	
	/*
	 * --> insert additional code -->
	 */

	return err;
}
	
/* ----------------------------- CFilter_IMoaBitmapFilter::RunFilter */
STDMETHODIMP CFilter_IMoaBitmapFilter::RunFilter(PIMoaMmGC pIMoaMmGC, const PMoaFilterInfo pDestScaleRect)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 * --> insert additional code -->
	 */

	return err;
}

/* ----------------------------- CFilter_IMoaBitmapFilter::StreamInProps */
STDMETHODIMP CFilter_IMoaBitmapFilter::StreamInProps(PIMoaStream pInStream)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 * --> insert additional code -->
	 */

	return err;
}	

/* ----------------------------- CFilter_IMoaBitmapFilter::StreamOutProps */
STDMETHODIMP CFilter_IMoaBitmapFilter::StreamOutProps(PIMoaStream pOutStream)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 * --> insert additional code -->
	 */

	return err;
}

/* ----------------------------- CFilter_IMoaBitmapFilter::GetStreamOutPropsSize */
STDMETHODIMP CFilter_IMoaBitmapFilter::GetStreamOutPropsSize(PMoaLong pPropSize)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 * --> insert additional code -->
	 */
	*pPropSize = 0;
	return err;
}

/* ----------------------------- CFilter_IMoaBitmapFilter::InvokePropsEditor */
STDMETHODIMP CFilter_IMoaBitmapFilter::InvokePropsEditor()
{
	MoaError err = kMoaErr_NoErr;

	/*
	 * --> insert additional code -->
	 */

	return err;
}

/* ----------------------------- CFilter_IMoaBitmapFilter::FinishPropsEditor */
STDMETHODIMP CFilter_IMoaBitmapFilter::FinishPropsEditor(MoaBoolParam bCommitChanges, PMoaMmModifyFlags pFlagModifyProp)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 * --> insert additional code -->
	 */

	return err;
}

/* ----------------------------- CFilter_IMoaBitmapFilter::GetOutputRect */
STDMETHODIMP CFilter_IMoaBitmapFilter::GetOutputRect(const PMoaFilterInfo  pInputScaleRect, PMoaFilterInfo pOutputScaleRect)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 * --> insert additional code -->
	 */

	*pOutputScaleRect = *pInputScaleRect;
	return err;
}

/* ----------------------------- CFilter_IMoaBitmapFilter::GetFilterCaps */
STDMETHODIMP CFilter_IMoaBitmapFilter::GetFilterCaps(PMoaBMFilterCaps pFilterCap)
{
	MoaError err = kMoaErr_NoErr;

	/*
	 * --> insert additional code -->
	 */
	pFilterCap->flags = kMoaMmXFilterCapsFlags_None;

	return err;
}

/* ----------------------------- CFilter_IMoaBitmapFilter::SetCallback */
STDMETHODIMP CFilter_IMoaBitmapFilter::SetCallback(PIMoaBitmapFilterCallback pBitmapFilterCallback)
{
	MoaError err = kMoaErr_NotImplemented;

	/*
	 * --> insert additional code -->
	 */

	return err;
}
/* ----------------------------- CFilter_IMoaBitmapFilter::Notify */
STDMETHODIMP CFilter_IMoaBitmapFilter::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	MoaError err = kMoaErr_NotImplemented;

	/*
	 * --> insert additional code -->
	 */

	return err;
}
