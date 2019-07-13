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
 *	Name: cutool.cpp
 *	
 * 	Purpose: Definitions of Lingo class interface(s) and methods for the 
 *           Computer User Tool Xtra example.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  XTRA SPECIFIC DEFINE(S)
 *****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *****************************************************************************/ 
#include "cutool.h"
#include "xmmvalue.h"

#ifdef MACINTOSH
#include <windows.h>
#include <string.h>
#endif

#ifdef WINDOWS
#include <lmcons.h>
#endif

/*****************************************************************************
 *  FILE SPECIFIC DEFINE(S)
 *****************************************************************************/ 

/*****************************************************************************
 *  CLASS INTERFACE(S)
 *****************************************************************************/ 
BEGIN_DEFINE_CLASS_INTERFACE(CUTool, IMoaMmXTool)
END_DEFINE_CLASS_INTERFACE

/*****************************************************************************
 *  CREATE AND DESTROY METHODS
 */

/* ----------------------------------------------------------- MoaCreate_CUTool */
STDMETHODIMP MoaCreate_CUTool(CUTool * This)
{
	This->mDrCastMem = NULL;
	This->mDrScore = NULL;
	This->mMmValue = NULL;
	This->mDrMovie = NULL;
	return kMoaErr_NoErr;
}

/* ---------------------------------------------------------- MoaDestroy_CUTool */
STDMETHODIMP_(void) MoaDestroy_CUTool(CUTool * This)
{
	if (This->mDrCastMem)
	{
		This->mDrCastMem->Release();
		This->mDrCastMem = NULL;
	}
	if (This->mDrScore)
	{
		This->mDrScore->Release();
		This->mDrScore = NULL;
	}
	if (This->mMmValue)
	{
		This->mMmValue->Release();
		This->mMmValue = NULL;
	}
	if (This->mDrMovie)
	{
		This->mDrMovie->Release();
		This->mDrMovie = NULL;
	}
}

/* --------------------------------------- CUTool_IMoaMmXTool Create/Destroy */
CUTool_IMoaMmXTool::CUTool_IMoaMmXTool(MoaError * pErr)
	{ *pErr = (kMoaErr_NoErr); }
CUTool_IMoaMmXTool::~CUTool_IMoaMmXTool() {}

/*****************************************************************************
 *  METHOD IMPLEMENTATION(S)
 *****************************************************************************/ 

/* ------------------------------------------------- CUTool_IMoaMmXTool::Invoke */
STDMETHODIMP CUTool_IMoaMmXTool::Invoke()
{
	MoaError err = kMoaErr_NoErr;
	PIMoaDrPlayer pDrPlayer;
	PIMoaDrCast pDrCast = NULL;
	MoaMmValue aValue = kVoidMoaMmValueInitializer;
	
	err = pObj->pCallback->QueryInterface(&IID_IMoaMmValue, (PPMoaVoid)&pObj->mMmValue);
	if (err != kMoaErr_NoErr) goto exit_gracefully;

	// grab a few symbols for later use
	err = GetSymbols();
	if (err!=kMoaErr_NoErr) goto exit_gracefully;

	err = pObj->pCallback->QueryInterface(&IID_IMoaDrPlayer, (PPMoaVoid)&pDrPlayer);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;

	// get the current movie
	err = pDrPlayer->GetActiveMovie(&pObj->mDrMovie);
	pDrPlayer->Release();
	if (err!=kMoaErr_NoErr) goto exit_gracefully;

	// get interface to the first internal cast
	err = pObj->mDrMovie->GetNthCast(1, &pDrCast);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
		
	// also pick up a score access interface, and set it to the current frame
	err = pObj->mDrMovie->GetScoreAccess(&pObj->mDrScore);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->mDrMovie->GetProp(pObj->mSymFrame, &aValue);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->mMmValue->ValueToInteger(&aValue, &pObj->mFrame);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;

	// create a new text member in the cast using the first empty position
	err = CreateNewTextMember(pDrCast);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	err = AddTextToMember();
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
		
	// add a sprite for the new member to the score
	err = AddNewSprite();
	
exit_gracefully:
	if (pObj->mMmValue)
		pObj->mMmValue->ValueRelease(&aValue);
	if (pDrCast)
		pDrCast->Release();

	return err;
}

/* --------------------------------------------- CUTool_IMoaMmXTool::GetSymbols */
STDMETHODIMP CUTool_IMoaMmXTool::GetSymbols()
{
	MoaError err = pObj->mMmValue->StringToSymbol("member", &pObj->mSymMember);
	if (err==kMoaErr_NoErr)
		err = pObj->mMmValue->StringToSymbol("text", &pObj->mSymText);
	if (err==kMoaErr_NoErr)
		err = pObj->mMmValue->StringToSymbol("loc", &pObj->mSymLoc);
	if (err==kMoaErr_NoErr)
		err = pObj->mMmValue->StringToSymbol("tweened", &pObj->mSymTweened);
	if (err==kMoaErr_NoErr)
		err = pObj->mMmValue->StringToSymbol("name", &pObj->mSymName);
	if (err==kMoaErr_NoErr)
		err = pObj->mMmValue->StringToSymbol("pathname", &pObj->mSymPathname);
	if (err==kMoaErr_NoErr)
		err = pObj->mMmValue->StringToSymbol("frame", &pObj->mSymFrame);
	return err;
}

/* --------------------------------------------- CUTool_IMoaMmXTool::GetEnabled */
STDMETHODIMP CUTool_IMoaMmXTool::GetEnabled(PMoaDrEnabledState pEnabledState)
{
	*pEnabledState = kMoaDrEnabledState_Enabled;	
	return kMoaErr_NoErr;
}

/* ------------------------------------------------- CUTool_IMoaMmXTool::CreateNewTextMember */
STDMETHODIMP CUTool_IMoaMmXTool::CreateNewTextMember(PIMoaDrCast pDrCast)
{
	MoaError err = kMoaErr_NoErr;

	err = pDrCast->GetFirstFreeMemberIndex(&pObj->mIndex);
	if (err != kMoaErr_NoErr) goto exit_gracefully;

	err = pDrCast->CreateCastMem(pObj->mIndex, pObj->mSymText);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
		
	err = pDrCast->GetCastMem(pObj->mIndex, &pObj->mDrCastMem);

exit_gracefully:
		
	return err;
}

/* ------------------------------------------------- CUTool_IMoaMmXTool::ComposeStrings */
// on Macintosh, create separate strings and use CFStringCreateByCombiningStrings
// on Windows, allocate a single buffer and fill in the strings as we go
#ifdef MACINTOSH
MoaError CUTool_IMoaMmXTool::ComposeStrings(MoaMmValue * resultValue)
{
	MoaError err = kMoaMmErr_NoMemForString;
	CFStringRef cfstrs[] = {NULL, NULL, NULL, NULL, NULL, NULL};
	CFArrayRef cfarr = NULL;
	char * sbuffer = NULL;
	MoaMmValue nValue = kVoidMoaMmValueInitializer;
	MoaMmValue pValue = kVoidMoaMmValueInitializer;
	const char * cStr;
	
	cfstrs[0] = CFSTR("This text has Computer User Name, Computer Name, _movie.name, _movie.path");
	// get some strings from the system
	cfstrs[1] = CSCopyUserName(true);
	cfstrs[2] = CSCopyMachineName();
	if (!cfstrs[2] || !cfstrs[1]) goto exit_gracefully;

	// get some strings from the movie
	err = pObj->mDrMovie->GetProp(pObj->mSymName, &nValue);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->mMmValue->ValueToStringPtr(&nValue, &cStr);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	cfstrs[3] = CFStringCreateWithCString(kCFAllocatorDefault, cStr, kCFStringEncodingUTF8);
	pObj->mMmValue->ValueReleaseStringPtr(&nValue);
	if (!cfstrs[3]) goto exit_gracefully;
	
	err = pObj->mDrMovie->GetProp(pObj->mSymPathname, &pValue);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->mMmValue->ValueToStringPtr(&pValue, &cStr);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	cfstrs[4] = CFStringCreateWithCString(kCFAllocatorDefault, cStr, kCFStringEncodingUTF8);
	pObj->mMmValue->ValueReleaseStringPtr(&pValue);
	if (!cfstrs[4]) goto exit_gracefully;

	// now we construct the CFString with all the info, in UTF8 format
	err = kMoaMmErr_NoMemForString;
	cfarr = CFArrayCreate(kCFAllocatorDefault, (const void **)cfstrs, 5, &kCFTypeArrayCallBacks);
	if (!cfarr)  goto exit_gracefully;
	cfstrs[5] = CFStringCreateByCombiningStrings(kCFAllocatorDefault, cfarr, CFSTR("\r"));
	if (!cfstrs[5])  goto exit_gracefully;
	cStr = CFStringGetCStringPtr(cfstrs[5], kCFStringEncodingUTF8);
	if (!cStr)
	{
		sbuffer = (char *)pObj->pCalloc->NRAlloc(1024);
		if (sbuffer && CFStringGetCString(cfstrs[5], sbuffer, 1024, kCFStringEncodingUTF8))
			cStr = sbuffer;
	}
	if (!cStr) goto exit_gracefully;
	
	// finally, we assign the UTF8 string to our result value
	err = pObj->mMmValue->StringToValue(cStr, resultValue);

exit_gracefully:
	
	for (int i=1; i<6; i++)
	{
		if (cfstrs[i])
			CFRelease(cfstrs[i]);
	}
	if (sbuffer)
		pObj->pCalloc->NRFree(sbuffer);
	pObj->mMmValue->ValueRelease(&nValue);
	pObj->mMmValue->ValueRelease(&pValue);
	return err;
}
#endif

#ifdef WINDOWS
MoaError CUTool_IMoaMmXTool::ComposeStrings(MoaMmValue * resultValue)
{
	MoaError err = kMoaMmErr_NoMemForString;
	MoaMmValue nValue = kVoidMoaMmValueInitializer;
	MoaMmValue pValue = kVoidMoaMmValueInitializer;
	int bufsize = sizeof(wchar_t) * (UNLEN + MAX_COMPUTERNAME_LENGTH + 260 * 2 + 128);
	wchar_t * wbuffer = NULL;
	char * cbuffer = NULL;
	wchar_t * target;
	ConstPMoaChar cStr;
	int ilen;
	DWORD dlen;

	target = wbuffer = (wchar_t *)pObj->pCalloc->NRAlloc(bufsize);
	if (!wbuffer) goto exit_gracefully;

	// start with a header line
	wcscpy(target, L"This text has Computer User Name, Computer Name, _movie.name, _movie.path\r");
	ilen = wcslen(target);
	bufsize -= ilen;
	target = target + ilen;

	// get some strings from the system
	dlen = UNLEN + 1;
	if (!GetUserNameW(target, &dlen)) goto exit_gracefully;
	wcscat(target, L"\r");
	ilen = wcslen(target);
	bufsize -= ilen;
	target = target + ilen;
	dlen = MAX_COMPUTERNAME_LENGTH + 1;
	if (!GetComputerNameW(target, &dlen)) goto exit_gracefully;
	wcscat(target, L"\r");
	ilen = wcslen(target);
	bufsize -= ilen;
	target = target + ilen;

	// get some strings from the movie
	err = pObj->mDrMovie->GetProp(pObj->mSymName, &nValue);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->mMmValue->ValueToStringPtr(&nValue, &cStr);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	ilen = MultiByteToWideChar(CP_UTF8, 0, cStr, -1, target, bufsize);
	pObj->mMmValue->ValueReleaseStringPtr(&nValue);
	wcscat(target, L"\r");
	ilen = wcslen(target);
	bufsize -= ilen;
	target = target + ilen;
	
	err = pObj->mDrMovie->GetProp(pObj->mSymPathname, &pValue);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->mMmValue->ValueToStringPtr(&pValue, &cStr);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;
	ilen = MultiByteToWideChar(CP_UTF8, 0, cStr, -1, target, bufsize);
	pObj->mMmValue->ValueReleaseStringPtr(&pValue);
	if (ilen==0) goto exit_gracefully;

	// now we convert the wide character buffer to utf8
	err = kMoaMmErr_NoMemForString;
	ilen = WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, NULL, 0, NULL, NULL);
	if (ilen==0) goto exit_gracefully;
	cbuffer = (char *)pObj->pCalloc->NRAlloc(ilen);
	if (!cbuffer) goto exit_gracefully;
	WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, cbuffer, ilen, NULL, NULL);

	// finally, we assign the UTF8 string to our result value
	err = pObj->mMmValue->StringToValue(cbuffer, resultValue);

exit_gracefully:
	if (wbuffer) 
		pObj->pCalloc->NRFree(wbuffer);
	if (cbuffer) 
		pObj->pCalloc->NRFree(cbuffer);
	pObj->mMmValue->ValueRelease(&nValue);
	pObj->mMmValue->ValueRelease(&pValue);

	return err;
}
#endif

/* ------------------------------------------------- CUTool_IMoaMmXTool::AddTextToMember */
STDMETHODIMP CUTool_IMoaMmXTool::AddTextToMember()
{
	MoaError err = kMoaErr_NoErr;
	MoaMmValue aValue = kVoidMoaMmValueInitializer;

	err = ComposeStrings(&aValue);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
		
	err = pObj->mDrCastMem->SetProp(pObj->mSymText, &aValue);

exit_gracefully:

	pObj->mMmValue->ValueRelease(&aValue);
	return err;
}

/* ------------------------------------------------- CUTool_IMoaMmXTool::AddNewSprite */
STDMETHODIMP CUTool_IMoaMmXTool::AddNewSprite()
{
	MoaError err = kMoaErr_NoErr;
	MoaDrSpriteChanIndex iChanIndex = 1;
	PIMoaDrScoreSprite pDrSprite = NULL;
	PIMoaDrUtils pDrUtils = NULL;
	MoaDrFrameIndex theLastFrame;
	int i;

	err = pObj->pCallback->QueryInterface(&IID_IMoaDrUtils, (PPMoaVoid)&pDrUtils);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	
	// if the score is empty, we need to make a new one
	pObj->mDrScore->GetLastFrameIndex(&theLastFrame);
	if (theLastFrame==0)
	{
		err = pObj->mDrScore->New();
		if (err != kMoaErr_NoErr) goto exit_gracefully;
	}
	
	// make sure we are referencing the expected frame
	err = pObj->mDrScore->SetCurFrameIndex(pObj->mFrame);
	if (err!=kMoaErr_NoErr) goto exit_gracefully;

	// check for a free channel
	while (iChanIndex<1000)
	{
		err = pObj->mDrScore->GetSprite(iChanIndex, &pDrSprite);
		if (err != kMoaErr_NoErr) goto exit_gracefully;
		// now we check to see if there is an actor in this channel
		{	
			MoaMmValue yValue = kVoidMoaMmValueInitializer;
			MoaDrCMRef cmscore;
			err = pDrSprite->GetProp(pObj->mSymMember, &yValue);
			pDrSprite->Release();
			if (err != kMoaErr_NoErr) goto exit_gracefully;
			
			err = pDrUtils->ValueToCMRef(&yValue, &cmscore);
			if (err != kMoaErr_NoErr) goto exit_gracefully;
			pObj->mMmValue->ValueRelease(&yValue);
			if (cmscore.memberIndex==0)
				break;
		}
		iChanIndex++;
	}

	err = pObj->mDrScore->BeginUpdate();
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->mDrScore->GetSprite(iChanIndex, &pDrSprite);
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	
	// we make a 5 frame sequence, if there is room
	for (i=0; i<=5; i++)
	{
		// member
		{	
			MoaMmValue cmValue = kVoidMoaMmValueInitializer;
			MoaDrCMRef cmref;
			cmref.movieCastIndex = 1;
			cmref.memberIndex = pObj->mIndex;
			err = pDrUtils->CMRefToValue(&cmref, &cmValue);
			if (err == kMoaErr_NoErr)
				err = pDrSprite->SetProp(pObj->mSymMember, &cmValue);
			pObj->mMmValue->ValueRelease(&cmValue);
			if (err != kMoaErr_NoErr) goto exit_gracefully;
		}

		// location
		{	
			MoaPoint loc = {20, 20};
			MoaMmValue ptValue = kVoidMoaMmValueInitializer;
			err = pObj->mMmValue->PointToValue(&loc, &ptValue);
			if (err == kMoaErr_NoErr)
				err = pDrSprite->SetProp(pObj->mSymLoc, &ptValue);
			pObj->mMmValue->ValueRelease(&ptValue);
			if (err != kMoaErr_NoErr) goto exit_gracefully;
		}

		// tweened so the frames are part of one sprite instead of being separate key frames
		{	
			MoaMmValue tValue = kVoidMoaMmValueInitializer;
			err = pObj->mMmValue->IntegerToValue(1, &tValue);
			if (err == kMoaErr_NoErr)
				err = pDrSprite->SetProp(pObj->mSymTweened, &tValue);
			pObj->mMmValue->ValueRelease(&tValue);
			if (err != kMoaErr_NoErr) goto exit_gracefully;
		}

		// now finish the update process for the score frame
		err = pObj->mDrScore->UpdateFrame();
		if (err != kMoaErr_NoErr) goto exit_gracefully;

		// we're pointing a new frame so make sure it is empty before we continue recording
		// this is to prevent over writing something in the score since this tool doesn't
		// have present a UI or limit itself to the score selection
		{	
			MoaMmValue nValue = kVoidMoaMmValueInitializer;
			MoaDrCMRef cmnext;
			err = pDrSprite->GetProp(pObj->mSymMember, &nValue);
			if (err != kMoaErr_NoErr) goto exit_gracefully;
			err = pDrUtils->ValueToCMRef(&nValue, &cmnext);
			if (err != kMoaErr_NoErr) goto exit_gracefully;
			pObj->mMmValue->ValueRelease(&nValue);
			if (cmnext.memberIndex!=0)
				break;
		}
	}
	
	// finalize our score edits
	err = pObj->mDrScore->EndUpdate();
	if (err != kMoaErr_NoErr) goto exit_gracefully;
	err = pObj->mDrScore->Commit();		
	if (err != kMoaErr_NoErr) goto exit_gracefully;

exit_gracefully: 	

	if (pDrUtils)
		pDrUtils->Release();
	if (pDrSprite)
		pDrSprite->Release();
	return err;
}
