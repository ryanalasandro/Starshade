//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CAppBase.h
//
//-------------------------------------------------------------------------------------------------

#include "CAppBase.h"
#include "CPlatform.h"
#include "CPanel.h"
#include "../Graphics/CGraphicsAPI.h"
#include "../Utilities/CMemory.h"
#include "../Utilities/CDebugLog.h"
#include "../Factory/CEngineFactory.h"

namespace App {
	CAppBase::CAppBase() : 
		m_pPlatform(nullptr),
		m_pPanel(nullptr),
		m_pGraphics(nullptr) {
	}

	CAppBase::~CAppBase() {
	}

	void CAppBase::Create() {
		Setup();

		//
		// Initialization stage.
		//

		Util::CMemory::Instance().Initialize();

		Util::CMemory::Instance().MoveToNextStage(); // To initialization stage.
		InitStage();

		//
		// Platform stage.
		//

		Util::CMemory::Instance().MoveToNextStage(); // To platform stage.
		m_pPlatform = Factory::CEngineFactory::Instance().GetPlatform(true);
		m_pPlatform->PreInitialize();
		
		m_pPanel = Factory::CEngineFactory::Instance().CreatePanel();
		m_pPanel->SetOnMoveAction(std::bind(&CAppBase::OnMove, this, std::placeholders::_1));
		m_pPanel->SetOnResizeAction(std::bind(&CAppBase::OnResize, this, std::placeholders::_1));

		PlatformStage();
		
		m_pPanel->Initialize();

		m_pPlatform->SetMainPanel(m_pPanel);
		m_pPlatform->Initialize();

		//
		// Graphics stage.
		//

		Util::CMemory::Instance().MoveToNextStage(); // To graphics stage.
		m_pGraphics = Factory::CEngineFactory::Instance().GetGraphics(true);
		m_pGraphics->PreInitialize();

		GraphicsStage();
		
		m_pGraphics->SetPanel(m_pPanel);
		m_pGraphics->Initialize();

		//
		// Asset stage.
		//

		Util::CMemory::Instance().MoveToNextStage(); // To assets stage.
		AssetStage();
	}

	int CAppBase::Run() {
		int exitCode = 0;
		while(m_pPlatform->Update(exitCode)) {
			Update();
			Render();
			m_pGraphics->Render([](){});
		}

		return exitCode;
	}

	void CAppBase::Destroy() {
		Release();
		m_pGraphics->Release();
		m_pPlatform->Release();
		m_pPanel->Release();
	}

	void CAppBase::OnMove(const class CPanel* pPanel) {
		Util::DebugLog(VERBOSITY_EVERYTHING, CHANNEL_EVERYTHING, L"OnMove()");
	}

	void CAppBase::OnResize(const class CPanel* pPanel) {
		Util::DebugLog(VERBOSITY_EVERYTHING, CHANNEL_EVERYTHING, L"OnResize()");
		m_pGraphics->AdjustToResize();

		m_pGraphics->FinalizeResize();
	}
};
