//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Globals/CDefine.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CDEFINE_H
#define CDEFINE_H

//-------------------------------------------------------------------------------------------------
// Platform architecture.
//-------------------------------------------------------------------------------------------------

// Check windows
#if _WIN32 || _WIN64
	#if _WIN64
		#define PLATFORM_64_BIT
	#else
		#define PLATFORM_32_BIT
	#endif
#endif

// Check GCC
#if __GNUC__
	#if __x86_64__ || __ppc64__
		#define PLATFORM_64_BIT
	#else
		#define PLATFORM_32_BIT
	#endif
#endif


//-------------------------------------------------------------------------------------------------
// Engine macros.
//-------------------------------------------------------------------------------------------------

// Platform.

#ifndef PLATFORM_WINDOWS
#define PLATFORM_WINDOWS 0
#endif
#ifndef PLATFORM_MAC
#define PLATFORM_MAC 1
#endif
#ifndef PLATFORM_LINUX
#define PLATFORM_LINUX 2
#endif

#ifndef TARGET_PLATFORM
#define TARGET_PLATFORM PLATFORM_WINDOWS
#endif

// Graphics API.

#ifndef NO_GFX
#define NO_GFX -1
#endif
#ifndef API_DX12
#define API_DX12 0
#endif
#ifndef API_VULKAN
#define API_VULKAN 1
#endif
#ifndef API_METAL
#define API_METAL 2
#endif

#ifndef GFX_API
#define GFX_API API_DX12
#endif

#endif
