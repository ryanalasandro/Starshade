//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Utilities/CMemoryUtil.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CMEMORYUTIL_H
#define CMEMORYUTIL_H

namespace Util {
	const size_t g_oneKB = 1024;
	const size_t g_oneMB = g_oneKB * 1024;
	const size_t g_oneGB = g_oneMB * 1024;

	inline constexpr size_t MemKB(size_t kb) { return g_oneKB * kb; }
	inline constexpr size_t MemMB(size_t mb) { return g_oneMB * mb; }
	inline constexpr size_t MemGB(size_t gb) { return g_oneGB * gb; }
};

#endif
