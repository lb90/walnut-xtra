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

#include "script.h"

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
 * SCRIPTING XTRA MESSAGE TABLE DESCRIPTION.
 *
 * The general format is:
 * xtra <nameOfXtra>
 * new object me [ args ... ]
 * <otherHandlerDefintions>
 * --
 * The first line must give the name of the Scripting xtra.
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
 * The enumerated list that follows must correspond directly with the msgTable 
 * (i.e. they must be in the same order).
 * 
 *******************************************************************************/ 
 
 /* This is the list of handlers for the xtra. The versionInfo string is combined
 /*	with the msgTable string in the register method to create a single string that
 /* used when registering the xtra as a scripting xtra. */

static char versionInfo[] = "xtra Script -- version %s.%s.%s\n";
static char msgTable[] = { 
	"new object me\n"  /* standard first handler entry in all message tables */
 	"-- Template handlers --\n" 
	"* globalHandler -- prints global handler message\n"
	"+ parentHandler  object xtraRef -- prints parent handler message\n" 
	"childHandler object me, integer number -- prints child handler message, returns a number\n"
		/*
		 * ---> insert additional handler(s) MUST MATCH WITH ENUMS BELOW -->
		 */ 
	};


/* 	This is the enumerated scripting method list. This list should
 *	directly correspond to the msgTable defined above. It is used
 *	to dispatch method calls via the methodSelector. The 'm_XXXX' method must
 *	be last.
 */

enum 
{
	m_new = 0, /* standard first entry */
	m_globalHandler,						
	m_parentHandler,
	m_childHandler,
		/*
		 * ---> insert additional names(s) MUST MATCH MESSAGE TABLE ABOVE -->
		 */ 
	m_XXXX
};


/* ============================================================================= */
/* Xtra Glue Stuff */
/* ============================================================================= */

#define XTRA_VERSION_NUMBER XTRA_CLASS_VERSION

BEGIN_XTRA
	BEGIN_XTRA_DEFINES_CLASS(TStdXtra, XTRA_CLASS_VERSION)
		CLASS_DEFINES_INTERFACE(TStdXtra, IMoaRegister, XTRA_VERSION_NUMBER)
		CLASS_DEFINES_INTERFACE(TStdXtra, IMoaMmXScript, XTRA_VERSION_NUMBER)
		/*
		 * ---> insert additional interface(s) -->
		 */ 
	END_XTRA_DEFINES_CLASS
END_XTRA


/* ============================================================================= */
/* Create/Destroy for class TStdXtra */
/* ============================================================================= */


STDMETHODIMP_(MoaError) MoaCreate_TStdXtra (TStdXtra * This)
{
moa_try
		
	ThrowErr (This->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid) &This->pValueInterface));
	ThrowErr (This->pCallback->QueryInterface(&IID_IMoaMmUtils2, (PPMoaVoid) &This->pMoaUtils));
	
moa_catch
moa_catch_end
moa_try_end
}

STDMETHODIMP_(void) MoaDestroy_TStdXtra(TStdXtra * This)
{
moa_try

	if (This->pValueInterface != NULL) 
		ThrowErr (This->pValueInterface->Release());
		
	if (This->pMoaUtils != NULL) 
		ThrowErr (This->pMoaUtils->Release());

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
	ThrowErr(pCache->AddRegistryEntry(pXtraDict, &CLSID_TStdXtra, &IID_IMoaMmXScript, &pReg));

	/* Register the method table */
	sprintf(versionStr, versionInfo, VER_MAJORVERSION_STRING, VER_MINORVERSION_STRING, VER_BUGFIXVERSION_STRING);
	pMemStr = pObj->pCalloc->NRAlloc(sizeof(msgTable) + sizeof(versionStr));
	ThrowNull(pMemStr);

	strcpy((char *)pMemStr, versionStr);
	strcat((char *)pMemStr, msgTable);

	ThrowErr(pReg->Put(kMoaDrDictType_MessageTable, pMemStr, 0, kMoaDrDictKey_MessageTable));

	/* Mark xtra as safe for shockwave - but only if it IS safe ! */
	/*
	ThrowErr(pReg->Put(kMoaMmDictType_SafeForShockwave, &bItsSafe, sizeof(bItsSafe), kMoaMmDictKey_SafeForShockwave));
	*/

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
STDMETHODIMP TStdXtra_IMoaMmXScript::Call (PMoaDrCallInfo callPtr)
{
moa_try
	switch	( callPtr->methodSelector ) 
	{
		case m_new:
			{
			/* Setup any instance vars for you Xtra here. new() is
			called via Lingo when creating a new instance. */
			/*
			 * --> insert additional code -->
		 	 */
			}
			break;
					
		/* Here is where new methods are added to the switch statement. Each
		   method should be defined in the msgTable defined in and have a 
		   constant defined in the associated enum. 
		*/  

		case m_globalHandler:
			ThrowErr(XScrpGlobalHandler(callPtr));
			break;

		case m_parentHandler:
			ThrowErr(XScrpParentHandler(callPtr));
			break;

		case m_childHandler:
			ThrowErr(XScrpChildHandler(callPtr));
			break;

		/*
		 * --> insert additional methodSelector cases -->
		 */

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

/* ----------------------------------------------------- XScrpGlobalHandler */
MoaError TStdXtra_IMoaMmXScript::XScrpGlobalHandler(PMoaDrCallInfo pCall)
{
moa_try
	
	ThrowErr(pObj->pMoaUtils->PrintMessage("GlobalHandler called\n"));

	/*
	 *  --> insert additional code -->
	 */

moa_catch
moa_catch_end	
moa_try_end	
}

/* ----------------------------------------------------- XScrpParentHandler */
MoaError TStdXtra_IMoaMmXScript::XScrpParentHandler(PMoaDrCallInfo pCall)
{	
moa_try

	ThrowErr(pObj->pMoaUtils->PrintMessage("ParentHandler called\n"));
	/*
	 *  --> insert additional code -->
	 */

moa_catch
moa_catch_end	
moa_try_end	
}

/* ------------------------------------------------------ XScrpChildHandler */
MoaError TStdXtra_IMoaMmXScript::XScrpChildHandler(PMoaDrCallInfo pCall)
{
	MoaMmValue argValue;
	MoaLong num;
	
moa_try

	/* This shows how to access an argument
	/  the first argument in the list is the "me" value, so the user arguments
	/  start at the second position in the list */
	pciGetArgByIndex( pCall, 2, &argValue );
	
	ThrowErr (pObj->pValueInterface->ValueToInteger( &argValue, &num));
	ThrowErr (pObj->pMoaUtils->PrintMessage1("ChildHandler called with (%d)\n", num));

	/*
	 *  --> insert additional code -->
	 */

	/* if we want the lingo handler to return something do it like this
	*/
	ThrowErr(pObj->pValueInterface->IntegerToValue(17, &pCall->resultValue));

moa_catch
moa_catch_end	
moa_try_end	
}
