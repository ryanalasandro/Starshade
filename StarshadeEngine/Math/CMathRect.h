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
		struct LTRB {
			int l, t, r, b;
		};
		
		int x, y, w, h;
		RectInt() { }
		RectInt(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) { }
		RectInt(const LTRB& r) : x(r.l), y(r.t), w(r.r - r.l), h(r.b - r.t) { }

		inline int Left() const { return x; }
		inline int Top() const { return y; }
		inline int Right() const { return x + w; }
		inline int Bottom() const { return y + h; }
	};

	struct RectFlt {
		struct LTRB {
			float l, t, r, b;
		};

		float x, y, w, h;
		RectFlt() { }
		RectFlt(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) { }
		RectFlt(const LTRB& r) : x(r.l), y(r.t), w(r.r - r.l), h(r.b - r.t) { }
		
		inline float Left() const { return x; }
		inline float Top() const { return y; }
		inline float Right() const { return x + w; }
		inline float Bottom() const { return y + h; }
	};
};

#endif
