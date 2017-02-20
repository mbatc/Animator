#pragma once
#include <stdio.h>
#include "CFileContainer.h"
#include "CGameobject.h"

#define _FILE_GMOBJ_COMPONENT				"c"
#define _FILE_GMOBJ_COMPONENT_PROPERTIES	"c_p"
#define _FILE_GMOBJ_TRANSFORM				"t"
#define _FILE_GMOBJ_ROTATION				"r"
#define _FILE_GMOBJ_SCALE					"s"
#define _FILE_GMOBJ_NAME					"nm"
#define _FILE_GMOBJ_CHILD_OBJECT			"c_obj"

enum FILE_GMOBJ_ATTRIBUTES
{
	GMOBJ_COMPONENT,
	GMOBJ_COMPONENT_PROPERTIES,
	GMOBJ_TRANSFORM,
	GMOBJ_ROTATION,
	GMOBJ_SCALE,
	GMOBJ_NAME,
	GMOBJ_CHILD_OBJECT,
};

class CGameobject;

class CGMOBJFileContainer : public CFileContainer
{
public:
	CGMOBJFileContainer();
	~CGMOBJFileContainer();

	void LoadCurrentFile(CGameobject* obj);

private:
	bool ProcessToken(char* buffer);

	bool LoadComponent();
	bool LoadObjectName();
	bool LoadComponentProperties();
	bool LoadChildObject();

	char* m_pObjectName;
	CGameobject* m_currentObject;
};

class CGMOBJFileComponentLoader : public CFileContainer
{
public:
	CGMOBJFileComponentLoader();
	~CGMOBJFileComponentLoader();

public:
	//ADD COMPONENTS
};