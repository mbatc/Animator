#ifdef _WIN32 || _WIN64

#pragma once

#include "CGameobject.h"
#include "Helper.h"

enum OBJECT_TYPE
{
	OBJ,
	LGT,
	CMR
};

struct ObjectList
{
	ObjectList()
		:
		obj(NULL),
		ID(0),
		name(NULL)
	{}
	void Cleanup()
	{
		MEMORY::SafeDelete(obj);
		MEMORY::SafeDeleteArr(name);
	}
	CGameobject* obj;
	int ID;
	char* name;
};

class CGameobjectList
{
public:
	CGameobjectList(OBJECT_TYPE type);
	~CGameobjectList();

	//Add a gameobject to the list
	int AddObject(char* name);
	//Add a gameobject loaded from a file
	int AddObjectFromFile(char* filename);

	//Delete A gameobject by Index in list
	void DeleteObject(int index);
	//Delete A gameobject by Name. Will remove the first object with that name
	void DeleteObject(char* name);
	//Delete A gameobject by Name and Index. Will remove and object with that name
	void DeleteObject(char* name, int ID);

	//Returns the number of objects in the list
	int GetNumObjects();
	
	//Returns the object at index
	CGameobject* GetGameObject(int index);
	//Returns the object with that name
	CGameobject* GetGameObject(char* uniqueName);
	//returns the object with that name after finding it ID times
	CGameobject* GetGameObject(char* name, int ID);

	void ClearList();
private:
	ObjectList* m_pObjList;
	int m_nObjects;
	OBJECT_TYPE m_objType;
};

#endif
