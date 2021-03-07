//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Utilities/CMemory.cpp
//
//-------------------------------------------------------------------------------------------------

#include "CMemory.h"
#include <memory>

namespace Util {
	CMemory::CMemory() :
		m_memoryStage(-1),
		m_mainBlockSize(0),
		m_pMainMemory(nullptr) {
	}

	CMemory::~CMemory() {
	}

	void CMemory::Initialize() {
		m_pMainMemory = reinterpret_cast<char*>(malloc(m_mainBlockSize));
		m_mainStack.Initialize(m_pMainMemory, m_mainBlockSize);
	}
	
	// Method for moving to the next memory stage if not already at the last one.
	int CMemory::MoveToNextStage() {
		if(m_memoryStage == MEM_STAGE_END)
			return m_memoryStage;

		m_markerList[++m_memoryStage] = reinterpret_cast<uintptr_t>(m_mainStack.GetFrontMarker());

		return m_memoryStage;
	}
	
	// Method for moving to the prior memory stage if not already at the first one.
	int CMemory::MoveToPriorStage() {
		if(m_memoryStage == -1)
			return m_memoryStage;

		m_mainStack.PopFront(reinterpret_cast<const void*>(m_markerList[m_memoryStage]));
		m_markerList[m_memoryStage] = 0;

		return --m_memoryStage;
	}
	
	// Method for pulling from the stack.
	// Used in relation to the current memory stage.
	void* CMemory::PullFromStack(size_t sz, size_t alignment) {
		const uintptr_t frontPtr = reinterpret_cast<uintptr_t>(m_mainStack.GetFrontMarker());
		const uintptr_t misalignment = frontPtr & (alignment - 1);

		if(misalignment == 0) { // There is no misalignment.
			return m_mainStack.PullFrontNoPop(sz);
		} else { // handle misalignment.
			const ptrdiff_t adjustment = alignment - misalignment;
			char* rawPtr = reinterpret_cast<char*>(m_mainStack.PullFrontNoPop(sz + adjustment));
			return reinterpret_cast<void*>(rawPtr + adjustment);
		}
	}
	
	// Method for pulling which is meant to be a tempory allocation from the stack.
	void* CMemory::PullTempory(size_t sz, size_t alignment, const void** pMarkerOut) {
		if(pMarkerOut) *pMarkerOut = m_mainStack.GetBackMarker();
		
		const uintptr_t frontPtr = reinterpret_cast<uintptr_t>(m_mainStack.GetBackMarker()) - sz;
		const uintptr_t misalignment = frontPtr & (alignment - 1);
		
		if(misalignment == 0) { // There is no misalignment.
			return m_mainStack.PullBackNoPop(sz);
		} else { // handle misalignment.
			char* rawPtr = reinterpret_cast<char*>(m_mainStack.PullBackNoPop(sz + misalignment));
			return reinterpret_cast<void*>(rawPtr);
		}
	}

	void CMemory::Release() {
		m_mainStack.Clear();
		free(m_pMainMemory);
		m_pMainMemory = nullptr;
	}
};
