#ifdef _WIN32 || _WIN64
#include "CGameobjectList.h"

#include "Helper.h"

CGameobjectList::CGameobjectList(OBJECT_TYPE type)
	:
	m_pObjList(nullptr),
	m_objType(type),
	m_nObjects(0)
{}

CGameobjectList::~CGameobjectList()
{}

int CGameobjectList::AddObject(char* name)
{
	if (!name)
		name = "NULL";
	//Creating an empty gameobject and adding it to the object list
	int index = 0;

	ObjectList* newObject = 0;

	//GETTING A UNIQUE ID FOR OBJECTS WITH THE SAME NAME
	bool isUnique = false;
	do
	{
		for (int i = 0; i < m_nObjects; i++)
		{
			if (!strcmp(m_pObjList[i].name, name))
			{
				if (index == m_pObjList[i].ID)
				{
					index++;
					break;
				}
			}

			if (i == m_nObjects - 1)
				isUnique = true;
		}
		if (m_nObjects == 0)
			break;
	} while (!isUnique);
	
	switch (m_objType)
	{
	case OBJ:
		newObject = new ObjectList;
		newObject->obj = new CGameobject;
		break;
	}
	if (!newObject)
		return -1;
	if (!newObject->obj)
		return -1;

	newObject->ID = index;
	STRING::Copy(newObject->name, name);

	char uniqueID[128] = { 0 };

	//SETTING UNIQUE NAME (CGameobject.SetName)
	sprintf(uniqueID, "%s.%d", newObject->name,index);
	newObject->obj->SetName(uniqueID);
	MEMORY::LIST::AddToList(m_pObjList,newObject,m_nObjects);

	return m_nObjects-1;
}

int CGameobjectList::AddObjectFromFile(char* filename)
{
	ObjectList* newObject = NULL;
	switch (m_objType)
	{
	case OBJ:
		newObject = new ObjectList;
		newObject->obj = new CGameobject;
		*newObject->obj = SYSTEM::RESOURCE::GetGameobject(filename);
		break;
	}

	if (!newObject)
		return -1;

	char* name = newObject->obj->GetName();
	int index = 0;
	//GETTING A UNIQUE ID FOR OBJECTS WITH THE SAME NAME
	bool isUnique = false;
	do
	{
		for (int i = 0; i < m_nObjects; i++)
		{
			if (!strcmp(m_pObjList[i].name, name))
			{
				if (index == m_pObjList[i].ID)
				{
					index++;
					break;
				}
			}

			if (i == m_nObjects - 1)
				isUnique = true;
		}
		if (m_nObjects == 0)
			break;
	} while (!isUnique);

	newObject->ID = index;
	STRING::Copy(newObject->name, name);

	char uniqueID[128] = { 0 };
	sprintf(uniqueID, "%s.%d", newObject->name, index);
	newObject->obj->SetName(uniqueID);
	newObject->obj->SetComponentsParent();
	newObject->obj->InitChildren();
	MEMORY::LIST::AddToList(m_pObjList, newObject, m_nObjects);
	
	return m_nObjects - 1;
}

void CGameobjectList::DeleteObject(int index)
{
	if (index >= m_nObjects)
		return;
	if (index < 0)
		return;

	ObjectList* tempList = new ObjectList[m_nObjects - 1];
	for (int i = 0, t_i = 0; i < m_nObjects; i++)
	{
		if (i != index)
			tempList[t_i] = m_pObjList[i];
		else
			t_i--;
		t_i++;
	}

	m_pObjList[index].Cleanup();
	MEMORY::SafeDeleteArr(m_pObjList);

	m_pObjList = tempList;
	m_nObjects--;
}

void CGameobjectList::DeleteObject(char* name)
{
	if (!name)
		return;

	for (int i = 0; i < m_nObjects; i++)
	{
		if (!strcmp(m_pObjList[i].obj->GetName(), name))
		{
			DeleteObject(i);
			break;
		}
	}
}

void CGameobjectList::DeleteObject(char* name, int ID)
{
	if (!name)
		return;

	for (int i = 0; i < m_nObjects; i++)
	{
		if (!strcmp(m_pObjList[i].name, name) &&
			ID == m_pObjList[i].ID)
		{
			DeleteObject(i);
			break;
		}
	}
}

CGameobject* CGameobjectList::GetGameObject(int index)
{
	if (index >= m_nObjects)
		return NULL;
	if (index < 0)
		return NULL;

	return m_pObjList[index].obj;
}

CGameobject* CGameobjectList::GetGameObject(char* uniqueName)
{
	if (!uniqueName)
		return NULL;

	for (int i = 0; i < m_nObjects; i++)
	{
		if (!strcmp(uniqueName, m_pObjList[i].obj->GetName()))
			return m_pObjList[i].obj;
	}

	return NULL;
}

CGameobject* CGameobjectList::GetGameObject(char* name, int ID)
{
	for (int i = 0; i < m_nObjects; i++)
	{
		if (!strcmp(name, m_pObjList[i].name) &&
			m_pObjList[i].ID == ID)
			return m_pObjList[i].obj;
	}
	return NULL;
}

void CGameobjectList::ClearList()
{
	MEMORY::SafeDeleteArr(m_pObjList);
	m_nObjects = 0;
}

int CGameobjectList::GetNumObjects()
{
	return m_nObjects;
}
#endif