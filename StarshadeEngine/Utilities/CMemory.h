//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Utilities/CMemory.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CMEMORY_H
#define CMEMORY_H

#include "CMemDblStack.h"
#include <cstdint>

namespace Util {
	class CMemory {
	public:
		enum MEM_STAGE {
			MEM_STAGE_INIT,
			MEM_STAGE_PLATFORM,
			MEM_STAGE_GRAPHICS_API,
			MEM_STAGE_ASSETS,
			MEM_STAGE_LEVEL,
			MEM_STAGE_END,
		};

	public:
		static CMemory& Instance() {
			static CMemory instance;
			return instance;
		}

	private:
		CMemory();
		~CMemory();
		CMemory(const CMemory&) = delete;
		CMemory(CMemory&&) = delete;
		CMemory& operator = (const CMemory&) = delete;
		CMemory& operator = (CMemory&&) = delete;

	public:
		void Initialize();
		int MoveToNextStage();
		int MoveToPriorStage();
		void* PullFromStack(size_t sz, size_t alignment);
		void* PullTempory(size_t sz, size_t alignment, const void** ppMarkerOut);
		inline void PopTempory(const void* pMarker) { m_mainStack.PopBack(pMarker); }
		inline void ClearTempory() { m_mainStack.ClearBack(); }
		void Release();
		
		inline void SetMainBlockSize(size_t blockSize) { m_mainBlockSize = blockSize; }

		inline const void* GetFrontMarker() const { return m_mainStack.GetFrontMarker(); }
		inline const void* GetBackMarker() const { return m_mainStack.GetBackMarker(); }

		/*template<typename T,  typename... Args>
		inline T* PullNew(size_t alignment, Args... args) {
			return new (reinterpret_cast<T*>(PullFromStack(sizeof(T), alignment))) T(args...);
		}*/

		template<typename T, typename... Args>
		inline T* PullNew(size_t alignment, bool bFromStack, Args... args) {
			if(bFromStack) {
				return new (reinterpret_cast<T*>(PullFromStack(sizeof(T), alignment))) T(args...);
			} else {
				return new (_aligned_malloc(sizeof(T), alignment))T(args...);
			}
		}

	private:
		int m_memoryStage;
		size_t m_mainBlockSize;

		uintptr_t m_markerList[MEM_STAGE_END];
		char* m_pMainMemory;
		CMemDblStack m_mainStack;
	};
};

#endif
