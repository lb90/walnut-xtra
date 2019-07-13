/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2007 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _carray_
#define _carray_

#include "moastdif.h"
#include "xmmvalue.h"
#include "mmiutil.h"
#include "mmixval.h"

/* --------------------------------------------------- */

DEFINE_GUID(CLSID_TStdXtra, 0x8C57598FL, 0x126D, 0x11D9, 0xA6, 0x55, 0x00, 0x03, 0x93, 0x54, 0xaa, 0x6a);
DEFINE_GUID(CLSID_ArrayVal, 0x665048B9L, 0x1273, 0x11D9, 0xbe, 0x09, 0x00, 0x03, 0x93, 0x54, 0xaa, 0x6a);

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(TStdXtra)
	PIMoaMmValue pValue;
	PIMoaMmXValueUtils fpXUtils;
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(ArrayVal)
	PIMoaMmValue fpValue;
	PMoaVoid fDataBlock;
	MoaMmValue fDescription;
	MoaLong fiType;
	MoaLong fiSize;
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
	EXTERN_DEFINE_METHOD(MoaError, CreateXArray, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, DoubleXArray, (PMoaDrCallInfo))
EXTERN_END_DEFINE_CLASS_INTERFACE

/*****************************************************************************
 * Xtra Moa XValue Interface - Class definition
 ****************************************************************************/

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(ArrayVal, IMoaMmXValue)
		EXTERN_DEFINE_METHOD(MoaError, SetData, (PMoaVoid pDatum))		
		EXTERN_DEFINE_METHOD(MoaError, GetPropCount, (MoaLong * pCount))		
		EXTERN_DEFINE_METHOD(MoaError, GetPropDescriptionByIndex, (MoaLong index, PMoaMmValueDescription pDescription))		
		EXTERN_DEFINE_METHOD(MoaError, GetCount, (MoaMmSymbol propName, MoaLong	* pCount))		
		EXTERN_DEFINE_METHOD(MoaError, GetProp, (ConstPMoaMmValue selfRef,
										MoaMmSymbol propName,
										MoaLong	indexCount,
										ConstPMoaMmValue pIndexValues,
										PMoaMmValue	pResult))		
		EXTERN_DEFINE_METHOD(MoaError, SetProp, (MoaMmSymbol propName,
										MoaLong	indexCount,
										ConstPMoaMmValue pIndexValues,
										ConstPMoaMmValue pNewValue))		
		EXTERN_DEFINE_METHOD(MoaError, AccessPropRef, (ConstPMoaMmValue	selfRef,
										MoaMmSymbol	propName,
										MoaLong	indexCount,
										ConstPMoaMmValue pIndexValues,
										PMoaMmValue	pResult))		
		EXTERN_DEFINE_METHOD(MoaError, GetContents, (ConstPMoaMmValue selfRef, PMoaMmValue	pResult))		
		EXTERN_DEFINE_METHOD(MoaError, SetContents, (PMoaMmValue pNewValue))		
		EXTERN_DEFINE_METHOD(MoaError, SetContentsBefore, (PMoaMmValue pNewValue))		
		EXTERN_DEFINE_METHOD(MoaError, SetContentsAfter, (PMoaMmValue pNewValue))		
		EXTERN_DEFINE_METHOD(MoaError, CallHandler, (PMoaMmCallInfo callPtr))		
		EXTERN_DEFINE_METHOD(MoaError, Ilk, (PMoaMmValue pArgument, PMoaMmValue	pResult))		
		EXTERN_DEFINE_METHOD(MoaError, StringRep, (PMoaMmValue pStringVal))		
		EXTERN_DEFINE_METHOD(MoaError, SymbolRep, (PMoaMmSymbol	pSymbol))		
		EXTERN_DEFINE_METHOD(MoaError, IntegerRep, (PMoaLong pIntVal))		
		EXTERN_DEFINE_METHOD(MoaError, StreamOut, (PIMoaStream2	pStream))		
		EXTERN_DEFINE_METHOD(MoaError, StreamIn, (PIMoaStream2 pStream))
// own method - not private so it can be called by the lingo handler
		EXTERN_DEFINE_METHOD(void, Double, ())
EXTERN_END_DEFINE_CLASS_INTERFACE

#endif /* _carray_ */
