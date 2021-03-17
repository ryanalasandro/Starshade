//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Utilities/CMemoryFree.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CMEMORYFREE_H
#define CMEMORYFREE_H

#include <memory>

// Memory free generics.
template<typename T, typename... Args>
inline void SAFE_RELEASE(T** x, Args... args) { if(*x) { (*x)->Release(args...); *x = nullptr; } }
template<typename T, typename... Args>
inline void SAFE_RELEASE_DELETE(T** x, Args... args) { if(*x) { (*x)->Release(args...); delete *x; *x = nullptr; } }
template<typename T, typename... Args>
inline void SAFE_RELEASE_DESTROY(T** x, Args... args) { if(*x) { (*x)->Release(args...); (*x)->~T(); *x = nullptr; } }

template<typename T>
inline void SAFE_DELETE(T** x) { if(*x) { delete *x; *x = nullptr; } }
template<typename T>
inline void SAFE_DELETE_ARRAY(T** x) { if(*x) { delete [] *x; *x = nullptr; } }

// Memory aligned free generics.
template<typename T, typename... Args>
inline void SAFE_RELEASE_DELETE_ALIGNED(T** x, Args... args) { if(*x) { (*x)->Release(args...); _aligned_free(*x); *x = nullptr; } }
template<typename T>
inline void SAFE_DELETE_ALIGNED(T** x) { if(*x) { _aligned_free(*x); *x = nullptr; } }

#endif
