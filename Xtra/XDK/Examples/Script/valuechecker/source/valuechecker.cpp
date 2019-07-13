/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

////////////////////////////////////////////////////////////////////////////////

#define INITGUID 1

#include "valuechecker.h"

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
#include "mmiclr.h"


/*******************************************************************************
 *
 * LINGO XTRA MESSAGE TABLE DESCRIPTION.
 *
 * The general format is:
 * xtra <nameOfXtra>
 * new object me [ args ... ]
 * <otherHandlerDefintions>
 * --
 * The first line must give the name of the Lingo xtra.
 * The remaining lines give the names of the handlers that this xtra implements
 * along with the required types of the arguments for each handler.
 * 
 * -- Pre-defined handler new 
 * The new handler will be called when a child object is created, 
 * the first argument is always the child object and you defined any remaining arguments.
 * The new handler is the place to initialize any memory required by the child object.
 * 
 * -- Simple Handler Definitions
 * Each handler definition line is format like this:
 * <handlerName> <argType1> <argName1>, <argType2> <argName2> ...
 * The first word is the handler name. Following this are types description for
 * the argument to the handler, each separated by a comma.
 * The argument name <argName>, may be omited.
 * Permited argument types are: 
 * 	integer 
 * 	float
 * 	string
 * 	symbol
 * 	object
 * 	any
 * 	*
 * For integer, float, string, symbol, and object, the type of the argument must 
 * match. The type any means allow any type. The asterisk (*) means any number and 
 * any type of arguments.
 * 
 * The first argument is the child object and is always declared to be of type object.
 * 
 * -- Global Handlers
 * An asterisk (*) preceeding the handler name signifies a global handler.
 * This handler is at the global scope level and can be called from any
 * movie.
 * 
 * -- Xtra level handlers
 * A plus (+) preceeding the handler name signifies an Xtra level handler. 
 * This kind of handler can be called directly from the Xtra reference,
 * without creating a child object.
 * 
 * The enumerated list in lgotmimp.H must correspond directly with the msgTable 
 * (i.e. they must be in the same order).
 * 
 *******************************************************************************/ 
 
static char versionInfo[] = "xtra ValueChecker -- version %s.%s.%s.r%s\n";
static char msgTable[] = { 
	"new object me\n" 
 	
	"-- ValueChecker Xtra --\n"
	"+testValue object me, integer aNum, symbol aSymbol, string aString, \
float aFloat, list aList, point aPoint, rect aRect, propList aPropList, \
color aColor, date aDate --> integer failNum\n"
	"+testValue3D object me, object vector, object vector, object transform --> integer failNum\n"
	"+testValue3DTransform object me --> integer failNum\n"
	};


/* 	This is the enumerated method list. This list should directly correspond
 *   to the msgTable. It is used to dispatch method calls via
 *   the methodSelector. The 'm_XXXX' method must be last.
 */

enum 
{
m_new = 0,						/* standard */
m_testValue,
m_testValue3D,
m_testValue3DTransform,
m_XXXX
};


/* ============================================================================= */
/* Xtra Glue Stuff */
/* ============================================================================= */

BEGIN_XTRA
	BEGIN_XTRA_DEFINES_CLASS(TStdXtra, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(TStdXtra, IMoaRegister, 1)
		CLASS_DEFINES_INTERFACE(TStdXtra, IMoaMmXScript, 1)
	END_XTRA_DEFINES_CLASS
END_XTRA


/* ============================================================================= */
/* Create/Destroy for class TStdXtra */
/* ============================================================================= */

//STD_CLASS_CREATE_DESTROY(TStdXtra)


STDMETHODIMP_(MoaError) MoaCreate_TStdXtra (TStdXtra * This)
{
moa_try
		
	ThrowErr (This->pCallback->QueryInterface(&IID_IMoaHandle, (PPMoaVoid) &This->pMoaHandle));
	ThrowErr (This->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid) &This->pValueInterface));
	ThrowErr (This->pCallback->QueryInterface(&IID_IMoaMmList, (PPMoaVoid) &This->pListInterface));
	/* running in D8, the vector interface isn't available */
	if (This->pCallback->QueryInterface(&IID_IMoa3dVectorValueUtils, (PPMoaVoid) &This->pVectorInterface) != kMoaErr_NoErr)
		This->pVectorInterface = NULL;
	
moa_catch
moa_catch_end
moa_try_end
}

STDMETHODIMP_(void) MoaDestroy_TStdXtra(TStdXtra * This)
{
moa_try

	if (This->pMoaHandle != NULL) 
		ThrowErr (This->pMoaHandle->Release());

	if (This->pValueInterface != NULL) 
		ThrowErr (This->pValueInterface->Release());
		
	if (This->pListInterface != NULL) 
		ThrowErr (This->pListInterface->Release());

	if (This->pVectorInterface != NULL) 
		ThrowErr (This->pVectorInterface->Release());

moa_catch
moa_catch_end
moa_try_end_void
}


/* ============================================================================= */
/* Methods in TStdXtra_IMoaRegister */
/* ============================================================================= */

STD_INTERFACE_CREATE_DESTROY(TStdXtra, IMoaRegister)
BEGIN_DEFINE_CLASS_INTERFACE(TStdXtra, IMoaRegister)
END_DEFINE_CLASS_INTERFACE

/* ----------------------------------------------------------------------------- */
STDMETHODIMP TStdXtra_IMoaRegister::Register(
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
	ThrowErr(pCache->AddRegistryEntry(
			pXtraDict, 
			&CLSID_TStdXtra, 
			&IID_IMoaMmXScript, 
			&pReg));

	/* Register the method table */
	sprintf(versionStr, versionInfo, DVER_MAJORVERSION_STRING, DVER_MINORVERSION_STRING, DVER_BUGFIXVERSION_STRING, DVER_BUILDNUM_STRING);
	pMemStr = pObj->pCalloc->NRAlloc(sizeof(msgTable) + sizeof(versionStr));
	ThrowNull(pMemStr);

	strcpy((char *)pMemStr, versionStr);
	strcat((char *)pMemStr, msgTable);

	ThrowErr(pReg->Put(kMoaDrDictType_MessageTable, pMemStr, 0, kMoaDrDictKey_MessageTable));

	/* Mark xtra as safe for shockwave */
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
TStdXtra_IMoaMmXScript::TStdXtra_IMoaMmXScript(MoaError *)
//------------------------------------------------------------------------------
{
moa_try
moa_catch
moa_catch_end
moa_try_end_void
}	

//******************************************************************************
TStdXtra_IMoaMmXScript::~TStdXtra_IMoaMmXScript()
//------------------------------------------------------------------------------
{
moa_try
moa_catch
moa_catch_end
moa_try_end_void
}


/* ----------------------------------------------------------------------------- */
STDMETHODIMP TStdXtra_IMoaMmXScript::Call (PMoaDrCallInfo callPtr)
{
moa_try
	switch	( callPtr->methodSelector ) 
	{
		case m_new:
			{
			/* Setup any instance vars for you Xtra here. new() is
			called via Lingo when creating a new instance. */
			}
			break;
					
		/***********************************
		*									
		* TStdXtra Method dispatch
		*
		***********************************/

		/* Here is where new methods are added to the switch statement. Each
		   method should also be listed in xxxIMP.H as well as defined in 
		   the msgTable defined in lgotmimp.C. 
		*/  

		case m_testValue:
			ThrowErr( TestValue(callPtr) );
			break;

		case m_testValue3D:
			ThrowErr( TestValue3D(callPtr) );
			break;
			
		case m_testValue3DTransform:
			ThrowErr (TestValue3DTransform(callPtr) );
			break;
	}
moa_catch
moa_catch_end
moa_try_end
}



#define FAIL_EXIT(x) do { failAt = x; goto failExit; } while(0)


/*******************************************************************************
 *******************************************************************************
 **
 ** TStdXtra Main Methods  
 **
 *******************************************************************************
 *******************************************************************************/

 /*
 *  This is the actual code for the defined methods. They are defined as 
 *  functions, and called from the switch statement in Call().
 */ 

//******************************************************************************
MoaError	TStdXtra_IMoaMmXScript::TestValue (PMoaMmCallInfo pCall)
//------------------------------------------------------------------------------
{
	MoaError		err;
	int				failAt = 0;
	MoaMmValue		argValue;
	MoaMmSymbol		aSymbol;
	MoaChar			buf[128];
	MoaMmValueType	valueType;
	int				index;
	MoaMmValueType	valueTypes[] = { 
		kMoaMmValueType_Integer,
		kMoaMmValueType_Symbol,
		kMoaMmValueType_String,
		kMoaMmValueType_Float,
		kMoaMmValueType_List,
		kMoaMmValueType_Point,
		kMoaMmValueType_Rect,
		kMoaMmValueType_PropList,
		kMoaMmValueType_Color,
		kMoaMmValueType_Date };
	char		myString[] = "someString2";		/* MSVC What's this here */
	
moa_try
	
	err = pObj->pValueInterface->StringToSymbol( "testSymbol", &aSymbol );
	if (aSymbol <= 0) FAIL_EXIT( 1 );
	err = pObj->pValueInterface->SymbolToString( aSymbol, buf, sizeof(buf) );
	if (strcmp(buf, "testSymbol") != 0) FAIL_EXIT( 2 );
	
	/*   err = testValue(x,1959,#testSymbol,"abc",123.0,[1,2,3],
	/ point(1,2),rect(1,2,3,4),[a:1,b:2])
	*/
	for (index = 0; index < 10; index++)
	{
		pciGetArgByIndex( pCall, index+2, &argValue );
	
		err = pObj->pValueInterface->ValueType( &argValue, &valueType );
		if (valueType != valueTypes[index])
			FAIL_EXIT( index + 3 );
			
		err = pObj->pValueInterface->ValueAddRef( &argValue);
		err = pObj->pValueInterface->ValueRelease( &argValue);
	}
	
	pciGetArgByIndex( pCall, 2, &argValue );
	long 	num;
	err = pObj->pValueInterface->ValueToInteger( &argValue, &num);
	if (err || num != 1959) FAIL_EXIT( 14 );
		
	pciGetArgByIndex( pCall, 3, &argValue );
	MoaMmSymbol	mySym;
	err = pObj->pValueInterface->ValueToSymbol( &argValue, &mySym);
	if (err || mySym != aSymbol) FAIL_EXIT( 15 );

	pciGetArgByIndex( pCall, 5, &argValue );
	MoaDouble	fnum;
	err = pObj->pValueInterface->ValueToFloat( &argValue, &fnum);
	if (err || fnum != 123.0) FAIL_EXIT( 16 );


	pciGetArgByIndex( pCall, 4, &argValue );
	err = pObj->pValueInterface->ValueStringLength( &argValue, &num);
	if (err || num != 3) FAIL_EXIT( 17 );

	pciGetArgByIndex( pCall, 4, &argValue );
	err = pObj->pValueInterface->ValueToString( &argValue, buf, sizeof(buf));
	if (err || strcmp(buf,"abc") != 0) FAIL_EXIT( 18 );

	pciGetArgByIndex( pCall, 4, &argValue );
	PMoaChar	pString;
	/* !!@ 20mar2000 JT: Const declaration bug in Value Interface */
	err = pObj->pValueInterface->ValueToStringPtr( &argValue, (ConstPMoaChar*)&pString);
	if (err || strcmp(pString,"abc") != 0) FAIL_EXIT( 19 );

	err = pObj->pValueInterface->ValueReleaseStringPtr( &argValue );
	if (err ) FAIL_EXIT( 20 );
		
	pciGetArgByIndex( pCall, 7, &argValue );
	MoaPoint		myPoint;
	err = pObj->pValueInterface->ValueToPoint( &argValue, &myPoint);
	if (err ||  myPoint.x != 1 || myPoint.y != 2) FAIL_EXIT( 21 );

	pciGetArgByIndex( pCall, 8, &argValue );
	MoaRect		myRect;
	err = pObj->pValueInterface->ValueToRect( &argValue, &myRect);
	if (err ||  myRect.left != 1 || myRect.top != 2 || myRect.right != 3 || myRect.bottom != 4) FAIL_EXIT( 22 );

	/* Create linear list */
	MoaMmValue		myList;
	err = pObj->pListInterface->NewListValue( &myList );
	if (err) FAIL_EXIT( 24 );

	/* Add an INTEGER to the list */
	MoaMmValue		tempValue;
	err = pObj->pValueInterface->IntegerToValue( 2001, &tempValue);
	if (err) FAIL_EXIT( 25);
	
	err = pObj->pListInterface->AppendValueToList( &myList, &tempValue);
	if (err) FAIL_EXIT(26);

	err = pObj->pValueInterface->ValueRelease( &tempValue);
	if (err) FAIL_EXIT(27);

	/* Add a SYMBOL to the list */
	err = pObj->pValueInterface->SymbolToValue( aSymbol, &tempValue);
	if (err) FAIL_EXIT( 28);
	
	err = pObj->pListInterface->AppendValueToList( &myList, &tempValue);
	if (err) FAIL_EXIT(29);

	err = pObj->pValueInterface->ValueRelease( &tempValue);
	if (err) FAIL_EXIT(30);


	/* Add a FLOAT to the list */
	err = pObj->pValueInterface->FloatToValue( 123456789.0, &tempValue);
	if (err) FAIL_EXIT(31);
	
	err = pObj->pListInterface->AppendValueToList( &myList, &tempValue);
	if (err) FAIL_EXIT(32);

	err = pObj->pValueInterface->ValueRelease( &tempValue);
	if (err) FAIL_EXIT(33);


	/* Add a STRING to the list */
	err = pObj->pValueInterface->StringToValue( "someString1", &tempValue);
	if (err) FAIL_EXIT(34);
	
	err = pObj->pListInterface->AppendValueToList( &myList, &tempValue);
	if (err) FAIL_EXIT(35);

	err = pObj->pValueInterface->ValueRelease( &tempValue);
	if (err) FAIL_EXIT(36);


	/* Add a STRING created from a HANDLE to the list */
	MoaHandle	myHandle;
	PMoaVoid	myStringPtr;
	
	myHandle = pObj->pMoaHandle->ZeroAlloc( sizeof(myString) );
	if (! myHandle) FAIL_EXIT(37);
		
	myStringPtr = pObj->pMoaHandle->Lock( myHandle );
	if (! myStringPtr) FAIL_EXIT(38);
	
	strcpy( (char *)myStringPtr, &myString[0]);
	
	pObj->pMoaHandle->Unlock( myHandle );

	err = pObj->pValueInterface->StringHandleToValue( myHandle, &tempValue);
	if (err) FAIL_EXIT(39);
	myHandle = 0;
	
	err = pObj->pListInterface->AppendValueToList( &myList, &tempValue);
	if (err) FAIL_EXIT(40);

	err = pObj->pValueInterface->ValueRelease( &tempValue);
	if (err) FAIL_EXIT(41);


	/* Add a POINT to the list */
	err = pObj->pValueInterface->PointToValue( &myPoint, &tempValue);
	if (err) FAIL_EXIT(42);
	
	err = pObj->pListInterface->AppendValueToList( &myList, &tempValue);
	if (err) FAIL_EXIT(43);

	err = pObj->pValueInterface->ValueRelease( &tempValue);
	if (err) FAIL_EXIT(44);

	/* Add a RECT to the list */
	err = pObj->pValueInterface->RectToValue( &myRect, &tempValue);
	if (err) FAIL_EXIT(45);
	
	err = pObj->pListInterface->AppendValueToList( &myList, &tempValue);
	if (err) FAIL_EXIT(46);

	err = pObj->pValueInterface->ValueRelease( &tempValue);
	if (err) FAIL_EXIT(47);

	/* Create a PROPERTY LIST */
	MoaMmValue		myPropList;
	err = pObj->pListInterface->NewPropListValue( &myPropList );
	if (err) FAIL_EXIT( 48 );

	/* Add an element to property list */
	err = pObj->pValueInterface->FloatToValue( 1.0e100, &tempValue);
	if (err) FAIL_EXIT(49);
		
	MoaMmValue		tempValue2;
	err = pObj->pValueInterface->StringToValue( "string3", &tempValue2);
	if (err) FAIL_EXIT(50);

	err = pObj->pListInterface->AppendValueToPropList( &myPropList , &tempValue, &tempValue2);
	if (err) FAIL_EXIT(51);

	err = pObj->pValueInterface->ValueRelease( &tempValue);
	if (err) FAIL_EXIT(52);

	err = pObj->pValueInterface->ValueRelease( &tempValue2);
	if (err) FAIL_EXIT(53);

	/* Add property list to list */
	err = pObj->pListInterface->AppendValueToList( &myList, &myPropList);
	if (err) FAIL_EXIT(54);

	err = pObj->pValueInterface->ValueRelease( &myPropList);
	if (err) FAIL_EXIT(55);

	/* Return with list as result.
	*/
	pCall->resultValue = myList;
	goto exit;


failExit:;
	pObj->pValueInterface->IntegerToValue(failAt, &pCall->resultValue);
	
exit:;

moa_catch
moa_catch_end	
moa_try_end	
}


//******************************************************************************
MoaError	TStdXtra_IMoaMmXScript::TestValue3D (PMoaMmCallInfo pCall)
//------------------------------------------------------------------------------
{
	MoaError		err;
	int				failAt = 0;
	MoaMmValue		argValue;
	MoaMmValueType	valueType;
	int				index;
	MoaMmValueType	valueTypes[] = { 
		kMoaMmValueType_3dVector,
		kMoaMmValueType_3dVector,
		kMoaMmValueType_3dTransform };
	Moa3dMatrix defaultMatrix = {1.0, 0.0, 0.0, 0.0,  0.0, 1.0, 0.0, 0.0,  0.0, 0.0, 1.0, 0.0,  0.0, 0.0, 0.0, 1.0};
	
moa_try
	
	if (pObj->pVectorInterface==NULL)
		FAIL_EXIT(1);
	/*   err = testValue3D(vector(0,0,0), vector(1,2,3), transform)
	     note that the transform() value is only available if the 3d Xtra is present.  The vector
	     value is built into lingo.
	*/
	for (index = 0; index < 2; index++)
	{
		pciGetArgByIndex( pCall, index+2, &argValue );
	
		err = pObj->pValueInterface->ValueType( &argValue, &valueType );
		if (err != 0)
			FAIL_EXIT( index + 3 );
		if (valueType != valueTypes[index])
			FAIL_EXIT( index + 7 );
			
		err = pObj->pValueInterface->ValueAddRef( &argValue);
		err = pObj->pValueInterface->ValueRelease( &argValue);
	}
	
	pciGetArgByIndex( pCall, 2, &argValue );
	long 	num;
	num = pObj->pVectorInterface->ValueIsAVector( &argValue);
	if ( num == 0) FAIL_EXIT( 14 );
	
	Moa3dVector aVector;
	err = pObj->pVectorInterface->ValueToMoa3dVector( &argValue, aVector );
	if (err != 0) FAIL_EXIT(15);
	if (aVector[0] != 0.0 || aVector[1] != 0.0 || aVector[2] != 0.0 || aVector[3] != 1.0)
		FAIL_EXIT(16);
		
	pciGetArgByIndex( pCall, 3, &argValue );
	num = pObj->pVectorInterface->ValueIsAVector( &argValue);
	if ( num == 0) FAIL_EXIT( 17 );
	Moa3dVector * pVector;
	err = pObj->pVectorInterface->ValueToMoa3dVectorPtr( &argValue, &pVector );
	if (err != 0) FAIL_EXIT(18);
	if ((*pVector)[0] != 1.0 || (*pVector)[1] != 2.0 || (*pVector)[2] != 3.0 || (*pVector)[3] != 1.0)
		FAIL_EXIT(19);
	err = pObj->pVectorInterface->ValueReleaseVectorPtr( &argValue, &pVector );
	if (err != 0) FAIL_EXIT(20);

	Moa3dMatrix aMatrix;
	pciGetArgByIndex( pCall, 4, &argValue );
	num = pObj->pVectorInterface->ValueIsATransform( &argValue);
	if ( num == 0) FAIL_EXIT( 21 );
	err = pObj->pVectorInterface->ValueToMoa3dMatrix( &argValue, aMatrix );
	if (err != 0) FAIL_EXIT(22);
	for (num=0; num<16; num++)
	{
		if (defaultMatrix[num] != aMatrix[num]) FAIL_EXIT(23+num);
	}
		
	MoaMmValue		myVectorValue;
	aVector[0] = (float)0.987; aVector[1] = (float)1.44; aVector[2] = (float)2.34; aVector[3] = (float)1.0;
	
	err = pObj->pVectorInterface->Moa3dVectorToValue( aVector, &myVectorValue );
	if (err) FAIL_EXIT(60);

	/* Return with vectorr as result.
	*/
	pCall->resultValue = myVectorValue;
	goto exit;


failExit:;
	pObj->pValueInterface->IntegerToValue(failAt, &pCall->resultValue);
	
exit:;

moa_catch
moa_catch_end	
moa_try_end	
}

//******************************************************************************
MoaError	TStdXtra_IMoaMmXScript::TestValue3DTransform (PMoaMmCallInfo pCall)
//------------------------------------------------------------------------------
{
	MoaError		err;
	int				failAt = 0;
	MoaMmValue		aValue;
	Moa3dMatrix testMatrix = {(float)0.99377,(float)0.08694,(float)-0.06976,0.00000, 
							  (float)-0.08340,(float)0.99515,(float)0.05221,0.00000, 
							  (float)0.07396,(float)-0.04607,(float)0.99620,0.00000, 
							  0.00000,0.00000,0.00000,1.00000};
	
moa_try
	
	if (pObj->pVectorInterface==NULL) FAIL_EXIT(1);
	
	// testMatrix = transform()
	// testMatrix = a.rotate(3, 4, 5)
	err = pObj->pVectorInterface->Moa3dMatrixToValue( testMatrix, &aValue );
	if (err) FAIL_EXIT(2);

	/* Return with vectorr as result.
	*/
	pCall->resultValue = aValue;
	goto exit;


failExit:;
	pObj->pValueInterface->IntegerToValue(failAt, &pCall->resultValue);
	
exit:;

moa_catch
moa_catch_end	
moa_try_end	
}

