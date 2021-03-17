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
	CApplication::CApplication() :
		m_pMainPanel(nullptr) {
	}

	CApplication::~CApplication() {
	}

	void CApplication::Setup() {
		Util::CMemory::Instance().SetMainBlockSize(Util::MemMB(32));
	}

	void CApplication::InitStage() {
	}

	void CApplication::PlatformStage() {
		m_pMainPanel = Factory::CEngineFactory::Instance().CreatePanel();
		m_pMainPanel->SetAllowDoubleClick(true);
		m_pMainPanel->SetAllowResize(false);
		m_pMainPanel->SetTargetDimensions(Math::VectorInt2(1280, 720));
		m_pMainPanel->SetTitle(L"App - Graphics Init");
		m_pMainPanel->Initialize();

		m_pPlatform->SetMainPanel(m_pMainPanel);
	}

	void CApplication::GraphicsStage() {
		m_pGraphics->SetPanel(m_pMainPanel);
	}

	void CApplication::AssetStage() {
	}

	void CApplication::Update() {
	}

	void CApplication::Render() {
	}

	void CApplication::Release() {
		m_pMainPanel->Release();
	}
};
