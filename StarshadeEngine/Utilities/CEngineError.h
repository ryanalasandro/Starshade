//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Utilities/CEngineError.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CENGINEERROR_H
#define CENGINEERROR_H

#include "../Globals/CGlobals.h"
#include "../Globals/CDefines.h"
#if TARGET_PLATFORM == PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include <stdlib.h>
#include <assert.h>

#if !defined(_DEBUG)
#include <sstream>
#endif

namespace Err {
	// Engine error codes. (0x0 - 0x0FFFFFFF)
	enum {
		NONE = 0x0,
		UNKNOWN = 0xFFFFFFFF,

		// Platform error codes(0x1 - 0x7FFFF)
			// Windows error codes(0x1 - 0x3FFF)
			PLATFORM_WIN_FAILED_TO_CREATE_WINDOW = 0x1,
	};
}

const wchar_t* GENERIC_FATAL_ERROR_MESSAGE = L"There Was A Fatal Error! Error Code: ";

// Function for posting a message box on non-debug builds when a assert fails.
inline void AssertMessage(int code) {
#if !defined(_DEBUG)
	#if TARGET_PLATFORM == PLATFORM_WINDOWS
		#ifdef UNICODE
			std::wostringstream stream;
			stream << GENERIC_FATAL_ERROR_MESSAGE;
			stream << "0x" << std::hex << code;
			MessageBox(nullptr, stream.str().c_str(), L"Fatal Error!", MB_OK);
		#else
			std::ostringstream stream;
			stream << GENERIC_FATAL_ERROR_MESSAGE;
			stream << "0x" << std::hex << code;
			MessageBox(nullptr, stream.str().c_str(), "Fatal Error!", MB_OK);
		#endif
	#else
	#endif
#endif

	exit(code);
}

#define AssertCode(cond, code)[](bool b, int c){ if(!b) { assert(false); AssertMessage(c); } }(cond, code) 
#define Assert(cond) { AssertCode(cond, Err::UNKNOWN); }

#if TARGET_PLATFORM == PLATFORM_WINDOWS
#define AssertHR(hr) { assert(SUCCEEDED(hr)); }
#endif

#endif
