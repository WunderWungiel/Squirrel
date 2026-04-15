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
#include <aknmessagequerydialog.h>
#include <f32file.h>
#include "SquirrelAppUi.h"
#include "SquirrelHistoryView.h"
#include "SquirrelModel.h"
#include "Squirrel.rsg"
#include "Squirrel.hrh"

_LIT(KHistoryStoreFileName, "History.dat");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSquirrelHistoryView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelHistoryView* CSquirrelHistoryView::NewL()
{
    CSquirrelHistoryView* self = CSquirrelHistoryView::NewLC();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelHistoryView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelHistoryView* CSquirrelHistoryView::NewLC()
{
    CSquirrelHistoryView* self = new (ELeave) CSquirrelHistoryView;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelHistoryView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSquirrelHistoryView::ConstructL()
{
    BaseConstructL(R_HISTORY_VIEW);

    TFileName fp;
    GetPrivateFilePathL(CCoeEnv::Static()->FsSession(), fp, KHistoryStoreFileName);
    iHistoryStore = CHistoryStore::OpenLC(fp);
}

// -----------------------------------------------------------------------------
// CSquirrelHistoryView::SquirrelAboutView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSquirrelHistoryView::CSquirrelHistoryView()
{
    iListbox = NULL;
    iHistoryStore = NULL;
}

// -----------------------------------------------------------------------------
// CSquirrelHistoryView::~SquirrelAboutView()
// Destructor.
// -----------------------------------------------------------------------------
//
CSquirrelHistoryView::~CSquirrelHistoryView()
{
   
    if ( iListbox )
    {
	AppUi()->RemoveFromStack( iListbox );
	delete iListbox;
    }

    if ( iHistoryStore)
    {
	delete iHistoryStore;
    }

}


TUid CSquirrelHistoryView::Id() const 
{ 
    return TUid::Uid(EHistoryView); 
}

void CSquirrelHistoryView::HandleCommandL( TInt aCommand )
{
    
    if (iListbox && iHistoryStore)
    {
	TInt itemIndex = iListbox->Listbox()->CurrentItemIndex();

	if (aCommand == ECmdDeleteHistory)
	{
	    iHistoryStore->RemoveItemL(itemIndex);
	    iListbox->RemoveItemL(itemIndex);
	    iListbox->Listbox()->SetCurrentItemIndex(0);
	    iListbox->Listbox()->DrawNow();
	}

	else if (aCommand == ECmdCleanHistory)
	{
	    iHistoryStore->RemoveItemsL();
	    iListbox->RemoveItemsL();
	}

    }
    
    AppUi()->HandleCommandL(aCommand); 
}


void CSquirrelHistoryView::HandleViewRectChange()
{
    if (iListbox)
    {
	iListbox->SetRect(ClientRect());
    }   
}


void CSquirrelHistoryView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
	TUid /*aCustomMessageId*/,
	const TDesC8& /*aCustomMessage*/ )
{
    SetAppTitleL(NULL, R_HISTORY_TITLE);
    iListbox = new (ELeave) TListBox<DOUBLE_STYLE>;
    iListbox->ConstructL(NULL, ClientRect(), R_HISTORYVIEW_LISTBOX);
    iListbox->Listbox()->ItemDrawer()->FormattedCellData()->EnableMarqueeL(ETrue);
    iListbox->SetObserver(this);
    iListbox->SetMopParent(this);
    AppUi()->AddToStackL(*this, iListbox); 


    iHistoryStore->GetItemsL(*iListbox->ItemsArray(), _L("\t"));

    iListbox->Listbox()->HandleItemAdditionL();

    Cba()->MakeCommandVisible(EAknSoftkeyOptions, !iHistoryStore->IsEmpty());

}

void CSquirrelHistoryView::DoDeactivate()
{

    if ( iListbox )
    {
	AppUi()->RemoveFromViewStack( *this, iListbox );
	delete iListbox;
	iListbox = NULL;
    }    
}


void CSquirrelHistoryView::HandleListBoxEventL(CEikListBox* aListBox , TListBoxEvent aEventType)
{
    if (iListbox && (aEventType == EEventEnterKeyPressed || aEventType == EEventItemClicked))
    {

	TFileName fn;
	iHistoryStore->GetItem(fn, iListbox->Listbox()->CurrentItemIndex());

	TPtrC8 msg((const TUint8*)fn.Ptr(), fn.Size());

	AppUi()->ActivateLocalViewL(TUid::Uid(EDecoderView), TUid::Uid(ECmdDecodeImageFile), msg);
    }
}

void CSquirrelHistoryView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane)
{
    if (!iHistoryStore || aResourceId != R_HISTORYVIEW_MENU) return;

    TBool dimm = iHistoryStore->IsEmpty();

    aMenuPane->SetItemDimmed(ECmdDeleteHistory, dimm);

    aMenuPane->SetItemDimmed(ECmdCleanHistory, dimm);

}

// End of File
