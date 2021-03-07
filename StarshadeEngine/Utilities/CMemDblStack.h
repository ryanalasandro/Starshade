//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Utilities/CMemDblStack.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CMEMDBLSTACK_H
#define CMEMDBLSTACK_H

namespace Util {
	class CMemDblStack {
	public:
		CMemDblStack();
		~CMemDblStack();
		CMemDblStack(const CMemDblStack&) = delete;
		CMemDblStack(CMemDblStack&&) = delete;
		CMemDblStack& operator = (const CMemDblStack&) = delete;
		CMemDblStack& operator = (CMemDblStack&&) = delete;
		
		void Initialize(char* pData, size_t sz);
		void* PullFront(size_t pullSz);
		void* PullBack(size_t pullSz);
		void* PullFrontNoPop(size_t pullSz);
		void* PullBackNoPop(size_t pullSz);
		void PopFront(const void* pMarker = nullptr);
		void PopBack(const void* pMarker = nullptr);
		
		inline const void* GetFrontMarker() const { return reinterpret_cast<const void*>(m_pFront); }
		inline const void* GetBackMarker() const { return reinterpret_cast<const void*>(m_pBack); }

		inline void ClearFront() { m_pFront = m_pHead; }
		inline void ClearBack() { m_pBack = m_pTail; }

		inline void Clear() {
			m_pFront = m_pHead;
			m_pBack = m_pTail;
		}

	private:
		size_t m_dataSize;
		char* m_pHead;
		char* m_pFront;
		char* m_pBack;
		char* m_pTail;
	};
};

#endif
