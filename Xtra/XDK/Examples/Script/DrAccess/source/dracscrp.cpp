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
 *	Name: DrAcScrp.cpp
 *	
 * 	Purpose: Definitions of scripting class interface(s) and methods for the 
 *           DrAccess Xtra project.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .cpp file should automatically include all the support files needed
 *	for this particular class. In addition, this file may include other .h
 *	files defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#include "DrAcScrp.h"

static	MoaMmValue	gTempArg;

extern "C" 
{
  void DrFixMediaHeader(const char * pMediaData);
}

#ifdef MACINTOSH
	#include <windows.h>
	#include <string.h>
#endif

/*****************************************************************************
 *  Private Methods
 *  -------------------
 *  Implementation of Private Class Methods
 ****************************************************************************/

/***** MISC *****/

MoaError CDrAccessScript_IMoaMmXScript::XX_CallAFunction(PMoaDrCallInfo callPtr)
/*( arg 1: handler symbol, arg 2: list)
Example code: put xtraInstance..callAfunction(#addNumbers, [1,2,3])
-- 6*/

{
	/* variable declarations */
	MoaError 				err = kMoaErr_NoErr;
	
	MoaMmValue			othersHandler;
	MoaMmSymbol			othersHandlerSym;

	/*
	 * Get the symbol for the other handler.
	 *
	 * Note that AccessArgByIndex() is not a method invocation
	 * macro, it's just a utility macro.  See drixlingo.h.
	 */
	AccessArgByIndex(2, &othersHandler);
	if ((err = pObj->pMmValue->ValueToSymbol(&othersHandler, &othersHandlerSym))
			!= kMoaErr_NoErr) 
		goto exit_gracefully;


	/* Invoke the other Xtra's handler */
	/* Note: If either Xtra doesn't handle errors properly, this Lingo
	 *       calling Lingo Xtra calling Lingo calling Lingo Xtra can
	 *       _seriously_ mess up the Lingo's interpreter.
	 */

	if ((err = pObj->pDrPlayer->CallHandler(othersHandlerSym,
												callPtr->nargs - 2, 
												&callPtr->pArgs[2],
												&callPtr->resultValue))
		!= kMoaErr_NoErr) 
		goto exit_gracefully;

exit_gracefully:		
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XX_CallACommand(PMoaDrCallInfo callPtr)
/*( arg 1: handler symbol)
Example code: xtraInstance.callACommand(#handlerName)*/
{
	/* variable declarations */
	MoaError 				err = kMoaErr_NoErr;
	
	MoaMmValue			othersHandler;
	MoaMmSymbol			othersHandlerSym;

	/*
	 * Get the symbol for the other handler.
	 *
	 * Note that AccessArgByIndex() is not a method invocation
	 * macro, it's just a utility macro.  See drixlingo.h.
	 */
	AccessArgByIndex(2, &othersHandler);
	if ((err = pObj->pMmValue->ValueToSymbol(&othersHandler, &othersHandlerSym)) != kMoaErr_NoErr) 
		goto exit_gracefully;


	/* Invoke the other Xtra's handler */
	/* Note: If either Xtra doesn't handle errors properly, this Lingo
	 *       calling Lingo Xtra calling Lingo calling Lingo Xtra can
	 *       _seriously_ mess up the Lingo's interpreter.
	 */

	if ((err = pObj->pDrPlayer->CallHandler(othersHandlerSym,
												callPtr->nargs - 2, 
												&callPtr->pArgs[2],
												NULL))
		!= kMoaErr_NoErr) 
		goto exit_gracefully;
	
exit_gracefully:	
	return (err);
}


MoaError CDrAccessScript_IMoaMmXScript::XX_MoaFileWrite(PMoaDrCallInfo callPtr)
/*( arg 1: any data)
Example code: xtraInstance.moaFileWrite("Please write this string to the file...")*/
	
{
	MoaError	err = kMoaErr_NoErr;
	MoaBool		myFileAlreadyExists;
	/* routine uses one buffer both to read file contents and convert
	 * data to be written argument to a string.
	 */
	MoaChar			myBuf[MY_BUF_SIZE];
	MoaStreamCount	bytesTransferred;
	MoaMmValue		value;
	
	// This section added since SetSpec behavior changed in Director 11 Macintosh and we
	// want the file to be placed next to the Director Application
	MoaMmSymbol		symbol;
	PIMoaPathName	pPathName = NULL;
	MoaChar			pathBuff[MOA_MAX_FILENAME];
	// In Director 11, we need to determine the full path to the file
	err = pObj->pCallback->MoaCreateInstance(&CLSID_CMoaPath, &IID_IMoaPathName, (PPMoaVoid)&pPathName);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->pMmValue->StringToSymbol("foldername", &symbol);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->pDrPlayer->GetProp(symbol, &value);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->pMmValue->ValueToString(&value, pathBuff, MOA_MAX_FILENAME);
	pObj->pMmValue->ValueRelease(&value);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	err = pPathName->InitFromString(pathBuff, kMoaPathDialect_LOCAL, FALSE, FALSE);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	err = pPathName->AddFinal(MYFILE_NAME);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	err = pPathName->GetPath(pathBuff, MOA_MAX_FILENAME);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	if ((err = pObj->pMoaFile->SetSpec(pathBuff)) != kMoaErr_NoErr) 
		goto exit_gracefully;
	// End on Director 11 change
	
	// Following is code that was used prior to Director 11
	//if ((err = pObj->pMoaFile->SetSpec(MYFILE_NAME)) != kMoaErr_NoErr) 
	//	goto exit_gracefully;

	if ((err = pObj->pMoaFile->GetStream(MY_BUF_SIZE, &pObj->pMoaStream)) != kMoaErr_NoErr) 
		goto exit_gracefully;

	myBuf[0] = 0;

	/* See if file already exists */
	if (pObj->pMoaFile->IsExisting()) 
	{
		myFileAlreadyExists = TRUE;
	} 
	else 
	{
		myFileAlreadyExists = FALSE;
		/* File doesn't exist, create one */
		if ((err = pObj->pMoaFile->CreateFile()) != kMoaErr_NoErr) 
			goto exit_gracefully;
	}

	/* Open stream.  Specify example doesn't seek. */
	if ((err = pObj->pMoaStream->Open(kMoaStreamOpenAccess_ReadWrite, kMoaStreamSetPositionType_None))
			!= kMoaErr_NoErr) 
		goto exit_gracefully;

	if (myFileAlreadyExists) 
	{
		/*
		 * File exists, read current contents.
		 * We're going to replace contents, so use ReadAhead()
		 *
		 * Note: my buffer is small, so should not exceed read-ahead
		 * limit.
		 *
		 * If there are only a few bytes in the file, the read could
		 * read past the end, which isn't an error here.
		 */
		if ((err = pObj->pMoaStream->ReadAhead(&myBuf, MY_BUF_SIZE-1, &bytesTransferred))
				== kMoaStreamErr_ReadPastEnd) err = kMoaErr_NoErr;
		if (err != kMoaErr_NoErr) 
			goto exit_gracefully;

				
		/* Make sure there's a trailing null */
		if (myBuf[bytesTransferred-1] != 0) myBuf[bytesTransferred] = 0;
		pObj->pMmValue->StringToValue(myBuf, &callPtr->resultValue);
	} 
	else 
	{
		pObj->pMmValue->StringToValue("new file", &callPtr->resultValue);
	}

	/* Write to the file */

	/* Get string to write from argument*/
	AccessArgByIndex(2, &value);

			/*
			 * IMPORTANT:  BUG ALERT:
			 * The size parameter you pass to ValueToString is the maximum
			 * string length you want, not including the trailing NULL.
			 * ValueToString will add a NULL to the end of the string,
			 * so the buffer you provide must be one byte larger than this
			 * parameter otherwise Director will overwrite the end of the 
			 * buffer and ruin everything.
			 *
			 * Despite the parameter's name "bufLen" in <mmiservc.h>, it 
			 * is _NOT_ the buffer length, it is one LESS than the buffer
			 * length.
			 */

	pObj->pMmValue->ValueToString(&value, myBuf, MY_BUF_SIZE - 1);

	err = pObj->pMoaStream->Write(myBuf, strlen(myBuf), &bytesTransferred);

exit_gracefully:
	if (pObj->pMoaStream) 
	{
		pObj->pMoaStream->Release();
		pObj->pMoaStream = NULL;
	}

	if (pPathName)
		pPathName->Release();

	return (err);
} /* XX_MoaFileWrite() */


MoaError CDrAccessScript_IMoaMmXScript::XX_SendSpriteMsg(PMoaDrCallInfo callPtr)
/* (arg 1: sprite number, arg 2: handler)
    Example call: xtraInstance.sendSpriteMsg(1, #doSomeThing)
    Note that this example does not pass back the standard "me" to the behavior. */
{
	MoaError	err = kMoaErr_NoErr;

	MoaMmValue		value;
	MoaBool			bHandled;
	MoaMmSymbol		handlerSymbol;
	
	MoaDrSpriteChanIndex	iChanIndex;

	/* get the 2nd argument, it's the sprite number.  */
	AccessArgByIndex(2, &value);

	/* convert the MoaMmValue into a sprite channel index */
	pObj->pMmValue->ValueToInteger(&value, &iChanIndex);

	/* get the 3rd argument, it's the message symbol */
	AccessArgByIndex(3, &value);

	/* convert the MoaMmValue into a symbol */
	pObj->pMmValue->ValueToSymbol(&value, &handlerSymbol);

	/*
	 * Get the the arguments to the message and put them in
	 * an args array.
	 *
	 * However, since we already have the arguments in the pArgs array
	 * passed in, we can just pass them on, starting with the third
	 * argument (first is object me, second is othersHandler symbol).
	 */

	/* ?? Have to correctly indicate whether a value is supposed to be
	 * returned or not so that Lingo can tell if it's invoking a 
	 * command or function.
	 */
	pObj->pDrMovie->SendSpriteMessage(iChanIndex, handlerSymbol, callPtr->nargs - 2, 
					&callPtr->pArgs[2], &value, &bHandled);

	return err;
} /* XX_SendSpriteMsg() */

MoaError CDrAccessScript_IMoaMmXScript::XX_WinGetParent(PMoaDrCallInfo callPtr)
/* Example call: put xtraInstance.winGetParent()
-- [536870912, 1508102] */

{
	MoaError	err = kMoaErr_NoErr;

#ifdef WINDOWS
	MoaMmHInst		hInst = NULL;
	MoaMmHWnd			hMainWnd = NULL;
	MoaMmValue		tempValue;

	if ((err = pObj->pMmWndWin->WinGetParent(&hInst, &hMainWnd)) != kMoaErr_NoErr) 
		 goto exit_gracefully;

	/* Make new list */
	pObj->pMmList->NewListValue(&callPtr->resultValue);
		
	/* Add hInst */
	pObj->pMmValue->IntegerToValue((MoaLong)hInst, &tempValue);
	pObj->pMmList->AppendValueToList(&callPtr->resultValue, &tempValue);
	pObj->pMmValue->ValueRelease(&tempValue);

	/* Add hMainWnd */
	pObj->pMmValue->IntegerToValue((MoaLong)hMainWnd, &tempValue);
	pObj->pMmList->AppendValueToList(&callPtr->resultValue, &tempValue);
	pObj->pMmValue->ValueRelease(&tempValue);

#endif

exit_gracefully:

	return err;
} /* XX_WinGetParent() */


MoaError CDrAccessScript_IMoaMmXScript::XX_GetStageRect(PMoaDrCallInfo callPtr)
/* Example call: put xtraInstance.getStageRect()
-- rect(382, 701, 702, 941) */
{
	MoaError	err = kMoaErr_NoErr;

	MoaRect						stageRect = {0};
	MoaMmNativeGCInfo nativeGCInfo;

	if ((err = pObj->pDrMovie->GetStageWindowGC(&pObj->pMmGC)) != kMoaErr_NoErr) 
		goto exit_gracefully;

	/* IMoaMmGc::GetContentRect seems always zero-relative, and absolute width and height are two pixels bigger than value from Windows GetWindowRect()
	 * To be screen-relative, use GetWindowRect() on Windows... */
	if ((err = pObj->pMmGC->GetNativeGCInfo(&nativeGCInfo)) != kMoaErr_NoErr) 
		goto exit_gracefully;


#ifdef WINVER

	/* On Windows, have to get the hWnd to go screen-relative */
	if (nativeGCInfo.type == kMoaMmGCType_WinWindow) 
	{
		RECT winStageRect;
		if (GetWindowRect((HWND)nativeGCInfo.data.win_window.hWND, &winStageRect))
			WinToMoaRect(&winStageRect, &stageRect);
	}

#else
	if (nativeGCInfo.type == kMoaMmGCType_MacWindow)
	{
#if (defined(__APPLE__) && defined(__MACH__))
		Rect macStageRect;
		GetWindowBounds((WindowRef)nativeGCInfo.data.mac_window.windowPtr, kWindowGlobalPortRgn, &macStageRect);
#else
		WindowPtr macWindow = (WindowPtr)nativeGCInfo.data.mac_window.windowPtr;
		Rect macStageRect = macWindow->portRect;
#endif
		MacToMoaRect(&macStageRect, &stageRect);
	}

#endif
			
	/* Convert rect to value in result */
	pObj->pMmValue->RectToValue(&stageRect,	&callPtr->resultValue);
	
exit_gracefully:

	if (pObj->pMmGC)
	{
		pObj->pMmGC->Release();
		pObj->pMmGC = NULL;
	}

	return err;
} /* XX_GetStageRect() */

MoaError CDrAccessScript_IMoaMmXScript::XX_DownloadFile(PMoaDrCallInfo callPtr)
/* Example call: xtraInstance.downloadFile() */
{
	MoaError	err = kMoaErr_NoErr;

	PMoaChar pUrlString = "http://www.macromedia.com";
	PIMoaPathName pPathName = NULL;
	PIMoaFile2 pMoaFile = NULL;

    // Need these interfaces
	pObj->pCallback->MoaCreateInstance(
		&CLSID_CMoaPath, &IID_IMoaPathName, ( void ** )&pPathName );
	pObj->pCallback->MoaCreateInstance(
		&CLSID_CNetFile, &IID_IMoaFile2, ( void ** )&pMoaFile );

	// IMoaPathName interface converts the URL C-string into a
    // structure we understand.
    pPathName->InitFromString(
        pUrlString, kMoaPathDialect_URL_STYLE, TRUE, FALSE );

    // IMoaFile2 interface knows it's dealing with a file, and can
    // give us a "stream" for the file.
    pMoaFile->SetPathName( pPathName );
    pMoaFile->GetStream( 0, &( pObj->pStream2 ) );

    // Now we can open the stream.  This initiates HTTP traffic
    // with the server.
    pObj->pStream2->Open(
        kMoaStreamOpenAccess_ReadOnly, kMoaStreamSetPositionType_None );

    // It may be a long time (many seconds) before anything comes
    // back from the server.  Return from this code and check
    // IsStreamDone() periodically, e.g. on idle or enterFrame messages.


	// release interfaces when done
	pMoaFile->Release();
	pPathName->Release();

	pMoaFile = NULL;
	pPathName = NULL;

	return err;

} /* XX_DownloadFile() */

MoaError CDrAccessScript_IMoaMmXScript::XX_IsStreamDone(PMoaDrCallInfo callPtr)
/* Example call: xtraInstance.isStreamDone() */
{
	MoaError	err = kMoaErr_NoErr;
	MoaStreamPosition endPos, length;

	// mark as false until we know it is true

	bool done = false;

	// find the end position (or length) of the download
	err = pObj->pStream2->GetEnd(&endPos);
	if (err == kMoaErr_NoErr) {
		// find out how much we have downloaded
		err = pObj->pStream2->GetCurrentLength(&length);
		// if no error and we have downloaded the entire file, we are done
		if ( ( err == kMoaErr_NoErr )&&( length >= endPos ) )
			done = true;
	}

	// if there is an error and the error is not data not available, we are done
	if (err && err != kMoaStreamErr_DataNotAvail) {
		done = true;
	}

	// send the return value back to director
	pObj->pMmValue->IntegerToValue(done, &callPtr->resultValue);

	return err;

} /* XX_IsStreamDone() */


/***** PLAYER *****/

MoaError CDrAccessScript_IMoaMmXScript::XPly_ResolveFileName(PMoaDrCallInfo callPtr)
	/*( arg 1: fileName)
	Example code: put xtraInstance.resolveFName("myFile.dir")
	-- "C:\myFiles.dir" */

{
	MoaError	err = kMoaErr_NoErr;
	MoaMmValue value;
	MoaChar	fileName[PROPBUF_SIZE];
	MoaChar	fullPath[PROPBUF_SIZE];
	
	/* Get filename */
	GetArgByIndex(2, &value);
	err = pObj->pMmValue->ValueToString(&value, fileName, PROPBUF_SIZE);
	
	/* Resolve it */
	if (err==kMoaErr_NoErr)
		err = pObj->pDrPlayer->ResolveFileName(fileName, fullPath, PROPBUF_SIZE );
	
	/* Convert back to value */
	if (err==kMoaErr_NoErr)
		err = pObj->pMmValue->StringToValue(fullPath, &callPtr->resultValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XPly_GetTypeCount(PMoaDrCallInfo callPtr)
	/*(No arguments)
	Example code: put xtraInstance.typeCount()
	-- 41 */

{
	MoaError	err = kMoaErr_NoErr;
	MoaLong	count;
		
	pObj->pDrPlayer->GetCastMemTypeCount( &count);
	pObj->pMmValue->IntegerToValue(count, &callPtr->resultValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XPly_GetTypeName(PMoaDrCallInfo callPtr)
	/*( arg 1: type string)
	Example code: put xtraInstance.typeName("flash")
	-- "Flash Movie..." */

{
	MoaError	err = kMoaErr_NoErr;
	
	MoaChar	typeName[MAX_STRING_SIZE];
	MoaChar typeSymStr[PROPNAME_SIZE];
	MoaMmSymbol typeSymbol;
	MoaMmValue value;

	GetArgByIndex(2, &value);	
	pObj->pMmValue->ValueToString( &value, typeSymStr, PROPNAME_SIZE);
	pObj->pMmValue->StringToSymbol(typeSymStr, &typeSymbol);
	pObj->pDrPlayer->GetCastMemTypeDisplayName( typeSymbol, typeName, MAX_STRING_SIZE);

	pObj->pMmValue->StringToValue(typeName, &callPtr->resultValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XPly_DumpTypes()
/* Example call: dumpTypes() */
{
	MoaError	err = kMoaErr_NoErr;

	MoaChar	typeName[MAX_STRING_SIZE];
	MoaChar typeSymStr[PROPNAME_SIZE];
	MoaMmSymbol typeSymbol;
	MoaLong	count, i;
	MoaMmSymbol sym; 
	MoaMmValue  val;
	
	pObj->pDrPlayer->GetCastMemTypeCount( &count);
	/* See "DR Developer's Guide" -> "DR Methods"*/

	for (i=1; i <= count; i++)
	{
		pObj->pDrPlayer->GetNthCastMemTypeSymbol( (MoaLong) i, &typeSymbol);
		pObj->pMmValue->SymbolToString(typeSymbol, typeSymStr, PROPNAME_SIZE);
		pObj->pDrPlayer->GetCastMemTypeDisplayName( typeSymbol, typeName, MAX_STRING_SIZE);
		
				/* Should display a list */
		pObj->pMmValue->StringToSymbol("put", &sym);

		pObj->pMmValue->StringToValue(typeSymStr, &val);
		pObj->pDrMovie->CallHandler(sym, 1, &val, 0 /* calling a proc; no result */ );
		pObj->pMmValue->ValueRelease(&val );

		pObj->pMmValue->StringToValue(typeName, &val);
		pObj->pDrMovie->CallHandler(sym, 1, &val, 0 /* calling a proc; no result */);
		pObj->pMmValue->ValueRelease(&val );

		pObj->pMmValue->StringToValue("-", &val);
		pObj->pDrMovie->CallHandler(sym, 1, &val, 0 /* calling a proc; no result */);
		pObj->pMmValue->ValueRelease(&val );
	}
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XPly_DumpAppInfo()
/* Example call: dumpAppInfo() */
{
	MoaError	err = kMoaErr_NoErr;

	MoaChar	buf[256];
	MoaLong	lastKey, i;
	MoaMmSymbol sym; 
	MoaMmValue  val;

	lastKey = kMoaAppInfo_ProductVersion;
	
	for (i=kMoaAppInfo_SerialNumber; i <= lastKey; i++)
	{
		pObj->pAppInfo->GetInfo(i, buf, MAX_STRING_SIZE );
			
#ifdef MACINTOSH
		if (i == kMoaAppInfo_AppFileSpec)
		{
			/* It's really an FSSpec, not a string, just grab the fileName part */
			FSSpec* pFSSpec = (FSSpec*) buf;
			pFSSpec->name[ pFSSpec->name[0]+1 ] = 0;	/* add null term */
			pObj->pMmValue->StringToValue((PMoaChar) &pFSSpec->name[1], &val);
		}
		else
#endif /* MACINTOSH */
		pObj->pMmValue->StringToValue(buf, &val);

		pObj->pMmValue->StringToSymbol("put", &sym);

		pObj->pDrMovie->CallHandler(sym, 1, &val, 0 /* calling a proc; no result */ );
		pObj->pMmValue->ValueRelease(&val );
	}
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XPly_GetPlayerProp(PMoaDrCallInfo callPtr)
{
	MoaError	err = kMoaErr_NoErr;

	MoaMmSymbol symbol;
	MoaChar		 propName[PROPNAME_SIZE];
	MoaMmValue	 moaValue;

	GetArgByIndex(2, &moaValue);
	pObj->pMmValue->ValueToString( &moaValue, propName, PROPNAME_SIZE);
	pObj->pMmValue->StringToSymbol(propName, &symbol);
		
	/* We have the symbol, get the movie property */
	err = pObj->pDrPlayer->GetProp( symbol, &moaValue);
	
	callPtr->resultValue = moaValue;
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XPly_SetPlayerProp(PMoaDrCallInfo callPtr)
{
	MoaError	err = kMoaErr_NoErr;

	MoaMmSymbol symbol;
	MoaChar		 propName[PROPNAME_SIZE];
	MoaMmValue	 moaValue;
	
	GetArgByIndex(2, &moaValue);
	pObj->pMmValue->ValueToString( &moaValue, propName, PROPNAME_SIZE);
	pObj->pMmValue->StringToSymbol(propName, &symbol);
		
	/* We have the symbol, set the player property w/arg 3 */
	GetArgByIndex(3, &moaValue);
	err = pObj->pDrPlayer->SetProp( symbol, &moaValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XPly_RGBToIndex(PMoaDrCallInfo callPtr)
	/*( arg 1: list of rgb values)
	Example code: put xtraInstance.rgbToIndex([255,123, 19])*/
{
	MoaError	err = kMoaErr_NoErr;

	MoaMmValue	 	listValue, componentValue;
	MoaLong		 	component;
	MoaMmRGBTriple 	rgbColor;
	MoaMmColorIndex	index;
	
	
	/* We assume the RGB value is stored as a list in arg 2 
		in the form [r, g, b]. 
	*/

	GetArgByIndex(2, &listValue);
	/* Get red */
	pObj->pMmList->GetValueByIndex(&listValue, 1, &componentValue);
	pObj->pMmValue->ValueToInteger(&componentValue, &component);
	rgbColor.red = (MoaByte) component;
	pObj->pMmValue->ValueRelease(&componentValue);
	/* Get green */
	pObj->pMmList->GetValueByIndex(&listValue, 2, &componentValue);
	pObj->pMmValue->ValueToInteger(&componentValue, &component);
	rgbColor.green = (MoaByte) component;
	pObj->pMmValue->ValueRelease(&componentValue);
	/* Get blue */
	pObj->pMmList->GetValueByIndex(&listValue, 3, &componentValue);
	pObj->pMmValue->ValueToInteger(&componentValue, &component);
	rgbColor.blue = (MoaByte) component;
	pObj->pMmValue->ValueRelease(&componentValue);
	
	/* Convert to index and return */
	pObj->pMmUtils2->RGBToIndex(&rgbColor, &index);
	pObj->pMmValue->IntegerToValue(index, &callPtr->resultValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XPly_IndexToRGB(PMoaDrCallInfo callPtr)
	/*( arg 1: palette index)
	put xtraInstance.indexToRgb(0)*/

{
	MoaError	err = kMoaErr_NoErr;

	MoaMmValue	 	moaValue, componentValue;
	MoaMmRGBTriple 	rgbColor;
	MoaLong			index;
	
	/* We return the RGB value as a list 
		in the form [r, g, b]. 
	*/

	/* Get index */
	GetArgByIndex(2, &moaValue);
	pObj->pMmValue->ValueToInteger(&moaValue, &index);
	
	/* Convert to RGB */
	pObj->pMmUtils2->IndexToRGB((MoaMmColorIndex) index, &rgbColor);
	
	/* Make new list */
	pObj->pMmList->NewListValue(&callPtr->resultValue );
	
	/* Add red */
	pObj->pMmValue->IntegerToValue(rgbColor.red, &componentValue);
	pObj->pMmList->AppendValueToList(&callPtr->resultValue, &componentValue);
	pObj->pMmValue->ValueRelease(&componentValue);

	/* Add green */
	pObj->pMmValue->IntegerToValue(rgbColor.green, &componentValue);
	pObj->pMmList->AppendValueToList(&callPtr->resultValue, &componentValue);
	pObj->pMmValue->ValueRelease(&componentValue);

	/* Add blue */
	pObj->pMmValue->IntegerToValue(rgbColor.blue, &componentValue);
	pObj->pMmList->AppendValueToList(&callPtr->resultValue, &componentValue);
	pObj->pMmValue->ValueRelease(&componentValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XPly_GoMovie(PMoaDrCallInfo callPtr)
	/*( arg 1: movie file)
	xtraInstance.goMovie("C:\myMovie.dir")*/
{
	MoaError	err = kMoaErr_NoErr;

	MoaMmValue	 	movieValue;
	MoaMmValue      callHandlerArgs[5];  /* array of arguments to CallHandler */
	MoaBool			callValueRelease = FALSE;	/* whether value needs to be released */
	MoaMmSymbol		gotoSym;
	int				len;
	MoaChar doCommandBuf[MAXDOCOMMAND]; /* long string for 'do' command */

	/* Try not to assume use of any interfaces or the Support macros, 
	 * do it all by hand
	 */

	/*
	 * Fabricate "go [to] movie <movieName>.  This _looks_ like 
	 * one symbol, 2 args.  But the movie is actually one argument.
	 * Not sure how to create this.  So use "do" with one argument.
	 */
	
	/* Turn "do" into a symbol */
	if ((err = pObj->pMmValue->StringToSymbol("do", &gotoSym)) != kMoaErr_NoErr)
		goto exit_gracefully;


#ifdef MOVIE_SYMBOL_IS_TWO_ARGS
	/* "movie" is a symbol that is part of the argument list */
	if ((err = pObj->pMmValue->StringToSymbol("movie", &movieSym)) != kMoaErr_NoErr) 
		goto exit_gracefully;


	if ((err = pObj->pMmValue->SymbolToValue(movieSym, &callHandlerArgs[0])) != kMoaErr_NoErr) 
		goto exit_gracefully;

#endif

	/* If we had the movie name as a string,
	 * we would have to convert it to a value here.
	 * but we have it as a value already
	 */

	/* Put ``go movie "'' into the do command's string buffer */
	strcpy(doCommandBuf, "go movie \"");

	/*
	 * Get the movie filename Lingo argument.
	 *
	 * Note that AccessArgByIndex() is not a method invocation
	 * macro, it's just a utility macro.  See drixlingo.h.
	 */
	AccessArgByIndex(2, &movieValue);

	/* Convert the movie name to a string in the do command's buffer.
	 * The length we use here for the string buffer is 1 smaller than
	 * the room available, since MmUtils does not count the trailing null.
	 */
	len = strlen(doCommandBuf);
	if ((err = pObj->pMmValue->ValueToString(&movieValue, &(doCommandBuf[len]), 
							MAXDOCOMMAND - len - 1))
			!= kMoaErr_NoErr) 
		goto exit_gracefully;
	
	/* We own the string value in the args, 
	 * so we'll need to release this string value later 
	 */
	callValueRelease = TRUE;

	/* Add a trailing ``"'' to the do command's buffer */
	len = strlen(doCommandBuf);
	if (len > (MAXDOCOMMAND-2)) 
		goto exit_gracefully;

	doCommandBuf[len] = '\"';
	doCommandBuf[len+1] = '\0';

	/* Now convert the do command's string buffer into a value */
	if ((err = pObj->pMmValue->StringToValue(doCommandBuf, &callHandlerArgs[0])) != kMoaErr_NoErr) 
		goto exit_gracefully;


	/* If we were doing "go frame MyLabel of movie MyMovie"
	 * we would fill other parts of the callHandlerArgs[] array
	 * with the StringToSymbol of "of" and the StringToValue of "label"
	 */

	/* Mimics the Lingo go "movie"  command */
	if ((err = pObj->pDrPlayer->CallHandler(gotoSym, 1, &callHandlerArgs[0], 0)) != kMoaErr_NoErr) 
		goto exit_gracefully;

			
	/*
	 * Release the string value here. 
	 * Only values we own need to be released.  This 
	 * does not include AccessArgByIndex() args passed in
	 */
exit_gracefully:
	if (callValueRelease)
		pObj->pMmValue->ValueRelease(&callHandlerArgs[0]);

	return err;
}



/***** MOVIE *****/

MoaError CDrAccessScript_IMoaMmXScript::XMov_GetCastCount(PMoaDrCallInfo callPtr)
	/* (No arguments) 
	Example call: put xtraInstance.castCount() */
{
	MoaError	err = kMoaErr_NoErr;

	MoaLong	count;
	
	pObj->pDrMovie->GetCastCount(&count);	
	pObj->pMmValue->IntegerToValue(count, &callPtr->resultValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XMov_GetMovieProp(PMoaDrCallInfo callPtr)
	/* (arg 1: "propName") 
	Example call: put movieProp(xtraInstance,"tempo") */

{
	MoaError	err = kMoaErr_NoErr;

	MoaMmSymbol  symbol;
	MoaChar		 propName[PROPNAME_SIZE];
	MoaMmValue	 moaValue;

	GetArgByIndex(2, &moaValue);
	pObj->pMmValue->ValueToString( &moaValue, propName, PROPNAME_SIZE);
	pObj->pMmValue->StringToSymbol(propName, &symbol);
		
	/* We have the symbol, get the movie property */
	err = pObj->pDrMovie->GetProp(symbol, &moaValue);
	
	callPtr->resultValue = moaValue;
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XMov_SetMovieProp(PMoaDrCallInfo callPtr)
	/* (arg 1: "propName", arg 2: value) 
	Example call: setMovieProp(xtraInstance, "tempo", 50) */

{
	MoaError	err = kMoaErr_NoErr;

	MoaMmSymbol symbol;
	MoaChar		 propName[PROPNAME_SIZE];
	MoaMmValue	 moaValue;
	
	GetArgByIndex(2, &moaValue);
	pObj->pMmValue->ValueToString( &moaValue, propName, PROPNAME_SIZE);
	pObj->pMmValue->StringToSymbol(propName, &symbol);
		
	/* We have the symbol, set the movie property w/arg 3 */
	GetArgByIndex(3, &moaValue);
	err = pObj->pDrMovie->SetProp(symbol, &moaValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XMov_MovieToFilmloop(PMoaDrCallInfo callPtr)
	/* (arg 1: castmem# for new filmloop) 
	Example call: movieToLoop(xtraInstance, 2)*/
{
	MoaError	err = kMoaErr_NoErr;

	MoaDrMemberIndex memberIndex;
	MoaDrMediaInfo 	mediaInfo;
	MoaMmSymbol		sym;
	MoaMmValue	 	moaValue;
	
	GetArgByIndex(2, &moaValue);
	
	/* Get copy of the score media for the movie */
	
	pObj->pMmValue->StringToSymbol("score", &mediaInfo.labelSymbol);
	pObj->pMmValue->StringToSymbol("moahandle", &mediaInfo.formatSymbol);
	pObj->pDrUtils->NewMediaInfo(mediaInfo.labelSymbol, mediaInfo.formatSymbol, NULL, kMoaDrMediaOpts_None, NULL, &mediaInfo);
	/*See the "Director Developer's Guide" -> "Director Methods" -> "IMoaDrCastMem" for information	regarding lables and formats"*/
	/*See the "Director Developer's Guide" -> "DRinterfaces" -> "IMoaDrUtils" for information regarding NewMediaInfo"*/

	pObj->pDrMovie->GetMedia(&mediaInfo);

	/* Make a new filmloop cast member in the specified slot 
	  (assume cast # 1) */
	
	pObj->pDrMovie->GetNthCast(1, &pObj->pDrCast);	
	pObj->pMmValue->ValueToInteger(&moaValue, &memberIndex);
	pObj->pMmValue->StringToSymbol("filmloop", &sym);
	pObj->pDrCast->CreateCastMem(memberIndex, sym);
	
	/* Attach the media of the filmloop to that we obtained from the movie
	   Using "attach" releases our hold on the media as well. */
	   
	pObj->pDrCast->GetCastMem(memberIndex, &pObj->pDrCastMem);
	pObj->pDrCastMem->AttachMedia(&mediaInfo);
	/*See the "Director Developer's Guide" -> "DRmethods" -> "for information regarding AttachMedia"*/
	/* Dispose of interfaces created here */
	
	pObj->pDrCastMem->Release();
	pObj->pDrCastMem = NULL;

	pObj->pDrCast->Release();
	pObj->pDrCast = NULL;

	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XMov_FilmloopToMovie(PMoaDrCallInfo callPtr)
{
	/* arg 1: cast mem # of filmloop 
	Example call: loopToMovie(xtraInstance, 6)*/
	
	MoaError	err = kMoaErr_NoErr;

	MoaDrMemberIndex memberIndex;
	MoaDrMediaInfo 	mediaInfo;
	
	/* Get copy of the score media of filmloop */

	/* Get filmloop cast member in the specified slot 
	   (assume cast # 1) */
				 
	pObj->pDrMovie->GetNthCast(1, &pObj->pDrCast);	
	GetArgByIndex(2, &gTempArg);
	pObj->pMmValue->ValueToInteger(&gTempArg, &memberIndex);				
	pObj->pDrCast->GetCastMem(memberIndex, &pObj->pDrCastMem);

	pObj->pMmValue->StringToSymbol("score", &mediaInfo.labelSymbol);
	pObj->pMmValue->StringToSymbol("moahandle", &mediaInfo.formatSymbol);

	pObj->pDrUtils->NewMediaInfo(mediaInfo.labelSymbol, mediaInfo.formatSymbol, NULL, kMoaDrMediaOpts_None, NULL, &mediaInfo);
	/*See the "Director Developer's Guide" -> "Director Methods" -> "IMoaDrCastMem" for information	regarding lables and formats"*/
	/*See the "Director Developer's Guide" -> "DRinterfaces" -> "IMoaDrUtils" for information regarding NewMediaInfo"*/

	pObj->pDrCastMem->GetMedia(&mediaInfo);
	
	/* Attach it to the movie.  This replaces the old movie score,
	   and releases the temporary mediaInfo we created here as well.
	*/
	
	pObj->pDrMovie->AttachMedia(&mediaInfo);
	/*See the "Director Developer's Guide" -> "DRmethods" -> "for information regarding AttachMedia"*/
	/* Dispose of interfaces created here */
	
	pObj->pDrCastMem->Release();
	pObj->pDrCastMem = NULL;

	pObj->pDrCast->Release();
	pObj->pDrCast = NULL;
	
	return err;
}

MoaError CDrAccessScript_IMoaMmXScript::XMov_MakeFilmloop(PMoaDrCallInfo callPtr)
{
	/*  castmem# for new filmloop, castmem# for sprite run, sprite #, #frames */

	MoaError	err = kMoaErr_NoErr;

	MoaDrMemberIndex memberIndex, spriteMemberIndex;
	MoaMmSymbol		sym, cmsym, locsym, fgsym, bgsym;
	MoaDrFrameIndex	frameIndex, frameCount;
	MoaDrSpriteChanIndex chanIndex;
	MoaDrCMRef		cmref;
	MoaPoint		loc;
	MoaMmValue		cmvalue, locvalue, bgvalue, fgvalue;
	
	/* Make a new filmloop cast member in the specified slot 
	  (assume cast # 1)
	*/
	
	pObj->pDrMovie->GetNthCast(1, &pObj->pDrCast);
	GetArgByIndex(2, &gTempArg);	
	pObj->pMmValue->ValueToInteger(&gTempArg, &memberIndex);
	pObj->pMmValue->StringToSymbol("filmloop", &sym);
	pObj->pDrCast->CreateCastMem(memberIndex, sym);
		
	/* Get a score access interface for the filmloop score */
	pObj->pDrCast->GetCastMem(memberIndex, &pObj->pDrCastMem);
	/* Dispose of our score if we already have one. */
	if (pObj->pDrScore)
		pObj->pDrScore->Release();
	pObj->pDrCastMem->GetScoreAccess(&pObj->pDrScore);
	
	/* We're now operating on a "copy" of the filmloop score
		data, held by pScore.  When we're done, we call 
		pScore->Commit() to save the changes back to its owner 
		(the filmloop).
		
		Note that we could use pScore->SetOwner() to change
		the owner at any time, so that Commit() could save it
		back to a different cast member, or a PIMoaMmMovie.
	*/
	
	/* Begin by clearing out the score */
	pObj->pDrScore->New();
	
	/* Get the frame count and sprite channel index from the arg list. */
	GetArgByIndex(5, &gTempArg);
	pObj->pMmValue->ValueToInteger(&gTempArg, &frameCount);
	GetArgByIndex(4, &gTempArg);
	pObj->pMmValue->ValueToInteger(&gTempArg, &chanIndex);
	
	/* Prepare symbols for props we want to access */
	pObj->pMmValue->StringToSymbol("member", &cmsym);
	pObj->pMmValue->StringToSymbol("loc", &locsym);
	pObj->pMmValue->StringToSymbol("forecolor", &fgsym);
	pObj->pMmValue->StringToSymbol("backcolor", &bgsym);
	
	/* Set up cast member reference value for the sprite run */
	GetArgByIndex(3, &gTempArg);
	pObj->pMmValue->ValueToInteger(&gTempArg, &spriteMemberIndex);
	CMRef_Make(&cmref, 1, spriteMemberIndex);
	pObj->pDrUtils->CMRefToValue(&cmref, &cmvalue);
	
	/* Set up location and color */
	loc.x = 200; loc.y = 200;
	pObj->pMmValue->IntegerToValue(0, &bgvalue);
	pObj->pMmValue->IntegerToValue(255, &fgvalue);

	/* Get a Sprite interface for the channel */
	err = pObj->pDrScore->GetSprite(chanIndex, &pObj->pDrSprite); 
	
	if (err == kMoaErr_NoErr)
	{
		/* Make some score */
		pObj->pDrScore->BeginUpdate();
		for (frameIndex = 1; frameIndex <= frameCount; frameIndex++)
		{
			pObj->pMmValue->PointToValue(&loc, &locvalue);
		
			pObj->pDrSprite->SetProp(cmsym, &cmvalue);
			pObj->pDrSprite->SetProp(locsym, &locvalue);
			pObj->pDrSprite->SetProp(fgsym, &fgvalue);
			pObj->pDrSprite->SetProp(bgsym, &bgvalue);
			
			/* The following commits the changes and moves us 
			   to the next frame */
			   
			pObj->pDrScore->UpdateFrame();
			
			/* move the sprite */
			pObj->pMmValue->ValueRelease(&locvalue);
			loc.x += 5; loc.y += 5;
		}
		pObj->pDrScore->EndUpdate();
		
		/* Release the sprite interface */
		pObj->pDrSprite->Release();
		pObj->pDrSprite = NULL;
	}
	/* Release values used above */
	pObj->pMmValue->ValueRelease(&cmvalue);
	pObj->pMmValue->ValueRelease(&fgvalue);
	pObj->pMmValue->ValueRelease(&bgvalue);
	
	/* Commit the changes in pScore to its owner, the filmloop.
		This relationship was made automatically when we called
		pCastMem->GetScoreAccess(), in the beginning.  
		
		Note that we can continue to make changes to the score
		after Commit(); simply call Commit() again to update
		the owner.  Calling Refresh() trashes pScore's current
		copy and refreshes it from the current score data
		from the owner.
	*/
	
	pObj->pDrScore->Commit();
		
	/* Dispose of interfaces created here */
	
	pObj->pDrCastMem->Release();
	pObj->pDrCastMem = NULL;

	pObj->pDrCast->Release();
	pObj->pDrCast = NULL;
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XMov_GetFrameIndexFromLabel(PMoaDrCallInfo callPtr)
{
	/* arg1: label name 
	Exampel code: put labelFrame(xtraInstance,"myLabel")*/

	MoaError	err = kMoaErr_NoErr;

	MoaChar			label[MAX_STRING_SIZE];
	MoaDrFrameIndex index;
	
	/* Get it. */
	GetArgByIndex(2, &gTempArg);
	pObj->pMmValue->ValueToString(&gTempArg, label, MAX_STRING_SIZE);
	pObj->pDrMovie->GetFrameIndexFromLabel(label, &index);
	
	/* Return it */
	pObj->pMmValue->IntegerToValue(index, &callPtr->resultValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XMov_GetFrameLabelFromIndex(PMoaDrCallInfo callPtr)
{
	/* arg1: frame # 
	Example code: put labelName(xtraInstance, 32)*/
	
	MoaError	err = kMoaErr_NoErr;

	MoaChar			label[MAX_STRING_SIZE];
	MoaDrFrameIndex index;
	
	/* Get it. */
	GetArgByIndex(2, &gTempArg);
	pObj->pMmValue->ValueToInteger(&gTempArg, &index);
	pObj->pDrMovie->GetFrameLabelFromIndex(index, label, MAX_STRING_SIZE);
	
	/* Return it */
	pObj->pMmValue->StringToValue(label, &callPtr->resultValue);
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XMov_SetFrameLabel(PMoaDrCallInfo callPtr)
{
	/* arg1: frame #, arg2: label 
	Example Code: setLabel(xtraInstance, 32, "newLabel")*/
	
	MoaError	err = kMoaErr_NoErr;

	MoaChar			label[MAX_STRING_SIZE];
	MoaDrFrameIndex index;
	
	/* Get frame #. */
	GetArgByIndex(2, &gTempArg);
	pObj->pMmValue->ValueToInteger(&gTempArg, &index);

	/* Get label string */
	GetArgByIndex(3, &gTempArg);
	if (pObj->pMmValue->ValueToString(&gTempArg, label, MAX_STRING_SIZE) == kMoaErr_NoErr)
		pObj->pDrMovie->SetFrameLabel(index, label);
		/* See "DR Developer's Guide" -> "Director Methods"*/
	else
		pObj->pDrMovie->SetFrameLabel(index, 0); /* delete label */
		
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XMov_AddExternalCast(PMoaDrCallInfo callPtr)
{
	/* arg1: "castName", arg2: "pathName": returns new movieCastIndex 
	This addes an existing external cast to the movie.
	Example call: addXcast(xtraInstance, "mynewcast","C:\myCast.cst")*/
	
	MoaError	err = kMoaErr_NoErr;

	MoaChar			castName[256];
	MoaChar			pathName[256];
	MoaDrCastIndex  mci;
	
	/* Get cast name */
	GetArgByIndex(2, &gTempArg);
	pObj->pMmValue->ValueToString(&gTempArg,castName, MAX_STRING_SIZE);

	/* Get path name */
	GetArgByIndex(3, &gTempArg);
	pObj->pMmValue->ValueToString(&gTempArg,pathName, MAX_STRING_SIZE);

	pObj->pDrMovie->AddExternalCast(castName, pathName, &mci);
	/*See "DR Developer's Guide" -> "Director Methods"*/

	pObj->pMmValue->IntegerToValue(mci, &callPtr->resultValue);	
	return err;
}



/***** CAST *****/

MoaError CDrAccessScript_IMoaMmXScript::XCst_GetCastProp(PMoaDrCallInfo callPtr)
{
	/* (cast# or "name", "propName") 
	Example call: put CastProp(xtraInstance, 1, "selection")*/
	
	MoaError	err = kMoaErr_NoErr;

	MoaMmSymbol  symbol;
	MoaChar		 propName[PROPNAME_SIZE];
	MoaMmValue	 moaValue;
	MoaLong		 castIndex;

	/* Determine if argument 1 (arg index 2) is a stringHdl or int */
	
	/* See if it's an int, get cast by # */
		
	GetArgByIndex(2, &gTempArg);

	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
		pObj->pDrMovie->GetNthCast(castIndex, &pObj->pDrCast);
	else
	{
		/* assume it's a string */
		MoaChar	castName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			pObj->pDrMovie->GetCastFromName(castName, &pObj->pDrCast);
	}
	
	if (err == kMoaErr_NoErr)
	{
		/* Get the symbol */
		GetArgByIndex(3, &gTempArg);
		pObj->pMmValue->ValueToString( &gTempArg, propName, PROPNAME_SIZE);
		pObj->pMmValue->StringToSymbol(propName, &symbol);
		
		/* We have a cast and symbol, so get the prop */
		err = pObj->pDrCast->GetProp(symbol, &moaValue);
	
		callPtr->resultValue = moaValue;
		
		pObj->pDrCast->Release();
		pObj->pDrCast = NULL;
	}
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCst_SetCastProp(PMoaDrCallInfo callPtr)
{
	/* (cast# or "name", "propName", value) 
	Example call: setCastProp(xtraInstance, 1, "selection", [[3,4], [9,9]])*/
	
	MoaError	err = kMoaErr_NoErr;

	MoaMmSymbol  symbol;
	MoaChar		 propName[PROPNAME_SIZE];
	MoaMmValue	 moaValue;
	MoaLong		 castIndex;

	/* Determine if arg 1 (index 2) is a stringHdl or int */
	
	/* See if it's an int, get cast by # */
		
	GetArgByIndex(2, &gTempArg);

	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
		pObj->pDrMovie->GetNthCast(castIndex, &pObj->pDrCast);
	else
	{
		/* assume it's a string */
		MoaChar	castName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			pObj->pDrMovie->GetCastFromName(castName, &pObj->pDrCast);
	}
	
	if (err == kMoaErr_NoErr)
	{
		/* Get the symbol */
		GetArgByIndex(3, &gTempArg);
		pObj->pMmValue->ValueToString( &gTempArg, propName, PROPNAME_SIZE);
		pObj->pMmValue->StringToSymbol(propName, &symbol);
		
		/* We have a cast and symbol selector, so set the prop */
		GetArgByIndex(4, &moaValue);
		err = pObj->pDrCast->SetProp(symbol, &moaValue);
		/* For cast properties see "DR Developer's Guide" -> "Director Types and Miscellaneous API"*/
		pObj->pDrCast->Release();
		pObj->pDrCast = NULL;
	}
		

	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCst_GetCastIndexInMovie(PMoaDrCallInfo callPtr)
{
	/* (cast# or "name") 
	Example code: put indexOfCast(xtraInstance, "myDigitalVideoCast")*/
	
	MoaError	err = kMoaErr_NoErr;

	MoaDrCastIndex resultCastIndex;
	MoaLong		 castIndex;

	/* Determine if prop 1 is a stringHdl or int */
	
	/* See if it's an int, get cast by # */

	GetArgByIndex(2, &gTempArg);
		
	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
		pObj->pDrMovie->GetNthCast(castIndex, &pObj->pDrCast);
	else
	{
		/* assume it's a string */
		MoaChar	castName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			pObj->pDrMovie->GetCastFromName(castName, &pObj->pDrCast);
	}
	
	if (err == kMoaErr_NoErr)
	{
		err = pObj->pDrCast->GetIndexInMovie(pObj->pDrMovie, &resultCastIndex);
	
		pObj->pMmValue->IntegerToValue(resultCastIndex, &callPtr->resultValue);
		pObj->pDrCast->Release();
		pObj->pDrCast = NULL;
	}


	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCst_GetUsedMemberList(PMoaDrCallInfo callPtr)
{
	/* (cast# or "name") 
	Example call: put usedMemList(xtraInstance, 1) */
	
	MoaError	err = kMoaErr_NoErr;

	MoaDrMemberIndex 	memberIndex;
	MoaMmValue			element;
	MoaLong				i;
	MoaMmValue			result;
	MoaLong		 		castIndex;

	/* Determine if prop 1 is a stringHdl or int */
	
	/* See if it's an int, get cast by # */
	
	GetArgByIndex(2, &gTempArg);
	
	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
		pObj->pDrMovie->GetNthCast(castIndex, &pObj->pDrCast);
	else
	{
		/* assume it's a string */
		MoaChar	castName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			pObj->pDrMovie->GetCastFromName(castName, &pObj->pDrCast);
	}
	
	if (err == kMoaErr_NoErr)
	{	
		/* Build a list of members */
		
		/* Make an empty list */
		pObj->pMmList->NewListValue(&result );
		
		err = pObj->pDrCast->GetFirstUsedMemberIndex(&memberIndex);
		i = 0;
		while (memberIndex && (err == kMoaErr_NoErr))
		{
			/* add memberIndex to list */
			i++;
			
			pObj->pMmValue->IntegerToValue(memberIndex, &element);
			pObj->pMmList->AppendValueToList(&result, &element);
			pObj->pMmValue->ValueRelease(&element);
			
			/* get next memberIndex */
			err = pObj->pDrCast->GetNextUsedMemberIndex(memberIndex, &memberIndex);
		}
		callPtr->resultValue = result;
		err = 0;
		pObj->pDrCast->Release();
		pObj->pDrCast = NULL;
	}
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCst_GetMemberIndexFromName(PMoaDrCallInfo callPtr)
{
	/* (cast# or "name", "memberName") 
	Example call: put memNameToInd(xtraInstance, 1,"foobar")*/
	
	MoaError	err = kMoaErr_NoErr;

	MoaDrMemberIndex memberIndex;
	MoaChar		castMemName[kMoaDrMaxCastName];
	MoaLong		 castIndex;

	/* Determine if arg 1 (arg index 2) is a stringHdl or int */
	
	/* See if it's an int, get cast by # */
		
	GetArgByIndex(2, &gTempArg);

	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
		pObj->pDrMovie->GetNthCast(castIndex, &pObj->pDrCast);
	else
	{
		/* assume it's a string */
		MoaChar	castName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			pObj->pDrMovie->GetCastFromName(castName, &pObj->pDrCast);
	}
	
	if (err == kMoaErr_NoErr)
	{		
		GetArgByIndex(3, &gTempArg);

		pObj->pMmValue->ValueToString( &gTempArg, castMemName, kMoaDrMaxCastName);
		err = pObj->pDrCast->GetMemberIndexFromName(castMemName, &memberIndex);
		pObj->pMmValue->IntegerToValue(memberIndex, &callPtr->resultValue);
		
		pObj->pDrCast->Release();
		pObj->pDrCast = NULL;
	}
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCst_CreateCastMem(PMoaDrCallInfo callPtr)
{
	/* (cast index or name, member index, type symbolStr) 
	Example call: createMem(xtraInstance, 1, 2, "flash")*/
	
	MoaError	err = kMoaErr_NoErr;

	MoaChar 		typeSymStr[PROPNAME_SIZE];
	MoaMmSymbol 	typeSymbol;
	MoaLong			memberIndex;
	MoaLong		 	castIndex;

	/* Determine if arg 1 (arg index 2) is a stringHdl or int */
	
	/* See if it's an int, get cast by # */

	GetArgByIndex(2, &gTempArg);
	
	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
		pObj->pDrMovie->GetNthCast(castIndex, &pObj->pDrCast);
	else
	{
		/* assume it's a string */
		MoaChar	castName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			pObj->pDrMovie->GetCastFromName(castName, &pObj->pDrCast);
	}
	
	if (err == kMoaErr_NoErr)
	{
		/* Get member index */
		GetArgByIndex(3, &gTempArg);
		pObj->pMmValue->ValueToInteger(&gTempArg, &memberIndex);
	
		/* Get symbol for cast mem type */
		GetArgByIndex(4, &gTempArg);
		pObj->pMmValue->ValueToString( &gTempArg, typeSymStr, PROPNAME_SIZE);
		pObj->pMmValue->StringToSymbol(typeSymStr, &typeSymbol);
		/*For information on stringToSymbol see "MM Developer's Guide" -> "Multimedia Methods"
		/* Create cast member */
		pObj->pDrCast->CreateCastMem(memberIndex,  typeSymbol);
		pObj->pDrCast->Release();
		pObj->pDrCast = NULL;
	}
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCst_DeleteCastMem(PMoaDrCallInfo callPtr)
{
	/* (cast# or "name", memberIndex or "memberName") 
	Example call: deleteMem(xtraInstance, 1, 2)*/
	
	MoaError	err = kMoaErr_NoErr;
	
	MoaDrMemberIndex memberIndex;
	MoaLong		 castIndex;

	/* Determine if arg 1 (arg index 2) is a stringHdl or int */
	
	/* See if it's an int, get cast by # */

	GetArgByIndex(2, &gTempArg);
	
	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
		pObj->pDrMovie->GetNthCast(castIndex, &pObj->pDrCast);
	else
	{
		/* assume it's a string */
		MoaChar	castName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			pObj->pDrMovie->GetCastFromName(castName, &pObj->pDrCast);
	}
	
	if (err == kMoaErr_NoErr)
	{
		/* At this point, we have a cast in pCast. */
		
		/* Determine if arg 2 (member) is a stringHdl or int */
		GetArgByIndex(3, &gTempArg);

		if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &memberIndex)) != kMoaErr_NoErr)
		{
			/* assume it's a string */
			MoaChar	castMemName[kMoaDrMaxCastName];
			
			if ((err = pObj->pMmValue->ValueToString( &gTempArg, castMemName, kMoaDrMaxCastName)) == kMoaErr_NoErr)
				pObj->pDrCast->GetMemberIndexFromName(castMemName, &memberIndex);
		}
			
		if (err == kMoaErr_NoErr)
		{
			/* At this point, we have a member in memberIndex. */
						
			err = pObj->pDrCast->DeleteCastMem(memberIndex);
		}	
		
		/* Release */
		pObj->pDrCast->Release();
		pObj->pDrCast = NULL;
	}

	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCst_DuplicateCastMem(PMoaDrCallInfo callPtr)
{
	/* (cast# or "name", memberIndex or "memberName", dest cast # or name, dest memberIndex or "memberName") 
	Example call: dupeMem(xtraInstance, 1, 1, 1, 2)*/
	
	MoaError	err = kMoaErr_NoErr;

	PIMoaDrCast	 		pSourceCast, pDestCast;
	MoaDrMemberIndex 	sourceMemberIndex, destMemberIndex, memInd;
	MoaLong		 		castIndex;

	/* Determine if arg 1 (index 2) is a stringHdl or int */
	
	/* See if it's an int, get cast by # */

	GetArgByIndex(2, &gTempArg);
		
	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
		pObj->pDrMovie->GetNthCast(castIndex, &pSourceCast);
	else
	{
		/* assume it's a string */
		MoaChar	castName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			pObj->pDrMovie->GetCastFromName(castName, &pSourceCast);
	}
	
	if (err == kMoaErr_NoErr)
	{
		/* At this point, we have a cast in pCast. */
		
		/* Determine if arg 2 (member) is a stringHdl or int */
		GetArgByIndex(3, &gTempArg);

		if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &memInd)) == kMoaErr_NoErr)
				sourceMemberIndex = memInd;
		else
		{
			/* assume it's a string */
			MoaChar	castMemName[kMoaDrMaxCastName];
			
			if ((err = pObj->pMmValue->ValueToString( &gTempArg, castMemName, kMoaDrMaxCastName)) == kMoaErr_NoErr)
				pSourceCast->GetMemberIndexFromName(castMemName, &sourceMemberIndex);
		}
			
		if (err == kMoaErr_NoErr)
		{
			/* At this point, we have a member in sourceMemberIndex. */
						
			/* Do the same for the dest cast / member
			   Determine if arg 3 (cast) is a stringHdl or int
		 	*/
			 
			GetArgByIndex(4, &gTempArg);

			if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
				pObj->pDrMovie->GetNthCast(castIndex, &pDestCast);
			else
			{
				/* assume it's a string */
				MoaChar	castName[PROPNAME_SIZE];
				
				if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
					pObj->pDrMovie->GetCastFromName(castName, &pDestCast);
			}
			
			if (err == kMoaErr_NoErr)
			{
				/* At this point, we have a cast in pDestCast. */
				
				/* Determine if arg 4 (member) is a stringHdl or int */

				GetArgByIndex(5, &gTempArg);

				if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &memInd)) == kMoaErr_NoErr)
						destMemberIndex = memInd;
				else
				{
					/* assume it's a string */
					MoaChar	castMemName[kMoaDrMaxCastName];
					
					if ((err = pObj->pMmValue->ValueToString( &gTempArg, castMemName, kMoaDrMaxCastName)) == kMoaErr_NoErr)
						pSourceCast->GetMemberIndexFromName(castMemName, &destMemberIndex);
				}
					
				if (err == kMoaErr_NoErr)
				{
					/* Now we have all of the goodies, do the dupe! */
							
					err = pSourceCast->DuplicateCastMem(sourceMemberIndex, 
													pDestCast, destMemberIndex);
				}	
				pDestCast->Release();
			}
		}		
		pSourceCast->Release();
	}
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCst_GetCastSel(PMoaDrCallInfo callPtr)
{
	/* (cast# or "name") 
	Example call: put getCastSel(xtraInstance,1)*/

	MoaError	err = kMoaErr_NoErr;
	MoaLong		 	castIndex;
	MoaMmSymbol		symbol;
	/* Selection is list of lists, so three "nested" values. */
	MoaMmValue		selValue, selRangeValue, value;
	MoaMmValueType	type;
	MoaLong			selCnt, selRangeCnt;
	long i, j, selRangeBegin, selRangeEnd;
	/* Determine if arg 1 (index 2) is a stringHdl or int */
	
	/* See if it's an int, get cast by # */

	GetArgByIndex(2, &gTempArg);
		
	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castIndex)) == kMoaErr_NoErr)
		pObj->pDrMovie->GetNthCast(castIndex, &pObj->pDrCast);
	else
	{
		/* assume it's a string */
		MoaChar	castName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			pObj->pDrMovie->GetCastFromName(castName, &pObj->pDrCast);
	}
	
	if (err == kMoaErr_NoErr)
	{
		/* At this point, we have a cast in pCast. */

		/* Make the symbol */
		pObj->pMmValue->StringToSymbol("selection", &symbol);
		
		/* We have a cast and symbol, so get the prop */
		err = pObj->pDrCast->GetProp(symbol, &selValue);
	
		pObj->pDrCast->Release();
		pObj->pDrCast = NULL;

		pObj->pMmValue->ValueType(&selValue, &type);
		if (type != kMoaMmValueType_List) 
		{
			MOA_ASSERT(FALSE, "cast selection prop is not a list?!");
			err = kMoaMmErr_ValueTypeMismatch;
			goto exit_gracefully;
		}

		/* Make new list for the return value */
		if ((err = pObj->pMmList->NewListValue(&callPtr->resultValue)) != kMoaErr_NoErr)
			goto exit_gracefully;

		selCnt = pObj->pMmList->CountElements( &selValue);

		/* Walk through selection list */
		/* Lingo lists are 1-based */
		for (i = 1; i <= selCnt; i++) 
		{

			pObj->pMmList->GetValueByIndex( &selValue, i, &selRangeValue);
			/* Each element should be a two-element range */
			if ((pObj->pMmValue->ValueType(&selRangeValue, &type) == kMoaErr_NoErr)
					&& type == kMoaMmValueType_List) 
			{
				selRangeCnt = pObj->pMmList->CountElements( &selRangeValue);
				MOA_ASSERT((selRangeCnt==2), "selection range should be two items?!");

				/* Get the start and end of the range */
				pObj->pMmList->GetValueByIndex( &selRangeValue, 1, &value );
				pObj->pMmValue->ValueToInteger( &value, &selRangeBegin );
				pObj->pMmValue->ValueRelease( &value );
				pObj->pMmList->GetValueByIndex( &selRangeValue, 2, &value );
				pObj->pMmValue->ValueToInteger( &value, &selRangeEnd );
				pObj->pMmValue->ValueRelease( &value );

				/* Add a number to the list for each item in the selection */
				for (j = selRangeBegin; j <= selRangeEnd; j++) 
				{
					pObj->pMmValue->IntegerToValue(j, &value);
					pObj->pMmList->AppendValueToList(&callPtr->resultValue, &value);
					pObj->pMmValue->ValueRelease(&value);
				}
			} 
			else 
			{
				MOA_ASSERT(type == kMoaMmValueType_List, "cast selection range is not a list?!");
			}

			pObj->pMmValue->ValueRelease( &selRangeValue );
		}
		pObj->pMmValue->ValueRelease(&selValue);
	}
		
exit_gracefully:

	return err;
} /* XCst_GetCastSel() */



/***** CAST MEMBER*****/

MoaError CDrAccessScript_IMoaMmXScript::XCmm_GetCastMemProp(PMoaDrCallInfo callPtr)
{
	/* (castmem# or "name", "propName") 
	Example call: put castMemProp(xtraInstance, 1, "name")*/

	MoaError	err = kMoaErr_NoErr;

	MoaMmSymbol symbol;
	MoaChar		 propName[PROPNAME_SIZE];
	MoaMmValue		moaValue;
	MoaDrMemberIndex	castMemIndex;

	/* Assume cast #1 */
	pObj->pDrMovie->GetNthCast(1, &pObj->pDrCast);
	
	/* Determine if arg 1 (arg index 2) is a stringHdl or int */
	
	GetArgByIndex(2, &gTempArg);

	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castMemIndex)) == kMoaErr_NoErr)
	{
		err = pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem);
		/* be sure to check for error here */
		if (err!=kMoaErr_NoErr)
			pObj->pDrCastMem = NULL;	/* this will be a random value if an error was returned */
	}
	if (err!=kMoaErr_NoErr)
	{
		/* assume it's a string */
		MoaChar	castMemName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castMemName, PROPNAME_SIZE)) == kMoaErr_NoErr)
		{
			err = pObj->pDrCast->GetMemberIndexFromName(castMemName, &castMemIndex);
			if (err == kMoaErr_NoErr)
				pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem);
			if (err!=kMoaErr_NoErr)
				pObj->pDrCastMem = NULL;	/* this will be a random value if an error was returned */
		}
	}
	
	if (err == kMoaErr_NoErr)
	{
		/* Get the symbol selector */

		GetArgByIndex(3, &gTempArg);

		pObj->pMmValue->ValueToString( &gTempArg, propName, PROPNAME_SIZE);
		pObj->pMmValue->StringToSymbol(propName, &symbol);
		
		/* We have a castmem and symbol selector, so get the prop */
		err = pObj->pDrCastMem->GetProp(symbol, &moaValue);
	
		callPtr->resultValue = moaValue;
		
		pObj->pDrCastMem->Release();
		pObj->pDrCastMem = NULL;
	}
	
	pObj->pDrCast->Release();
	pObj->pDrCast = NULL;

	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCmm_SetCastMemProp(PMoaDrCallInfo callPtr)
{
	/* (castmem# or "name", "propName", value) 
	Example call: setCastMemProp(xtraInstance, 1, "name" , "foobar")
	For information about the various cast member properties see the "Director Developer's Guide" -> "Director Types and Miscellaneous API" */

	MoaError err = kMoaErr_NoErr;
	
	MoaMmSymbol symbol;
	MoaChar		 propName[PROPNAME_SIZE];
	MoaMmValue		moaValue;
	MoaDrMemberIndex	castMemIndex;
	
	/* Assume cast #1 */
	pObj->pDrMovie->GetNthCast(1, &pObj->pDrCast);
	
	/* Determine if arg 1 (arg index 2) is a stringHdl or int */
	
	GetArgByIndex(2, &gTempArg);

	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castMemIndex)) == kMoaErr_NoErr)
	{
		err = pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem);
		/* be sure to check for error here */
		if (err!=kMoaErr_NoErr)
			pObj->pDrCastMem = NULL;	/* this will be a random value if an error was returned */
	}
	if (err!=kMoaErr_NoErr)
	{
		/* assume it's a string */
		MoaChar	castMemName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castMemName, PROPNAME_SIZE)) == kMoaErr_NoErr)
		{
			err = pObj->pDrCast->GetMemberIndexFromName(castMemName, &castMemIndex);
			if (err == kMoaErr_NoErr)
				pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem);
			if (err!=kMoaErr_NoErr)
				pObj->pDrCastMem = NULL;	/* this will be a random value if an error was returned */
		}
	}
	
	if (err == kMoaErr_NoErr)
	{
		/* Get the symbol selector */
		GetArgByIndex(3, &gTempArg);

		pObj->pMmValue->ValueToString( &gTempArg, propName, PROPNAME_SIZE);
		pObj->pMmValue->StringToSymbol(propName, &symbol);
		
		/* We have a castmem and symbol selector, so set the prop */
		GetArgByIndex(4, &moaValue);
		err = pObj->pDrCastMem->SetProp(symbol, &moaValue);
		
		pObj->pDrCastMem->Release();
		pObj->pDrCastMem = NULL;
	}
	
	pObj->pDrCast->Release();
	pObj->pDrCast = NULL;
	
	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCmm_TransferMedia(PMoaDrCallInfo callPtr)
{
	/* (castmem# or "name", castmem# or "name", "label", "format") 
	Example call: xferMedia(xtraInstance, 2, 3, "composite", "moaHandle")
	See the "Director Developer's Guide" -> "DRinterfaces" -> "IMoaDrCastMem" for information
	regarding lables and formats"*/
	
	MoaError			err = kMoaErr_NoErr;

	MoaChar		 		propName[PROPNAME_SIZE];
	MoaDrMediaInfo		mediaInfo;
	MoaDrMemberIndex	castMemIndex;
	
	/* Set up media info
	
	/* Get label from arg #3 */
	GetArgByIndex(4, &gTempArg);
	pObj->pMmValue->ValueToString( &gTempArg, propName, PROPNAME_SIZE);
	pObj->pMmValue->StringToSymbol(propName, &mediaInfo.labelSymbol);
	
	/* Get format from arg #4 */
	GetArgByIndex(5, &gTempArg);
	pObj->pMmValue->ValueToString( &gTempArg, propName, PROPNAME_SIZE);
	pObj->pMmValue->StringToSymbol(propName, &mediaInfo.formatSymbol);
	
	/* Assume cast #1 */
	pObj->pDrMovie->GetNthCast(1, &pObj->pDrCast);
	
	/* Determine if prop 1 is a stringHdl or int */
	
	GetArgByIndex(2, &gTempArg);
	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castMemIndex)) == kMoaErr_NoErr)
		pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem);
	else
	{
		/* assume it's a string */
		MoaChar	castMemName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castMemName, PROPNAME_SIZE)) == kMoaErr_NoErr)
		{
			pObj->pDrCast->GetMemberIndexFromName(castMemName, &castMemIndex);
			pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem);
		}
	}
	
	if (err == kMoaErr_NoErr)
	{
		/* We have the source cast mem, get the media */
		pObj->pDrUtils->NewMediaInfo(mediaInfo.labelSymbol, mediaInfo.formatSymbol, NULL, kMoaDrMediaOpts_None, NULL, &mediaInfo);
		/*See the "Director Developer's Guide" -> "Director Methods" -> "IMoaDrCastMem" for information	regarding lables and formats"*/
		/*See the "Director Developer's Guide" -> "DRinterfaces" -> "IMoaDrUtils" for information regarding NewMediaInfo"*/

		pObj->pDrCastMem->GetMedia(&mediaInfo);
		
		/* Release the source cast mem */
		pObj->pDrCastMem->Release();
		pObj->pDrCastMem = NULL;
		
		/* Determine if prop 2 is a stringHdl or int */
		
		GetArgByIndex(3, &gTempArg);

		if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castMemIndex)) == kMoaErr_NoErr)
			pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem);
		else
		{
			/* assume it's a string */
			MoaChar	castMemName[PROPNAME_SIZE];
			
			if ((err = pObj->pMmValue->ValueToString( &gTempArg, castMemName, PROPNAME_SIZE)) == kMoaErr_NoErr)
			{
				pObj->pDrCast->GetMemberIndexFromName(castMemName, &castMemIndex);
				pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem);
			}
		}
		
		if (err == kMoaErr_NoErr)
		{
			/* We have the dest cast mem, attach the media 
			   (this releases it too). */
			pObj->pDrCastMem->AttachMedia(&mediaInfo);
			/*See the "Director Developer's Guide" -> "DRmethods" -> "for information regarding AttachMedia"*/
			/* Release the dest cast mem */
			pObj->pDrCastMem->Release();
			pObj->pDrCastMem = NULL;
		}
	}
	
	/* Release the cast */
	pObj->pDrCast->Release();
	pObj->pDrCast = NULL;
	
	return err;
}

#ifdef _MACINTOSH
static void S_AdjustPath(MoaChar * filename)
{
	CFStringRef cstr = NULL;
	CFURLRef cref = NULL;
	cstr = CFStringCreateWithCString(kCFAllocatorDefault, (char *)filename, kCFStringEncodingUTF8);
	if (cstr)
	{
		cref = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, cstr, kCFURLHFSPathStyle, false);
		if (cref)
		{
			CFURLGetFileSystemRepresentation(cref, false, (UInt8 *)filename, PROPBUF_SIZE);
			CFRelease(cref);
		}
		CFRelease(cstr);
	}
}
#endif

#ifdef WINDOWS
#define S_AdjustPath(filename)
#endif


MoaError CDrAccessScript_IMoaMmXScript::XCmm_SaveMedia(PMoaDrCallInfo callPtr)
	/*This function saves the file in a composite date format. You cannot save it for example as
	*.bmp as it will not be recognizable to outside applications. The file can be read back into
	Director via the XCmm_ReadMedia function in this file

	Example call: saveMedia(xtraInstance, 2, "C:\TEMP\Fotos\foo")*/
{
	MoaError			err = kMoaErr_NoErr;

	MoaDrMemberIndex	castMemIndex;
	MoaDrMediaInfo mediaInfo;
				
	/* Assume cast #1 */
	pObj->pDrMovie->GetNthCast(1, &pObj->pDrCast);
	
	/* Determine if "first" arg (the cast member number) is a string or int */
	GetArgByIndex(2, &gTempArg);
	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castMemIndex)) != kMoaErr_NoErr) 
	{
		/* if it's not an integer, assume it's a string */
		MoaChar	castMemName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castMemName, PROPNAME_SIZE)) == kMoaErr_NoErr)	
		{
			err = pObj->pDrCast->GetMemberIndexFromName(castMemName, &castMemIndex);
		}
	}
	
	/* Get the cast member */
	err = pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem);

	/* Get its media as #composite */
	pObj->pMmValue->StringToSymbol("Composite", &mediaInfo.labelSymbol);
	pObj->pMmValue->StringToSymbol("moaHandle", &mediaInfo.formatSymbol);
	(void) pObj->pDrUtils->NewMediaInfo(mediaInfo.labelSymbol, mediaInfo.formatSymbol, NULL, kMoaDrMediaOpts_None, NULL, &mediaInfo);
	/*See the "Director Developer's Guide" -> "Director Methods" -> "IMoaDrCastMem" for information	regarding lables and formats"*/
	/*See the "Director Developer's Guide" -> "DRinterfaces" -> "IMoaDrUtils" for information regarding NewMediaInfo"*/

	if (err == kMoaErr_NoErr) 
	{
		err = pObj->pDrCastMem->GetMedia(&mediaInfo);
		
		/* Release the source cast mem */
		(void) pObj->pDrCastMem->Release();
		pObj->pDrCastMem = NULL;

	}

	if (err == kMoaErr_NoErr) 
	{
		PMoaVoid pBlob;	 
		FILE* f = NULL; 
		MoaChar	filename[PROPBUF_SIZE];	
		long siz;		

		/* pathname is "second" argument" */
		GetArgByIndex(3, &gTempArg);
		if (( err = pObj->pMmValue->ValueToString( &gTempArg, filename, PROPBUF_SIZE)) == kMoaErr_NoErr) 
		{
			siz = pObj->pHandle->GetSize((MoaHandle)mediaInfo.mediaData);
			
			S_AdjustPath(filename);

			f = fopen(filename, "wb+");
			if ( f ) 
			{
			
				fseek(f, 0L, SEEK_SET);
				
				pBlob = pObj->pHandle->Lock((MoaHandle)mediaInfo.mediaData);
#ifdef WINDOWS
					/* Byte-swap here due to D5 bug. */
					DrFixMediaHeader((const char *)pBlob);
#endif
				fwrite(pBlob, (size_t)siz, (size_t)1, f);
				pObj->pHandle->Unlock((MoaHandle)mediaInfo.mediaData);

				fclose(f);
			}
			
			pObj->pDrUtils->MediaRelease(&mediaInfo); // will do equivalent of pObj->pHandle->Free((MoaHandle *)mediaInfo.mediaData);
		}
	}

	/* Release the cast */
	pObj->pDrCast->Release();
	pObj->pDrCast = NULL;

	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XCmm_ReadMedia(PMoaDrCallInfo callPtr)
{
	/*object me, any dstCMem, string pathname -- Read composite media from pathname into a #bitmap
	This function WILL NOT read in a standard format file, such as *.bmp, etc, it will only properly
	work with files that have been saved in composite format such as with the XCmm_SaveMedia function
	in this file.
	
	Example call: readMedia(xtraInstance, 1, "C:\3dsmax3_1\AMcache\foo")*/

	MoaError	err = kMoaErr_NoErr;

	MoaDrMemberIndex	castMemIndex;
	MoaLong typeSymbol;
	MoaDrMediaInfo mediaInfo;
				
	/* Assume cast #1 */
	pObj->pDrMovie->GetNthCast(1, &pObj->pDrCast);
	
	/* Determine if "first" arg (the cast member number) is a string or int */
	AccessArgByIndex(2, &gTempArg);
	if ((err = pObj->pMmValue->ValueToInteger(&gTempArg, &castMemIndex)) != kMoaErr_NoErr)	
	{
		/* if it's not an integer, assume it's a string */
		MoaChar	castMemName[PROPNAME_SIZE];
		
		if ((err = pObj->pMmValue->ValueToString( &gTempArg, castMemName, PROPNAME_SIZE)) == kMoaErr_NoErr)
		{
			err = pObj->pDrCast->GetMemberIndexFromName(castMemName, &castMemIndex);
		}
	}

	/* Need to have a destination cast member, though it appears that it doesn't 
	 * matter what kind.
	 * Could use GetCastMem then GetProp("type") and if type is #empty,
	 * then need to create.
	 * Instead, just create a low-overhead cast member, and let it be overridden.
	 */


	/* Create a new #shape cast member, will delete anything in the slot. */
	pObj->pMmValue->StringToSymbol("BITMAP", &typeSymbol);

	(void) pObj->pDrCast->CreateCastMem(castMemIndex, typeSymbol);

	/* get newly-created destination cast member */ 
	if ((err = pObj->pDrCast->GetCastMem(castMemIndex, &pObj->pDrCastMem))== kMoaErr_NoErr) 
	{
		MoaHandle hBlob;
		PMoaVoid pBlob;	 
		FILE* f = NULL; 
		MoaChar	filename[PROPBUF_SIZE];	
		long siz;	
		long trash = 0;

		/* pathname is "second" argument*/ 
		AccessArgByIndex(3, &gTempArg);
		if ( (err = pObj->pMmValue->ValueToString( &gTempArg, filename, PROPBUF_SIZE)) == kMoaErr_NoErr) 
		{
			S_AdjustPath(filename);

			f = fopen(filename, "rb+");

			if ( f ) 
			{				
				trash = fseek(f, 0L, SEEK_END);
				siz = ftell(f);
				
				hBlob = pObj->pHandle->Alloc(siz);
				
				if ( hBlob ) 
				{
					pBlob = pObj->pHandle->Lock(hBlob);
					
				trash = fseek(f, 0L, SEEK_SET);
				trash = fread(pBlob, (size_t)siz, (size_t)1, f);
			
				#ifdef WINDOWS
					/* Byte-swap here due to D5 bug. */
					DrFixMediaHeader((const char *)pBlob);
				#endif

					pObj->pHandle->Unlock(hBlob);
				}

				fclose(f);
				
				if ( hBlob ) 
				{	/* We have the dest cast mem, attach the media (this releases it too). */

					pObj->pMmValue->StringToSymbol("Composite", &mediaInfo.labelSymbol);
					pObj->pMmValue->StringToSymbol("moaHandle", &mediaInfo.formatSymbol);

					(void) pObj->pDrUtils->NewMediaInfo(mediaInfo.labelSymbol, mediaInfo.formatSymbol, hBlob, kMoaDrMediaOpts_None, NULL, &mediaInfo);
                   	/*See the "Director Developer's Guide" -> "Director Methods" -> "IMoaDrCastMem" for information	regarding lables and formats"*/
					/*See the "Director Developer's Guide" -> "DRinterfaces" -> "IMoaDrUtils" for information regarding NewMediaInfo"*/

					err = pObj->pDrCastMem->AttachMedia(&mediaInfo);
					/*See the "Director Developer's Guide" -> "DRmethods" -> "for information regarding AttachMedia"
   					  Note: don't need to call pObj->pHandle->Free(hBlob), the AttachMedia does it */
			}
			} /* if file found */
		} /* if pathname string */
	} /* if castmem found */
	/* Release the dest cast mem */

	if ( pObj->pDrCastMem ) 
	pObj->pDrCastMem->Release();
	pObj->pDrCastMem = NULL;

	/* Release the cast */
	if ( pObj->pDrCast ) 
	pObj->pDrCast->Release();
	pObj->pDrCast = NULL;

	return err;
}



/***** SCORE SPRITE*****/

MoaError CDrAccessScript_IMoaMmXScript::XScs_GetScoreSpriteProp(PMoaDrCallInfo callPtr)
/* (arg 1: sprite number, arg 2: property string)
Example call: put xtraInstance.scoreSpriteProp(1, "foreColor")
--234 */

{
	/* (channel index, "propName") */
	
	MoaError	err = kMoaErr_NoErr;

	MoaMmSymbol symbolFrame;
	MoaMmSymbol symbolSpriteProp;
	MoaChar				propName[PROPNAME_SIZE];
	MoaMmValue			gTempArg, moaValue;
	MoaDrSpriteChanIndex	iChanIndex;
	MoaDrFrameIndex		theFrame;

	/* Arg 1 is channel index */
	AccessArgByIndex(2, &gTempArg);
	pObj->pMmValue->ValueToInteger(&gTempArg, &iChanIndex);

	/* Get the property symbol */
	AccessArgByIndex(3, &gTempArg);
	pObj->pMmValue->ValueToString( &gTempArg, propName, PROPNAME_SIZE);
	pObj->pMmValue->StringToSymbol(propName, &symbolSpriteProp);

	/* Note that the Score Access is not magically at the right frame! */
	/* Get the movie "frame" property */

	pObj->pMmValue->StringToSymbol("frame", &symbolFrame);
	err = pObj->pDrMovie->GetProp(symbolFrame, &moaValue);
	pObj->pMmValue->ValueToInteger(&moaValue, &theFrame);

	/* Release the "frame" value.  Caller owns, and we aren't passing it to our caller. */
	pObj->pMmValue->ValueRelease(&moaValue);

	/* Move to the score accessor to the current frame */
	if ((err = pObj->pDrScore->SetCurFrameIndex(theFrame)) == kMoaErr_NoErr) 
	{

		if ((err = pObj->pDrScore->GetSprite(iChanIndex, &pObj->pDrSprite)) == kMoaErr_NoErr) 
		{
			
			/* We have the sprite and the symbol, so get the prop */
			err = pObj->pDrSprite->GetProp(symbolSpriteProp, &moaValue);
		
			callPtr->resultValue = moaValue;
			
		}
	}	
	if (pObj->pDrSprite) pObj->pDrSprite->Release();
	pObj->pDrSprite = NULL;

	return err;
}


MoaError CDrAccessScript_IMoaMmXScript::XScs_SetScoreSpriteProp(PMoaDrCallInfo callPtr)
/* ( arg 1: sprite number, arg 2: property string, arg 3: new value)
Example call: xtraInstance.setScoreSpriteProp(1, "foreColor", 7) */

{
	/* (channel index, "propName", value) */

	MoaError	err = kMoaErr_NoErr;

	MoaMmSymbol symbolFrame;
	MoaMmSymbol symbolSpriteProp;
	MoaChar				propName[PROPNAME_SIZE];
	MoaMmValue			gTempArg, moaValue;
	MoaDrSpriteChanIndex	iChanIndex;
	MoaDrFrameIndex		theFrame;

	/* Arg 1 is channel index */
	AccessArgByIndex(2, &gTempArg);
	pObj->pMmValue->ValueToInteger(&gTempArg, &iChanIndex);

	/* Get the property symbol */
	AccessArgByIndex(3, &gTempArg);
	pObj->pMmValue->ValueToString( &gTempArg, propName, PROPNAME_SIZE);
	pObj->pMmValue->StringToSymbol(propName, &symbolSpriteProp);

	/* Note that the Score Access is not magically at the right frame! */
	/* Get the movie "frame" property */
	pObj->pMmValue->StringToSymbol("frame", &symbolFrame);
	err = pObj->pDrMovie->GetProp(symbolFrame, &moaValue);
	pObj->pMmValue->ValueToInteger(&moaValue, &theFrame);
	/* Release the "frame" value.  Caller owns, and we aren't passing it to our caller. */
	pObj->pMmValue->ValueRelease(&moaValue);

	/* Move the score accessor to the current frame */
	if ((err = pObj->pDrScore->SetCurFrameIndex(theFrame)) == kMoaErr_NoErr) 
	{

		if ((err = pObj->pDrScore->GetSprite(iChanIndex, &pObj->pDrSprite)) == kMoaErr_NoErr) 
		{
			
			pObj->pDrScore->BeginUpdate();
			/* We have the sprite and the symbol, so set the prop to value specified */
			AccessArgByIndex(4, &moaValue);
			err = pObj->pDrSprite->SetProp(symbolSpriteProp, &moaValue);
			pObj->pDrScore->UpdateFrame();
			pObj->pDrScore->EndUpdate();
			pObj->pDrScore->Commit();		
		}
	}	
	if (pObj->pDrSprite) pObj->pDrSprite->Release();
	pObj->pDrSprite = NULL;

	return err;
}

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
BEGIN_DEFINE_CLASS_INTERFACE(CDrAccessScript, IMoaMmXScript)
END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
BEGIN_DEFINE_CLASS_INTERFACE(CDrAccessScript, IMoaInitFromDict)
END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
BEGIN_DEFINE_CLASS_INTERFACE(CDrAccessScript, IMoaNotificationClient)
END_DEFINE_CLASS_INTERFACE
#endif


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

/* ------------------------------------------------------ MoaCreate_CDrAccessScript */
STDMETHODIMP MoaCreate_CDrAccessScript(CDrAccessScript * This)
{

	/* variable declarations */
	MoaError err = kMoaErr_NoErr;

	// Set up all our interfaces
	This->pCallback->QueryInterface(&IID_IMoaHandle, (PPMoaVoid)&This->pHandle);
	This->pCallback->QueryInterface(&IID_IMoaAppInfo, (PPMoaVoid)&This->pAppInfo);
	This->pCallback->MoaCreateInstance(&CLSID_CMoaFile2, &IID_IMoaFile2,(PPMoaVoid)&This->pMoaFile);
	This->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid)&This->pMmValue);
	This->pCallback->QueryInterface(&IID_IMoaMmUtils2, (PPMoaVoid)&This->pMmUtils2);
	This->pCallback->QueryInterface(&IID_IMoaMmList, (PPMoaVoid)&This->pMmList);
#ifdef WINDOWS
	This->pCallback->QueryInterface(&IID_IMoaMmWndWin, (PPMoaVoid)&This->pMmWndWin);
	if(!This->pMmWndWin)
	{
		err = kMoaErr_BadInterface;
		goto exit_gracefully;
	}
#endif
	This->pCallback->QueryInterface(&IID_IMoaDrPlayer, (PPMoaVoid)&This->pDrPlayer);
	This->pCallback->QueryInterface(&IID_IMoaDrUtils, (PPMoaVoid)&This->pDrUtils);

	// If we failed to get any of these interfaces, bail out
	if( !This->pHandle || !This->pAppInfo || !This->pMoaFile || !This->pMmValue || 
			!This->pMmUtils2 || !This->pMmList || !This->pDrPlayer || !This->pDrUtils)
	{
		err = kMoaErr_BadInterface;
		goto exit_gracefully;
	}

	This->pMoaStream = NULL;
	This->pMmGC = NULL;
	This->pDrMovie = NULL;
	This->pDrCast = NULL;
	This->pDrCastMem = NULL;
	This->pDrScore = NULL;
	This->pDrSprite = NULL;	

exit_gracefully:

	return(err);
}

/* ----------------------------------------------------- MoaDestroy_CDrAccessScript */
STDMETHODIMP_(void) MoaDestroy_CDrAccessScript(CDrAccessScript * This)
{
	// Release all saved interfaces.

	if (This->pHandle) This->pHandle->Release();
	This->pHandle = NULL;

	if (This->pAppInfo) This->pAppInfo->Release();
	This->pAppInfo = NULL;

	if (This->pMmValue) This->pMmValue->Release();
	This->pMmValue = NULL;

	if (This->pMmUtils2) This->pMmUtils2->Release();
	This->pMmUtils2 = NULL;

	if (This->pMmList) This->pMmList->Release();
	This->pMmList = NULL;

	if (This->pMoaFile) This->pMoaFile->Release();
	This->pMoaFile = NULL;

#ifdef WINDOWS
	if (This->pMmWndWin) This->pMmWndWin->Release();
	This->pMmWndWin = NULL;
#endif
	if (This->pDrPlayer) This->pDrPlayer->Release();
	This->pDrPlayer = NULL;

	if (This->pDrUtils) This->pDrUtils->Release();
	This->pDrUtils = NULL;
	
	return;
}

/* ----------------------------------- CDrAccessScript_IMoaMmXScript Create/Destroy */
CDrAccessScript_IMoaMmXScript::CDrAccessScript_IMoaMmXScript(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CDrAccessScript_IMoaMmXScript::~CDrAccessScript_IMoaMmXScript() {}

#ifdef USING_INIT_FROM_DICT
/* -------------------------------- CDrAccessScript_IMoaInitFromDict Create/Destroy */
CDrAccessScript_IMoaInitFromDict::CDrAccessScript_IMoaInitFromDict(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CDrAccessScript_IMoaInitFromDict::~CDrAccessScript_IMoaInitFromDict() {}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* -------------------------- CDrAccessScript_IMoaNotificationClient Create/Destroy */
CDrAccessScript_IMoaNotificationClient::CDrAccessScript_IMoaNotificationClient(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CDrAccessScript_IMoaNotificationClient::~CDrAccessScript_IMoaNotificationClient() {}
#endif

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

/* -------------------------------------------- CDrAccessScript_IMoaMmXScript::Call */
STDMETHODIMP CDrAccessScript_IMoaMmXScript::Call(PMoaMmCallInfo callPtr)
{

	/* variable declarations */
	MoaError err = kMoaErr_NoErr;
	
	/* Get the active movie */
	if ((err = pObj->pDrPlayer->GetActiveMovie(&pObj->pDrMovie)) != kMoaErr_NoErr) 
		goto exit_gracefully;
	
	/* Get a score access interface for the movie */
	if ((err = pObj->pDrMovie->GetScoreAccess(&pObj->pDrScore)) != kMoaErr_NoErr) 
		goto exit_gracefully;

	switch	( callPtr->methodSelector ) 
	{
		case m_new:
			{
				/* Optionally handle new() here */
			}
			break;
			
		/***********************************
		*									
		* MISC.
		*
		***********************************/
		
		case m_callAFunction:			// test generic call handler
			err = XX_CallAFunction(callPtr);
			break;

		case m_callACommand:			// test generic call handler
			err = XX_CallACommand(callPtr);
			break;

		case m_moaFileWrite:			// test IMoaFile
			err = XX_MoaFileWrite(callPtr);
			break;

		case m_sendSpriteMsg:
			err = XX_SendSpriteMsg(callPtr);
			break;

		case m_winGetParent:			// test IMoaMmUtils::WinGetParent()
			err = XX_WinGetParent(callPtr);
			break;

		case m_getStageRect:			// test IMoaDrMovie::GetStageWindowGC()
			err = XX_GetStageRect(callPtr);
			break;

		case m_downloadFile:			// downloads a file
			err = XX_DownloadFile(callPtr);
			break;

		case m_isStreamDone:			// sees if a file downloads
			err = XX_IsStreamDone(callPtr);
			break;

		/***********************************
		*									
		* PLAYER 
		*
		***********************************/
		
		case m_resolveFileName:			/* "filename", returns a full pathname string */
			err = XPly_ResolveFileName(callPtr);
			break;
		case m_getTypeCount:			/* no args */
			err = XPly_GetTypeCount(callPtr);
			break;		
		case m_getTypeName:				/* ("symbolString") */
			err = XPly_GetTypeName(callPtr);
			break;
		case m_dumpTypes:				/* ("symbolString") */
			err = XPly_DumpTypes();
			break;
		case m_dumpAppInfo:				/* no args */
			err = XPly_DumpAppInfo();
			break;
		case m_getPlayerProp:				/* ("symbolString") */
			err = XPly_GetPlayerProp(callPtr);
			break;
		case m_setPlayerProp:				/* ("symbolString") */
			err = XPly_SetPlayerProp(callPtr);
			break;
		case m_rgbToIndex:					/* [r, g, b] --> index */
			err = XPly_RGBToIndex(callPtr);
			break;
		case m_indexToRgb:					/* index --> [r, g, b] */
			err = XPly_IndexToRGB(callPtr);
			break;

		case m_goMovie:		/* movieName */
			err = XPly_GoMovie(callPtr);
			break;

		
		/***********************************
		*									
		* MOVIE 
		*
		***********************************/

		case m_getCastCount:			/* () */
			err = XMov_GetCastCount(callPtr);
			break;
		case m_getMovieProp:			/* ("propName") */
			err = XMov_GetMovieProp(callPtr);
			break;
		case m_setMovieProp:			/* ("propName", value) */					
			err = XMov_SetMovieProp(callPtr);
			break;			
		case m_movieToFilmloop:			/* arg 1: cast mem # for new filmloop */
			err = XMov_MovieToFilmloop(callPtr);
			break;
		case m_filmloopToMovie:			/* arg 1: cast mem # of filmloop */
			err = XMov_FilmloopToMovie(callPtr);
			break;
		case m_makeFilmloop:			/*  castmem# for new filmloop, castmem# for sprite run, sprite #, #frames */
			err = XMov_MakeFilmloop(callPtr);
			break;
		case m_getLabelFrameIndex:		/* arg1: label name */
			err = XMov_GetFrameIndexFromLabel(callPtr);
			break;
		case m_getFrameLabelName:		/* arg1: frame # */
			err = XMov_GetFrameLabelFromIndex(callPtr);
			break;
		case m_setFrameLabelName:		/* arg1: frame # */
			err = XMov_SetFrameLabel(callPtr);
			break;
		case m_addExternalCast:			/* arg1: "castName", arg2: "pathName": returns new movieCastIndex */
			err = XMov_AddExternalCast(callPtr);
			break;
			
			
		/***********************************
		*									
		* CAST 
		*
		***********************************/
			
			
		case m_getCastProp:				/* (cast# or "name", "propName") */
			err = XCst_GetCastProp(callPtr);
			break;
		case m_setCastProp:				/* (cast# or "name", "propName", value) */
			err = XCst_SetCastProp(callPtr);
			break;
		case m_getCastIndexInMovie:		/* (cast# or "name") */
			err = XCst_GetCastIndexInMovie(callPtr);
			break;
		case m_getUsedMemberList:		/* (cast# or "name") */
			err = XCst_GetUsedMemberList(callPtr);
			break;
		case m_getMemberIndexFromName:	/* (cast# or "name", "memberName") */
			err = XCst_GetMemberIndexFromName(callPtr);
			break;
		case m_createCastMem:			/* (cast index or name, member index, type symbolStr) */
			err = XCst_CreateCastMem(callPtr);
			break;
		case m_deleteCastMem:			/* (cast# or "name", memberIndex or "memberName") */
			err = XCst_DeleteCastMem(callPtr);
			break;
		case m_dupeCastMem:				/* (cast# or "name", memberIndex or "memberName", dest cast # or name, dest memberIndex or "memberName") */
			err = XCst_DuplicateCastMem(callPtr);
			break;
		case m_getCastSel:				/* (cast# or "name") */
			err = XCst_GetCastSel(callPtr);
			break;
			
		/***********************************
		*									
		* CAST MEMBER 
		*
		***********************************/
			
		case m_getCastMemProp:			/* (castmem# or "name", "propName") */
			err = XCmm_GetCastMemProp(callPtr);
			break;
		case m_setCastMemProp:			/* (castmem# or "name", "propName", value) */
			err = XCmm_SetCastMemProp(callPtr);
			break;
		case m_transferMedia:			/* (castmem# or "name", castmem# or "name", "label", "format") */
			err = XCmm_TransferMedia(callPtr);
			break;
		case m_saveMedia:			    /* (castmem# or "name", "pathname") */
			err = XCmm_SaveMedia(callPtr);
			break;
		case m_readMedia:			    /* (castmem# or "name", "pathname") */
			err = XCmm_ReadMedia(callPtr);
			break;

			
		/***********************************
		*									
		* SCORE SPRITE
		*
		***********************************/
			
		case m_scoreSpriteProp:			/* (channel#, "propName") */
			err = XScs_GetScoreSpriteProp(callPtr);
			break;
		case m_setScoreSpriteProp:		/* (channel#, "propName", value) */
			err = XScs_SetScoreSpriteProp(callPtr);
			break;

	}

exit_gracefully:
	// release the movie and score interfaces
	if(pObj->pDrMovie)
	{
		pObj->pDrMovie->Release();
		pObj->pDrMovie = NULL;
	}

	if(pObj->pDrScore)
	{
		pObj->pDrScore->Release();
		pObj->pDrScore = NULL;
	}

	return(err);
}

#ifdef USING_INIT_FROM_DICT
/* --------------------------------- CDrAccessScript_IMoaInitFromDict::InitFromDict */
STDMETHODIMP CDrAccessScript_IMoaInitFromDict::InitFromDict(PIMoaRegistryEntryDict pRegistryDict)
{
	UNUSED(pRegistryDict);
	
	/* variable declarations */
	MoaError err = kMoaErr_NoErr;

	return(err);
}
#endif

#ifdef USING_NOTIFICATION_CLIENT
/* --------------------------------- CDrAccessScript_IMoaNotificationClient::Notify */
STDMETHODIMP CDrAccessScript_IMoaNotificationClient::Notify(ConstPMoaNotifyID nid, PMoaVoid pNData, PMoaVoid pRefCon)
{
	UNUSED(nid);
	UNUSED(pNData);
	UNUSED(pRefCon);
	
	/* variable declarations */
	MoaError err = kMoaErr_NoErr;

	return(err);
}
#endif
