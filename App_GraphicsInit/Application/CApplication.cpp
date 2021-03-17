//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Windows Application: App_GraphicsInit
//
// File: Application/CApplication.cpp
//
//-------------------------------------------------------------------------------------------------

#include "CApplication.h"
#include <Application/CPanel.h>
#include <Application/CPlatform.h>
#include <Graphics/CGraphicsAPI.h>
#include <Factory/CEngineFactory.h>
#include <Utilities/CMemory.h>
#include <Utilities/CMemoryUtil.h>

namespace App {
	CApplication::CApplication() {
	}

	CApplication::~CApplication() {
	}

	void CApplication::Setup() {
		Util::CMemory::Instance().SetMainBlockSize(Util::MemMB(32));
	}

	void CApplication::InitStage() {
	}

	void CApplication::PlatformStage() {
		Panel()->SetAllowDoubleClick(true);
		Panel()->SetAllowResize(true);
		Panel()->SetTargetDimensions(Math::VectorInt2(1280, 720));
		Panel()->SetTitle(L"App - Graphics Init");
	}

	void CApplication::GraphicsStage() {
	}

	void CApplication::AssetStage() {
	}

	void CApplication::Update() {
	}

	void CApplication::Render() {
	}

	void CApplication::Release() {
	}
};
