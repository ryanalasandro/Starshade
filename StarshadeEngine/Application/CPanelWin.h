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
	protected:
		CPanelWin();
		virtual ~CPanelWin();

		void Initialize() final;
		void Release() final;

	protected:

	private:
		static LRESULT CALLBACK DisplayWinProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WinProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam);

	private:
		HICON m_hIcon;
		HINSTANCE m_hInstance;
		HWND m_hWnd;
	};
};

#endif
