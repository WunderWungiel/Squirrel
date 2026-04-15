/*
 ============================================================================
 Name		: SquirrelDecoderView.h
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __SQUIRRELDECODERVIEW_h__
#define __SQUIRRELDECODERVIEW_h__

// INCLUDES
#include <aknview.h>
#include <eikimage.h>
#include <eiklabel.h>
#include "uicommons.h"
#include "ImageHandler.h"
#include "SquirrelModel.h"

// CLASS DECLARATION
class CSquirrelDecoderView : public CAknView
{
public:
	// New methods
	static CSquirrelDecoderView* NewL();
	static CSquirrelDecoderView* NewLC();

	/**
	 * ~CSquirrelDecoderView
	 * Virtual Destructor.
	 */
	virtual ~CSquirrelDecoderView();

public:
    
        /**
        * From CAknView.
        * Returns views id.
        *
        * @return Id for this view.
        */
        TUid Id() const;
    
        /**
        * From CAknView.
        * Handles commands
        *
        * @param aCommand A command to be handled.
        */
        void HandleCommandL( TInt aCommand );
 	
	void HandleViewRectChange();

   
        /**
        * From CAknView.
        * Activates view
        *
        * @param aPrevViewId Specifies the view previously active.
        * @param aCustomMessageId Specifies the message type.
        * @param aCustomMessage The activation message.
        */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );
    
        /**
        * From CAknView.
        * Deactivates view.
        */
        void DoDeactivate();

	void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane);

	//void HandleForegroundEventL(TBool aForeground);

private:
	// Constructors
	/**
	 * CSquirrelDecoderView.
	 * C++ default constructor.
	 */

	CSquirrelDecoderView();

	void ConstructL();

private:
	// show filebrowser dialog
	void SelectDriveRootL(TBool aCalledFromMenu=EFalse);
	void SelectImageL(const TDesC& aPath);
	// show previous view
	void ActivatePrevViewL();

public:
	CDecoderModel* iDecoderModel;
private:
	TVwsViewId iPrevViewId;
};

#endif // __SQUIRRELDECODERVIEW_h__
// End of File
