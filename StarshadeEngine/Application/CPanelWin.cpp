//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPanelWin.cpp
//
//-------------------------------------------------------------------------------------------------

#include "CPanelWin.h"
#include "CPlatform.h"
#include "../Utilities/CEngineError.h"

namespace App {
	CPanelWin::CPanelWin() : 
		m_hIcon(nullptr),
		m_hInstance(nullptr),
		m_hWnd(nullptr) {
	}

	CPanelWin::~CPanelWin() {
	}

	void CPanelWin::Initialize() {
		CPanel::Initialize();

		WNDCLASSEX wc = { };

		// Setup the window class structure.
		wc.cbSize = sizeof(WNDCLASSEX);

		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		if(m_bAllowDoubleClick) wc.style |= CS_DBLCLKS;
		
		wc.lpszClassName = m_title;
		wc.lpfnWndProc = CPanelWin::DisplayWinProc;
		wc.hInstance = m_hInstance = (HINSTANCE)GetModuleHandle(0);
		wc.hCursor = nullptr;
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		
		if(m_pPlatform->GetIconPath()) m_hIcon = (HICON)LoadImage(nullptr, m_pPlatform->GetIconPath(), IMAGE_ICON, 0, 0, 
			LR_DEFAULTSIZE | LR_LOADFROMFILE);
		wc.hIcon = m_hIcon;
		
		// Register class for window createion prep.
		RegisterClassEx(&wc);

		// Create the window handle.
		m_hWnd = CreateWindowEx(
			0,
			m_title,
			m_title,
			0,
			0,
			0,
			1280,
			720,
			m_pParent ? reinterpret_cast<const CPanelWin*>(m_pParent)->m_hWnd : nullptr,
			nullptr,
			m_hInstance,
			nullptr
		);

		AssertCode(m_hWnd, Err::PLATFORM_WIN_FAILED_TO_CREATE_WINDOW);

		// Display window.
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
		ShowWindow(m_hWnd, SW_SHOW);
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);
	}
	
	// Method for releasing this panel, and all of its assets from memory.
	void CPanelWin::Release() {
		if(m_hIcon) {
			DestroyIcon(m_hIcon);
			m_hIcon = nullptr;
		}

		if(m_hWnd) {
			DestroyWindow(m_hWnd);
			m_hWnd = nullptr;
		}

		UnregisterClass(m_title, m_hInstance);
	}

	//-----------------------------------------------------------------------------------------------
	// Window procedures.
	//-----------------------------------------------------------------------------------------------

	LRESULT CALLBACK CPanelWin::DisplayWinProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
		if(msg == WM_CREATE) {
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		}

		CPanelWin* pDestination = reinterpret_cast<CPanelWin*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if(pDestination) return pDestination->WinProc(hWnd, msg, wParam, lParam);

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK CPanelWin::WinProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
		switch(msg) {
			case WM_CREATE:
				break;
			case WM_CLOSE:
			case WM_DESTROY:
				PostQuitMessage(0);
			default:
				break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
};
