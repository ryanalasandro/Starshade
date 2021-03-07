//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Windows Application: Story Box
//
// File: Application/CApplication.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CAPPLICATION_H
#define CAPPLICATION_H

#include <Application/CAppBase.h>

namespace App {
	class CApplication : public CAppBase {
	public:
		CApplication();
		~CApplication();

	protected:
		void Setup() final;
		void InitStage() final;
		void PlatformStage() final;
		void GraphicsStage() final;
		void AssetStage() final;
		void Update() final;
		void Render() final;
		void Release() final;

	private:
		class CPanel* m_pMainPanel;
	};
};

#endif
