//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Math/CMathVector3.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CMATHVECTORINT3_H
#define CMATHVECTORINT3_H

#include "CMathVectorInt4.h"

namespace Math {
	struct VectorInt3 {
		union {
			struct { int x, y, z; };
			int v[3];
		};

		VectorInt3() { }
		VectorInt3(int i) : x(i), y(i), z(i) { }
		VectorInt3(int x, int y, int z) : x(x), y(y), z(z) { }

		// Unary operators.
		inline int& operator [] (size_t index) { return v[index]; }
		inline VectorInt3 operator + () { return VectorInt3(x, y, z); }
		inline VectorInt3 operator - () { return VectorInt3(-x, -y, -z); }

		inline operator const VectorInt4 () { return VectorInt4(x, y, z, 0); }

		// Binary operators.
		inline VectorInt3 operator + (const VectorInt3& v) { return VectorInt3(x + v.x, y + v.y, z + v.z); }
		inline VectorInt3 operator + (int i) { return VectorInt3(x + i, y + i, z + i); }
		
		inline VectorInt3 operator - (const VectorInt3& v) { return VectorInt3(x - v.x, y - v.y, z - v.z); }
		inline VectorInt3 operator - (int i) { return VectorInt3(x - i, y - i, z - i); }
		
		inline VectorInt3 operator * (int i) { return VectorInt3(x * i, y * i, z * i); }
		inline VectorInt3 operator / (int i) { return VectorInt3(x / i, y / i, z / i); }

		// Assignment operators.
		inline VectorInt3& operator += (const VectorInt3& v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}
		
		inline VectorInt3& operator += (int i) {
			x += i; y += i; z += i;
			return *this;
		}
		
		inline VectorInt3& operator -= (const VectorInt3& v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}
		
		inline VectorInt3& operator -= (int i) {
			x -= i; y -= i; z -= i;
			return *this;
		}
		
		inline VectorInt3& operator *= (int i) {
			x *= i; y *= i; z *= i;
			return *this;
		}
		
		inline VectorInt3& operator /= (int i) {
			x /= i; y /= i; z /= i;
			return *this;
		}

		// Conditions.
		inline bool operator == (const VectorInt3& v) {
			return x == v.x && y == v.y && z == v.z;
		}

		inline bool operator != (const VectorInt3& v) {
			return x != v.x || y != v.y || z != v.z;
		}

		// Products.
		static VectorInt3 PointwiseProduct(const VectorInt3& v0, const VectorInt3& v1) { return v0.PointwiseProduct(v1); }
		static VectorInt3 PointwiseQuotient(const VectorInt3& v0, const VectorInt3& v1) { return v0.PointwiseQuotient(v1); }

		inline VectorInt3 PointwiseProduct(const VectorInt3& v) const {
			return VectorInt3(x * v.x, y * v.y, z * v.z);
		}

		inline VectorInt3 PointwiseQuotient(const VectorInt3& v) const {
			return VectorInt3(x / v.x, y / v.y, z / v.z);
		}
	};
};

#endif
