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
			// Generic error codes(0x1 - 0x3FFF)
			// Windows error codes(0x4000 - 0x7FFF)
			PLATFORM_WIN_FAILED_TO_CREATE_WINDOW = 0x4000,

		// Graphics API error codes(0x80000 - 0xFFFFF)
			// Generic API error codes(0x80000 - 0x83FFF)
			// DX12 API error codes(0x84000 - 0x87FFF)
			GRAPHICS_DX12_FAILED_TO_CREATE_DXGI_FACTORY = 0x84000,
			GRAPHICS_DX12_FAILED_TO_CREATE_DEVICE = 0x84001,
			GRAPHICS_DX12_FAILED_TO_CREATE_SWAPCHAIN = 0x84002,
			GRAPHICS_DX12_FAILED_TO_CONVERT_SWAPCHAIN = 0x84003,
			GRAPHICS_DX12_FAILED_TO_GET_VALID_ADAPTER = 0x84004,
	};
}

const wchar_t GENERIC_FATAL_ERROR_MESSAGE[] = L"There Was A Fatal Error! Error Code: ";

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
#define AssertHRCode(hr, code)[](HRESULT b, int c){ if(FAILED(b)) { assert(false); AssertMessage(c); } }(hr, code)
#define AssertHR(hr) { assert(SUCCEEDED(hr)); }
#endif

#endif
