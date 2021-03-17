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
		m_bAllowResize(false),
		m_bCursorInPanel(false),
		m_screenMode(PSM_WINDOWED),
		m_screenState(PSS_NORMAL),
		m_position(0, 0),
		m_targetDim(1280, 720),
		m_minDim(640, 360),
		m_bounds(0, 0, 1280, 720),
		m_monitorBounds(0, 0, 0, 0),
		m_title(nullptr),
		m_funcOnMove(nullptr),
		m_funcOnResize(nullptr),
		m_pParent(nullptr),
		m_pPlatform(nullptr) {
	}

	CPanel::~CPanel() {
	}

	void CPanel::Initialize() {
		m_pPlatform = Factory::CEngineFactory::Instance().GetPlatform();
	}
	
	void CPanel::OnMove() {
		if(m_funcOnMove) m_funcOnMove(this);
	}

	void CPanel::OnResize() {
		if(m_funcOnResize) m_funcOnResize(this);
	}
};
