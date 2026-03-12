/*
 ============================================================================
 Name		: SquirrelAppView.h
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __SQUIRRELHISTORYVIEW_h__
#define __SQUIRRELHISTORYVIEW_h__

// INCLUDES
#include <aknview.h>
#include <eiklbo.h>
#include "uicommons.h"
#include "HistoryStore.h"


// CLASS DECLARATION
class CSquirrelHistoryView : public CAknView, MEikListBoxObserver
{
public:
	// New methods
	static CSquirrelHistoryView* NewL();
	static CSquirrelHistoryView* NewLC();

	/**
	 * ~CSquirrelHistoryView
	 * Virtual Destructor.
	 */
	virtual ~CSquirrelHistoryView();

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

	void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane);

private:
	// Constructors
	void ConstructL();

	/**
	 * CSquirrelHistoryView.
	 * C++ default constructor.
	 */
	CSquirrelHistoryView();


private:
	TListBox<DOUBLE_STYLE> *iListbox;
public:
	CHistoryStore* iHistoryStore;
};

#endif // __SQUIRRELHISTORYVIEW_h__
// End of File
