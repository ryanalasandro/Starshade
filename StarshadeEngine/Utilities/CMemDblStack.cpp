//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Utilities/CMemDblStack.cpp
//
//-------------------------------------------------------------------------------------------------

#include "CMemDblStack.h"
#include <cstdint>
#include <assert.h>
#include <cstring>

namespace Util {
	CMemDblStack::CMemDblStack() {
		memset(this, 0, sizeof(CMemDblStack));
	}

	CMemDblStack::~CMemDblStack() {
	}

	// Initialize double-ended stack with pre-allocated data.
	void CMemDblStack::Initialize(char* pData, size_t sz) {
		m_dataSize = sz;
		m_pHead = pData;
		m_pTail = pData + (sz - 1);
		m_pFront = m_pHead;
		m_pBack = m_pTail;
	}
	
	// Method for pulling from front stack.
	void* CMemDblStack::PullFront(size_t pullSz) {
		void* ptr = reinterpret_cast<void*>(m_pFront);
		m_pFront += pullSz;
		
		// Calculate adjustment for alignment.
		const uintptr_t newAddress = reinterpret_cast<uintptr_t>(m_pFront);
		const size_t mask = sizeof(size_t) - 1;
		const uintptr_t misalignment = newAddress & mask;
		const ptrdiff_t adjustment = misalignment > 0 ? sizeof(size_t) - misalignment : 0;

		pullSz += adjustment;
		m_pFront += adjustment;
		*reinterpret_cast<size_t*>(m_pFront) = pullSz;
		m_pFront += sizeof(size_t);

		assert(reinterpret_cast<uintptr_t>(m_pFront) < reinterpret_cast<uintptr_t>(m_pBack));
		return ptr;
	}
	
	// Method for pulling from back stack.
	void* CMemDblStack::PullBack(size_t pullSz) {
		m_pBack -= pullSz;
		void* ptr = reinterpret_cast<void*>(m_pBack);
		m_pBack -= sizeof(size_t);
		
		// Calculate adjustment for alignment.
		const uintptr_t newAddress = reinterpret_cast<uintptr_t>(m_pBack);
		const size_t mask = sizeof(size_t) - 1;
		const uintptr_t misalignment = newAddress & mask;

		pullSz += misalignment;
		m_pBack -= misalignment;
		*reinterpret_cast<size_t*>(m_pBack) = pullSz;

		assert(reinterpret_cast<uintptr_t>(m_pFront) < reinterpret_cast<uintptr_t>(m_pBack));
		return ptr;
	}
	
	// Method for pulling from front stack without support for popping to prior address.
	// Should only be used if you're using a marker for popping from the stack.
	void* CMemDblStack::PullFrontNoPop(size_t pullSz) {
		void* ptr = reinterpret_cast<void*>(m_pFront);
		m_pFront += pullSz;
		assert(reinterpret_cast<uintptr_t>(m_pFront) < reinterpret_cast<uintptr_t>(m_pBack));
		return ptr;
	}
	
	// Method for pulling from back stack without support for popping to prior address.
	// Should only be used if you're using a marker for popping from the stack.
	void* CMemDblStack::PullBackNoPop(size_t pullSz) {
		m_pBack -= pullSz;
		void* ptr = reinterpret_cast<void*>(m_pBack);
		assert(reinterpret_cast<uintptr_t>(m_pFront) < reinterpret_cast<uintptr_t>(m_pBack));
		return ptr;
	}
	
	// Method for popping the front stack to an optional marker.
	void CMemDblStack::PopFront(const void* pMarker) {
		if(pMarker) {
			m_pFront = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(pMarker));
			return;
		}

		assert(m_pFront != m_pHead);
		m_pFront -= sizeof(size_t);
		m_pFront -= *reinterpret_cast<size_t*>(m_pFront);
	}
	
	// Method for popping the back stack to an optional marker.
	void CMemDblStack::PopBack(const void* pMarker) {
		if(pMarker) {
			m_pBack = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(pMarker));
			return;
		}

		assert(m_pBack != m_pTail);
		m_pBack += *reinterpret_cast<size_t*>(m_pBack) + sizeof(size_t);
	}
};
