/*
   ============================================================================
Name		: SquirrelDummyView.cpp
Author	  : JigokuMaster
Copyright   : Your copyright notice
Description : Application view implementation
============================================================================
*/

// INCLUDE FILES
#include <coemain.h>
#include <Squirrel.rsg>
#include "SquirrelAppUi.h"
#include "SquirrelDummyView.h"
#include "Squirrel.hrh"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSquirrelDummyView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelDummyView* CSquirrelDummyView::NewL()
{
    CSquirrelDummyView* self = CSquirrelDummyView::NewLC();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelDummyView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelDummyView* CSquirrelDummyView::NewLC()
{
    CSquirrelDummyView* self = new (ELeave) CSquirrelDummyView;
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

// -----------------------------------------------------------------------------
// CSquirrelDummyView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSquirrelDummyView::ConstructL()
{
    BaseConstructL(R_DUMMY_VIEW);
}

// -----------------------------------------------------------------------------
// CSquirrelDummyView::CSquirrelDummyView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSquirrelDummyView::CSquirrelDummyView()
{
    // No implementation required
}

// -----------------------------------------------------------------------------
// CSquirrelDummyView::~CSquirrelDummyView()
// Destructor.
// -----------------------------------------------------------------------------
//
CSquirrelDummyView::~CSquirrelDummyView()
{
   

}


TUid CSquirrelDummyView::Id() const 
{ 
    return TUid::Uid(EDummyView); 
}

void CSquirrelDummyView::HandleCommandL( TInt aCommand )
{
    AppUi()->HandleCommandL(aCommand);
}

void CSquirrelDummyView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
	TUid /*aCustomMessageId*/,
	const TDesC8& /*aCustomMessage*/ )
{
}

void CSquirrelDummyView::DoDeactivate()
{   
}

// End of File
