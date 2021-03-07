//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Objects/CObject.h
//
//-------------------------------------------------------------------------------------------------

#ifndef COBJECT_H
#define COBJECT_H

namespace App {
	class CPanel;
};

namespace Resources {
	class CManager;
};

class IObject {
public:
	friend Resources::CManager;

protected:
	IObject() { }
	virtual ~IObject() { }
	IObject(const IObject&) = delete;
	IObject(IObject&&) = delete;
	IObject& operator = (const IObject&) = delete;
	IObject& operator = (IObject&&) = delete;

	virtual void PreInitialize() { }
	virtual void Initialize() { }
	virtual void PostInitialize() { }
	virtual void Reset() { }

	virtual void OnConnect() { }
	virtual void OnDisconnect() { }

	virtual void PreUpdate() { }
	virtual void Update() { }
	virtual void PostUpdate() { }

	virtual void PreRender() { }
	virtual void Render() { }
	virtual void PostRender() { }

	virtual void PreRenderUI() { }
	virtual void RenderUI() { }
	virtual void PostRenderUI() { }

	virtual void OnResize(const App::CPanel* pPanel) { }
	
	virtual void Release() { }
};

class CGameObject : public IObject {
protected:
	CGameObject() { }
	virtual ~CGameObject() { }
};

#endif
