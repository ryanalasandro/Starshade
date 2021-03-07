//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Factory/CEngineFactory.cpp
//
//-------------------------------------------------------------------------------------------------

#include "CEngineFactory.h"
#include "../Globals/CDefines.h"
#include "../Utilities/CMemory.h"
#include <cstring>

#if TARGET_PLATFORM == PLATFORM_WINDOWS
#include "../Application/CPanelWin.h"
#include "../Application/CPlatformWin.h"
#elif TARGET_PLATFORM == PLATFORM_LINUX
#elif TARGET_PLATFORM == PLATFORM_MAC
#endif

#if GFX_API == NO_GFX
#elif GFX_API == API_DX12
#elif GFX_API == API_VULKAN
#elif GFX_API == API_METAL
#endif

namespace Factory {
	CEngineFactory::CEngineFactory() {
		memset(this, 0, sizeof(CEngineFactory));
	}

	CEngineFactory::~CEngineFactory() {
	}

	App::CPanel* CEngineFactory::CreatePanel(bool bFromStack) {
		#if TARGET_PLATFORM == PLATFORM_WINDOWS
			return Util::CMemory::Instance().PullNew<App::CPanelWin>(4, bFromStack);
		#elif TARGET_PLATFORM == PLATFORM_LINUX
			return nullptr;
		#elif TARGET_PLATFORM == PLATFORM_MAC
			return nullptr;
		#endif
	}

	App::CPlatform* CEngineFactory::GetPlatform(bool bAllowCreation, bool bFromStack) {
		if(m_pPlatform == nullptr && bAllowCreation) {
		#if TARGET_PLATFORM == PLATFORM_WINDOWS
			m_pPlatform = Util::CMemory::Instance().PullNew<App::CPlatformWin>(4, bFromStack);
		#elif TARGET_PLATFORM == PLATFORM_LINUX
		#elif TARGET_PLATFORM == PLATFORM_MAC
		#endif
		}

		return m_pPlatform;
	}
};
