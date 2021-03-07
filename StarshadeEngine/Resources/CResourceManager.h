//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Resources/CResourceManager.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

namespace Resources {
	class CManager {
	public:
	public:
		static CManager& Instance() {
			static CManager instance;
			return instance;
		}

	private:
		CManager();
		~CManager();
		CManager(const CManager&) = delete;
		CManager(CManager&&) = delete;
		CManager& operator = (const CManager&) = delete;
		CManager& operator = (CManager&&) = delete;
	};
};

#endif
