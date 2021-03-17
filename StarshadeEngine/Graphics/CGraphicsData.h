//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Graphics/CGraphicsData.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CGRAPHICSDATA_H
#define CGRAPHICSDATA_H

#include "../Globals/CGlobals.h"

namespace Graphics {
	struct ADAPTER_DATA {
		wchar_t description[128];
    bool bSupported;
    u32 index;
    u32 vendorId;
    u32 deviceId;
    u32 subSysId;
    u32 revision;
    size_t dedicatedVideoMemory;
    size_t dedicatedSystemMemory;
    size_t sharedSystemMemory;
	};
};

#endif
