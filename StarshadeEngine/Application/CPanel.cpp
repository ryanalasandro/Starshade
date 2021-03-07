//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPanel.cpp
//
//-------------------------------------------------------------------------------------------------

#include "CPanel.h"
#include "CPlatform.h"
#include "../Factory/CEngineFactory.h"

namespace App {
	CPanel::CPanel() : 
		m_bAllowDoubleClick(false),
		m_title(nullptr),
		m_pParent(nullptr),
		m_pPlatform(nullptr) {
	}

	CPanel::~CPanel() {
	}

	void CPanel::Initialize() {
		m_pPlatform = Factory::CEngineFactory::Instance().GetPlatform();
	}
};
