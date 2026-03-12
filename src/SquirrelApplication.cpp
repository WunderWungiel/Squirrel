/*
 ============================================================================
 Name		: SquirrelApplication.cpp
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include "Squirrel.hrh"
#include "SquirrelDocument.h"
#include "SquirrelApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSquirrelApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CSquirrelApplication::CreateDocumentL()
	{
	// Create an Squirrel document, and return a pointer to it
	return CSquirrelDocument::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CSquirrelApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CSquirrelApplication::AppDllUid() const
	{
	// Return the UID for the Squirrel application
	return KUidSquirrelApp;
	}

// End of File
