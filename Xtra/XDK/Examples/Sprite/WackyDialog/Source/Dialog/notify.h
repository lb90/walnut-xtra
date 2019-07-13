/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2008 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

///////////////////////////////////////////////////////////////////////////////
//
// Notify.h
//
///////////////////////////////////////////////////////////////////////////////


#ifndef _H_Notify
#define _H_Notify

#include "MuiNotif.h"
#include "Dialog.h"


class DialogNotify : public MuiNotify
{
	public:
		DialogNotify( PMuiDialogWrapper, PIMoaMmSpriteCallback&, PIMoaMmAssetCallback&, TWackyDlgProperties& );
		~DialogNotify();
	
		// for live preview
		void ToggleCheckboxGroup( PMuiDialogWrapper pDialog, bool on );
		
		// Implemented
		virtual MoaBool 	HandleClick( PMuiDialogWrapper, MoaLong );
		virtual MoaBool 	HandleChanged( PMuiDialogWrapper, MoaLong );
		virtual MoaBool 	HandleShowHide( PMuiDialogWrapper, MoaBool show );
		virtual MoaBool 	HandleZoom( PMuiDialogWrapper, MoaBool zoomIn );
		virtual MoaBool 	HandleResize( PMuiDialogWrapper, PMoaRect newSize );
		virtual MoaBool 	HandleFocus( PMuiDialogWrapper, MoaLong, MoaBool enteringFocus );
		// following method added to XSupport library in  D11
		virtual MoaBool		HandleSelectionChange( PMuiDialogWrapper, MoaLong, void* pSelectionArray );
		virtual MoaBool		HandleDoubleClick( PMuiDialogWrapper, MoaLong, void* pSelectionArray );
		
	protected:
		virtual void		DoHelp();
		bool 				TestDialogState();
		void				UpdatePreview();
		
		PIMoaMmSpriteCallback& 		mSpriteCallback;	// this needs to stay live
		PIMoaMmAssetCallback& 		mMmAssetCallback;	// this needs to stay live
		TWackyDlgProperties&		mProps;
};
typedef DialogNotify * PDialogNotify;

#endif	// !_H_Notify

// EOF
