//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPlatform.cpp
//
//-------------------------------------------------------------------------------------------------

#include "CPlatform.h"
#include "../Utilities/CEngineError.h"

/*
	Platform holds application systems on the OS GUI API side of things.
	This means that panels are located here.
*/

namespace App {
	CPlatform::CPlatform() : 
		m_iconPath(nullptr),
		m_pMainPanel(nullptr) {
	}

	CPlatform::~CPlatform() {
	}

	void CPlatform::Initialize() {
		Assert(m_pMainPanel);
	}
};
