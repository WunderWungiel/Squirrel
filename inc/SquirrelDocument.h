/*
 ============================================================================
 Name		: SquirrelDocument.h
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : Declares document class for application.
 ============================================================================
 */

#ifndef __SQUIRRELDOCUMENT_h__
#define __SQUIRRELDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CSquirrelAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CSquirrelDocument application class.
 * An instance of class CSquirrelDocument is the Document part of the
 * AVKON application framework for the Squirrel example application.
 */
class CSquirrelDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CSquirrelDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CSquirrelDocument.
	 */
	static CSquirrelDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CSquirrelDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CSquirrelDocument.
	 */
	static CSquirrelDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CSquirrelDocument
	 * Virtual Destructor.
	 */
	virtual ~CSquirrelDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CSquirrelAppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CSquirrelDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CSquirrelDocument(CEikApplication& aApp);

	};

#endif // __SQUIRRELDOCUMENT_h__
// End of File
