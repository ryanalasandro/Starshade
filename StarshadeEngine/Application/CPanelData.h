//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPanelData.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CPANELDATA_H
#define CPANELDATA_H

namespace App {
	enum PANEL_SCREEN_MODE {
		PSM_WINDOWED,
		PSM_BORDERLESS,
		PSM_FULLSCREEN,
		PSM_UNKNOWN,
	};

	enum PANEL_SCREEN_STATE {
		PSS_NORMAL,
		PSS_MINIMIZED,
		PSS_MAXIMIZED,
		PSS_UNKNOWN,
	};
};

#endif
