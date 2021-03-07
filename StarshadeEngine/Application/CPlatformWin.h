//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPlatformWin.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CPLATFORMWIN_H
#define CPLATFORMWIN_H

#include "CPlatform.h"
#include <Windows.h>

namespace App {
	class CPlatformWin : public CPlatform {
	public:
		CPlatformWin();
		virtual ~CPlatformWin();

		void Initialize() final;
		bool Update(int& exitCode) final;

	protected:
	private:
		void DetectConnectedMonitors();

		void MessageHandler(const MSG& msg);
		
		static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM lParam);
		BOOL CALLBACK MonitorProc(HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM lParam);

	private:
	};
};

#endif
