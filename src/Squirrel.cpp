/*
 ============================================================================
 Name		: Squirrel.cpp
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : Main application class
 ============================================================================
 */

// INCLUDE FILES
#include <eikstart.h>
#include "SquirrelApplication.h"

LOCAL_C CApaApplication* NewApplication()
	{
	return new CSquirrelApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}

