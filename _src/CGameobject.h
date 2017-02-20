#pragma once

#include "CD3DGraphics.h"
#include "CComponent.h"
#include "CTransform.h"

#include "MemoryHelper.h"

#define DEFAULT_LAYER 4
#define BACKGROUND_LAYER 1
#define DECORB_LAYER 2
#define DECORF_LAYER 5
#define FOREGROUND_LAYER 6
enum GAMEOBJECT_TYPE
{
	OBJ_DEFAULT,
	OBJ_LIGHT,
	OBJ_CAMERA
};

class CGameobject
{
public:
	CGameobject();
	~CGameobject();

	void Update(float deltaTime);
	void Render(CD3DGraphics& gfx);
	void Collider();

	void AddComponent(CComponent* add);

	//Returns the first component it finds
	//with the specified ID.
	CComponent* GetComponent(COMPONENTID id);
	int GetNumberOfComponents() { return m_nComponents; }
	void SetParent(CGameobject* obj) { m_pParent = obj; }
	int AddChild(CGameobject* obj) { MEMORY::LIST::AddToListArr(m_ppChild, obj, m_nChild); return m_nChild - 1; }
	CGameobject* GetChild(int index) { if (index >= 0 && index < m_nChild) return m_ppChild[index]; }
	void RemoveChild(int index) { MEMORY::LIST::RemoveItem(m_ppChild, m_nChild, index); }
	void InitChildren();
	/*
	The index is for the specific id.
	E.G. if the index is 2 then it will return
	the 3rd component with the specied ID.
	
	Useful if you know the order that the components where added in.*/
	CComponent* GetComponent(COMPONENTID id, int index);

	CTransform& GetTransform() {return m_transform;}
	CTransform& GetLocalTransform() { return m_localTransform; }
	void SetLocalTransform(CTransform transform) { m_localTransform = transform; }

	void SetName(char* src);
	char* GetName(){return m_pName;}
	void SetComponentsParent();

protected:
	CTransform m_transform;
	CTransform m_localTransform;

	CGameobject* m_pParent;

	CGameobject**	m_ppChild;
	int				m_nChild;

	CComponent**	m_component;
	int				m_nComponents;

	GAMEOBJECT_TYPE m_type;
	char* m_pName;
};