//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Math/CSIMDVector.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CSIMDVECTOR_H
#define CSIMDVECTOR_H

#include "../Globals/CGlobals.h"
#include "CSIMDMath.h"
#include <cmath>

namespace Math {
	class CSIMDVector {
	public:
		CSIMDVector() { }
		CSIMDVector(float f, float w = 0.0f) : m_xmm(_mm_setr_ps(f, f, f, w)) {}
		CSIMDVector(vf32 xmm) : m_xmm(xmm) {}
		CSIMDVector(float x, float y, float z, float w = 0.0f) : m_xmm(_mm_setr_ps(x, y, z, w)) { }

		// Unary operators.
		inline CSIMDVector operator + () const  {
			return *this;
		}

		inline CSIMDVector operator - () const {
			return _mm_xor_ps(m_xmm, _mm_set_ps1(-0.0f));
		}

		// Binary operators.
		inline CSIMDVector operator + (const CSIMDVector& v) const {
			return _mm_add_ps(m_xmm, v.m_xmm);
		}

		inline CSIMDVector operator + (float f) const {
			return _mm_add_ps(m_xmm, _mm_setr_ps(f, f, f, 0.0f));
		}
		
		inline CSIMDVector operator - (const CSIMDVector& v) const {
			return _mm_sub_ps(m_xmm, v.m_xmm);
		}

		inline CSIMDVector operator - (float f) const {
			return _mm_sub_ps(m_xmm, _mm_setr_ps(f, f, f, 0.0f));
		}

		inline CSIMDVector operator * (float f) const {
			return _mm_mul_ps(m_xmm, _mm_setr_ps(f, f, f, 0.0f));
		}

		inline CSIMDVector operator / (float f) const {
			return _mm_div_ps(m_xmm, _mm_setr_ps(f, f, f, 1.0f));
		}

		// Assignment operators.
		inline CSIMDVector& operator += (const CSIMDVector& v) {
			m_xmm = _mm_add_ps(m_xmm, v.m_xmm);
			return *this;
		}

		inline CSIMDVector& operator += (float f) {
			m_xmm = _mm_add_ps(m_xmm, _mm_setr_ps(f, f, f, 0.0f));
			return *this;
		}
		
		inline CSIMDVector& operator -= (const CSIMDVector& v) {
			m_xmm = _mm_sub_ps(m_xmm, v.m_xmm);
			return *this;
		}

		inline CSIMDVector& operator -= (float f) {
			m_xmm = _mm_sub_ps(m_xmm, _mm_setr_ps(f, f, f, 0.0f));
			return *this;
		}

		inline CSIMDVector& operator *= (float f) {
			m_xmm = _mm_mul_ps(m_xmm, _mm_setr_ps(f, f, f, 0.0f));
			return *this;
		}

		inline CSIMDVector& operator /= (float f) {
			m_xmm = _mm_div_ps(m_xmm, _mm_setr_ps(f, f, f, 1.0f));
			return *this;
		}

		// Conditional operators.
		inline bool operator == (const CSIMDVector& v) const {
			vf32 c0 = _mm_cmpeq_ps(m_xmm, v.m_xmm);

			vf32 shuf = _mm_moveldup_ps(c0);
			c0 = _mm_and_ps(c0, shuf);
			shuf = _mm_movehl_ps(shuf, c0);
			c0 = _mm_and_ps(c0, shuf);

			return _mm_cvtss_f32(c0) != 0.0f;
		}

		inline bool operator != (const CSIMDVector& v) const {
			vf32 c0 = _mm_cmpneq_ps(m_xmm, v.m_xmm);

			vf32 shuf = _mm_movehdup_ps(c0);
			c0 = _mm_and_ps(c0, shuf);
			shuf = _mm_movehl_ps(shuf, c0);
			c0 = _mm_and_ps(c0, shuf);

			return _mm_cvtss_f32(c0) != 0.0f;
		}

		//
		// Specialty methods.
		//

		static float LengthSq(const CSIMDVector& v) { return v.LengthSq(); }
		static float Length(const CSIMDVector& v) { return v.Length(); }
		static void Normalize(CSIMDVector& v) { v.Normalize(); }
		static CSIMDVector Normalized(const CSIMDVector& v) { return v.Normalized(); }

		inline float LengthSq() const {
			return Dot(*this);
		}

		inline float Length() const {
			return sqrtf(LengthSq());
		}

		inline void Normalize() {
			float l = Length();
			m_xmm = _mm_div_ps(m_xmm, _mm_setr_ps(l, l, l, 1.0f));
		}

		inline CSIMDVector Normalized() const {
			float l = Length();
			return _mm_div_ps(m_xmm, _mm_setr_ps(l, l, l, 1.0f));
		}

		inline float* ToFloat() const {
			return (float*)m_xmm.m128_f32;
		}

		//
		// Products.
		//

		static CSIMDVector PointwiseProduct(const CSIMDVector& v0, const CSIMDVector& v1) { return v0.PointwiseProduct(v1); }
		static CSIMDVector PointwiseQuotient(const CSIMDVector& v0, const CSIMDVector& v1) { return v0.PointwiseQuotient(v1); }
		static float Dot(const CSIMDVector& v0, const CSIMDVector& v1) { return v0.Dot(v1); }
		static CSIMDVector Cross(const CSIMDVector& v0, const CSIMDVector& v1) { return v0.Cross(v1); }

		inline CSIMDVector PointwiseProduct(const CSIMDVector& v) const {
			return _mm_mul_ps(m_xmm, v.m_xmm);
		}

		// 'w' remains 0 with implicit 1 where needed, so 'or' on a 1 to the 'w' component for valid divide.
		inline CSIMDVector PointwiseQuotient(const CSIMDVector& v) const {
			return _mm_div_ps(m_xmm, _mm_or_ps(v.m_xmm, _mm_setr_ps(0.0f, 0.0, 0.0, 1.0f)));
		}

		inline float Dot(const CSIMDVector& v) const {
			vf32 prod = _mm_mul_ps(m_xmm, v.m_xmm);

			vf32 shuf = _mm_movehdup_ps(prod);
			prod = _mm_add_ps(prod, shuf);
			shuf = _mm_movehl_ps(shuf, prod);
			prod = _mm_add_ss(prod, shuf);

			return _mm_cvtss_f32(prod);
		}

		inline CSIMDVector Cross(const CSIMDVector& v) const {
			/* yz - zy
				 zx - xz
				 xy - yx */
			vf32 res = _mm_sub_ps(
				_mm_mul_ps(m_xmm, _mm_shuffle_ps(v.m_xmm, v.m_xmm, _MM_SHUFFLE(3, 0, 2, 1))), // x y z w * y z x w
				_mm_mul_ps(v.m_xmm, _mm_shuffle_ps(m_xmm, m_xmm, _MM_SHUFFLE(3, 0, 2, 1))));
			return _mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 0, 2, 1));
		}

		//
		// Interpolations.
		//

		static CSIMDVector Lerp(const CSIMDVector& from, const CSIMDVector& to, float t) { return from.Lerp(to, t); }
		static CSIMDVector MoveTowards(const CSIMDVector& from, const CSIMDVector& to, float t) { return from.MoveTowards(to, t); }

		inline CSIMDVector Lerp(const CSIMDVector& to, float t) const {
			return _mm_add_ps(m_xmm, _mm_mul_ps(_mm_sub_ps(to.m_xmm, m_xmm), _mm_set_ps1(t)));
		}

		inline CSIMDVector MoveTowards(const CSIMDVector& to, float t) const {
			const CSIMDVector diff = to - *this;
			if(diff.LengthSq() < 1e-8f) return to;
			const CSIMDVector target = *this + diff.Normalized() * t;
			if(Dot(diff, to - target) <= 0.0f) return to;
			return target;
		}

	private:
		vf32 m_xmm;
	};

	const CSIMDVector SIMDVEC_ZERO(0.0f, 0.0f);
	const CSIMDVector SIMDVEC_ONE(1.0f, 1.0f);

	const CSIMDVector SIMDVEC_LEFT(1.0f, 0.0f, 0.0f);
	const CSIMDVector SIMDVEC_RIGHT(-1.0f, 0.0f, 0.0f);
	const CSIMDVector SIMDVEC_UP(0.0f, 1.0f, 0.0f);
	const CSIMDVector SIMDVEC_DOWN(0.0f, -1.0f, 0.0f);
	const CSIMDVector SIMDVEC_FORWARD(0.0f, 0.0f, 1.0f);
	const CSIMDVector SIMDVEC_BACKWARD(0.0f, 0.0f, -1.0f);
};

#endif
