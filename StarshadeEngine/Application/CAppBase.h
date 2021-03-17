//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CAppBase.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CAPPBASE_H
#define CAPPBASE_H

#include "../Objects/CObject.h"

namespace Graphics {
	class CGraphicsAPI;
};

namespace App {
	class CAppBase : private IObject {
	protected:
		CAppBase();
		virtual ~CAppBase();

	public:
		void Create();
		int Run();
		void Destroy();

	protected:
		virtual void Setup() { }
		virtual void InitStage() { }
		virtual void PlatformStage() { }
		virtual void GraphicsStage() { }
		virtual void AssetStage() { }
		void Update() override = 0;
		void Render() override = 0;
		void Release() override = 0;

		inline class CPanel* Panel() { return m_pPanel; }

	private:
		void OnMove(const class CPanel* pPanel);
		void OnResize(const class CPanel* pPanel) final;

	protected:
		class CPlatform* m_pPlatform;
		Graphics::CGraphicsAPI* m_pGraphics;

	private:
		class CPanel* m_pPanel;
	};
};

#endif
