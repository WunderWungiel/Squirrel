/*
 ============================================================================
 Name		: SquirrelDummyView.h
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : Declares view class for application.
 ============================================================================
 */

#ifndef __SQUIRRELDUMMYVIEW_h__
#define __SQUIRRELDUMMYVIEW_h__

// INCLUDES
#include <aknview.h>


class CSquirrelDummyView : public CAknView
{
public:
	// New methods
	static CSquirrelDummyView* NewL();
	static CSquirrelDummyView* NewLC();

	/**
	 * ~CSquirrelDummyView
	 * Virtual Destructor.
	 */
	virtual ~CSquirrelDummyView();

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

private:
	// Constructors
	void ConstructL();

	/**
	 * CSquirrelDummyView.
	 * C++ default constructor.
	 */
	CSquirrelDummyView();
};

#endif // __SQUIRRELAPPVIEW_h__
// End of File
