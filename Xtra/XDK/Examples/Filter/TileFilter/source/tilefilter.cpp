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
 *	Name: tilefilter.cpp
 *	
 * 	Purpose: Definitions of class interface(s) and methods for the 
 *           Filter Xtra skeleton project.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 ****************************************************************************/ 
#include "tilefilter.h"

#include <String.h>
  
/*****************************************************************************
 *  INTERNAL PROTOTYPES(S)
 ****************************************************************************/
#define kTilePropSize sizeof(MoaShort)		// 2 bytes used for property
#define kTileNumProps 4						// 4 properties
/*****************************************************************************
 *  INTERNAL ROUTINE(S)
 ****************************************************************************/

/*****************************************************************************
 *  CLASS INTERFACE(S)
 ****************************************************************************/ 
BEGIN_DEFINE_CLASS_INTERFACE(TileFilter, IMoaBitmapFilter)
END_DEFINE_CLASS_INTERFACE

/*****************************************************************************
 *  CREATE AND DESTROY METHODS
 ****************************************************************************/ 

/* -------------------------------------------------- MoaCreate_TileFilter */
STDMETHODIMP MoaCreate_TileFilter(TileFilter * This)
{
	MoaError err;
	This->mUtils = NULL;
	err = This->pCallback->QueryInterface(&IID_IMoaMmUtils, (PPMoaVoid)&This->mUtils);
	if(err == kMoaErr_NoErr)
		err = This->mUtils->StringToSymbol("stampsize", &This->mSymStampSize);
	if(err == kMoaErr_NoErr)
		err = This->mUtils->StringToSymbol("style", &This->mSymStyle);
	if(err == kMoaErr_NoErr)
		err = This->mUtils->StringToSymbol("dbgproplist", &This->mSymDbgPropList);
	This->mSize = 32;	// default size
	This->mStyle = 0;
	This->mParam1 = 0;
	This->mParam2 = 0;
	return err;
}

/* ------------------------------------------------- MoaDestroy_TileFilter */
STDMETHODIMP_(void) MoaDestroy_TileFilter(TileFilter * This)
{
	if (This->mUtils)
	{
		This->mUtils->Release();
		This->mUtils = NULL;
	}
}

/* ---------------------- TileFilter_IMoaBitmapFilter Create/Destroy */
TileFilter_IMoaBitmapFilter::TileFilter_IMoaBitmapFilter(MoaError * pErr) { *pErr = (kMoaErr_NoErr); }

TileFilter_IMoaBitmapFilter::~TileFilter_IMoaBitmapFilter() {}


/*****************************************************************************
 *  METHOD IMPLEMENTATION(S)
 ****************************************************************************/ 

/* ----------------------------- TileFilter_IMoaBitmapFilter::CallFunction */
STDMETHODIMP TileFilter_IMoaBitmapFilter::CallFunction(MoaMmSymbol symbol, MoaLong nArgs, ConstPMoaMmValue pArgs, PMoaMmValue pResult)
{
	MoaError err = kMoaMmErr_FunctionNotFound;
	if (symbol == pObj->mSymDbgPropList)
	{
		IMoaMmList *pList;
		MoaMmValue value;

		// filling out this data allows the properties to be displayed in the object inspector
		err = pObj->mUtils->QueryInterface(&IID_IMoaMmList, (PPMoaVoid) &pList);
		if (err == kMoaErr_NoErr)
		{
			pList->NewListValue(pResult);
			pObj->mUtils->SymbolToValue(pObj->mSymStampSize, &value);
			pList->AppendValueToList(pResult, &value);
			pObj->mUtils->ValueRelease(&value);
			pObj->mUtils->SymbolToValue(pObj->mSymStyle, &value);
			pList->AppendValueToList(pResult, &value);
			pObj->mUtils->ValueRelease(&value);
			// not actually using the other two options yet, so they are left out for now
			pList->Release();
		}
	}
	return err;
}

/* -------------------------------- TileFilter_IMoaBitmapFilter::GetProp */
STDMETHODIMP TileFilter_IMoaBitmapFilter::GetProp(MoaMmSymbol symbol, PMoaMmValue pPropValue)
{
	MoaError err = kMoaMmErr_PropertyNotFound;
	if( symbol == pObj->mSymStampSize )
		err = pObj->mUtils->IntegerToValue(pObj->mSize, pPropValue);
	else if( symbol == pObj->mSymStyle )
		err = pObj->mUtils->IntegerToValue(pObj->mStyle, pPropValue);
	
	return err;
}
	
/* --------------------------- TileFilter_IMoaBitmapFilter::SetProp */
STDMETHODIMP TileFilter_IMoaBitmapFilter::SetProp(MoaMmSymbol symbol, ConstPMoaMmValue pPropValue)
{
	MoaError err = kMoaMmErr_PropertyNotFound;
	MoaLong aLong;

	if( symbol == pObj->mSymStampSize )
	{
		err = pObj->mUtils->ValueToInteger(pPropValue, &aLong);
		// we just do tile sizes that are divisible by 16
		if (err == kMoaErr_NoErr)
		{
			if (aLong<=16) aLong = 16;
			else if (aLong>=256) aLong = 256;
			else aLong = ((aLong + 15)/16) * 16;
			pObj->mSize = aLong;
		}
	}
	else if (symbol == pObj->mSymStyle)
	{
		err = pObj->mUtils->ValueToInteger(pPropValue, &aLong);
		if (err == kMoaErr_NoErr)
		{
			pObj->mStyle = aLong;
		}
	}
	
	return err;
}
	
/* ----------------------------- TileFilter_IMoaBitmapFilter::RunFilter */
STDMETHODIMP TileFilter_IMoaBitmapFilter::RunFilter(PIMoaMmGC pIMoaMmGC, const PMoaFilterInfo pDestScaleRect)
{
	MoaError err = kMoaErr_NoErr;
	MoaRect dstRect = pDestScaleRect->rect;
	MoaRect srcImageRect;
	pIMoaMmGC->GetContentRect(&srcImageRect);
	MoaMmNativeGCInfo gcInfo;
	MoaRect moabounds;
	MoaRect stamp;
	PIMoaMmGC pMyGC = NULL;
	MoaMmInk myInk = kMoaMmInk_Copy;
	
	if (pObj->mStyle)
		myInk = kMoaMmInk_BackgroundTransparent;

	// define the size of the tile based on our property setting
	moabounds.left = moabounds.top = 0;
	moabounds.right = moabounds.bottom = pObj->mSize;
	
#ifdef MACINTOSH
	GWorldPtr pGWorld = NULL;
	Rect macbounds;
	
	// create our own stamp-sized GC to use for imaging
	MoaToMacRect(&moabounds, &macbounds);
	err = (OSErr)NewGWorld(&pGWorld, 8, &macbounds, NULL, NULL, kNativeEndianPixMap);
	if (err != noErr) goto exit_gracefully;	// just leave without doing anything
	gcInfo.type = kMoaMmGCType_MacGWorld;
	gcInfo.data.mac_gworld.gworldPtr = pGWorld;
#endif

#ifdef WINDOWS
	PMoaChar pImageBuffer = NULL;
	// Determine the size of a 24-bit pixel buffer in bytes.
	int rowBytes = pObj->mSize * 4;
	int size = rowBytes * pObj->mSize;

	// Allocate memory for an RGB DIB, to be the image buffer.
	pImageBuffer = (PMoaChar)pObj->pCalloc->NRAlloc(sizeof(BITMAPINFOHEADER) + size);
	if (!pImageBuffer) goto exit_gracefully;

	// Set the BITMAPINFOHEADER fields at the beginning of the DIB.
	BITMAPINFOHEADER * pHeader;
	pHeader = (BITMAPINFOHEADER *)pImageBuffer;
	pHeader->biSize = sizeof(BITMAPINFOHEADER);
	pHeader->biWidth = pObj->mSize;
	pHeader->biHeight = pObj->mSize;
	pHeader->biPlanes = 1;
	pHeader->biBitCount = 24; // RGB pixels
	pHeader->biCompression = 0;
	pHeader->biSizeImage = size;
	pHeader->biXPelsPerMeter = 0;
	pHeader->biYPelsPerMeter = 0;
	pHeader->biClrUsed = 0;
	pHeader->biClrImportant = 0;

	// Create a GC to allow us to blit into the DIB we have just created.
	gcInfo.type = kMoaMmGCType_WinDIBDC;
	gcInfo.data.win_dibdc.hDC = NULL; // don't need DC for DIB blit	
	gcInfo.data.win_dibdc.lpBitMapInfo = (BITMAPINFOHEADER *)pImageBuffer;
	gcInfo.data.win_dibdc.lpBits = pImageBuffer + sizeof(BITMAPINFOHEADER);
	gcInfo.data.win_dibdc.flags = 0;
#endif

	err = pObj->mUtils->NewGC(&gcInfo, &pMyGC);
	if (err != kMoaErr_NoErr) goto exit_gracefully;

	// copy the original image over to our GC - making a small version of it
	err = pMyGC->Blit(&moabounds, pIMoaMmGC, &srcImageRect, kMoaMmInk_Copy, NULL, NULL);
	if (err != kMoaErr_NoErr) goto exit_gracefully;

	// now we recover the destination rect with little versions of the original image
	for (stamp.top = dstRect.top; stamp.top <= dstRect.bottom; stamp.top = stamp.bottom)
	{
		stamp.bottom = stamp.top + pObj->mSize;
		for (stamp.left = dstRect.left; stamp.left <= dstRect.right; stamp.left = stamp.right)
		{
			stamp.right = stamp.left + pObj->mSize;
				err = pIMoaMmGC->Blit(&stamp, pMyGC, &moabounds, myInk, NULL, NULL);
			if (err != kMoaErr_NoErr) goto exit_gracefully;
		}
	}	
	
exit_gracefully:

	if (pMyGC)
		pMyGC->Release();

#ifdef MACINTOSH
	if (pGWorld)
		DisposeGWorld(pGWorld);
#endif
#ifdef WINDOWS
	if (pImageBuffer)
		pObj->pCalloc->NRFree(pImageBuffer);
#endif

	return err;
}

/* ----------------------------- TileFilter_IMoaBitmapFilter::StreamInProps */
STDMETHODIMP TileFilter_IMoaBitmapFilter::StreamInProps(PIMoaStream pInStream)
{
	MoaError err;
	MoaShort aData[kTileNumProps];

	if(!pInStream)
		return kMoaErr_BadParam;

	err = pInStream->Open(kMoaStreamOpenAccess_ReadOnly, kMoaStreamSetPositionType_None);
	if (err != kMoaErr_NoErr)
		return err;
	
	err = pInStream->Read(aData, sizeof(aData), NULL);
	
	if (err == kMoaErr_NoErr)
	{
	// on ppc mac we need to swap these values
#if (defined(MACINTOSH) && defined(__BIG_ENDIAN__))
		pObj->mSize = Endian16_Swap(aData[0]);
		pObj->mStyle = Endian16_Swap(aData[1]);
		pObj->mParam1 = Endian16_Swap(aData[2]);
		pObj->mParam2 = Endian16_Swap(aData[3]);
#else
		pObj->mSize = aData[0];
		pObj->mStyle = aData[1];
		pObj->mParam1 = aData[2];
		pObj->mParam2 = aData[3];
#endif
	}
		
	pInStream->Close();

	return err;
}	

/* ----------------------------- TileFilter_IMoaBitmapFilter::StreamOutProps */
STDMETHODIMP TileFilter_IMoaBitmapFilter::StreamOutProps(PIMoaStream pOutStream)
{
	MoaError err;
	MoaShort aData[kTileNumProps];

	// these properties will be stored in little endian order, so we adjust if necessary
#if (defined(MACINTOSH) && defined(__BIG_ENDIAN__))
	aData[0] = Endian16_Swap((MoaShort)pObj->mSize);
	aData[1] = Endian16_Swap((MoaShort)pObj->mStyle);
	aData[2] = Endian16_Swap((MoaShort)pObj->mParam1);
	aData[3] = Endian16_Swap((MoaShort)pObj->mParam2);
#else
	aData[0] = (MoaShort)pObj->mSize;
	aData[1] = (MoaShort)pObj->mStyle;
	aData[2] = (MoaShort)pObj->mParam1;
	aData[3] = (MoaShort)pObj->mParam2;
#endif

	if(!pOutStream)
		return kMoaErr_BadParam;

	err = pOutStream->Open(kMoaStreamOpenAccess_WriteOnly, kMoaStreamSetPositionType_None);
	if (err == kMoaErr_NoErr)
	{
		err = pOutStream->Write(aData, sizeof(aData), NULL);
		pOutStream->Close();
	}

	return err;
}

/* ----------------------------- TileFilter_IMoaBitmapFilter::GetStreamOutPropsSize */
STDMETHODIMP TileFilter_IMoaBitmapFilter::GetStreamOutPropsSize(PMoaLong pPropSize)
{
	*pPropSize = kTilePropSize * kTileNumProps;
	return kMoaErr_NoErr;
}

/* ----------------------------- TileFilter_IMoaBitmapFilter::InvokePropsEditor */
STDMETHODIMP TileFilter_IMoaBitmapFilter::InvokePropsEditor()
{
	MoaError err = kMoaErr_NoErr;
	return err;
}

/* ----------------------------- TileFilter_IMoaBitmapFilter::FinishPropsEditor */
STDMETHODIMP TileFilter_IMoaBitmapFilter::FinishPropsEditor(MoaBoolParam bCommitChanges, PMoaMmModifyFlags pFlagModifyProp)
{
	MoaError err = kMoaErr_NoErr;
	return err;
}

/* ----------------------------- TileFilter_IMoaBitmapFilter::GetOutputRect */
STDMETHODIMP TileFilter_IMoaBitmapFilter::GetOutputRect(const PMoaFilterInfo  pInputScaleRect, PMoaFilterInfo pOutputScaleRect)
{
	*pOutputScaleRect = *pInputScaleRect;
	return kMoaErr_NoErr;
}

/* ----------------------------- TileFilter_IMoaBitmapFilter::GetFilterCaps */
STDMETHODIMP TileFilter_IMoaBitmapFilter::GetFilterCaps(PMoaBMFilterCaps pFilterCap)
{
	pFilterCap->flags = kMoaMmXFilterCapsFlags_None;
	return kMoaErr_NoErr;
}

/* ----------------------------- TileFilter_IMoaBitmapFilter::SetCallback */
STDMETHODIMP TileFilter_IMoaBitmapFilter::SetCallback(PIMoaBitmapFilterCallback pBitmapFilterCallback)
{
	MoaError err = kMoaErr_NotImplemented;
	return err;
}
/* ----------------------------- TileFilter_IMoaBitmapFilter::Notify */
STDMETHODIMP TileFilter_IMoaBitmapFilter::Notify(MoaLong msgCode, PMoaVoid refCon)
{
	MoaError err = kMoaErr_NotImplemented;
	return err;
}
