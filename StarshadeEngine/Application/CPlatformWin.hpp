//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPlatformWin.hpp
//
//-------------------------------------------------------------------------------------------------

//#include "CPlatformWin.h"

namespace App {
	CPlatformWin::CPlatformWin() {
	}

	CPlatformWin::~CPlatformWin() {
	}
	
	void CPlatformWin::PreInitialize() {
		PopulateMonitorList();
	}

	void CPlatformWin::Initialize() {
		CPlatform::Initialize();
	}

	// Method for handling one pass of the application loop.
	bool CPlatformWin::Update(int& exitCode) {
		MSG msg;
		while(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);

			MessageHandler(msg);

			if(msg.message == WM_QUIT) {
				exitCode = static_cast<int>(msg.wParam);
				return false;
			}
		}

		return true;
	}
	
	//-----------------------------------------------------------------------------------------------
	// Platform utility methods.
	//-----------------------------------------------------------------------------------------------

	// Method for refreshing the list of connected monitors and their rects.
	void CPlatformWin::PopulateMonitorList() {
		m_monitorDataList.clear();
		EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(this));
	}

	// Method for handling non-window specific messages.
	void CPlatformWin::MessageHandler(const MSG& msg) {
		switch(msg.message) {
			case WM_DISPLAYCHANGE: {
				PopulateMonitorList();
			} break;
			default:
				break;
		}
	}

	//-----------------------------------------------------------------------------------------------
	// Monitor population procedures.
	//-----------------------------------------------------------------------------------------------
	
	BOOL CALLBACK CPlatformWin::MonitorEnumProc(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM lParam) {
		return reinterpret_cast<CPlatformWin*>(lParam)->MonitorProc(hMonitor, hdc, rect, lParam);
	}

	BOOL CALLBACK CPlatformWin::MonitorProc(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM lParam) {
		m_monitorDataList.push_back({ Math::RectInt(rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top) });
		return TRUE;
	}
};
