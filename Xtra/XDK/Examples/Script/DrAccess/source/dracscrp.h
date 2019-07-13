/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2007 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

#ifndef _H_cscript
#define _H_cscript

/*****************************************************************************
 *
 *	Name: DrAcScrp.h
 *	
 * 	Purpose: Declarations of the CLSID, class instance variables, and  
 *           class interface(s) for the DrAccess Xtra's scripting class.
 *
 ****************************************************************************/ 

/*****************************************************************************
 *  INCLUDE FILE(S)
 *  ---------------
 *	This .h file should automatically include all the support files needed for
 *	this particular class. In addition, this file may include other .h files
 *	defining additional callback interfaces for use by a third party.   
 ****************************************************************************/ 
#ifndef _H_moaxtra		  
	#include "moaxtra.h"
#endif

#include "mmixscrp.h"

#include "moafile2.h"
#include "moastr2.h"

#include "mmiservc.h"

#include "driservc.h"

#if defined(WINDOWS) && defined(WIN32)
#include <winuser.h>  //  for GetStageRect
#endif

#ifdef MACINTOSH
	#include <windows.h>
	#include <string.h>
#endif

/*****************************************************************************
 *  CONFIGURATION DEFINE(S)
 *  -----------------------
 *	Uncomment any appropriate lines below to implement the indicated Xtra
 *	interfaces within this class.
 ****************************************************************************/ 
//#define USING_INIT_FROM_DICT
//#define USING_NOTIFICATION_CLIENT

/*****************************************************************************
 *	CLASS SPECIFIC DEFINE(S)
 *	-----------------------
 *	Any #define's specific to this class 
 ****************************************************************************/ 
#ifndef UNUSED
	#define UNUSED(x) x
#endif

#define MYFILE_NAME  "myfile.txt"

/* some locally useful #defines etc. */

#define PROPBUF_SIZE	256
#define PROPNAME_SIZE	32
#define MY_BUF_SIZE		32
#define MAXDOCOMMAND	1024
#define MAX_STRING_SIZE	255

                                
/*****************************************************************************
 *	CLSID
 *	-----
 *	The CLSID is a GUID that unquely identifies your MOA class.  To generate a
 *	GUID, use the genUID.app utility (Mac) or the GUIDGEN.EXE utility (Win).
 *	The following line will produce a pre-compiler error if not replaced with
 *	a valid CLSID.
 ****************************************************************************/ 
DEFINE_GUID(CLSID(CDrAccessScript), 0x68F026AEL, 0x97EA, 0x11D0, 0x95, 0x0C, 0x00, 0x05, 0x02, 0x70, 0xB2, 0x08);

/*****************************************************************************
 *  CLASS INSTANCE VARIABLES
 *  ------------------------
 *  Class instance variable are variables whose scope is exclusive to the
 *  methods implemented by your MOA class.  Variables necessary for the
 *  implementation of your MOA class should be placed here.
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS(CDrAccessScript)
	PIMoaHandle					pHandle;
	PIMoaAppInfo				pAppInfo;
	PIMoaFile2					pMoaFile;
	PIMoaStream2				pMoaStream;
	PIMoaMmValue				pMmValue;
	PIMoaMmUtils2				pMmUtils2;
	PIMoaMmList					pMmList;
	PIMoaMmGC					pMmGC;
	PIMoaStream2				pStream2;
#ifdef WINDOWS
	PIMoaMmWndWin				pMmWndWin;
#endif
	PIMoaDrPlayer				pDrPlayer;
	PIMoaDrMovie				pDrMovie;
	PIMoaDrCast					pDrCast;
	PIMoaDrCastMem				pDrCastMem;
	PIMoaDrScoreAccess 			pDrScore;
	PIMoaDrScoreSprite 			pDrSprite;
	PIMoaDrUtils				pDrUtils;

EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

/*****************************************************************************
 *  CLASS INTERFACE(S)
 *  ------------------
 *  The interface(s) implemented by your MOA class are specified here.  Note
 *	that at least one class in your Xtra should implement the IMoaRegister
 *	interface.
 *  NOTE: Because C++ does not use a lpVtbl to reference an interface's methods, 
 *  the actual method declaration is done here.
 *
 *  Syntax:
 *  EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(<class-name>, <interface-name>) 
 *		EXTERN_DEFINE_METHOD(<return-type>, <method-name>,(<argument-list>)) 
 *	EXTERN_END_DEFINE_CLASS_INTERFACE
 ****************************************************************************/ 
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CDrAccessScript, IMoaMmXScript)
	EXTERN_DEFINE_METHOD(MoaError, Call, (PMoaMmCallInfo))
   private:
/***** MISC *****/
	EXTERN_DEFINE_METHOD(MoaError, XX_CallAFunction, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XX_CallACommand, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XX_MoaFileWrite, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XX_SendSpriteMsg, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XX_WinGetParent, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XX_GetStageRect, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XX_DownloadFile, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XX_IsStreamDone, (PMoaDrCallInfo))
/***** PLAYER *****/
	EXTERN_DEFINE_METHOD(MoaError, XPly_ResolveFileName, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XPly_GetTypeCount, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XPly_GetTypeName, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XPly_DumpTypes, ())
	EXTERN_DEFINE_METHOD(MoaError, XPly_DumpAppInfo, ())
	EXTERN_DEFINE_METHOD(MoaError, XPly_GetPlayerProp, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XPly_SetPlayerProp, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XPly_RGBToIndex, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XPly_IndexToRGB, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XPly_GoMovie, (PMoaDrCallInfo))
/***** MOVIE *****/
	EXTERN_DEFINE_METHOD(MoaError, XMov_GetCastCount, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XMov_GetMovieProp, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XMov_SetMovieProp, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XMov_MovieToFilmloop, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XMov_FilmloopToMovie, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XMov_MakeFilmloop, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XMov_GetFrameIndexFromLabel, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XMov_GetFrameLabelFromIndex, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XMov_SetFrameLabel, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XMov_AddExternalCast, (PMoaDrCallInfo))
/***** CAST *****/
	EXTERN_DEFINE_METHOD(MoaError, XCst_GetCastProp, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCst_SetCastProp, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCst_GetCastIndexInMovie, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCst_GetUsedMemberList, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCst_GetMemberIndexFromName, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCst_CreateCastMem, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCst_DeleteCastMem, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCst_DuplicateCastMem, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCst_GetCastSel, (PMoaDrCallInfo))
/***** CAST MEMBER*****/
	EXTERN_DEFINE_METHOD(MoaError, XCmm_GetCastMemProp, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCmm_SetCastMemProp, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCmm_TransferMedia, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCmm_SaveMedia, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XCmm_ReadMedia, (PMoaDrCallInfo))
/***** SCORE SPRITE*****/
	EXTERN_DEFINE_METHOD(MoaError, XScs_GetScoreSpriteProp, (PMoaDrCallInfo))
	EXTERN_DEFINE_METHOD(MoaError, XScs_SetScoreSpriteProp, (PMoaDrCallInfo))

EXTERN_END_DEFINE_CLASS_INTERFACE

#ifdef USING_INIT_FROM_DICT
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CDrAccessScript, IMoaInitFromDict)
	EXTERN_DEFINE_METHOD(MoaError, InitFromDict, (PIMoaRegistryEntryDict))
EXTERN_END_DEFINE_CLASS_INTERFACE
#endif

#ifdef USING_NOTIFICATION_CLIENT
EXTERN_BEGIN_DEFINE_CLASS_INTERFACE(CDrAccessScript, IMoaNotificationClient)
	EXTERN_DEFINE_METHOD(MoaError, Notify, (ConstPMoaNotifyID, PMoaVoid, PMoaVoid))
EXTERN_END_DEFINE_CLASS_INTERFACE
#endif


// Method List...

enum 
{
m_new = 0,					/* standard */

							/* misc */
m_callAFunction,	
m_callACommand,		
m_moaFileWrite,		
m_sendSpriteMsg,	
m_winGetParent,		
m_getStageRect,
m_downloadFile,
m_isStreamDone,		

m_resolveFileName,			/* player */
m_getTypeCount,
m_getTypeName,
m_dumpTypes,
m_dumpAppInfo,
m_getPlayerProp,
m_setPlayerProp,
m_rgbToIndex,
m_indexToRgb,
m_goMovie,			

m_getCastCount,				/* movie */
m_getMovieProp,
m_setMovieProp,
m_movieToFilmloop,
m_filmloopToMovie,
m_makeFilmloop,
m_getLabelFrameIndex,
m_getFrameLabelName,
m_setFrameLabelName,
m_addExternalCast,

m_getCastProp,				/* cast */
m_setCastProp,
m_getCastIndexInMovie,
m_getUsedMemberList,
m_getMemberIndexFromName,
m_createCastMem,			
m_deleteCastMem,
m_dupeCastMem,
m_getCastSel,

m_getCastMemProp,			/* cast mem */
m_setCastMemProp,
m_transferMedia,
m_saveMedia,
m_readMedia,


m_scoreSpriteProp,			/* score sprite */
m_setScoreSpriteProp,

m_XXXX
};

#endif /* _H_cscript */
