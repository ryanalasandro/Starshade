//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPanel.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CPANEL_H
#define CPANEL_H

#include "../Objects/CObject.h"

namespace App {
	class CPanel : private IObject {
	protected:
		CPanel();
		virtual ~CPanel();

	public:
		void Initialize() override;
		void Release() override = 0;

		inline bool GetAllowDoubleClick() const { return m_bAllowDoubleClick; }
		inline void SetAllowDoubleClick(bool bAllow) { m_bAllowDoubleClick = bAllow; }

		inline bool GetTitle() const { return m_title; }
		inline void SetTitle(const wchar_t* title) { m_title = title; }

	protected:
		bool m_bAllowDoubleClick;

		const wchar_t* m_title;

		const CPanel* m_pParent;
		const class CPlatform* m_pPlatform;

	private:
	};
};

#endif
