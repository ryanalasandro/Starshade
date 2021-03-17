//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPanelWin.hpp
//
//-------------------------------------------------------------------------------------------------

//#include "CPanelWin.h"
#include "CPlatform.h"
#include "../Utilities/CEngineError.h"
#include "../Utilities/CDebugLog.h"

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
		wc.hInstance = m_hInstance = (HINSTANCE)GetModuleHandleW(0);
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		
		if(m_pPlatform->GetIconPath()) m_hIcon = (HICON)LoadImage(nullptr, m_pPlatform->GetIconPath(), IMAGE_ICON, 0, 0, 
			LR_DEFAULTSIZE | LR_LOADFROMFILE);
		wc.hIcon = m_hIcon;
		
		// Register class for window creation prep.
		RegisterClassExW(&wc);

		// Force target dimensions >= minimum dimensions.
		m_targetDim.x = max(m_targetDim.x, m_minDim.x);
		m_targetDim.y = max(m_targetDim.y, m_minDim.y);

		// Adjust window based on calculated window style.
		const DWORD style = CalculateStyle();
		RECT styleRect = { 0, 0, m_targetDim.x, m_targetDim.y };
		const bool bAdjusted = AdjustWindowRect(&styleRect, style, FALSE);
		Assert(bAdjusted);
		
		const int width = styleRect.right - styleRect.left;
		const int height = styleRect.bottom - styleRect.top;
		{ // Update target with style.
			m_monitorBounds = m_pPlatform->GetMonitor(0).bounds;
			m_position.x = (m_monitorBounds.w >> 1) - (width >> 1);
			m_position.y = (m_monitorBounds.h >> 1) - (height >> 1);
		}

		// Create the window handle.
		m_hWnd = CreateWindowExW(
			0,
			m_title,
			m_title,
			style,
			m_position.x,
			m_position.y,
			width,
			height,
			m_pParent ? reinterpret_cast<const CPanelWin*>(m_pParent)->m_hWnd : nullptr,
			nullptr,
			m_hInstance,
			nullptr
		);

		AssertCode(m_hWnd, Err::PLATFORM_WIN_FAILED_TO_CREATE_WINDOW);
		
		// Calculate adjusted bounds.
		m_bounds.x = m_position.x - styleRect.left;
		m_bounds.y = m_position.y - styleRect.top;
		m_bounds.w = m_targetDim.x;
		m_bounds.h = m_targetDim.y;

		// Display window.
		SetWindowLongPtrW(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
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

		UnregisterClassW(m_title, m_hInstance);
	}
	
	//-----------------------------------------------------------------------------------------------

	// Method for changing the screen mode and/or the target monitor for this panel.
	void CPanelWin::ChangeScreenMode(PANEL_SCREEN_MODE mode, u32 targetMonitor) {
		Resize(m_targetDim.x, m_targetDim.y, targetMonitor, mode);
	}

	// Method for resizing, switching monitors and/or changing the screen mode for this panel.
	void CPanelWin::Resize(int width, int height, u32 targetMonitor, PANEL_SCREEN_MODE mode) {
		if(mode == PSM_UNKNOWN) { mode = m_screenMode; }
		m_targetDim.x = width;
		m_targetDim.y = height;
		
		const bool bScreenModeSwitch = m_screenMode != mode;
		RECT winRect;
		GetWindowRect(m_hWnd, &winRect);

		// Update window style.
		const DWORD style = CalculateStyle(mode);
		SetWindowLongPtrW(m_hWnd, GWL_STYLE, style);

		// Update panel bounds.
		switch(mode) 	{
			case App::PSM_WINDOWED: {
				RECT styleRect = { 0, 0, m_targetDim.x, m_targetDim.y };
				AdjustWindowRect(&styleRect, style, FALSE);
				
				m_bounds.w = styleRect.right - styleRect.left;
				m_bounds.h = styleRect.bottom - styleRect.top;
			} break;
			case App::PSM_BORDERLESS:
				m_bounds = m_pPlatform->GetMonitor(targetMonitor).bounds;
				m_position.x = m_bounds.x;
				m_position.y = m_bounds.y;
				break;
			case App::PSM_FULLSCREEN:
				m_bounds.w = m_targetDim.x;
				m_bounds.h = m_targetDim.y;
				break;
			default:
				break;
		}

		// Finalize panel resize/reposition and screen mode switch.
		if(mode != PANEL_SCREEN_MODE::PSM_FULLSCREEN) {
			const bool bResChange = 
				(winRect.right - winRect.left) != m_bounds.w || 
				(winRect.bottom - winRect.top) != m_bounds.h;
			
			// When the panel's dimensions change the WM_SIZE message will be sent.
			//	Setting state to unknown allows the WM_SIZE message to be handled gracefully.
			if(bResChange) { m_screenState = PSS_UNKNOWN; }

			// Adjust window position and size.
			SetWindowPos(m_hWnd, HWND_TOP, m_position.x, m_position.y,
				m_bounds.w, m_bounds.h, SWP_SHOWWINDOW);

			if(bScreenModeSwitch) {
				m_screenMode = mode;
				if(!bResChange) {
					OnResize();
				}
			}
		} else {
			if(bScreenModeSwitch) {
				m_screenMode = mode;
				OnResize();
			}
		}
	}
	
	//-----------------------------------------------------------------------------------------------
	// Event methods.
	//-----------------------------------------------------------------------------------------------

	void CPanelWin::OnMove() {
		CPanel::OnMove();
	}

	void CPanelWin::OnResize() {
		CPanel::OnResize();
	}

	//-----------------------------------------------------------------------------------------------
	// Utility methods.
	//-----------------------------------------------------------------------------------------------

	// Method for calculating the bounds of the monitor in which the panel inhabits.
	void CPanelWin::CalculateMonitorBounds() {
		MONITORINFO minfo;
		minfo.cbSize = sizeof(minfo);
		if(GetMonitorInfoW(MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST), &minfo)) {
			m_monitorBounds.x = minfo.rcMonitor.left;
			m_monitorBounds.y = minfo.rcMonitor.top;
			m_monitorBounds.w = minfo.rcMonitor.right - minfo.rcMonitor.left;
			m_monitorBounds.h = minfo.rcMonitor.bottom - minfo.rcMonitor.top;
		}
	}

	DWORD CPanelWin::CalculateStyle() const {
		return CalculateStyle(m_screenMode);
	}

	// Method for calculating the window style based on relevant variables.
	DWORD CPanelWin::CalculateStyle(PANEL_SCREEN_MODE mode) const {
		switch(mode) 	{
			case App::PSM_BORDERLESS:
			case App::PSM_FULLSCREEN:
				return (WS_VISIBLE | WS_POPUP);
			case App::PSM_WINDOWED:
			default: {
				if(m_bAllowResize) return (WS_VISIBLE | WS_OVERLAPPEDWINDOW);
				else return (WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU);
			}
		}
	}

	//-----------------------------------------------------------------------------------------------
	// Window procedures.
	//-----------------------------------------------------------------------------------------------

	LRESULT CALLBACK CPanelWin::DisplayWinProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
		if(msg == WM_CREATE) {
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		}

		CPanelWin* pDestination = reinterpret_cast<CPanelWin*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
		if(pDestination) return pDestination->WinProc(hWnd, msg, wParam, lParam);

		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK CPanelWin::WinProc(HWND hWnd, u32 msg, WPARAM wParam, LPARAM lParam) {
		static bool bMoving = false;
		static bool bResizeDrag = false;

		switch(msg) {
			case WM_CREATE:
				break;
			case WM_CLOSE:
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			case WM_GETMINMAXINFO: { // Specify the minimum dimensions of this panel.
				LPMINMAXINFO pMMI = (LPMINMAXINFO)lParam;
				pMMI->ptMinTrackSize.x = m_minDim.x;
				pMMI->ptMinTrackSize.y = m_minDim.y;
			} break;
			case WM_MOUSEMOVE: {
				if(!m_bCursorInPanel) {
					// Make sure that an on mouse leave tracker is active when the cursor is in the client area.
					TRACKMOUSEEVENT trackEvent;
					trackEvent.cbSize = sizeof(trackEvent);
					trackEvent.dwFlags = TME_LEAVE;
					trackEvent.dwHoverTime = HOVER_DEFAULT;
					trackEvent.hwndTrack = m_hWnd;
					TrackMouseEvent(&trackEvent);
					m_bCursorInPanel = true;
				}
			} break;
			case WM_MOUSELEAVE: {
				if(m_bCursorInPanel) {
					m_bCursorInPanel = false;
				}
			} break;
			case WM_MOVE: { // Update inhabited monitor bounds and panel position.
				bMoving = true;
				m_bounds.x = m_position.x = static_cast<int>(LOWORD(lParam));
				m_bounds.y = m_position.y = static_cast<int>(HIWORD(lParam));
			} break;
			case WM_SIZE: { // If the window has been resized.
				const int width = static_cast<int>(LOWORD(lParam));
				const int height = static_cast<int>(HIWORD(lParam));
				
				switch(wParam) { // Detect type of size change.
					case SIZE_MINIMIZED:
						m_screenState = PSS_MINIMIZED;
						break;
					case SIZE_MAXIMIZED: {
						if(m_screenState != PSS_MAXIMIZED) {
							m_screenState = PSS_MAXIMIZED;
							if(width != m_bounds.w || height != m_bounds.h) {
								m_bounds.w = width; m_bounds.h = height;
								OnResize();
							}
						}
					} break;
					case SIZE_RESTORED: {
						const bool bResized = width != m_bounds.w || height != m_bounds.h;
						if(m_screenState != PSS_UNKNOWN) {
							if(m_screenState != PSS_NORMAL) {
								m_screenState = PSS_NORMAL;
								if(bResized) {
									m_bounds.w = width; m_bounds.h = height;
									OnResize();
								}
							} else {
								if(bResized) { // Resize is occurring via edge/corner drag.
									m_bounds.w = width; m_bounds.h = height;
									bResizeDrag = true;
								}
							}
						} else {
							m_screenState = PSS_NORMAL;
							m_bounds.w = width; m_bounds.h = height;
							OnResize();
						}

					} break;
					default:
						break;
				}
			} break;
			case WM_EXITSIZEMOVE: {
				if(bMoving) {
					if(!bResizeDrag) {
						CalculateMonitorBounds();
						OnMove();
					}
					
					bMoving = false;
				}

				if(bResizeDrag) { // Panel resize via edge/corner drag.
					CalculateMonitorBounds();
					OnResize();
					bResizeDrag = false;
				}
			} break;
			default:
				break;
		}

		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
};
