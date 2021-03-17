//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Graphics/CGraphicsAPI.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CGRAPHICSAPI_H
#define CGRAPHICSAPI_H

#include "CGraphicsData.h"
#include "../Objects/CObject.h"
#include "../Math/CMathColor.h"
#include <vector>
#include <functional>

namespace App {
	class CAppBase;
	class CPanel;
};

namespace Graphics {
	class CGraphicsAPI : private IObject {
	public:
		friend App::CAppBase;

	protected:
		CGraphicsAPI();
		virtual ~CGraphicsAPI();

		void PreInitialize() override;
		void Initialize() override;
		
		virtual void Render(const std::function<void()>& RenderScene) = 0;
		virtual void AdjustToResize() { }
		virtual void FinalizeResize() { }
		virtual void SetFullscreen(bool bFullscreen) { }

		void Release() override;

	public:
		inline bool GetTearingSupport() const { return m_bTearingSupport; }
		inline bool GetRaytracingSupport() const { return m_bRaytracingSupport; }

		inline u8 GetVBlankCount() const { return m_vBlanks; }
		inline void SetVBlankCount(u8 vBlanks) { m_vBlanks = vBlanks; }

		inline const Math::Color& GetBackgroundColor() const { return m_bgColor; }
		inline void SetBackgroundColor(const Math::Color& bgColor) { m_bgColor = bgColor; }

		inline const size_t GetAdapterCount() const { return m_adapterDataList.size(); }
		inline const ADAPTER_DATA& GetAdapter(size_t index) const { return m_adapterDataList[index]; }
		
		inline void SetPanel(const App::CPanel* pPanel) { m_pPanel = pPanel; }

	protected:
		bool m_bTearingSupport;
		bool m_bRaytracingSupport;
		u8 m_bufferCount;
		u8 m_vBlanks;

		Math::Color m_bgColor;
		std::vector<ADAPTER_DATA> m_adapterDataList;

		const App::CPanel* m_pPanel;
	};
};

#endif
