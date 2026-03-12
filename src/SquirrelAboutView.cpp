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
//#include <aknnotewrappers.h>
//#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <utf.h> // CnvUtfConverter
#include "SquirrelAppUi.h"
#include "SquirrelAboutView.h"
#include "Squirrel.rsg"
#include "Squirrel.hrh"
#include "SquirrelModel.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSquirrelAboutView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelAboutView* CSquirrelAboutView::NewL()
{
    CSquirrelAboutView* self = CSquirrelAboutView::NewLC();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelAboutView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelAboutView* CSquirrelAboutView::NewLC()
{
    CSquirrelAboutView* self = new (ELeave) CSquirrelAboutView;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelAboutView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSquirrelAboutView::ConstructL()
{
    BaseConstructL(R_ABOUT_VIEW);
}

// -----------------------------------------------------------------------------
// CSquirrelAboutView::SquirrelAboutView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSquirrelAboutView::CSquirrelAboutView()
{
    iListbox = NULL;
    iOSNoticeContent = NULL;

}

// -----------------------------------------------------------------------------
// CSquirrelAboutView::~SquirrelAboutView()
// Destructor.
// -----------------------------------------------------------------------------
//
CSquirrelAboutView::~CSquirrelAboutView()
{
   
    if ( iListbox )
    {
	AppUi()->RemoveFromStack( iListbox );
	delete iListbox;
    } 

    if (iOSNoticeContent) delete iOSNoticeContent;
    iOSNoticeContent = NULL;
	
}


TUid CSquirrelAboutView::Id() const 
{ 
    return TUid::Uid(EAboutView); 
}


void CSquirrelAboutView::HandleCommandL( TInt aCommand )
{
    AppUi()->HandleCommandL(aCommand); 
}



void CSquirrelAboutView::HandleViewRectChange()
{
    if (iListbox)
    {
	iListbox->SetRect(ClientRect());
    }   
}


void CSquirrelAboutView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
	TUid /*aCustomMessageId*/,
	const TDesC8& /*aCustomMessage*/ )
{

    iListbox = new (ELeave) TListBox<DOUBLE_STYLE>;
    iListbox->ConstructL(NULL, ClientRect(), R_ABOUTVIEW_LISTBOX);
    iListbox->SetObserver(this);

    iListbox->SetMopParent(this);
    
    Cba()->MakeCommandVisible(EAknSoftkeyOptions, EFalse);

    AppUi()->AddToStackL(*this, iListbox);    
}




void CSquirrelAboutView::DoDeactivate()
{

    if ( iListbox )
    {
	AppUi()->RemoveFromViewStack( *this, iListbox );
	delete iListbox;
	iListbox = NULL;
    }    
}


void CSquirrelAboutView::HandleListBoxEventL(CEikListBox* aListBox , TListBoxEvent aEventType)
{
    if (iListbox && (aEventType == EEventEnterKeyPressed || aEventType == EEventItemDoubleClicked))
    {

	TInt itemIndex = iListbox->Listbox()->CurrentItemIndex();
	if (itemIndex == 3) ShowOSNoticeL();

    }
}


void CSquirrelAboutView::ShowOSNoticeL()
{

    if (!iOSNoticeContent)
    {
	RFile file;
	_LIT(KFileName, "opensource.txt");
	TInt fileSize;

	User::LeaveIfError(file.Open(iCoeEnv->FsSession(), KFileName, EFileRead));
	//CleanupClosePushL(file);
	User::LeaveIfError(file.Size(fileSize));
	
	HBufC8* fileContentBuf = HBufC8::NewLC(fileSize);
	TPtr8 fileContentBufPtr(fileContentBuf->Des());
        User::LeaveIfError(file.Read(fileContentBufPtr));
	//CleanupStack::PopAndDestroy(1); // file
	file.Close();

	iOSNoticeContent = HBufC::NewL(fileSize*2);
	TPtr buf(iOSNoticeContent->Des());
	TInt err = CnvUtfConverter::ConvertToUnicodeFromUtf8(buf, fileContentBufPtr);
	CleanupStack::PopAndDestroy(fileContentBuf);

	if (err != KErrNone){
	    ShowErrorL(err);
	    return;
	}
	
    }

    CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog;

    dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
   
    if (iOSNoticeContent && iOSNoticeContent->Length() > 0)
    {
	dlg->SetMessageTextL(*iOSNoticeContent);
    }
    else
    {
	dlg->SetMessageTextL(_L("empty."));
    }  
    dlg->RunLD();

}


// End of File
