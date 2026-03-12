/*
 ============================================================================
 Name		: SquirrelDocument.cpp
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : CSquirrelDocument implementation
 ============================================================================
 */

// INCLUDE FILES
#include "SquirrelAppUi.h"
#include "SquirrelDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSquirrelDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelDocument* CSquirrelDocument::NewL(CEikApplication& aApp)
	{
	CSquirrelDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CSquirrelDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSquirrelDocument* CSquirrelDocument::NewLC(CEikApplication& aApp)
	{
	CSquirrelDocument* self = new (ELeave) CSquirrelDocument(aApp);

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CSquirrelDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSquirrelDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CSquirrelDocument::CSquirrelDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CSquirrelDocument::CSquirrelDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CSquirrelDocument::~CSquirrelDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CSquirrelDocument::~CSquirrelDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CSquirrelDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CSquirrelDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new (ELeave) CSquirrelAppUi;
	}

// End of File
