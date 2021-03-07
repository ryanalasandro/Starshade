//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Math/CMathFloat.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CMATHFLOAT_H
#define CMATHFLOAT_H

#include <cmath>

namespace Math {
	// Mathematical constants.
	const float g_Pi = 3.14159265f;
	const float g_2Pi = 2.0f * g_Pi;
	const float g_PiOver2 = 0.5f * g_Pi;
	const float g_PiOver4 = 0.25f * g_Pi;
	const float g_PiOver180 = g_Pi / 180.0f;
	const float g_180OverPi = 180.0f / g_Pi;
	const float g_1OverPi = 1.0f / g_Pi;

	const float g_SqrtOf2 = sqrtf(2.0f);
	const float g_1OverSqrtOf2 = 1.0f / g_SqrtOf2;
	const float g_SqrtOf2Over2 = g_SqrtOf2 / 2.0f;

	const float g_1Over3 = 1.0f / 3.0f;
	const float g_2Over3 = 2.0f / 3.0f;
	const float g_1Over255 = 1.0f / 255.0f;
	
	const float g_MaxFlt = 3.402823466e+38F;
	const float g_Epsilon = 1e-7f;

	const float g_nan = NAN;
	const float g_negNan = -g_nan;
	
	//
	// Mathematical utilities.
	//

	inline int Sign(int i) { return -(i < 0) + (i > 0); }
	inline float Sign(float f) { return -(f < 0.0f) + (f > 0.0f); }

	inline int Clamp(int i, int mn, int mx) { return (i < mn) ? mn : ((i > mx) ? mx : i); }
	inline float Clamp(float f, float mn, float mx) { return (f < mn) ? mn : ((f > mx) ? mx : f); }
	
	inline float Frac(float f) { return f - floorf(f); }
	inline float Saturate(float f) { return f < 0.0f ? 0.0f : f > 1.0f ? 1.0f : f; }
	
	inline float SmoothStep(float mn, float mx, float f) {
		const float t = Saturate((f - mn) / (mx - mn));
		return t * t * (3.0f - 2.0f * t);
	}
	

	//
	// Interpolation.
	//

	inline float Lerp(float from, float to, float t) {
		return from + (to - from) * t;
	}

	inline float MoveTowards(float from, float to, float t) {
		return Clamp(from + Sign(to - from) * t, from, to);
	}
};

#endif
