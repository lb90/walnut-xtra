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
 *	Name: dialog.cpp
 *
 ****************************************************************************/ 


///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////

#include "Dialog.h"
#include "Notify.h"
#include "Sprite.h"
#include "MuiView.h"

// XSupport
#include "MuiHelpr.h"
#include "MuiProp.h"
#include "XMmList.h"
#include "XThrwErr.h"
#include "xclassver.h"


// ANSI
#include "stdlib.h"

///////////////////////////////////////////////////////////////////////////////
//
// Constants / defines
//
///////////////////////////////////////////////////////////////////////////////

#ifdef MACINTOSH
const MoaUlong kDialog_EditnameWidth	 = 220;
const MoaUlong kDialog_EditnameHeight	 = 30;
const MoaUlong kDialog_PopupWidth		 = kDialog_EditnameWidth;
const MoaUlong kDialog_PopupHeight		 = 18;
const MoaUlong kDialog_EmptyRadioWidth	 = 16;
const MoaUlong kDialog_EmptyRadioHeight	 = 16;
const MoaUlong kDialog_MoreLabelWidth 	 = 75;
const MoaUlong kDialog_MoreLabelHeight = kDialog_EmptyRadioHeight;
const MoaUlong kDialog_DefaultEditWidth1 = kDialog_EditnameWidth - kDialog_EmptyRadioWidth - kDialog_MoreLabelWidth;
const MoaUlong kDialog_DefaultEditWidth2 = kDialog_EditnameWidth - kDialog_MoreLabelWidth - (kDialog_EmptyRadioWidth * 2);
const MoaUlong kDialog_DefaultEditHeight = kDialog_EditnameHeight;
const MoaUlong kDialog_PreviewWidth 	 = 120;
const MoaUlong kDialog_PreviewHeight	 = 106;
// for D11, there are some problems with the default layout of checkbox items, so we ensure there is the minimum space we need
const MoaUlong kDialog_CheckBoxesWidth = 110;
const MoaUlong kDialog_CheckBoxesHeight = 20;

#else // WINDOWS

const MoaUlong kDialog_EditnameWidth	 = 160;
const MoaUlong kDialog_EditnameHeight	 = 12;
const MoaUlong kDialog_PopupWidth		 = kDialog_EditnameWidth;
const MoaUlong kDialog_PopupHeight		 = 12;
const MoaUlong kDialog_EmptyRadioWidth	 = 12;
const MoaUlong kDialog_EmptyRadioHeight	 = 12;
const MoaUlong kDialog_MoreLabelWidth  = 40;
const MoaUlong kDialog_MoreLabelHeight = kDialog_EmptyRadioHeight;
const MoaUlong kDialog_DefaultEditWidth1 = kDialog_EditnameWidth - kDialog_EmptyRadioWidth - kDialog_MoreLabelWidth;
const MoaUlong kDialog_DefaultEditWidth2 = kDialog_EditnameWidth - kDialog_MoreLabelWidth - (kDialog_EmptyRadioWidth * 2);
const MoaUlong kDialog_DefaultEditHeight= kDialog_EditnameHeight;
const MoaUlong kDialog_PreviewWidth 	= 88; 
const MoaUlong kDialog_PreviewHeight	= 60; 
#endif

///////////////////////////////////////////////////////////////////////////////
//
// MOA Stuff
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// Xtra definition
//
///////////////////////////////////////////////////////////////////////////////


 BEGIN_XTRA
	BEGIN_XTRA_DEFINES_CLASS(CMoaPropsDialog, XTRA_CLASS_VERSION)
 		CLASS_DEFINES_INTERFACE(CMoaPropsDialog, IMoaPropsDialog, XTRA_CLASS_VERSION)
	END_XTRA_DEFINES_CLASS
 END_XTRA


///////////////////////////////////////////////////////////////////////////////
//
// Class implementation
//
///////////////////////////////////////////////////////////////////////////////

BEGIN_DEFINE_CLASS_INTERFACE( CMoaPropsDialog, IMoaPropsDialog )
END_DEFINE_CLASS_INTERFACE


STDMETHODIMP MoaCreate_CMoaPropsDialog( CMoaPropsDialog * This )
{
	X_ENTER
	X_STD_RETURN( XSupport::Initialize( This->pCallback, kXSupportDefaultReserve, XDEBUG ) );
	X_EXIT
}


/* ---------------------------------------------------------- MoaDestroy_CMoaFontPlayer */
STDMETHODIMP_(void) MoaDestroy_CMoaPropsDialog( CMoaPropsDialog * /*This*/ )
{
	X_ENTER
	
	XSupport::Shutdown();

	X_RETURN_VOID;
	X_EXIT
}


///////////////////////////////////////////////////////////////////////////////
//
// Interface implementation
//
///////////////////////////////////////////////////////////////////////////////

/* macros that create constructors and destructors for the following interface(s) */
STD_INTERFACE_CREATE_DESTROY( CMoaPropsDialog, IMoaPropsDialog )

MoaError CMoaPropsDialog_IMoaPropsDialog::InvokePropsDialog( PMoaVoid asset, PIMoaDrAssetCallback pDrCallback, PIMoaMmAssetCallback pMmCallback )
{
	XMOAEXCEPT_ENTER
	
	if( !asset )
		X_STD_RETURN( kMoaErr_BadParam );	
		
	// note: this is a stack based object that destroys itself when we leave this function
	WackyPropsDialog propsDialog( (CWackyDlgAsset_IMoaMmXAsset *)asset, pDrCallback, pMmCallback );

	X_STD_RETURN( propsDialog.DoPropsDialog() );
	
	XMOAEXCEPT_EXIT
}


///////////////////////////////////////////////////////////////////////////////
//
// Methods
//
///////////////////////////////////////////////////////////////////////////////

WackyPropsDialog::WackyPropsDialog( CWackyDlgAsset_IMoaMmXAsset * pAsset, PIMoaDrAssetCallback pDrCallback, PIMoaMmAssetCallback pMmCallback ) :
	mUsePreview( false ),
	mHideStuff( false ),
	mMmAssetCallback( pMmCallback ),
	mAsset( pAsset )
{	
	if( mMmAssetCallback )
		mMmAssetCallback->AddRef();
}


WackyPropsDialog::~WackyPropsDialog( void )
{
	if( mMmAssetCallback )
		mMmAssetCallback->Release();
}


void WackyPropsDialog::FixMuiItemSize(PMuiItemWrapper pItemWrapper, MoaUlong width, MoaUlong height)
{	
  	// Set the width & height
	if( width != 0 )
		pItemWrapper->SetWidth( width );
  	
  	if( height != 0 )
  		pItemWrapper->SetHeight( height );

	// now do the locking
	MoaMmValue position = kVoidMoaMmValueInitializer;
	MuiLayoutStyleListHelper  layoutHelper( position, kReleaseValue );
	
	layoutHelper.SetLockSize( TRUE );
	
	// shove that into the item attributes
	MuiPropListHelper( pItemWrapper->GetAttributes() ).SetLayoutStyle( position );
}



MoaLong WackyPropsDialog::DoPropsDialog( void )
{
	PDialogNotify					pNotifier = NULL;
	PMuiDialogWrapper  				pMuiDialog = NULL;
	autoptr<PDialogNotify>			throwOutNotify( pNotifier );
	autoptr<PMuiDialogWrapper>		throwOutDialog( pMuiDialog );
	TMoaError						err;
	
	// we need an array of our C++ item wrappers -- allocate in the heap
	PMuiItemWrapper*			myItemArray	= new PMuiItemWrapper[kNumItems];
	PTMuiItem 					cItemArray  = new TMuiItem[kNumItems];

	// clear them
	::memset( myItemArray, 0, (kNumItems * sizeof( PMuiItemWrapper )) );
	::memset( cItemArray,  0, (kNumItems * sizeof( TMuiItem )) );
	
	// we need this item as this is where the font list hangs out
	myItemArray[kPopupMenu] = new MuiItemPopup( cItemArray, kPopupMenu );
	
	// load and set the default string.
	PMoaChar pChar[4] = {"One","Two","Three"};
	
	// here we cast, you could use a dynamic cast here too. note this only adds one item
	((PMuiItemPopup)myItemArray[kPopupMenu])->AppendItems( pChar );
	
	// here we add multiple items starting from the second one in.
	((PMuiItemPopup)myItemArray[kPopupMenu])->AppendItems( &pChar[1], 2 );

// --------------------------------------------------------------------------------
// Build window item list
// --------------------------------------------------------------------------------
	

	myItemArray[kWindowBegin] 					= new MuiItemWindowBegin( cItemArray, kWindowBegin );

		myItemArray[k3ColumnHGroupBegin] 		= new MuiItemGroupBegin( cItemArray, k3ColumnHGroupBegin, kMuiDirectionHorizontal );
			
			// preview group
			myItemArray[kPreviewHGroupBegin]	= new MuiItemGroupBegin( cItemArray, kPreviewHGroupBegin, kMuiDirectionHorizontal );
				
				BuildLivePreview( myItemArray, cItemArray, "Preview unavailable" );
				
			myItemArray[kPreviewHGroupEnd]		= new MuiItemGroupEnd( cItemArray, kPreviewHGroupEnd, kMuiDirectionHorizontal );

			// first divider
			myItemArray[kButtonDivider]			= new MuiItemDivider( cItemArray, kButtonDivider, kMuiDirectionVertical );

			// main controls and labels	
			myItemArray[kMainControlsHGroupBegin] 		= new MuiItemGroupBegin( cItemArray, kMainControlsHGroupBegin, kMuiDirectionHorizontal );

				// labels group -----------------------------------------------------------------------------------
				myItemArray[kVGroupLabelBegin] 			= new MuiItemGroupBegin( cItemArray, kVGroupLabelBegin, kMuiDirectionVertical );
					myItemArray[kNewSpriteTextLabel]	= new MuiItemText( cItemArray, kNewSpriteTextLabel, "Type something:", FALSE, FALSE, kMuiTextAlignRight );
					myItemArray[kPopupMenuLabel] 		= new MuiItemText( cItemArray, kPopupMenuLabel, "Popup Menu", FALSE, FALSE, kMuiTextAlignRight );
					myItemArray[kExtraSpaceLabel]		= new MuiItemText( cItemArray, kExtraSpaceLabel, "", FALSE, FALSE, kMuiTextAlignRight );
					myItemArray[kHorizRadioGroupLabel]	= new MuiItemText( cItemArray, kHorizRadioGroupLabel, "Radio Group", FALSE, FALSE, kMuiTextAlignRight );
					myItemArray[kVertRadioGroupLabel]	= new MuiItemText( cItemArray, kVertRadioGroupLabel, "Checkbox Group", FALSE, FALSE, kMuiTextAlignRight );
				myItemArray[kVGroupLabelEnd] 			= new MuiItemGroupEnd( cItemArray, kVGroupLabelEnd, kMuiDirectionVertical );
				
				// control group -----------------------------------------------------------------------------------
				myItemArray[kVGroupBegin] 							= new MuiItemGroupBegin( cItemArray, kVGroupBegin, kMuiDirectionVertical );
					myItemArray[kNewSpriteTextEdit]					= new MuiItemText( cItemArray, kNewSpriteTextEdit, mAsset->pObj->mProps.outputString, TRUE, FALSE, kMuiTextAlignLeft );

					// the popup is initialized above.
					
					myItemArray[kSlider] 							= new MuiItemSlider( cItemArray, kSlider, MoaLong(0), MoaLong(100), mAsset->pObj->mProps.fontSize, TRUE /*showTicks*/, TRUE /*showValue*/ );
					
					// bitmap option radios -- select first one
					myItemArray[kHorizRadioVGroupBegin] 			= new MuiItemGroupBegin( cItemArray, kHorizRadioVGroupBegin, kMuiDirectionVertical );
						myItemArray[kHorizGroupRadioNone] 			= new MuiItemRadio( cItemArray, kHorizGroupRadioNone, "Radio None", TRUE );
						myItemArray[kHorizSubHGroupBegin] 			= new MuiItemGroupBegin( cItemArray, kHorizSubHGroupBegin, kMuiDirectionHorizontal );
							myItemArray[kHorizSubRadio] 			= new MuiItemRadio( cItemArray, kHorizSubRadio, "Editbox:", FALSE );
							myItemArray[kHorizSubEdit]  			= new MuiItemText( cItemArray, kHorizSubEdit, "", TRUE, FALSE, kMuiTextAlignLeft );
						myItemArray[kHorizSubHGroupEnd]			 	= new MuiItemGroupEnd( cItemArray, kHorizSubHGroupEnd, kMuiDirectionHorizontal );

						myItemArray[kCheckboxHGroupBegin]			= new MuiItemGroupBegin( cItemArray, kCheckboxHGroupBegin, kMuiDirectionHorizontal );
							myItemArray[kCheckboxSpacer]		 	= new MuiItemText( cItemArray, kCheckboxSpacer, "", FALSE, FALSE, kMuiTextAlignRight );							
							myItemArray[kCheckbox1]					= new MuiItemCheckbox( cItemArray, kCheckbox1, "Checkbox one", FALSE  );
							myItemArray[kCheckbox2]					= new MuiItemCheckbox( cItemArray, kCheckbox2, "Checkbox two", FALSE );
						myItemArray[kCheckboxHGroupEnd]				= new MuiItemGroupEnd( cItemArray, kCheckboxHGroupEnd, kMuiDirectionHorizontal );
						myItemArray[kCheckboxSpacer2]				= new MuiItemText( cItemArray, kCheckboxSpacer2, "", FALSE, FALSE, kMuiTextAlignRight );
					myItemArray[kHorizRadioVGroupEnd] 				= new MuiItemGroupEnd( cItemArray, kHorizRadioVGroupEnd, kMuiDirectionVertical );

					// character option radios -- select first one
					myItemArray[kVertRadioVGroupBegin] 				= new MuiItemGroupBegin( cItemArray, kVertRadioVGroupBegin, kMuiDirectionVertical );
						myItemArray[kVertGroupRadioNone]		 	= new MuiItemRadio( cItemArray, kVertGroupRadioNone, "Deactivate Checkboxes", TRUE );

						myItemArray[kVertRadioActivate] 			= new MuiItemRadio( cItemArray, kVertRadioActivate, "Activate Checkboxes" );

						myItemArray[kVertCheckboxHGroupBegin] 		= new MuiItemGroupBegin( cItemArray, kVertCheckboxHGroupBegin, kMuiDirectionHorizontal );
							
							// this is the spacer
							myItemArray[kVertCheckboxSpacer] 		= new MuiItemText( cItemArray, kVertCheckboxSpacer, "", FALSE, FALSE, kMuiTextAlignRight );							

							myItemArray[kVertSubSetVGroupBegin]		= new MuiItemGroupBegin( cItemArray, kVertSubSetVGroupBegin, kMuiDirectionVertical );
								myItemArray[kVertCheckbox1] 		= new MuiItemCheckbox( cItemArray, kVertCheckbox1, "Red", mAsset->pObj->mProps.colorBits & kShowRed );
								myItemArray[kVertCheckbox2] 		= new MuiItemCheckbox( cItemArray, kVertCheckbox2, "Green", mAsset->pObj->mProps.colorBits & kShowGreen );
								myItemArray[kVertCheckbox3] 		= new MuiItemCheckbox( cItemArray, kVertCheckbox3, "Blue", mAsset->pObj->mProps.colorBits & kShowBlue );

								myItemArray[kMoreTextHGroupBegin]	= new MuiItemGroupBegin( cItemArray, kMoreTextHGroupBegin, kMuiDirectionHorizontal );
									myItemArray[kMoreTextCheckbox] 	= new MuiItemCheckbox( cItemArray, kMoreTextCheckbox, "Watch:", FALSE );
									myItemArray[kMoreTextEdit] 		= new MuiItemText( cItemArray, kMoreTextEdit, "", TRUE, FALSE, kMuiTextAlignLeft );
								myItemArray[kMoreTextHGroupEnd] 	= new MuiItemGroupEnd( cItemArray, kMoreTextHGroupEnd, kMuiDirectionHorizontal );
							myItemArray[kVertSubSetVGroupEnd] 		= new MuiItemGroupEnd( cItemArray, kVertSubSetVGroupEnd, kMuiDirectionVertical );

						myItemArray[kVertCheckboxHGroupEnd] 		= new MuiItemGroupEnd( cItemArray, kVertCheckboxHGroupEnd, kMuiDirectionHorizontal );
						myItemArray[kExtraSpaceLabel2]				= new MuiItemText( cItemArray, kExtraSpaceLabel2, "", FALSE, FALSE, kMuiTextAlignRight );							
						
					myItemArray[kVertRadioVGroupEnd] 				= new MuiItemGroupEnd( cItemArray, kVertRadioVGroupEnd, kMuiDirectionVertical );

				myItemArray[kVGroupEnd] 							= new MuiItemGroupEnd( cItemArray, kVGroupEnd, kMuiDirectionVertical );

			// end of main controls -------------
			myItemArray[kMainControlsHGroupEnd]	= new MuiItemGroupEnd( cItemArray, kMainControlsHGroupEnd, kMuiDirectionHorizontal );
			
			// Button Divider - for import, cancel, help
			myItemArray[kButtonDivider2]		= new MuiItemDivider( cItemArray, kButtonDivider2, kMuiDirectionVertical );
			
			// button group ---------------------------------------------------------------------------------
			myItemArray[kButtonVGroupBegin]		= new MuiItemGroupBegin( cItemArray, kButtonVGroupBegin, kMuiDirectionVertical );
				// platform option radios
				myItemArray[kDefaultButton] 	= new MuiItemButton( cItemArray, kDefaultButton, "OK", FALSE, kMuiItemDefault );
				myItemArray[kCancelButton] 		= new MuiItemButton( cItemArray, kCancelButton, "Cancel", FALSE, kMuiItemCancel );
				myItemArray[kHelpButton]		= new MuiItemButton( cItemArray, kHelpButton, "Help" );
			myItemArray[kButtonVGroupEnd]		= new MuiItemGroupEnd( cItemArray, kButtonVGroupEnd, kMuiDirectionVertical );
		
		// end of all columns
		myItemArray[k3ColumnHGroupEnd]			= new MuiItemGroupEnd( cItemArray, k3ColumnHGroupEnd, kMuiDirectionHorizontal );
	
	// window end
	myItemArray[kWindowEnd] 					= new MuiItemWindowEnd( cItemArray, kWindowEnd );
			
	// --------------------------------------------------------------------------------
	// Instantiate window
	// --------------------------------------------------------------------------------

	// en/dis-able all of the items
	MoaLong count;
	for( count = 0; count < kNumItems; count++ )
	{
		if( myItemArray[count] )
			myItemArray[count]->SetEnabled( TRUE );
		else
			kThrowMoaException( CMoaRuntimeError );
	}
	
	// fix up the dynamic layout by locking certain items to a particular size
	FixMuiItemSize( myItemArray[kNewSpriteTextEdit], kDialog_EditnameWidth, kDialog_EditnameHeight );
	FixMuiItemSize( myItemArray[kPopupMenu], kDialog_PopupWidth, kDialog_PopupHeight );

#ifdef WINDOWS
	FixMuiItemSize( myItemArray[kHorizSubEdit], kDialog_DefaultEditWidth1, kDialog_DefaultEditHeight );
	FixMuiItemSize( myItemArray[kMoreTextEdit], kDialog_DefaultEditWidth2, kDialog_DefaultEditHeight );
#else
	FixMuiItemSize( myItemArray[kHorizSubEdit], kDialog_DefaultEditWidth1, 0 );
	FixMuiItemSize( myItemArray[kMoreTextEdit], kDialog_DefaultEditWidth2, 0 );
	
	// For D11, we need to give a bit more room to these two items due to text sizing changes that aren't properly accounted for in MUI
	FixMuiItemSize( myItemArray[kCheckbox1], kDialog_CheckBoxesWidth, kDialog_CheckBoxesHeight );
	FixMuiItemSize( myItemArray[kCheckbox2], kDialog_CheckBoxesWidth, kDialog_CheckBoxesHeight );
#endif

	FixMuiItemSize( myItemArray[kHorizSubRadio], 	kDialog_MoreLabelWidth, kDialog_MoreLabelHeight );
	FixMuiItemSize( myItemArray[kMoreTextCheckbox], kDialog_MoreLabelWidth, kDialog_MoreLabelHeight );
	
	FixMuiItemSize( myItemArray[kCheckboxSpacer], 		kDialog_EmptyRadioWidth, kDialog_EmptyRadioWidth );
	FixMuiItemSize( myItemArray[kVertCheckboxSpacer], 	kDialog_EmptyRadioWidth, kDialog_EmptyRadioWidth );
	FixMuiItemSize( myItemArray[kCheckboxSpacer2], 		kDialog_EmptyRadioWidth, 1 );
	FixMuiItemSize( myItemArray[kExtraSpaceLabel2],		kDialog_EmptyRadioWidth, 1 );

	// create the dialog wrapper
	pMuiDialog = new MuiDialogWrapper( kMuiWindowType_Standard, kMuiLayout_Dynamic, myItemArray, kNumItems, "Wacky Dialog" );
	
	// our notifier requires a reference to MuiDialog and to the sprite callback which is probably null right now
	pNotifier = new DialogNotify( pMuiDialog, mAsset->pObj->mSpriteCallback, mMmAssetCallback, mAsset->pObj->mProps );
	
	// set callback
	pMuiDialog->SetCallback( pNotifier );

	// position the window
	pMuiDialog->SetModal( TRUE );
	pMuiDialog->SetWindowPosition( kMuiWindowPosition_Alert );

	// before we run the dialog make a backup of the properties in case the user cancels.
	TWackyDlgProperties savedProps = mAsset->pObj->mProps;

	// run the dialog -- doesn't return until dialog is brought down
	MoaLong result = (MoaLong)pMuiDialog->Run();

	// we have been editing the actual data so if they hit okay there is nothing to do!
	if( result == kCancelButton )
	{
		// they cancelled so back out their changes...
		mAsset->pObj->mProps = savedProps;
	}	


	// bye bye --  dump each individual 'rapper
	for( count = 0; count < kNumItems; count++ )
		delete myItemArray[count];

	// dump array
	delete [] myItemArray;
	delete [] cItemArray;

	return (MoaLong)(result != kDefaultButton);
}


void WackyPropsDialog::BuildLivePreview( PMuiItemWrapper* myItemArray, PTMuiItem cItemArray, ConstPMoaChar missingString )
{
	MoaError   	err 	= kMoaErr_NoErr;
	PTMuiItem 	pItem 	= &cItemArray[kPreviewActor];

	MuiDefaultStrucHelper().strucCreateDefaultInput( pItem );

	pItem->iType	  = kMuiWidgetType_Actor;

	pItem->iWidth	  = kDialog_PreviewWidth;
	pItem->iHeight	  = kDialog_PreviewHeight;

	kClearMoaMmValue( pItem->vAttributeList );
	{
		MoaMmValue			vClsID, vIAsset;
		PIMoaUnknown		pDummy = NULL;
	
		// See if asset control is around
		err = XSupport::GetCallback()->MoaCreateInstance( &CLSID(CAssetControl), &IID(IMuiControl), (PPMoaVoid)&pDummy );
		if( err == kMoaErr_NoErr )
			pDummy->Release();
			
		if( err != kMoaErr_NoErr ) 
		{
			myItemArray[kPreviewActor] = new MuiItemText( cItemArray, kPreviewActor, missingString, FALSE, TRUE );
		}
		else
		{
			// we initialize it with some static text that says it isn't available
			if( myItemArray[kPreviewActor] )
				delete myItemArray[kPreviewActor];
			
			// generic item				
			myItemArray[kPreviewActor] = new MuiItemWrapper( cItemArray, kPreviewActor );

			// Continue constructing an actor item.
			IMoaMmValueHelper	mmvHelper;
			MuiPropListHelper	listHelper( pItem->vAttributeList );
			mmvHelper.Use()->IntegerToValue( (MoaLong)&CLSID(CAssetControl), &vClsID );
			listHelper.SetClassID( vClsID );
			
			// setup item value
			mAsset->AddRef();
			mmvHelper.Use()->IntegerToValue( (MoaLong)mAsset, &vIAsset );
			listHelper.SetProp( kMuiLingoPropItemValue, vIAsset );
			
			// lock size of preview
			MoaMmValue position = kVoidMoaMmValueInitializer;
			MuiLayoutStyleListHelper( position ).SetLockSize( TRUE );
			listHelper.SetLayoutStyle( position );	
		}
	}
}

// EOF
