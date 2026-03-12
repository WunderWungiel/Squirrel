/*
 ============================================================================
 Name		: SquirrelAppView.h
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __SQUIRRELAPPVIEW_h__
#define __SQUIRRELAPPVIEW_h__

// INCLUDES
#include <aknview.h>
#include <eiklbo.h>
#include "uicommons.h"


// CLASS DECLARATION
class CSquirrelAppView : public CAknView, 
MEikListBoxObserver
{
public:
	// New methods
	static CSquirrelAppView* NewL();
	static CSquirrelAppView* NewLC();

	/**
	 * ~CSquirrelAppView
	 * Virtual Destructor.
	 */
	virtual ~CSquirrelAppView();

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

	void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType);

private:
	// Constructors
	void ConstructL();

	/**
	 * CSquirrelAppView.
	 * C++ default constructor.
	 */
	CSquirrelAppView();

private:
	CGridListBox* iGridListbox;
};

#endif // __SQUIRRELAPPVIEW_h__
// End of File
