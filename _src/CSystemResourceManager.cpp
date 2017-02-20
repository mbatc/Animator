#include "CSystemResourceManager.h"

#include "CGMOBJFileContainer.h"

#include "Helper.h"

#include <iostream>
using namespace std;

CSystemResourceManager::CSystemResourceManager()
	:
	m_pSystemTexture(NULL),
	m_pSystemObject(NULL),
	m_nTextures(0),
	m_nObjects(0)
{}

CSystemResourceManager::~CSystemResourceManager()
{
	MEMORY::SafeDeleteArr(m_pSystemObject);
	MEMORY::SafeDeleteArr(m_pSystemTexture);
}

void CSystemResourceManager::Init(CD3DGraphics* gfx)
{
	m_gfx = gfx;
}

CTexture* CSystemResourceManager::GetTexture(char* filename)
{
	int index = DoesResourceExist(SYSTEM_RESOURCE_TYPE::TEXTURE, filename);

	if (index != -1)
	{
		return m_pSystemTexture[index].m_pTexture;
	}
	else
	{
		CreateTextureResource(filename);
		index = DoesResourceExist(SYSTEM_RESOURCE_TYPE::TEXTURE, filename);

		if (index == -1)
		{
			cout << "Could Not Create Font From File: " << filename << '...\n';
			return NULL;
		}

		return m_pSystemTexture[index].m_pTexture;
	}
}

CGameobject CSystemResourceManager::GetGameobject(char * filename)
{
	CreateObjectResource(filename);
	return m_pSystemObject[m_nObjects-1].m_obj;
}
bool CSystemResourceManager::CreateTextureResource(char* filename)
{
	CTexture* temp = new CTexture();
	temp->SetFilename(filename);
	temp->InitTexture(*m_gfx);

	SYSTEM_RESOURCE::ResourceTexture texture;
	texture.m_pTexture = temp;
	texture.m_type = SYSTEM_RESOURCE_TYPE::TEXTURE;
	STRING::Copy(texture.filename, filename);

	MEMORY::LIST::AddToList(m_pSystemTexture, &texture, m_nTextures);
	return true;
}


bool CSystemResourceManager::CreateObjectResource(char * filename)
{
	CGMOBJFileContainer constructor;
	SYSTEM_RESOURCE::ResourceObject object;

	constructor.SetFilename(filename);
	constructor.LoadCurrentFile(&object.m_obj);
	object.m_type = SYSTEM_RESOURCE_TYPE::OBJECT;
	STRING::Copy(object.filename, filename);
	
	MEMORY::LIST::AddToList(m_pSystemObject, &object, m_nObjects);
	return true;
}

int CSystemResourceManager::DoesResourceExist(SYSTEM_RESOURCE_TYPE type,char* filename)
{
	switch (type)
	{
	case SYSTEM_RESOURCE_TYPE::DEFAULT:
		return -1;
	case SYSTEM_RESOURCE_TYPE::TEXTURE:
		for (int i = 0; i < m_nTextures; i++)
		{
			if (m_pSystemTexture[i].m_type == type &&
				!strcmp(m_pSystemTexture[i].filename, filename))
				return i;
		}
		break;
	case SYSTEM_RESOURCE_TYPE::OBJECT:
		for (int i = 0; i < m_nObjects; i++)
		{
			if (m_pSystemObject[i].m_type == type &&
				!strcmp(m_pSystemObject[i].filename, filename))
				return i;
		}
	}
	return -1;
}