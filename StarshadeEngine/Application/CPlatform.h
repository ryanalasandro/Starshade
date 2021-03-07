//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Application/CPlatform.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CPLATFORM_H
#define CPLATFORM_H

#include "CPlatformData.h"
#include "../Objects/CObject.h"
#include <vector>

namespace App {
	class CPlatform : private IObject {
	protected:
		CPlatform();
		virtual ~CPlatform();

	public:
		void Initialize() override;
		virtual bool Update(int& exitCode) = 0;

		const size_t GetMonitorCount() const { return m_monitorDataList.size(); }
		const const MONITOR_DATA& GetMonitor(size_t index) const { return m_monitorDataList[index]; }

		inline const wchar_t* GetIconPath() const { return m_iconPath; }
		inline void SetIconPath(const wchar_t* path) { m_iconPath = path; }

		inline void SetMainPanel(class CPanel* pPanel) { m_pMainPanel = pPanel; }

	protected:
		std::vector<MONITOR_DATA> m_monitorDataList;

	private:
		const wchar_t* m_iconPath;
		
		class CPanel* m_pMainPanel;
	};
};

#endif
