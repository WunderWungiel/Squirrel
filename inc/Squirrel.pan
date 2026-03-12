/*
 ============================================================================
 Name		: Squirrel.pan
 Author	  : JigokuMaster
 Copyright   : Your copyright notice
 Description : This file contains panic codes.
 ============================================================================
 */

#ifndef __SQUIRREL_PAN__
#define __SQUIRREL_PAN__
#include <e32std.h>

/** Squirrel application panic codes */
enum TSquirrelPanics
	{
	ESquirrelUi = 1
	// add further panics here
	};

inline void Panic(TSquirrelPanics aReason)
	{
	_LIT(applicationName, "Squirrel");
	User::Panic(applicationName, aReason);
	}

#endif // __SQUIRREL_PAN__
