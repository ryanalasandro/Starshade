//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Graphics/CGraphicsAPI.cpp
//
//-------------------------------------------------------------------------------------------------

#include "CGraphicsAPI.h"

namespace Graphics {
	CGraphicsAPI::CGraphicsAPI() :
		m_bTearingSupport(false),
		m_bRaytracingSupport(false),
		m_bufferCount(2),
		m_vBlanks(0),
		m_bgColor(0.0f, 1.0f),
		m_pPanel(nullptr) {
	}

	CGraphicsAPI::~CGraphicsAPI() {
	}

	void CGraphicsAPI::PreInitialize() {
	}

	void CGraphicsAPI::Initialize() {
	}

	void CGraphicsAPI::Release() {
	}
};
