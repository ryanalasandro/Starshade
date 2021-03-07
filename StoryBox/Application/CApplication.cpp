//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Windows Application: Story Box
//
// File: Application/CApplication.cpp
//
//-------------------------------------------------------------------------------------------------

#include "CApplication.h"
#include <Application/CPanel.h>
#include <Application/CPlatform.h>
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
		m_pPlatform->SetMainPanel(m_pMainPanel);
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
		m_pMainPanel->Release();
	}
};
