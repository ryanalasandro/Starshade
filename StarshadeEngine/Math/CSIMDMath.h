//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Math/CSIMDMath.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CSIMDMATH_H
#define CSIMDMATH_H

#include "../Globals/CGlobals.h"

namespace Math {
	// SIMD constants.
	const vf32 SIMD_ZERO = _mm_set_ps1(0.0f);
	const vf32 SIMD_ONE = _mm_set_ps1(1.0f);

	//
	// SIMD utility functions.
	//

	inline vf32 Sign(vf32 xmm) {
		return _mm_or_ps(_mm_and_ps(_mm_cmplt_ps(xmm, SIMD_ZERO), _mm_set_ps1(-1.0f)), 
			_mm_and_ps(_mm_cmpgt_ps(xmm, SIMD_ZERO), _mm_set_ps1(1.0f)));
	}

	inline vf32 Clamp(vf32 xmm, vf32 mn, vf32 mx) {
		return _mm_max_ps(_mm_min_ps(xmm, mx), mn);
	}

	inline vf32 Frac(vf32 xmm) {
		return _mm_sub_ps(_mm_floor_ps(xmm), xmm);
	}

	inline vf32 Saturate(vf32 xmm) {
		return _mm_max_ps(_mm_min_ps(xmm, SIMD_ONE), SIMD_ZERO);
	}
};

#endif
