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
// Dialog.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _H_Dialog
#define _H_Dialog

///////////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////////

#include "Asset.h"

// XSupport
#include "MuiItem.h"

// XDK
#include "MoaFile2.h"
#include "acclsid.h"

#ifdef MACINTOSH
#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif
#endif // MACINTOSH


///////////////////////////////////////////////////////////////////////////////////
//
// Constants
//
///////////////////////////////////////////////////////////////////////////////////

enum
{
	kWindowBegin = 0,
		k3ColumnHGroupBegin,
			kPreviewHGroupBegin,
				kPreviewActor,
			kPreviewHGroupEnd,

			kButtonDivider,
			
			kMainControlsHGroupBegin,
				kVGroupLabelBegin,
					kNewSpriteTextLabel,			// Right aligned
					kPopupMenuLabel,  
					kExtraSpaceLabel,
					kHorizRadioGroupLabel,
					kVertRadioGroupLabel,
				kVGroupLabelEnd,

				kVGroupBegin,
					kNewSpriteTextEdit,
					kPopupMenu,
					kSlider,

					kHorizRadioVGroupBegin,				
						kHorizGroupRadioNone,
						kHorizSubHGroupBegin,
							kHorizSubRadio,
							kHorizSubEdit,
						kHorizSubHGroupEnd,

						kCheckboxHGroupBegin,
							kCheckboxSpacer,
							kCheckbox1,
							kCheckbox2,
						kCheckboxHGroupEnd,
						kCheckboxSpacer2,
					kHorizRadioVGroupEnd,

					kVertRadioVGroupBegin,				
						kVertGroupRadioNone,
						kVertRadioActivate,
						// we make an H group with a spacer to indent the other items and then a V group for the checkboxes.
						kVertCheckboxHGroupBegin,
							kVertCheckboxSpacer,
							kVertSubSetVGroupBegin,
								kVertCheckbox1,
								kVertCheckbox2,
								kVertCheckbox3,
								kMoreTextHGroupBegin,
									kMoreTextCheckbox,
									kMoreTextEdit,
								kMoreTextHGroupEnd,
							kVertSubSetVGroupEnd,
						kVertCheckboxHGroupEnd,
						kExtraSpaceLabel2,
					kVertRadioVGroupEnd,
				kVGroupEnd,
			kMainControlsHGroupEnd,
			
			kButtonDivider2,
			
			// import, cancel and help buttons
			kButtonVGroupBegin,
				kDefaultButton,
				kCancelButton,
				kHelpButton,
			kButtonVGroupEnd,
		k3ColumnHGroupEnd,
	kWindowEnd,	
	kNumItems
};

#ifndef UNUSED
	#define UNUSED(x) x
#endif


///////////////////////////////////////////////////////////////////////////////////
//
// MOA Stuff
//
///////////////////////////////////////////////////////////////////////////////////

#undef INTERFACE
#define	INTERFACE IMoaPropsDialog

// here is where we declare our own interface
DECLARE_INTERFACE_(IMoaPropsDialog, IMoaUnknown)
{
	STD_IUNKNOWN_METHODS

	/* display the dialog */
	STDMETHOD(InvokePropsDialog) (THIS_ PMoaVoid asset, PIMoaDrAssetCallback, PIMoaMmAssetCallback)	PURE;
};

#undef INTERFACE

typedef IMoaPropsDialog * PIMoaPropsDialog;

// you MUST define your own GUID here!  These ones are invalid!
DEFINE_GUID( CLSID( CMoaPropsDialog ), 0xF1FF3EFFL, 0x295E, 0x1FD1, 0xEC, 0xBA, 0x0F, 0x0E, 0xE2, 0x88, 0x9D, 0xEF );
DEFINE_GUID( IID( IMoaPropsDialog ), 0xFFAE3EFCL, 0x2A5A, 0xA2D1, 0xAF, 0xB3, 0x30, 0x45, 0x09, 0x84, 0xF8, 0x44 );

EXTERN_BEGIN_DEFINE_CLASS_INSTANCE_VARS( CMoaPropsDialog )
EXTERN_END_DEFINE_CLASS_INSTANCE_VARS

EXTERN_BEGIN_DEFINE_CLASS_INTERFACE( CMoaPropsDialog, IMoaPropsDialog )
	EXTERN_DEFINE_METHOD( MoaError, InvokePropsDialog, ( PMoaVoid asset, PIMoaDrAssetCallback, PIMoaMmAssetCallback ) )
EXTERN_END_DEFINE_CLASS_INTERFACE


///////////////////////////////////////////////////////////////////////////////////
//
// WackyPropsDialog
//
///////////////////////////////////////////////////////////////////////////////////
  
class WackyPropsDialog
{
	public:
		WackyPropsDialog( CWackyDlgAsset_IMoaMmXAsset *, PIMoaDrAssetCallback, PIMoaMmAssetCallback );
		~WackyPropsDialog();
		
		// accessors
		MoaLong				DoPropsDialog( void );			// returns whether or not to stop process.
	
	protected:
		void 				FixMuiItemSize( PMuiItemWrapper pItemWrapper, MoaUlong width, MoaUlong height );

		void 	 			CheckForLivePreview();
		void 	 			BuildLivePreview( PMuiItemWrapper* myItemArray, PTMuiItem cItemArray, ConstPMoaChar missingString );
		
	private:
		bool				 mUsePreview;
		bool 				 mHideStuff;
		PIMoaMmAssetCallback mMmAssetCallback;
		
		CWackyDlgAsset_IMoaMmXAsset * mAsset;
};

#endif	// !_H_Dialog

// EOF
