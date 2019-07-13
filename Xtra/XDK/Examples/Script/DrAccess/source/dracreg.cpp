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
 *	Name: dracreg.cpp
 *	
 * 	Purpose: Definitions of Register class interface(s) and methods for the 
 *           DrAccess Xtra.
 *	
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#ifndef INITGUID        /* INITGUID causes the actual GUIDs to be declared, */ 
	#define INITGUID      /* not just references to them. Must be declared in */
#endif                  /* only one file. */          

#include "DrAcReg.h"

#include <string.h> 

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
BEGIN_DEFINE_CLASS_INTERFACE(CDrAccessRegister, IMoaRegister) 
END_DEFINE_CLASS_INTERFACE


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
 * The enumerated list in beeplngo.H must correspond directly with the msgTable 
 * (i.e. they must be in the same order).
 * 
 *******************************************************************************/ 
 
 /* MODIFY:  */

const static char msgTable1[] = { 
	"xtra DrAccess -- this Xtra provides access to various movie components\n" \
	"new object me\n"

	"-- MISC --\n"
	"callAFunction object me, symbol otherHandler, * -- call another function handler\n"
	"callACommand  object me, symbol otherHandler, * -- call another command (no result) handler\n"
	"moaFileWrite  object me, any data -- write to file \" MYFILE_NAME \" using IMoaFile, returns current contents\n"
	"sendSpriteMsg object me, any sprite, symbol message, * -- send message to sprite\n"
	"winGetParent  object me -- Returns app instance and main window (only useful on Win)\n"
	"getStageRect  object me -- Returns stage's rect in screen coordinates\n"
	"downloadFile  object me -- Begins downloading other file\n"
	"isStreamDone  object me -- Determines whether file has completed download or not\n"

	"-- PLAYER --\n"
	"resolveFName object me, string fileName -- resolve filename\n"
	"typeCount object me -- get # cmtypes\n"
	"typeName object me, string typeSymbol -- get displayname of cmtype\n"
	"* dumpTypes -- dump list of cmtypes\n"
	"* dumpAppInfo -- dump app info\n"
	"playerProp object me, string prop -- Get player prop\n"
	"setPlayerProp object me, string prop, any value -- Set player prop\n"
	"rgbToIndex object me, list rgb\n"
	"indexToRgb object me, integer index\n"
	"goMovie object me, string movie -- Go to a movie\n"
	};

const static char msgTable2[] = {
	"-- MOVIE --\n"
	"castCount object me -- get # casts in active movie\n"
	"movieProp object me, string prop -- Get movie prop\n"
	"setMovieProp object me, string prop, any value -- Set movie prop\n"
	"movieToLoop object me, integer memIndex -- New filmloop from movie score\n"
	"loopToMovie object me, integer memIndex -- Set movie score from filmloop\n"
	"makeLoop object me, integer destMemInd, integer runMemInd, integer chanInd, integer nFrames -- make filmloop\n"
	"labelFrame object me, string labelName -- frame# from label\n"
	"labelName object me, integer frameNum -- label from frame#\n"
	"setLabel object me, integer frmNum, any lblstr\n"
	"addXcast object me, string name, string path\n"

	"-- CAST --\n"
	"castProp object me, any cast, string propName -- Get cast prop\n"
	"setCastProp object me, any cast, string propName, any value -- Set cast prop\n"
	"indexOfCast object me, any cast -- Get cast ind in active movie\n"
	"usedMemList object me, any cast -- list used indexes in cast\n"
	"memNameToInd object me, any cast, string memName -- get mem ind from name\n"
	"createMem object me, any cast, integer memInd, string typeSymbol -- create mem\n"
	"deleteMem object me, any cast, any mem -- delete mem from cast\n"
	"dupeMem object me, any srcCast, any srcMem, any dstCast, any dstMem -- dupe mem\n"
	"getCastSel object me, any cast -- Get cast selection as a list of cast members\n"

	"-- CASTMEM --\n"
	"castMemProp object me, any castmem, string prop -- Get castmem prop (cast#1)\n"
	"setCastMemProp object me, any castmem, string prop, any value -- Set mem prop (cast#1)\n"
	"xferMedia object me, any srcCMem, any dstCMem, string label, string format -- xfer media (cast#1)\n"
	"saveMedia object me, any srcCMem, string pathname -- Save media as composite to pathname\n"
	"readMedia object me, any dstCMem, string pathname -- Read composite media from pathname into a #bitmap\n"

	"-- SCORE SPRITE --\n"
	"scoreSpriteProp object me, int chanInd, string prop -- Get score sprite prop\n"
	"setScoreSpriteProp object me, int chanInd, string prop, any value -- Set score sprite prop\n"
};

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

/* ---------------------------------------------------- MoaCreate_CDrAccessRegister */
STDMETHODIMP MoaCreate_CDrAccessRegister(CDrAccessRegister * This)
{
	MoaError		err = kMoaErr_NoErr;
	return(err);
}

/* --------------------------------------------------- MoaDestroy_CDrAccessRegister */
STDMETHODIMP_(void) MoaDestroy_CDrAccessRegister(CDrAccessRegister * This)
{
	return;
}

/* ---------------------------------- CDrAccessRegister_IMoaRegister Create/Destroy */
CDrAccessRegister_IMoaRegister::CDrAccessRegister_IMoaRegister(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CDrAccessRegister_IMoaRegister::~CDrAccessRegister_IMoaRegister() {}


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
 *****************************************************************************/ 

/* ------------------------------------------- CDrAccessRegister_IMoaRegister::Register */
STDMETHODIMP_(MoaError) CDrAccessRegister_IMoaRegister::Register(PIMoaCache pCache, 
	PIMoaDict pXtraDict)
{

	/* variable declaration */
	MoaError 		err = kMoaErr_NoErr;
	PMoaVoid		pMemStr = NULL;
	PIMoaDict 		pRegDict = NULL;
#ifdef USING_LOAD_CONTROL
	MoaLong			loadOptions = (kMoaXlc_PreloadAtLaunch | kMoaXlc_NeverUnload);
#endif

	// This Xtra will only register in Director 6 or later
	PIMoaAppInfo	pAppInfo;
	MoaChar			buf[64];

	if (err = pObj->pCallback->QueryInterface(&IID_IMoaAppInfo, (PPMoaVoid)&pAppInfo))
		goto exit_gracefully;

	if (err = pAppInfo->GetInfo(kMoaAppInfo_ProductName, buf, 64))
		goto exit_gracefully;

	if (strcmp(buf, "Director")) 
	{
		err = kMoaErr_BadInterface;
		goto exit_gracefully;
	}
	if (err = pAppInfo->GetInfo(kMoaAppInfo_ProductVersion, buf, 64))
		goto exit_gracefully;

	if (*buf < '6' && (buf[1]==0 || buf[1]=='.'))	// check string length so we don't get fooled by version 10 or higher
	{
		err = kMoaErr_InternalError;
		goto exit_gracefully;
	}

	// End check for Director 6 or earlier

#ifdef USING_LOAD_CONTROL
	err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CDrAccessScript, &IID_IMoaXtraLoadControl, &pRegDict);
	err = pRegDict->Put(kMoaRegType_XtraLoadControlOptions, &loadOptions, 0, kMoaRegKey_XtraLoadControlOptions);
#endif

	/* Register the scripting xtra */
	if (err = pCache->AddRegistryEntry(pXtraDict, &CLSID_CDrAccessScript, &IID_IMoaMmXScript, &pRegDict))
		goto exit_gracefully;
			
	/* Register the method table */
	/* msgTable is too long, have to concatenate it */
	if (!(pMemStr = pObj->pCalloc->NRAlloc(sizeof(msgTable1) + sizeof(msgTable2)))) 
	{
		MOA_DEBUGSTR("calloc failed!\n");
		err = kMoaErr_OutOfMem;
		goto exit_gracefully;
	}

	strcpy((char *)pMemStr, msgTable1);
	strcat((char *)pMemStr, msgTable2);

	err = pRegDict->Put(kMoaDrDictType_MessageTable, pMemStr, 0, kMoaDrDictKey_MessageTable);

exit_gracefully:

	if(pMemStr)
	{
		pObj->pCalloc->NRFree(pMemStr);
		pMemStr = NULL;
	}

	if (pAppInfo) 
	{
		pAppInfo->Release();
		pAppInfo = NULL;
	}

	return(err);
}

