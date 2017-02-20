#pragma once

#include "CD3DGraphics.h"
#include "ComponentHelper.h"
#include "StringHelper.h"

class CGameobject;

class CComponent
{
public:
	CComponent(COMPONENTID m_ID)
		:
	m_ID(m_ID),
	m_parent(NULL),
	m_pName(NULL)
	{}
	~CComponent()
	{}

	virtual void Update(float deltaTime){}
	virtual void Render(CD3DGraphics& gfx){}

	void SetParentObject(CGameobject* parent)
	{
		m_parent = parent;
	}

	void SetName(char* src) { STRING::Copy(m_pName, src); }
	char* GetName() { return m_pName; }

	COMPONENTID GetID() const
	{
		return m_ID;
	}

protected:
	COMPONENTID m_ID;

	CGameobject* m_parent;
	char* m_pName;
};

class CGraphicsComponent : public CComponent
{
public:
	CGraphicsComponent(COMPONENTID m_ID)
		:
		CComponent(m_ID)
	{}
	~CGraphicsComponent() {}

	virtual bool InitGFX(CD3DGraphics& gfx) = 0;
};