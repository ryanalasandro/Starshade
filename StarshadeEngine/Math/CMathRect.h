//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Math/CMathRect.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CMATHRECT_H
#define CMATHRECT_H

namespace Math {
	struct RectInt {
		int x, y, w, h;
		RectInt() { }
		RectInt(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) { }
	};

	struct RectFlt {
		float x, y, w, h;

		RectFlt() { }
		RectFlt(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) { }
	};
};

#endif
