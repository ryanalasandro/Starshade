//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Factory/CEngineFactory.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CENGINEFACTORY_H
#define CENGINEFACTORY_H

namespace App {
	class CPlatform;
	class CPanel;
};

namespace Graphics {
	class CGraphicsAPI;
};

namespace Factory {
	class CEngineFactory {
	public:
		static CEngineFactory& Instance() {
			static CEngineFactory  instance;
			return instance;
		}
		
	private:
		CEngineFactory();
		~CEngineFactory();
		CEngineFactory(const CEngineFactory&) = delete;
		CEngineFactory(CEngineFactory&&) = delete;
		CEngineFactory& operator = (const CEngineFactory&) = delete;
		CEngineFactory& operator = (CEngineFactory&&) = delete;

	public:
		App::CPanel* CreatePanel(bool bFromStack = true);

		App::CPlatform* GetPlatform(bool bAllowCreation = false, bool bFromStack = true);
		Graphics::CGraphicsAPI* GetGraphics(bool bAllowCreation = false, bool bFromStack = true);

	private:
		App::CPlatform* m_pPlatform;
		Graphics::CGraphicsAPI* m_pGraphics;
	};
};

#endif
