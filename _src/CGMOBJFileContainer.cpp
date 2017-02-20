#include "CGMOBJFileContainer.h"
#include "CGameobject.h"
#include "Helper.h"
#include "CWindow.h"
#include "CSystem.h"

#include <iostream>

using namespace std;

FILE_GMOBJ_ATTRIBUTES GetGMOBJFileAttribute(char* buffer)
{
	if (!strcmp(buffer, _FILE_GMOBJ_NAME))
		return FILE_GMOBJ_ATTRIBUTES::GMOBJ_NAME;
	if (!strcmp(buffer, _FILE_GMOBJ_COMPONENT))
		return FILE_GMOBJ_ATTRIBUTES::GMOBJ_COMPONENT;
	if (!strcmp(buffer, _FILE_GMOBJ_COMPONENT_PROPERTIES))
		return FILE_GMOBJ_ATTRIBUTES::GMOBJ_COMPONENT_PROPERTIES;
	if (!strcmp(buffer, _FILE_GMOBJ_CHILD_OBJECT))
		return FILE_GMOBJ_ATTRIBUTES::GMOBJ_CHILD_OBJECT;
	if (!strcmp(buffer, _FILE_GMOBJ_ROTATION))
		return FILE_GMOBJ_ATTRIBUTES::GMOBJ_ROTATION;
	if (!strcmp(buffer, _FILE_GMOBJ_TRANSFORM))
		return FILE_GMOBJ_ATTRIBUTES::GMOBJ_TRANSFORM;
	if (!strcmp(buffer, _FILE_GMOBJ_SCALE))
		return FILE_GMOBJ_ATTRIBUTES::GMOBJ_SCALE;

	return (FILE_GMOBJ_ATTRIBUTES)UNKNOWN;
}



CGMOBJFileContainer::CGMOBJFileContainer()
{}

CGMOBJFileContainer::~CGMOBJFileContainer()
{}

void CGMOBJFileContainer::LoadCurrentFile(CGameobject* obj)
{
	if (!m_pCurrentFile)
	{
		cout << "CGMOBJFileContainer: Attempted to load but no file has been set...\n";
		return;
	}
	if (!obj)
	{
		cout << "CFileGMOBContainer: Failed to load object - NULL object pointer...\n";
		return;
	}

	m_currentObject = obj;

	m_pFile = NULL;
	m_pFile = fopen(m_pCurrentFile, "rb");

	if (!m_pFile)
	{
		cout << "CFileGMOBJContainer: Could not open file '" << m_pCurrentFile << '...\n';
	}

	if (m_pFile)
	{
		do
		{
			char buffer[32] = { 0 };
			if (!feof(m_pFile))
			{
				fscanf(m_pFile, "%s", buffer);
				if (!ProcessToken(buffer))
				{
					cout << "CGMOBJFileContainer: Something When Wrong Wile Loading Object '" << m_pCurrentFile << "'...\n";
					return;
				}
			}
		} while (!feof(m_pFile));
	}

	if(m_pFile)
		fclose(m_pFile);
}

bool CGMOBJFileContainer::ProcessToken(char* buffer)
{
	switch (GetGMOBJFileAttribute(buffer))
	{
	case FILE_GMOBJ_ATTRIBUTES::GMOBJ_COMPONENT:
		if (!LoadComponent())
			return false;
		break;
	case FILE_GMOBJ_ATTRIBUTES::GMOBJ_COMPONENT_PROPERTIES:
		if (!LoadComponentProperties())
			return false;
		break;
	case FILE_GMOBJ_ATTRIBUTES::GMOBJ_CHILD_OBJECT:
		if (!LoadChildObject())
			return false;
		break;
	case FILE_GMOBJ_ATTRIBUTES::GMOBJ_NAME:
		if (!LoadObjectName())
			return false;
	case (FILE_GMOBJ_ATTRIBUTES)UNKNOWN:
		break;
	}

	return true;
}

bool CGMOBJFileContainer::LoadComponent()
{
	int len = GetNextScanLength();
	char* buffer = new char[len];
	fscanf(m_pFile, "%s", buffer);

	CGMOBJFileComponentLoader c_loader;

	switch (GetGMOBJFileAttribute(buffer))
	{
	case UNKNOWN:
		cout << "Unkown component: " << buffer << "...\n";
		return false;
	}
	return true;
}

bool CGMOBJFileContainer::LoadObjectName()
{
	char* buffer = NULL;
	FILEH::LOAD::SafeScanString(m_pFile, buffer);
	if (!buffer)
		return false;

	m_currentObject->SetName(buffer);
	return true;
}

bool CGMOBJFileContainer::LoadComponentProperties()
{
	return true;
}

bool CGMOBJFileContainer::LoadChildObject()
{
	char* buffer = NULL;
	char* file_ext = NULL;
	GAMEOBJECT_TYPE type = OBJ_DEFAULT;
	CGameScene* scn = NULL;
	CGameobject* child = NULL;

	FILEH::LOAD::GetStringQuotation(m_pFile, buffer);
	file_ext = STRING::ScanFromEnd(buffer,".");

	if (!strcmp(file_ext, ".gmobj"))
	{
		type = OBJ_DEFAULT;
	}
	else if (!strcmp(file_ext, ".gmlgt"))
	{
		type = OBJ_LIGHT;
	}
	else if (!strcmp(file_ext, ".gmcmr"))
	{
		type = OBJ_CAMERA;
	}
	else
	{
		cout << "CGMOBJFileContainer: Invalid Gameobject file '" << buffer << 
				"'...\n\t- while loading child object..." <<
				"\n\t- check '" << m_pCurrentFile << "' for this error...\n";

		return false;
	}



	if (PUMP::g_system)
		scn = PUMP::g_system->GetGameData()->GetScene();

	int scn_index = -1;
	if (scn)
	{
		switch (type)
		{
		case OBJ_DEFAULT:
			scn_index = scn->AddGameobject(buffer);
			child = scn->GetGameobject(scn_index);
			break;
		}
	}
	else
	{
		child = new CGameobject();
		*child = SYSTEM::RESOURCE::GetGameobject(buffer);
	}

	float xPos = 0.0f, yPos = 0.0f, zPos = 0.0f,
		xRot = 0.0f, yRot = 0.0f, zRot = 0.0f,
		xScl = 0.0f, yScl = 0.0f, zScl = 0.0f;

	fscanf(m_pFile, "%f%f%f%f%f%f%f%f%f", &xPos, &yPos, &zPos,
		&xRot, &yRot, &zRot,
		&xScl, &yScl, &zScl);


	child->SetLocalTransform(CTransform(CVector3(xPos, yPos, zPos),
		CVector3(xRot, yRot, zRot),
		CVector3(xScl, yScl, zScl)));

	child->SetParent(m_currentObject);
	m_currentObject->AddChild(child);


	return true;
}
