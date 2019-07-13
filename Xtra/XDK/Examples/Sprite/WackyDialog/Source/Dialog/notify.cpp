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
// Notify.cpp
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
///////////////////////////////////////////////////////////////////////////////

#include "Notify.h"

// XSupport stuff
#include "MuiItem.h"
#include "MuiProp.h"
#include "XMmList.h"
#include "XMmValue.h"
#include "XMoaHelp.h"
#include "XMacHelp.h"


///////////////////////////////////////////////////////////////////////////////
//
// Constants
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// Implementation
//
///////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------------------------
// Constructor/Destructor
// --------------------------------------------------------------------------------

DialogNotify::DialogNotify( PMuiDialogWrapper dialogRef, PIMoaMmSpriteCallback& pSpriteCallback, PIMoaMmAssetCallback& pAssetCallback, TWackyDlgProperties& props ) :
	MuiNotify( dialogRef ),
	mSpriteCallback( pSpriteCallback ),
	mMmAssetCallback(pAssetCallback),
	mProps( props )
{
}


DialogNotify::~DialogNotify()
{
}

// --------------------------------------------------------------------------------
// kMuiDialogItemChanged
// --------------------------------------------------------------------------------
MoaBool DialogNotify::HandleClick( PMuiDialogWrapper pDialog, MoaLong index )
{
	try
	{
		switch( index )
		{
				
			case kDefaultButton:
				// only end it if we say it's okay to go.
				if( TestDialogState() )	
				{	
					// Tell Director our props are dirty
					// A better way would be to check all the props to see if they
					// have changed and only set dirty if one has.					
					mMmAssetCallback->SetModified(kMoaMmModifyFlags_Props);

					/* currently a bug (yikes!) in Director.  If you have the asset cap
					kMoaMmAssetCaps_StaticThumbNeedsMedia set, and you dirty your props,
					then Director should call your thumbnail to redraw (It currently doesn't).
					This is new to Director.     To workaround this,
					we dirty our media (even though we have none) */
					mMmAssetCallback->SetModified(kMoaMmModifyFlags_Media);

					// Tell all the sprites to redraw as our data has changed 
					mMmAssetCallback->NotifySprites(kSprNotifyMsg_Redraw, NULL);

					// Die, dialog, Die!
					pDialog->Stop( kDefaultButton );	
				}
				break;
			
				
			case kCancelButton:
				pDialog->Stop( kCancelButton );	
				break;
				
			case kHelpButton:
				DoHelp();
				break;
		}
	}
	catch( CMoaException& ex )
	{
		ex.DescribeToUser();
		
		return FALSE;
	}
	
	return TRUE;
}

// --------------------------------------------------------------------------------
// kMuiDialogItemClicked
// --------------------------------------------------------------------------------
MoaBool DialogNotify::HandleChanged( PMuiDialogWrapper pDialog, MoaLong index )
{
	try
	{
		switch( index )
		{
			case kNewSpriteTextEdit:
				UpdatePreview();
				break;

			// check to see if the font popup menu has changed, if so update the text field
			case kPopupMenu:
			{
				MuiItemWrapper& popupItem    = pDialog->GetItem( kPopupMenu );
				MuiItemWrapper& editItem  	 = pDialog->GetItem( kNewSpriteTextEdit );
				MuiItemWrapper& sliderItem 	 = pDialog->GetItem( kSlider );

				popupItem.GetStringValue( mProps.popupItemString );

				// set the edit text field with the currently selected menu item
				char popupString[256];
				char newString[256];
				char sliderString[5];
				::memset( popupString, 0, sizeof( popupString ) );
				::memset( newString, 0, sizeof( newString ) );
				::memset( sliderString, 0, sizeof( sliderString ) );
				
				popupItem.GetStringValue( popupString, sizeof( popupString ) );
				sliderItem.GetStringValue( sliderString, sizeof( sliderString ) );

				MoaLong totalStringSize = ::strlen( " " ) + ::strlen( popupString ) + ::strlen( sliderString );
				MoaLong sizeToCopy = totalStringSize > sizeof( newString ) ? sizeof( newString ) : totalStringSize;
				
				// okay now mangle the string				
				::strncat( newString, popupString, sizeToCopy );
				::strncat( newString, " ", sizeToCopy );
				::strncat( newString, sliderString, sizeToCopy );
				editItem.SetValue( newString );
				
				pDialog->ItemUpdate( kNewSpriteTextEdit );

				// make sure that the little checkboxes update too.
				// we do this by pressing the right button.
				MuiItemWrapper& radioItem = pDialog->GetItem( kHorizSubRadio );
				if( radioItem.GetBoolValue() )
					HandleChanged( pDialog, kHorizSubRadio );
				else
					HandleChanged( pDialog, kHorizGroupRadioNone );
				
				UpdatePreview();
			}
			break;

			case kSlider:
				// tickle the popup menu, which updates the name and preview
				HandleChanged( pDialog, kPopupMenu );
				break;

			
			case kVertCheckbox1:
			case kVertCheckbox2:
			case kVertCheckbox3:
				UpdatePreview();
				break;

			case kCheckbox1:
			{ 
				MuiItemWrapper& checkbox1 = pDialog->GetItem( kCheckbox1 );
				mProps.checkbox1State = (MoaLong) checkbox1.GetBoolValue();
			}
				break;

			case kCheckbox2:
			{
				MuiItemWrapper& checkbox2 = pDialog->GetItem( kCheckbox2 );
				mProps.checkbox2State = (MoaLong) checkbox2.GetBoolValue();
			}
				break;

			// deal with the selection logic for the radio buttons - typing into the edit
			// also selects the radio associated with that edit.
			case kVertGroupRadioNone:
			{
				// disable the edit box
				MuiItemWrapper& edit = pDialog->GetItem( kMoreTextEdit );

				mProps.checkboxGroupButton = 1;

				edit.SetEnabled( false );
				pDialog->ItemUpdate( kMoreTextEdit );
				
				// disable the other radio item.
				ToggleCheckboxGroup( pDialog, false );
				
				// enable ourselves (not really redundant)
				MuiItemWrapper& radio2 = pDialog->GetItem( kVertGroupRadioNone );
				radio2.SetValue( MoaLong( true ) );
				pDialog->ItemUpdate( kVertGroupRadioNone );
			}
			break;

			case kVertRadioActivate:
			{
				// disable the edit box
				MuiItemWrapper& edit = pDialog->GetItem( kMoreTextEdit );
				
				mProps.checkboxGroupButton = 0;

				edit.SetEnabled( true );
				pDialog->ItemUpdate( kMoreTextEdit );
				
				// enable ourselves
				ToggleCheckboxGroup( pDialog, true );
				
				// disable all radio
				MuiItemWrapper& radio2 = pDialog->GetItem( kVertGroupRadioNone );
				radio2.SetValue( MoaLong( false ) );
				pDialog->ItemUpdate( kVertGroupRadioNone );
			}
			break;

			case kMoreTextEdit:
			{
				// if something is typed in check the box, otherwise uncheck it
				MuiItemWrapper& editBox  = pDialog->GetItem( kMoreTextEdit );
				MuiItemWrapper& checkbox = pDialog->GetItem( kMoreTextCheckbox );
				MoaBool			currentValue = checkbox.GetBoolValue();
			
				if( !::strcmp( editBox.GetTitle(), "" ) )
				{
					if( currentValue != MoaBool( false ) )
						checkbox.SetValue( MoaBool( false ) );
				}
				else
				{
					if( currentValue != MoaBool( true ) )
						checkbox.SetValue( MoaBool( true ) );
				}
				
				// is it different?
				if( currentValue != checkbox.GetBoolValue() )
					pDialog->ItemUpdate( kMoreTextCheckbox );
			}
			break;

			case kHorizGroupRadioNone:
			{
				// disable the edit box
				MuiItemWrapper& edit = pDialog->GetItem( kHorizSubEdit );
				
				mProps.radioGroupButton = 0;

				edit.SetEnabled( false );
				pDialog->ItemUpdate( kHorizSubEdit );

				pDialog->GetItem( kCheckbox1 ).SetEnabled( false );
				pDialog->GetItem( kCheckbox1 ).SetValue( (MoaLong)mProps.checkbox1State );
				pDialog->GetItem( kCheckbox2 ).SetEnabled( false );
				pDialog->GetItem( kCheckbox2 ).SetValue((MoaLong)mProps.checkbox2State );
				pDialog->ItemUpdate( kCheckbox1 );
				pDialog->ItemUpdate( kCheckbox2 );

				// disable the other radio item.
				MuiItemWrapper& radio = pDialog->GetItem( kHorizSubRadio );
				radio.SetValue( MoaLong( false ) );
				pDialog->ItemUpdate( kHorizSubRadio );

				// enable ourselves (not really redundant)
				MuiItemWrapper& radio2 = pDialog->GetItem( kHorizGroupRadioNone );
				radio2.SetValue( MoaLong( true ) );
				pDialog->ItemUpdate( kHorizGroupRadioNone );

				UpdatePreview();
			}
			break;

			case kHorizSubRadio:
			{
				// enable the edit box
				MuiItemWrapper& edit = pDialog->GetItem( kHorizSubEdit );
				MuiItemPopup& 	popup = (MuiItemPopup&)pDialog->GetItem( kPopupMenu );
				
				mProps.radioGroupButton = 1;

				edit.SetEnabled( true );
				pDialog->ItemUpdate( kHorizSubEdit );

				// enable the checkboxes only if the popup is on the first item
				MoaLong index = 0;
				popup.GetPopupValueIndex( index );

				// this wierd logic is just here so you can see how to manipulate these types of things
				if( index == 1 )
				{
					pDialog->GetItem( kCheckbox1 ).SetEnabled( true );
					pDialog->GetItem( kCheckbox2 ).SetEnabled( true );
				}
				else
				{
					pDialog->GetItem( kCheckbox1 ).SetEnabled( false );
					pDialog->GetItem( kCheckbox2 ).SetEnabled( false );
				}

				pDialog->ItemUpdate( kCheckbox1 );
				pDialog->ItemUpdate( kCheckbox2 );
				
				// disable the other radio item.
				MuiItemWrapper& radio = pDialog->GetItem( kHorizGroupRadioNone );
				radio.SetValue( MoaLong( false ) );
				pDialog->ItemUpdate( kHorizGroupRadioNone );

				// enable ourselves (not really redundant)
				MuiItemWrapper& radio2 = pDialog->GetItem( kHorizSubRadio );
				radio2.SetValue( MoaLong( true ) );
				pDialog->ItemUpdate( kHorizSubRadio );

				UpdatePreview();
			}
			break;
		}
	}
	catch( CMoaException& ex )
	{
		ex.DescribeToUser();
		
		return FALSE;
	}
	
	return TRUE;
}

// --------------------------------------------------------------------------------
// kMuiDialogWindowOpening/kMuiDialogWindowClosed
// --------------------------------------------------------------------------------
MoaBool DialogNotify::HandleShowHide( PMuiDialogWrapper pDialog, MoaBool show )
{
	// here is where I setup the dialog before it is shown
	try
	{
		if( show )
		{
			MuiItemWrapper& edit1 = pDialog->GetItem( kHorizSubEdit );
			MuiItemWrapper& edit2 = pDialog->GetItem( kMoreTextEdit );

			// default to disabled
			edit1.SetEnabled( false );
			edit2.SetEnabled( false );
		
			// set the popup to the saved value
			MuiItemWrapper& popupItem    = pDialog->GetItem( kPopupMenu );
			popupItem.SetValue( mProps.popupItemString );

			// set the middle checkboxes to saved values
			pDialog->GetItem( kCheckbox1 ).SetValue( (MoaLong)mProps.checkbox1State );
			pDialog->GetItem( kCheckbox2 ).SetValue((MoaLong)mProps.checkbox2State );

			// click that radio to default state of checkboxes, etc.
			if(mProps.radioGroupButton)
				HandleChanged( pDialog, kHorizSubRadio );	
			else
				HandleChanged( pDialog, kHorizGroupRadioNone );	

			if(mProps.checkboxGroupButton)
				HandleChanged( pDialog, kVertGroupRadioNone );	
			else
				HandleChanged( pDialog, kVertRadioActivate );	

			// fill in the characters and call the changed routine, this will select it for us.
			edit1.SetValue( mProps.editString );
			edit2.SetValue( mProps.watchString );

			pDialog->ItemUpdate( kHorizSubEdit );
			pDialog->ItemUpdate( kMoreTextEdit );
			
			// select that checkbox? 
			HandleChanged( pDialog, kMoreTextEdit );	

		}
	}
	catch( CMoaException& ex )
	{
		ex.DescribeToUser();
		return FALSE;
	}

	return TRUE;
}
					
				
// --------------------------------------------------------------------------------
// kMuiDialogWindowZoom
// --------------------------------------------------------------------------------
MoaBool DialogNotify::HandleZoom( PMuiDialogWrapper, MoaBool )
{
	return TRUE;
}


// --------------------------------------------------------------------------------
// kMuiDialogWindowResize
// --------------------------------------------------------------------------------

MoaBool DialogNotify::HandleResize( PMuiDialogWrapper, PMoaRect )
{
	return TRUE;
}


// --------------------------------------------------------------------------------
// kMuiDialogItemEnteringFocus/kMuiDialogItemLosingFocus
// --------------------------------------------------------------------------------

MoaBool DialogNotify::HandleFocus( PMuiDialogWrapper pDialog, MoaLong index, MoaBool entering )
{
	MoaBool returnVal = TRUE;

	if( !entering )
	{
		if( index == kMoreTextEdit )
		{
			MuiItemWrapper&		moreTextItem = pDialog->GetItem( kMoreTextEdit );
			// set the string size from the edit field
			moreTextItem.GetStringValue( mProps.watchString, sizeof( mProps.watchString ) );

		}
		else if( index == kHorizSubEdit )
		{
			MuiItemWrapper&		subEditItem = pDialog->GetItem( kHorizSubEdit );
			// set the string size from the edit field
			subEditItem.GetStringValue( mProps.editString, sizeof( mProps.editString ) );

		}
	}
	else
	{	
		// if the edit boxes gain the focus check the corresponding radio button
		if( index == kHorizSubEdit )
			HandleClick( pDialog, kHorizSubRadio );
		
		if( index == kMoreTextEdit )
			HandleClick( pDialog, kMoreTextCheckbox );

	}

	return returnVal;
}


MoaBool DialogNotify::HandleSelectionChange( PMuiDialogWrapper, MoaLong, void* )
{
	return TRUE;
}

// following method is needed for XSupport library in D11
MoaBool DialogNotify::HandleDoubleClick( PMuiDialogWrapper, MoaLong, void* )
{
	return TRUE;
}


void DialogNotify::ToggleCheckboxGroup( PMuiDialogWrapper pDialog, bool on )
{
	// toggle the item.
	MuiItemWrapper& radio = pDialog->GetItem( kVertRadioActivate );
	radio.SetValue( MoaLong( on ) );
	pDialog->ItemUpdate( kVertRadioActivate );
	
	// now do the check boxes
	for( MoaLong count = kVertCheckbox1; count <= kVertCheckbox3; count++ )
	{
		MuiItemWrapper& checkbox = pDialog->GetItem( count );

		checkbox.SetEnabled( on );

		pDialog->ItemUpdate( count );
	}
	
	// do custom, note this one is seperate because there are a bunch
	// of other items in between so doing a for loop would be bad here
	MuiItemWrapper& custombox = pDialog->GetItem( kMoreTextCheckbox );
	custombox.SetEnabled( on );
	pDialog->ItemUpdate( kMoreTextCheckbox );
}

// --------------------------------------------------------------------------------
// DoHelp
// --------------------------------------------------------------------------------
void DialogNotify::DoHelp( void )
{
}



// --------------------------------------------------------------------------------
// TestDialogState - this figures out if it is safe to bring down the dialog
// return true if it is okay to proceed
// --------------------------------------------------------------------------------
bool DialogNotify::TestDialogState()
{
	bool okayToClose	= true;
	PMuiDialogWrapper 	pDialog = GetDialogRef();
	PIMuiAlert			pAlert	= NULL;				// make sure this is null (for MoaReleaser)
	
	// don't allow them to record a font with no characters!
	MuiItemWrapper& activateRadio = pDialog->GetItem( kVertRadioActivate );
	MuiItemWrapper& check1		  = pDialog->GetItem( kVertCheckbox1 );
	MuiItemWrapper& check2		  = pDialog->GetItem( kVertCheckbox2 );
	MuiItemWrapper& check3	 	  = pDialog->GetItem( kVertCheckbox3 );
	MuiItemWrapper& moreText 	  = pDialog->GetItem( kMoreTextCheckbox );

	// make sure something is selected!
	if( activateRadio.GetBoolValue() )
	{
		if( !check1.GetBoolValue() && !check2.GetBoolValue() && !check3.GetBoolValue() )
		{
			if( moreText.GetBoolValue() )
			{
				// check to see if there is text in there!
				MuiItemWrapper& editBox = pDialog->GetItem( kMoreTextEdit );
				
				if( !::strcmp( editBox.GetTitle(), "" ) )
					okayToClose = false;
			}
			else
				okayToClose = false;
		}
	}
	
	if( !okayToClose )
	{
		// dump the dialog on our way out.
		MoaReleaser	releaseMe( (PIMoaUnknown&)pAlert );

		// put up alert explaining they need to select a character set to record
		MoaError err = XSupport::GetCallback()->MoaCreateInstance( &CLSID(CMuiAlert), &IID(IMuiAlert), (PMoaVoid*)&pAlert );
		if( err || !pAlert )
			kThrowMoaExceptionErr( CMoaBadInterface, err );
		
		pAlert->MovableAlert( "Select one of the checkboxes or the Deactivate radio button", "An Error Dialog",
							kMuiAlertButtons_Ok, kMuiAlertButtons_Ok, kMuiAlertIcon_Caution );
		
		// bust on out of here!
		return false;
	}
	
	return true;
}



void DialogNotify::UpdatePreview()
{
	// interpret value in the menu if not on "plain" item
	PMuiDialogWrapper 	pDialog    = GetDialogRef();
	MuiItemWrapper&		editItem   = pDialog->GetItem( kNewSpriteTextEdit );
	MuiItemWrapper&		sliderItem = pDialog->GetItem( kSlider );
	MuiItemWrapper&		checkboxRed	  = pDialog->GetItem( kVertCheckbox1 );
	MuiItemWrapper&		checkboxGreen = pDialog->GetItem( kVertCheckbox2 );
	MuiItemWrapper&		checkboxBlue  = pDialog->GetItem( kVertCheckbox3 );
	
	// set the text from the edit box
	editItem.GetStringValue( mProps.outputString, sizeof( mProps.outputString ) );

	// set the font size from the slider
	mProps.fontSize = (MoaShort)sliderItem.GetIntValue();
					
	// set color bits
	mProps.colorBits = 0;
	
	if( checkboxRed.GetBoolValue() )
		mProps.colorBits |= kShowRed;
	
	if( checkboxGreen.GetBoolValue() )
		mProps.colorBits |= kShowGreen;

	if( checkboxBlue.GetBoolValue() )
		mProps.colorBits |= kShowBlue;
	
	// update the preview.				
	if( mSpriteCallback )
		mSpriteCallback->InvalSpriteRect();
}

// EOF

