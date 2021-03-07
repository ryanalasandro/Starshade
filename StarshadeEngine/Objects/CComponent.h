//-------------------------------------------------------------------------------------------------
//
// Copyright (c) Ryan Alasandro
//
// Static Library: Starshade Engine
//
// File: Objects/CComponent.h
//
//-------------------------------------------------------------------------------------------------

#ifndef CCOMPONENT_H
#define CCOMPONENT_H

class IObject;

class CComponent {
protected:
	CComponent(const IObject* pObject) : 
		m_pObject(pObject) {}

protected:
	const class IObject* m_pObject;
};

#endif
