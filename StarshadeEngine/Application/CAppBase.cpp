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
#include "../Utilities/CMemory.h"
#include "../Factory/CEngineFactory.h"

namespace App {
	CAppBase::CAppBase() : m_pPlatform(nullptr) {
	}

	CAppBase::~CAppBase() {
	}

	void CAppBase::Create() {
		Setup();

		Util::CMemory::Instance().Initialize();

		Util::CMemory::Instance().MoveToNextStage(); // To initialization stage.
		InitStage();

		Util::CMemory::Instance().MoveToNextStage(); // To platform stage.
		m_pPlatform = Factory::CEngineFactory::Instance().GetPlatform(true);
		PlatformStage();
		m_pPlatform->Initialize();

		Util::CMemory::Instance().MoveToNextStage(); // To graphics stage.
		GraphicsStage();

		Util::CMemory::Instance().MoveToNextStage(); // To assets stage.
		AssetStage();
	}

	int CAppBase::Run() {
		while(true) {
			Update();
			Render();
		}

		return 0;
	}

	void CAppBase::Destroy() {
		Release();
	}
};
