//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPanelWin.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CPANELWIN_H
#define CPANELWIN_H

#include "CPanel.h"
#include "../Globals/CGlobals.h"
#include <Windows.h>

namespace App {
	class CPanelWin : public CPanel {
	public:
		CPanelWin();
		virtual ~CPanelWin();

		inline const HWND GetWindowHandle() const { return m_hWnd; }

	protected:
		void Initialize() final;
		void Release() final;
		
		void ChangeScreenMode(PANEL_SCREEN_MODE mode, u32 targetMonitor) final;
		void Resize(int width, int height, u32 targetMonitor, PANEL_SCREEN_MODE mode = PSM_UNKNOWN) final;

		inline bool InFocus() const final { return GetFocus() == m_hWnd; }

		void OnMove() final;
		void OnResize() final;
		
	private:
		void CalculateMonitorBounds();
		DWORD CalculateStyle() const;
		DWORD CalculateStyle(PANEL_SCREEN_MODE mode) const;

		static LRESULT CALLBACK DisplayWinProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WinProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);

	private:
		HICON m_hIcon;
		HINSTANCE m_hInstance;
		HWND m_hWnd;
	};
};

#include "CPanelWin.hpp"

#endif
