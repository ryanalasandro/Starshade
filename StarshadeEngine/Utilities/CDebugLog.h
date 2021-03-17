//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Utilities/CDebugLog.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CDEBUGLOG_H
#define CDEBUGLOG_H

#include "../Globals/CDefines.h"
#include "../Globals/CGlobals.h"
#include <cstdio>
#include <fstream>
#include <ctime>
#include <chrono>
#include <mutex>

#if TARGET_PLATFORM == PLATFORM_WINDOWS
	#include <Windows.h>
#endif

enum CHANNEL : u32 {
	CHANNEL_GENERAL = 0x0,
	CHANNEL_GAMEPLAY = 0x1,
	CHANNEL_PHYSICS = 0x2,
	CHANNEL_AUDIO = 0x4,
	CHANNEL_INPUT = 0x8,
	CHANNEL_RENDERING = 0x10,
	CHANNEL_IO = 0x20,
	CHANNEL_RESOURCES = 0x40,
	CHANNEL_NETWORK = 0x80,
	CHANNEL_MISCELLANEOUS = 0x100,
	CHANNEL_MEMORY = 0x200,
	CHANNEL_INTELLIGENCE = 0x400,
	CHANNEL_EVERYTHING = 0x7FF,
};

enum VERBOSITY : u32 {
	VERBOSITY_NONE = 0x0,
	VERBOSITY_ERRORS = 0x1,
	VERBOSITY_WARNINGS = 0x2,
	VERBOSITY_NOTICES = 0x4,
	VERBOSITY_EVERYTHING = 0x7,
};

namespace Util {
	const u32 g_channel = CHANNEL_EVERYTHING;
	const u32 g_verbosity = VERBOSITY_EVERYTHING;
	
#if TARGET_PLATFORM == PLATFORM_WINDOWS
	inline void Log(const wchar_t* msg) {
		OutputDebugStringW(msg);
		OutputDebugStringW(L"\n");
	}

	inline void DebugLog(const wchar_t* format, va_list argList) {
		static const u32 MAX_CHARS = 256;
		static thread_local wchar_t wbuffer[MAX_CHARS];
		const int c = vswprintf(wbuffer, MAX_CHARS, format, argList);

		OutputDebugStringW(wbuffer);
		OutputDebugStringW(L"\n");
	}

	inline void DebugLog(u32 verbosity, u32 channel, const wchar_t* format, ...) {
		if((g_verbosity & verbosity) != verbosity || (g_channel & channel) != channel) return;
		va_list argList;
		va_start(argList, format);
		DebugLog(format, argList);
		va_end(argList);
	}
#else
	inline void Log(const chX* msg) { }
	inline void DebugLog(const char* format, va_list argList) { }
	inline void DebugLog(VERBOSITY verbosity, CHANNEL channel, const char* format, ...) { }
#endif
};

#endif
