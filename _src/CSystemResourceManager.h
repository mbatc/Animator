#pragma once
#include "CTexture.h"
#include "CGameobject.h"
#include "CD3DGraphics.h"

enum SYSTEM_RESOURCE_TYPE
{
	DEFAULT,
	TEXTURE,
	OBJECT,
};

namespace SYSTEM_RESOURCE
{
	struct Resource
	{
		Resource()
			:
			m_type(DEFAULT),
			resourceName(0),
			filename(0)
		{}
		SYSTEM_RESOURCE_TYPE m_type;
		char* resourceName;
		char* filename;
	};
	struct ResourceTexture : public Resource
	{
		ResourceTexture()
			:
			m_pTexture(NULL)
		{}
		CTexture* m_pTexture;
	};
	struct ResourceObject : public Resource
	{
		CGameobject m_obj;
	};
};

class CSystemResourceManager
{
public:
	CSystemResourceManager();
	~CSystemResourceManager();

	void Init(CD3DGraphics* gfx);

	CTexture*	GetTexture(char* filename);
	CGameobject GetGameobject(char* filename);
	
	int DoesResourceExist(SYSTEM_RESOURCE_TYPE type, char* filename);
private:

	bool CreateTextureResource(char* filename);
	bool CreateObjectResource(char* filename);

private:
	CD3DGraphics* m_gfx;

	SYSTEM_RESOURCE::ResourceTexture*	m_pSystemTexture;
	SYSTEM_RESOURCE::ResourceObject*	m_pSystemObject;

	int m_nTextures;
	int m_nObjects;
};
