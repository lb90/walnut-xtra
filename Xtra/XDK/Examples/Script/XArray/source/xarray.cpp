/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/


#define INITGUID 1

#include "xarray.h"

#include <string.h>
#include <stdlib.h>
#include "xclassver.h"
#include "moatry.h"

#include "driservc.h"
#include "drivalue.h"

#include "mmivalue.h"
#include "mmillist.h"
#include "mmiplist.h"
#include "mmidate.h"
#include "mmiservc.h"
#include "mmiclr.h"


/*******************************************************************************
 * SCRIPTING XTRA MESSAGE TABLE.
 *
 *******************************************************************************/ 
 
 /* This is the list of handlers for the xtra. The versionInfo string is combined
 /*	with the msgTable string in the register method to create a single string that
 /* used when registering the xtra as a scripting xtra. */

static char versionInfo[] = "xtra XArray -- version %s.%s.%s\n";
static char msgTable[] = { 
	"* newXArray integer numberOfElements, symbol integerOrFloat -- creates new XArray\n"
	"* doubleXArray object XArray -- doubles each item in the XArray\n"
	};


/* 	This is the enumerated scripting method list. This list should
 *	directly correspond to the msgTable defined above. It is used
 *	to dispatch method calls via the methodSelector. The 'm_XXXX' method must
 *	be last.
 */

enum 
{
	m_newXArray = 0,
	m_doubleXArray,
	m_XXXX
};

// ArrayVal structures and constants

#define kTypeInteger 0
#define kTypeFloat 1

typedef struct ArrayValData
{
	MoaLong iSize;
	MoaLong iType;
} ArrayValData, *PArrayValData;



/* ============================================================================= */
/* Xtra Glue Stuff */
/* ============================================================================= */

#define XTRA_VERSION_NUMBER XTRA_CLASS_VERSION

BEGIN_XTRA
	BEGIN_XTRA_DEFINES_CLASS(TStdXtra, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(TStdXtra, IMoaRegister, XTRA_VERSION_NUMBER)
		CLASS_DEFINES_INTERFACE(TStdXtra, IMoaMmXScript, XTRA_VERSION_NUMBER)
	END_XTRA_DEFINES_CLASS
	BEGIN_XTRA_DEFINES_CLASS(ArrayVal, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(ArrayVal, IMoaMmXValue, XTRA_VERSION_NUMBER)
	END_XTRA_DEFINES_CLASS
END_XTRA


/* ============================================================================= */
/* Create/Destroy for class TStdXtra */
/* ============================================================================= */


MoaError MoaCreate_TStdXtra (TStdXtra * This)
{
moa_try
		
	ThrowErr (This->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid) &This->pValue));
	ThrowErr (This->pCallback->QueryInterface(&IID_IMoaMmXValueUtils, (PPMoaVoid) &This->fpXUtils));	

moa_catch
moa_catch_end
moa_try_end
}

void MoaDestroy_TStdXtra(TStdXtra * This)
{
moa_try

	if (This->pValue != NULL) 
		ThrowErr (This->pValue->Release());
		
	if (This->fpXUtils != NULL) 
		ThrowErr (This->fpXUtils->Release());

moa_catch
moa_catch_end
moa_try_end_void
}


/* ============================================================================= */
/* Methods in TStdXtra_IMoaRegister */
/* ============================================================================= */

/*****************************************************************************
 *  Data needed for Registering
 *	---------------------------
 *	Specific code needed to register different types of Xtras.  The skeleton
 *	source files include minimal implementations appropriate for each Xtra
 *	type.  Current necessary actions:
 *
 *	Scripting Xtra:				Add the Scripting Xtra Message Table
 *	Sprite Asset Xtra:			Nothing
 *	Tool Xtra:					Nothing
 *	Transition Asset Xtra		Nothing
 *
 *  ****optional: Register as Safe for Shockwave!
 *****************************************************************************/ 

STD_INTERFACE_CREATE_DESTROY(TStdXtra, IMoaRegister)

BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaRegister)
END_DEFINE_CLASS_INTERFACE

/* ----------------------------------------------------------------------------- */
MoaError TStdXtra_IMoaRegister::Register(
	PIMoaCache pCache, 
	PIMoaXtraEntryDict pXtraDict
)
{	
moa_try
	PIMoaRegistryEntryDict		pReg;
	MoaBool						bItsSafe = TRUE;
	char versionStr[256];
	PMoaVoid pMemStr = NULL;

	/* Register the lingo xtra */
	ThrowErr(pCache->AddRegistryEntry(pXtraDict, &CLSID_TStdXtra, &IID_IMoaMmXScript, &pReg));

	/* Register the method table */
	sprintf(versionStr, versionInfo, DVER_MAJORVERSION_STRING, DVER_MINORVERSION_STRING, DVER_BUGFIXVERSION_STRING);
	pMemStr = pObj->pCalloc->NRAlloc(sizeof(msgTable) + sizeof(versionStr));
	ThrowNull(pMemStr);

	strcpy((char *)pMemStr, versionStr);
	strcat((char *)pMemStr, msgTable);

	ThrowErr(pReg->Put(kMoaDrDictType_MessageTable, pMemStr, 0, kMoaDrDictKey_MessageTable));

	/* Mark xtra as safe for shockwave - but only if it IS safe ! */
	ThrowErr(pReg->Put(kMoaMmDictType_SafeForShockwave, &bItsSafe, sizeof(bItsSafe), kMoaMmDictKey_SafeForShockwave));

moa_catch
moa_catch_end
	if (pMemStr)
		pObj->pCalloc->NRFree(pMemStr);
moa_try_end
}



/* ============================================================================= */
/*  Methods in TStdXtra_IMoaMmXScript */
/* ============================================================================= */

BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaMmXScript)
END_DEFINE_CLASS_INTERFACE

//******************************************************************************
TStdXtra_IMoaMmXScript::TStdXtra_IMoaMmXScript(MoaError * pError)
//------------------------------------------------------------------------------
{
	*pError = kMoaErr_NoErr;
}	

//******************************************************************************
TStdXtra_IMoaMmXScript::~TStdXtra_IMoaMmXScript()
//------------------------------------------------------------------------------
{
}


/* ----------------------------------------------------------------------------- */
MoaError TStdXtra_IMoaMmXScript::Call (PMoaDrCallInfo callPtr)
{
moa_try
	switch	( callPtr->methodSelector ) 
	{
		case m_newXArray:
			ThrowErr(CreateXArray(callPtr));
			break;

		case m_doubleXArray:
			ThrowErr(DoubleXArray(callPtr));
			break;
	}
moa_catch
moa_catch_end
moa_try_end
}


/*****************************************************************************
 *  Private Methods
 *  -------------------
 *  Implementation of Private Class Methods
 /*
 *  This is the actual code for the defined methods. They are defined as 
 *  functions, and called from the switch statement in Call().
 *
 ****************************************************************************/

/* ----------------------------------------------------- CreateXArray */
MoaError TStdXtra_IMoaMmXScript::CreateXArray(PMoaDrCallInfo pCall)
{
moa_try
	
	PIMoaMmXValue pNewXVal = NULL;
	MoaMmSymbol sym;
	MoaMmSymbol iSym, fSym;
	ArrayValData aData;

	ThrowErr(pObj->pValue->StringToSymbol("integer", &iSym));
	ThrowErr(pObj->pValue->StringToSymbol("float", &fSym));
	ThrowErr(pObj->pCallback->MoaCreateInstance(&CLSID_ArrayVal, &IID_IMoaMmXValue, (PPMoaVoid)&pNewXVal));
	ThrowErr(pObj->pValue->ValueToInteger(&pCall->pArgs[0], &aData.iSize));
	ThrowErr(pObj->pValue->ValueToSymbol(&pCall->pArgs[1], &sym));
	if (sym==iSym)
		aData.iType = kTypeInteger;
	else if (sym==fSym)
		aData.iType = kTypeFloat;
	else
		Throw(kMoaMmErr_ValueTypeMismatch);
	ThrowErr(pNewXVal->SetData(&aData));
	ThrowErr(pObj->fpXUtils->IXValueToValue(pNewXVal, &pCall->resultValue));
	pNewXVal->Release();	// now the resultValue is holding this object

moa_catch
moa_catch_end
moa_try_end
}

/* ----------------------------------------------------- DoubleXArray */
MoaError TStdXtra_IMoaMmXScript::DoubleXArray(PMoaDrCallInfo pCall)
{
moa_try
	
	PIMoaMmXValue pXValParam = NULL;
	MoaMmValue ilkValue = kVoidMoaMmValueInitializer;
	MoaMmSymbol ilkSym, xvalSym;

	if (pObj->fpXUtils->valueIsIXValue(&pCall->pArgs[0]))
	{
		ThrowErr( pObj->fpXUtils->ValueToIXValue(&pCall->pArgs[0], &pXValParam) );
		// See if it's our type of XValue object
		ThrowErr( pXValParam->Ilk(NULL, &ilkValue) );
		ThrowErr( pObj->pValue->ValueToSymbol(&ilkValue, &ilkSym) );
		ThrowErr( pObj->pValue->StringToSymbol("XArray", &xvalSym) );
		if (xvalSym==ilkSym)
			((ArrayVal_IMoaMmXValue *)pXValParam)->Double();
		else
			Throw(kMoaMmErr_ValueTypeMismatch);
	}
	else
		Throw(kMoaMmErr_NotIMoaMmXValue);
		
moa_catch
moa_catch_end
	pObj->pValue->ValueRelease(&ilkValue);
	if (pXValParam)
		pXValParam->Release();
moa_try_end
}


// ArrayVal implementation

/* ============================================================================= */
/* Create/Destroy for class ArrayVal */
/* ============================================================================= */

MoaError MoaCreate_ArrayVal (ArrayVal * This)
{
moa_try
		
	This->fpValue = NULL;
	This->fDataBlock = NULL;
	This->fDescription.ty = 2; // this is constant for type VOID in Director
	This->fiType = kTypeInteger;
	This->fiSize = 0;
	
	ThrowErr (This->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid) &This->fpValue));

moa_catch
moa_catch_end
moa_try_end
}

void MoaDestroy_ArrayVal(ArrayVal * This)
{
	if (This->fDataBlock)
	{
		This->pCalloc->NRFree(This->fDataBlock);
		This->fDataBlock = NULL;
	}

	if (This->fpValue != NULL) 
	{
		This->fpValue->ValueRelease(&This->fDescription);
		This->fpValue->Release();
	}
}

/* ============================================================================= */
/*  Construction/Destruction in ArrayVal_IMoaMmXValue */
/* ============================================================================= */

BEGIN_DEFINE_CLASS_INTERFACE(ArrayVal, IMoaMmXValue)
END_DEFINE_CLASS_INTERFACE

//******************************************************************************
ArrayVal_IMoaMmXValue::ArrayVal_IMoaMmXValue(MoaError * pError)
//------------------------------------------------------------------------------
{
	*pError = kMoaErr_NoErr;
}	

//******************************************************************************
ArrayVal_IMoaMmXValue::~ArrayVal_IMoaMmXValue()
//------------------------------------------------------------------------------
{
}

/* ============================================================================= */
/*  Method Implementation in ArrayVal_IMoaMmXValue */
/* ============================================================================= */

// ----------------------------------------------------------------------------
// SetData()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::SetData( PMoaVoid pDatum )
{
	PArrayValData pavd = (PArrayValData)pDatum;
	MoaLong iBytes = sizeof(MoaLong); // default is integer
	pObj->fiType = pavd->iType;
	pObj->fiSize = pavd->iSize;
	if (pObj->fiSize)
	{
		if (pObj->fiType==kTypeFloat)
			iBytes = sizeof(MoaDouble);
		iBytes *= pObj->fiSize;
		pObj->fDataBlock = pObj->pCalloc->NRAlloc( iBytes );
		if (!pObj->fDataBlock)
			pObj->fiSize = 0;
	}
	return kMoaErr_NoErr;
}
								  
// ----------------------------------------------------------------------------
// GetPropCount()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::GetPropCount( MoaLong * pCount )
{
	*pCount = 0;
	return kMoaMmErr_AccessNotSupported;
}

// ----------------------------------------------------------------------------
// GetPropDescriptionByIndex()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::GetPropDescriptionByIndex( MoaLong index, PMoaMmValueDescription pDescription )
{
	return kMoaMmErr_AccessNotSupported;
}

// ----------------------------------------------------------------------------
// GetCount()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::GetCount( MoaMmSymbol propName, MoaLong * pCount )
{
	*pCount = 0;
	return kMoaErr_NoErr;
}

// ----------------------------------------------------------------------------
// GetProp()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::GetProp( ConstPMoaMmValue selfRef, MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, PMoaMmValue pResult )
{
	MoaError err;
	MoaMmSymbol sym;

	if (indexCount==0)
	{
		// object inspector calls with an "ilk" property, so we forward to the Ilk method
		// flashvalu comment says do NOT do this for script execution style = 10
//		err = pObj->fpUtils->StringToSymbol("ilk", &sym);
//		if (err==kMoaErr_NoErr && propName==sym)
//			return Ilk(NULL, pResult);
			
		err = pObj->fpValue->StringToSymbol("type", &sym);
		if (err==kMoaErr_NoErr && propName==sym)
		{
			PMoaChar cp = "integer";
			if (pObj->fiType==kTypeFloat)
				cp = "float";
			err = pObj->fpValue->StringToSymbol(cp, &sym);
			if (err==kMoaErr_NoErr)
				return pObj->fpValue->SymbolToValue(sym, pResult);
		}

		err = pObj->fpValue->StringToSymbol("size", &sym);
		if (err==kMoaErr_NoErr && propName==sym)
		{
			return pObj->fpValue->IntegerToValue(pObj->fiSize, pResult);
		}

		err = pObj->fpValue->StringToSymbol("description", &sym);
		if (err==kMoaErr_NoErr && propName==sym)
		{
			*pResult = pObj->fDescription;
			return pObj->fpValue->ValueAddRef(pResult);
		}
		
		// we choose to treat ".count" as a synonym for ".size"
		err = pObj->fpValue->StringToSymbol("count", &sym);
		if (err==kMoaErr_NoErr && propName==sym)
		{
			return pObj->fpValue->IntegerToValue(pObj->fiSize, pResult);
		}
	}
	else	// indexCount is 1 or 2
	{
		// for lingo (not javascript) indexed access, we use a hidden "data" property
		err = pObj->fpValue->StringToSymbol("data", &sym);
		if (err==kMoaErr_NoErr && propName==sym)
		{
			MoaLong index1, index2;
			err = pObj->fpValue->ValueToInteger(&pIndexValues[0], &index1);
			if (err!=kMoaErr_NoErr)
				return err;
			if (index1<0 || index1>=pObj->fiSize)
				return kMoaMmErr_ArgOutOfRange;
			if (indexCount==2)
			{
				err = pObj->fpValue->ValueToInteger(&pIndexValues[1], &index2);
				if (err!=kMoaErr_NoErr)
					return err;
				if (index2<index1 || index2>=pObj->fiSize)
					return kMoaMmErr_ArgOutOfRange;
			}
				
			if (indexCount==1)
			{
				switch (pObj->fiType)
				{
				 case kTypeInteger:
					return pObj->fpValue->IntegerToValue(*(((PMoaLong)pObj->fDataBlock)+index1), pResult);
				 	break;

				 case kTypeFloat:
					return pObj->fpValue->FloatToValue(*(((MoaDouble *)pObj->fDataBlock)+index1), pResult);
				 	break;
				}
			}
			if (indexCount==2)
			{
				PIMoaMmList pList = NULL;
				MoaMmValue xList = kVoidMoaMmValueInitializer;
				int ix;
				err = pObj->pCallback->QueryInterface(&IID_IMoaMmList, (PPMoaVoid)&pList);
				if (err!=kMoaErr_NoErr)
					goto exit_list;
				err = pList->NewListValue(&xList);
				if (err!=kMoaErr_NoErr)
					goto exit_list;
				for (ix=index1; ix<=index2; ix++)
				{
					MoaMmValue aValue = kVoidMoaMmValueInitializer;
					switch (pObj->fiType)
					{
					 case kTypeInteger:
						err = pObj->fpValue->IntegerToValue(*(((PMoaLong)pObj->fDataBlock)+ix), &aValue);
						if (err!=kMoaErr_NoErr)
							goto exit_list;
						err = pList->AppendValueToList(&xList, &aValue);
						pObj->fpValue->ValueRelease(&aValue);
						if (err!=kMoaErr_NoErr)
							goto exit_list;
					 	break;

					 case kTypeFloat:
						err = pObj->fpValue->FloatToValue(*(((MoaDouble *)pObj->fDataBlock)+ix), &aValue);
						if (err!=kMoaErr_NoErr)
							goto exit_list;
						err = pList->AppendValueToList(&xList, &aValue);
						pObj->fpValue->ValueRelease(&aValue);
						if (err!=kMoaErr_NoErr)
							goto exit_list;
					 	break;
					}
				}
				*pResult = xList;
exit_list:
				if (pList)
					pList->Release();
				if (err!=kMoaErr_NoErr)
					pObj->fpValue->ValueRelease(&xList);
				return err;
			}
		}
	}
	
	return kMoaMmErr_PropertyNotFound;
}

// ----------------------------------------------------------------------------
// SetProp()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::SetProp( MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, ConstPMoaMmValue pNewValue )
{
	MoaError err = kMoaMmErr_CannotSetProperty;
	MoaMmSymbol sym;
	if (indexCount==0)
	{
		err = pObj->fpValue->StringToSymbol("description", &sym);
		if (err==kMoaErr_NoErr && propName==sym)
		{
			MoaMmValueType typ;
			err = pObj->fpValue->ValueType(pNewValue, &typ);
			if (err==kMoaErr_NoErr && typ==kMoaMmValueType_String)
			{
				pObj->fDescription = *pNewValue;
				return pObj->fpValue->ValueAddRef(&pObj->fDescription);
			}
			err = kMoaMmErr_StringExpected;
		}
	}
	else	// indexCount is 1 or 2
	{
		// for lingo (not javascript) indexed access, we use a hidden "data" property
		err = pObj->fpValue->StringToSymbol("data", &sym);
		if (err==kMoaErr_NoErr && propName==sym)
		{
			MoaLong index1, index2;
			err = pObj->fpValue->ValueToInteger(&pIndexValues[0], &index1);
			if (err!=kMoaErr_NoErr)
				return err;
			if (index1<0 || index1>=pObj->fiSize)
				return kMoaMmErr_ArgOutOfRange;
			if (indexCount==2)
			{
				err = pObj->fpValue->ValueToInteger(&pIndexValues[1], &index2);
				if (err!=kMoaErr_NoErr)
					return err;
				if (index2<index1 || index2>=pObj->fiSize)
					return kMoaMmErr_ArgOutOfRange;
			}
				
			switch (pObj->fiType)
			{
			 case kTypeInteger:
			 	err = pObj->fpValue->ValueToInteger(pNewValue, (((PMoaLong)pObj->fDataBlock)+index1));
			 	break;

			 case kTypeFloat:
			 	err = pObj->fpValue->ValueToFloat(pNewValue, (((MoaDouble *)pObj->fDataBlock)+index1));
			 	break;
			}

			if (err==kMoaErr_NoErr && indexCount==2)
			{
				for (int ix = index1+1; ix<=index2; ix++)
				{
					switch (pObj->fiType)
					{
					 case kTypeInteger:
					 	*(((PMoaLong)pObj->fDataBlock)+ix) = *(((PMoaLong)pObj->fDataBlock)+index1);
					 	break;

					 case kTypeFloat:
					 	*(((MoaDouble *)pObj->fDataBlock)+ix) = *(((MoaDouble *)pObj->fDataBlock)+index1);
					 	break;
					}
				}
			}
			return err;
		}
	}
	return err;
}

// ----------------------------------------------------------------------------
// AccessPropRef()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::AccessPropRef( ConstPMoaMmValue selfRef, MoaMmSymbol propName, MoaLong indexCount, ConstPMoaMmValue pIndexValues, PMoaMmValue pResult )
{
	return kMoaMmErr_AccessNotSupported;
//	return GetProp(selfRef, propName, indexCount, pIndexValues, pResult);
}

// ----------------------------------------------------------------------------
// GetContents()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::GetContents( ConstPMoaMmValue selfRef, PMoaMmValue pResult )
{
	return kMoaMmErr_AccessNotSupported;
}

// ----------------------------------------------------------------------------
// SetContents()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::SetContents( PMoaMmValue pNewValue )
{
	return kMoaMmErr_AccessNotSupported;
}

// ----------------------------------------------------------------------------
// SetContentsBefore()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::SetContentsBefore( PMoaMmValue pNewValue )
{
	return kMoaMmErr_AccessNotSupported;
}

// ----------------------------------------------------------------------------
// SetContentsAfter()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::SetContentsAfter( PMoaMmValue pNewValue )
{
	return kMoaMmErr_AccessNotSupported;
}

static MoaLong S_GetArrayIndex(PIMoaMmValue pValue, PMoaMmValue pValu)
{
	MoaLong iResult = -1;
	MoaMmValueType vtype = kMoaMmValueType_Void;
	pValue->ValueType(pValu, &vtype);
	if (vtype==kMoaMmValueType_Integer)
	{
		pValue->ValueToInteger( pValu, &iResult );
	}
	return iResult;
}

// ----------------------------------------------------------------------------
// CallHandler()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::CallHandler( PMoaMmCallInfo callPtr )
{
	MoaError err;
	MoaMmSymbol sym;
	int i;
	err = pObj->fpValue->StringToSymbol("sum", &sym);
	if (err==kMoaErr_NoErr && callPtr->methodSelector==sym)
	{
		switch (pObj->fiType)
		{
		 case kTypeInteger:
		 	MoaLong iSum;
		 	PMoaLong p;
		 	p = (PMoaLong)pObj->fDataBlock;
		 	for (i=0, iSum=0; i<pObj->fiSize; i++, p++)
		 		iSum += *p;
			return pObj->fpValue->IntegerToValue(iSum, &callPtr->resultValue);
		 	break;

		 case kTypeFloat:
		 	MoaDouble fSum;
		 	MoaDouble * f;
		 	f = (MoaDouble *)pObj->fDataBlock;
		 	for (i=0, fSum=0.0; i<pObj->fiSize; i++, f++)
		 		fSum += *f;
			return pObj->fpValue->FloatToValue(fSum, &callPtr->resultValue);
		 	break;
		}
	}
	
	err = pObj->fpValue->StringToSymbol("fill", &sym);
	if (err==kMoaErr_NoErr && callPtr->methodSelector==sym)
	{
		switch (pObj->fiType)
		{
		 case kTypeInteger:
		 	MoaLong iVal;
		 	PMoaLong p;
		 	p = (PMoaLong)pObj->fDataBlock;
		 	err = pObj->fpValue->ValueToInteger(&callPtr->pArgs[1], &iVal);
		 	if (err!=kMoaErr_NoErr)
		 		return err;
		 	for (i=0; i<pObj->fiSize; i++, p++)
		 		*p = iVal;
			return kMoaErr_NoErr;
		 	break;

		 case kTypeFloat:
		 	MoaDouble fVal;
		 	MoaDouble * f;
		 	f = (MoaDouble *)pObj->fDataBlock;
		 	err = pObj->fpValue->ValueToFloat(&callPtr->pArgs[1], &fVal);
		 	if (err!=kMoaErr_NoErr)
		 		return err;
		 	for (i=0; i<pObj->fiSize; i++, f++)
		 		*f = fVal;
			return kMoaErr_NoErr;
		 	break;
		}
	}

	// this is how bracket[] access is forwarded to this object 
	err = pObj->fpValue->StringToSymbol("getat", &sym);
	if (err==kMoaErr_NoErr && callPtr->methodSelector==sym)
	{
		MoaLong index = S_GetArrayIndex(pObj->fpValue, &callPtr->pArgs[1]);
		if (index>=0)
		{
			if (index>=pObj->fiSize)
				return kMoaMmErr_ArgOutOfRange;
			switch (pObj->fiType)
			{
			 case kTypeInteger:
				return pObj->fpValue->IntegerToValue(*(((PMoaLong)pObj->fDataBlock)+index), &callPtr->resultValue);
			 	break;

			 case kTypeFloat:
				return pObj->fpValue->FloatToValue(*(((MoaDouble *)pObj->fDataBlock)+index), &callPtr->resultValue);
			 	break;
			}
		}
		else
			return kMoaMmErr_IntegerExpected;
	}
	
	// this is how [] access is forwarded to this object 
	err = pObj->fpValue->StringToSymbol("setat", &sym);
	if (err==kMoaErr_NoErr && callPtr->methodSelector==sym)
	{
		MoaLong index = S_GetArrayIndex(pObj->fpValue, &callPtr->pArgs[1]);
		if (index>=0)
		{
			if (index>=pObj->fiSize)
				return kMoaMmErr_ArgOutOfRange;
			switch (pObj->fiType)
			{
			 case kTypeInteger:
			 	return pObj->fpValue->ValueToInteger(&callPtr->pArgs[2], (((PMoaLong)pObj->fDataBlock)+index));
			 	break;

			 case kTypeFloat:
			 	return pObj->fpValue->ValueToFloat(&callPtr->pArgs[2], (((MoaDouble *)pObj->fDataBlock)+index));
			 	break;
			}
		}
		else
			return kMoaMmErr_IntegerExpected;
	}
	
	err = kMoaMmErr_FunctionNotFound;
	return err;
}

// ----------------------------------------------------------------------------
// Ilk()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::Ilk( PMoaMmValue pArgument, PMoaMmValue pResult )
{
	MoaMmSymbol sym;
	MoaError err;
	if (pArgument!=NULL)
	{
		err = kMoaMmErr_AccessNotSupported;
	}
	err = SymbolRep(&sym);
	if (err==kMoaErr_NoErr)
		err = pObj->fpValue->SymbolToValue(sym, pResult);
	return err;
}

// ----------------------------------------------------------------------------
// StringRep()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::StringRep( PMoaMmValue pStringVal )
{
moa_try
	MoaChar stringBuf[64];
	sprintf(stringBuf, "<XArray %x>", this);
	// could add the "description" here - but watch for buffer overflow!
	ThrowErr( pObj->fpValue->StringToValue( stringBuf, pStringVal) );

moa_catch
moa_catch_end
moa_try_end
}

// ----------------------------------------------------------------------------
// SymbolRep()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::SymbolRep( PMoaMmSymbol pSymbol )
{
	return pObj->fpValue->StringToSymbol("XArray", pSymbol);
}

// ----------------------------------------------------------------------------
// IntegerRep()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::IntegerRep( PMoaLong pIntVal )
{
	*pIntVal = 0;
	return kMoaMmErr_AccessNotSupported;
}

// ----------------------------------------------------------------------------
// StreamOut()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::StreamOut( PIMoaStream2 pStream )
{
	return kMoaMmErr_AccessNotSupported;
}
  
// ----------------------------------------------------------------------------
// StreamIn()
// ----------------------------------------------------------------------------
MoaError ArrayVal_IMoaMmXValue::StreamIn( PIMoaStream2 pStream )
{
	return kMoaMmErr_AccessNotSupported;
}

// ----------------------------------------------------------------------------
// Double()
// ----------------------------------------------------------------------------
void ArrayVal_IMoaMmXValue::Double(  )
{
	PMoaLong p = (PMoaLong)pObj->fDataBlock;
	MoaDouble * f = (MoaDouble *)pObj->fDataBlock;
	for (int ix = 0; ix<pObj->fiSize; ix++, p++, f++)
	{
		switch (pObj->fiType)
		{
		 case kTypeInteger:
		 	*p *= 2;
		 	break;

		 case kTypeFloat:
		 	*f *= 2.0;
		 	break;
		}
	}
}

