//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Math/CMathVector2.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CMATHVECTORINT2_H
#define CMATHVECTORINT2_H

#include "CMathVectorInt3.h"
#include "CMathVectorInt4.h"

namespace Math {
	struct VectorInt2 {
		union {
			struct { int x, y; };
			int v[2];
		};

		VectorInt2() { }
		VectorInt2(int i) : x(i), y(i) { }
		VectorInt2(int x, int y) : x(x), y(y) { }

		// Unary operators.
		inline int& operator [] (size_t index) { return v[index]; }
		inline VectorInt2 operator + () { return VectorInt2(x, y); }
		inline VectorInt2 operator - () { return VectorInt2(-x, -y); }

		inline operator const VectorInt3 () const { return VectorInt3(x, y, 0); }
		inline operator const VectorInt4 () const { return VectorInt4(x, y, 0, 0); }

		// Binary operators.
		inline VectorInt2 operator + (const VectorInt2& v) { return VectorInt2(x + v.x, y + v.y); }
		inline VectorInt2 operator + (int i) { return VectorInt2(x + i, y + i); }
		
		inline VectorInt2 operator - (const VectorInt2& v) { return VectorInt2(x - v.x, y - v.y); }
		inline VectorInt2 operator - (int i) { return VectorInt2(x - i, y - i); }
		
		inline VectorInt2 operator * (int i) { return VectorInt2(x * i, y * i); }
		inline VectorInt2 operator / (int i) { return VectorInt2(x / i, y / i); }

		// Assignment operators.
		inline VectorInt2& operator += (const VectorInt2& v) {
			x += v.x; y += v.y;
			return *this;
		}
		
		inline VectorInt2& operator += (int i) {
			x += i; y += i;
			return *this;
		}
		
		inline VectorInt2& operator -= (const VectorInt2& v) {
			x -= v.x; y -= v.y;
			return *this;
		}
		
		inline VectorInt2& operator -= (int i) {
			x -= i; y -= i;
			return *this;
		}
		
		inline VectorInt2& operator *= (int i) {
			x *= i; y *= i;
			return *this;
		}
		
		inline VectorInt2& operator /= (int i) {
			x /= i; y /= i;
			return *this;
		}

		// Conditions.
		inline bool operator == (const VectorInt2& v) {
			return x == v.x && y == v.y;
		}

		inline bool operator != (const VectorInt2& v) {
			return x != v.x || y != v.y;
		}

		// Products.
		static VectorInt2 PointwiseProduct(const VectorInt2& v0, const VectorInt2& v1) { return v0.PointwiseProduct(v1); }
		static VectorInt2 PointwiseQuotient(const VectorInt2& v0, const VectorInt2& v1) { return v0.PointwiseQuotient(v1); }

		inline VectorInt2 PointwiseProduct(const VectorInt2& v) const {
			return VectorInt2(x * v.x, y * v.y);
		}

		inline VectorInt2 PointwiseQuotient(const VectorInt2& v) const {
			return VectorInt2(x / v.x, y / v.y);
		}
	};
};

#endif
