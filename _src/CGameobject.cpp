#include "CGameobject.h"
#include "Helper.h"

CGameobject::CGameobject()
	:
	m_nComponents(0),
	m_component(NULL),
	m_type(GAMEOBJECT_TYPE::OBJ_DEFAULT),
	m_pName(NULL),
	m_pParent(NULL),
	m_ppChild(NULL),
	m_nChild(0)
{}

CGameobject::~CGameobject()
{}

void CGameobject::Render(CD3DGraphics& gfx)
{
	for(int i = 0; i < m_nComponents; i++)
	{
		if(m_component[i])
			m_component[i]->Render(gfx);
	}
}

void CGameobject::Update(float deltaTime)
{
	if (m_pParent)
	{
		//2D transformations Man (cus 2d game lmao).
	}

	for(int i = 0; i < m_nComponents; i++)
	{
		if(m_component[i])
			m_component[i]->Update(deltaTime);
	}
}

void CGameobject::AddComponent(CComponent* add)
{
	add->SetParentObject(this);
	MEMORY::LIST::AddToListArr(m_component,add,m_nComponents);
}

CComponent* CGameobject::GetComponent(COMPONENTID id, int index)
{
	if(index < 0)
		return NULL;

	for(int i = 0, ci = 0; i < m_nComponents; i++)
	{
		if(m_component[i]->GetID() == id)
		{
			if(ci == index)
			{
				return m_component[i];
			}
			else
			{
				ci++;
			}
		}
	}

	return NULL;
}

void CGameobject::SetName(char * src)
{
	STRING::Copy(m_pName, src);
}

void CGameobject::SetComponentsParent()
{
	for (int i = 0; i < m_nComponents; i++)
	{
		if (m_component[i])
			m_component[i]->SetParentObject(this);
	}
}

CComponent* CGameobject::GetComponent(COMPONENTID id)
{
	return GetComponent(id, 0);
}

void CGameobject::InitChildren()
{
	for (int i = 0; i < m_nChild; i++)
	{
		m_ppChild[i]->SetParent(this);
	}
}
