/*
   ============================================================================
Name		: SquirrelAppView.cpp
Author	  : JigokuMaster
Copyright   : Your copyright notice
Description : Application view implementation
============================================================================
*/

// INCLUDE FILES
#include <coemain.h>
#include <Squirrel.rsg>
#include "SquirrelAppUi.h"
#include "SquirrelAppView.h"
#include "Squirrel.hrh"
#include "Squirrel.mbg"

#define KNumberOfItems 5

_LIT(KMainViewIconsFile, "\\resource\\apps\\Squirrel_ui.mif");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSquirrelAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelAppView* CSquirrelAppView::NewL()
{
    CSquirrelAppView* self = CSquirrelAppView::NewLC();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelAppView* CSquirrelAppView::NewLC()
{
    CSquirrelAppView* self = new (ELeave) CSquirrelAppView;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSquirrelAppView::ConstructL()
{
    iGridListbox = NULL;
    BaseConstructL(R_MAIN_VIEW);
}

// -----------------------------------------------------------------------------
// CSquirrelAppView::CSquirrelAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSquirrelAppView::CSquirrelAppView()
{
    // No implementation required
}

// -----------------------------------------------------------------------------
// CSquirrelAppView::~CSquirrelAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CSquirrelAppView::~CSquirrelAppView()
{
   
    if ( iGridListbox )
    {
	AppUi()->RemoveFromStack( iGridListbox );
	delete iGridListbox;
    } 
	
}


TUid CSquirrelAppView::Id() const 
{ 
    return TUid::Uid(EMainView); 
}

void CSquirrelAppView::HandleCommandL( TInt aCommand )
{
    switch( aCommand )
    {
	default:
	    AppUi()->HandleCommandL(aCommand);  
	    break;
    }
}

void CSquirrelAppView::HandleViewRectChange()
{
    if (iGridListbox)
    {
	iGridListbox->SetRect(ClientRect());
    }   
}

void CSquirrelAppView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
	TUid /*aCustomMessageId*/,
	const TDesC8& /*aCustomMessage*/ )
{

    iGridListbox = new (ELeave)CGridListBox;
    iGridListbox->ConstructL(NULL, ClientRect() ,R_MAINVIEW_GRID);

    iGridListbox->SetFont(iEikonEnv->DenseFont());
    iGridListbox->SetupGrid(KNumberOfItems, 2, 2);
    int icon_ids[] = {
	EMbmSquirrelLoad,
	EMbmSquirrelHistory,	
	EMbmSquirrelDecode,
	EMbmSquirrelGenerate,
	EMbmSquirrelAbout
    };

    for (TInt i = 0; i < KNumberOfItems; i++)
    {
	iGridListbox->AddIconL(KMainViewIconsFile, icon_ids[i]);
    }


    iGridListbox->Grid()->SetListBoxObserver(this);

    iGridListbox->SetMopParent(this);

    AppUi()->AddToStackL(*this, iGridListbox); 

    Cba()->MakeCommandVisible(EAknSoftkeyOptions, EFalse);

}

void CSquirrelAppView::DoDeactivate()
{

    if ( iGridListbox )
    {
	AppUi()->RemoveFromViewStack( *this, iGridListbox );
	delete iGridListbox;
	iGridListbox = NULL;
    }    
}


void CSquirrelAppView::HandleListBoxEventL(CEikListBox* aListBox , TListBoxEvent aEventType)
{
    if (iGridListbox && (aEventType == EEventEnterKeyPressed || aEventType == EEventItemDoubleClicked))
    {

	TInt viewIndex = iGridListbox->Grid()->CurrentItemIndex() + 2;
	
	if (viewIndex == EDecoderView)
	{
	    AppUi()->ActivateLocalViewL(TUid::Uid(EDecoderView), TUid::Uid(ECmdOpenImage), KNullDesC8);

	}

	else AppUi()->ActivateLocalViewL(TUid::Uid(viewIndex));

    }
}


// End of File
