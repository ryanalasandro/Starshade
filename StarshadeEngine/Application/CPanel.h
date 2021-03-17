//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPanel.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CPANEL_H
#define CPANEL_H

#include "CPanelData.h"
#include "../Objects/CObject.h"
#include "../Math/CMathVectorInt2.h"
#include "../Math/CMathRect.h"
#include "../Globals/CGlobals.h"
#include <functional>

namespace App {
	class CPanel : private IObject {
	private:
		friend class CAppBase;

		typedef std::function<void(const CPanel*)> PanelAdjustFunc;

	protected:
		CPanel();
		virtual ~CPanel();

		void Initialize() override;
		void Release() override = 0;

		virtual void OnMove();
		virtual void OnResize();

	public:
		virtual void ChangeScreenMode(PANEL_SCREEN_MODE mode, u32 targetMonitor) { }
		virtual void Resize(int width, int height, u32 targetMonitor, PANEL_SCREEN_MODE mode = PSM_UNKNOWN) { }

		inline bool GetAllowDoubleClick() const { return m_bAllowDoubleClick; }
		inline void SetAllowDoubleClick(bool bAllow) { m_bAllowDoubleClick = bAllow; }

		inline bool GetAllowResize() const { return m_bAllowResize; }
		inline void SetAllowResize(bool bAllow) { m_bAllowResize = bAllow; }

		inline bool IsCursorInPanel() const { return m_bCursorInPanel; }
		inline PANEL_SCREEN_MODE GetScreenMode() const { return m_screenMode; }
		inline PANEL_SCREEN_STATE GetScreenState() const { return m_screenState; }
		
		inline const Math::VectorInt2& GetPosition() const { return m_position; }

		inline const Math::VectorInt2& GetTargetDimensions() const { return m_targetDim; }
		inline void SetTargetDimensions(const Math::VectorInt2& targetDim) { m_targetDim = targetDim; }

		inline const Math::VectorInt2& GetMinDimensions() const { return m_minDim; }
		inline void SetMinDimensions(const Math::VectorInt2& minDim) { m_minDim = minDim; }

		inline const Math::RectInt& GetBounds() const { return m_bounds; }

		inline const wchar_t* GetTitle() const { return m_title; }
		inline void SetTitle(const wchar_t* title) { m_title = title; }

		inline virtual bool InFocus() const = 0;
		inline const CPanel* GetParent() const { return m_pParent; }
		inline void SetParent(const CPanel* pParent) { m_pParent = pParent; }

	protected:
		inline void SetOnMoveAction(const PanelAdjustFunc& func) { m_funcOnMove = func; }
		inline void SetOnResizeAction(const PanelAdjustFunc& func) { m_funcOnResize = func; }

	protected:
		bool m_bAllowDoubleClick;
		bool m_bAllowResize;

		bool m_bCursorInPanel;

		PANEL_SCREEN_MODE m_screenMode;
		PANEL_SCREEN_STATE m_screenState;
		Math::VectorInt2 m_position;
		Math::VectorInt2 m_targetDim;
		Math::VectorInt2 m_minDim;
		Math::RectInt m_bounds;
		Math::RectInt m_monitorBounds;

		const wchar_t* m_title;

		const CPanel* m_pParent;
		const class CPlatform* m_pPlatform;

	private:
		PanelAdjustFunc m_funcOnMove;
		PanelAdjustFunc m_funcOnResize;
	};
};

#endif
