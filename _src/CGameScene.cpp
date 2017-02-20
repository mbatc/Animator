#include "CGameScene.h"
#include "Helper.h"

using namespace std;

CGameScene::CGameScene()
	:
	m_cameraList(CMR),
	m_lightList(LGT),
	m_objectList(OBJ),
	m_pFilename(NULL)
{}

CGameScene::~CGameScene()
{}

void CGameScene::PrintSceneInfo()
{
	cout << "SCENE INFO - \n";
	cout << "---------------\n";
	cout << "Filename >> " << (m_pFilename ? m_pFilename : "(null)") << endl;
	cout << "Object Count >> " << m_objectList.GetNumObjects() << endl;
	cout << "Light Count >> " << m_lightList.GetNumObjects() << endl;
	cout << "Camera Count >> " << m_cameraList.GetNumObjects() << endl << endl;

	cout << "\nSCENE OBECTS - \n";
	cout << "-----------------\n";
	
	for (int i = 0; i < m_objectList.GetNumObjects(); i++)
	{
		cout << m_objectList.GetGameObject(i)->GetName() << endl;
	}

	cout << "\nSCENE LIGHTS - \n";
	cout << "-----------------\n";

	for (int i = 0; i < m_lightList.GetNumObjects(); i++)
	{
		cout << m_lightList.GetGameObject(i)->GetName() << endl;
	}

	cout << "\nSCENE CAMERAS - \n";
	cout << "-----------------\n";

	for (int i = 0; i < m_cameraList.GetNumObjects(); i++)
	{
		cout << m_cameraList.GetGameObject(i)->GetName() << endl;
	}
}

void CGameScene::SetSceneDataFile(char* filename)
{
	if (!filename)
		return;

	STRING::Copy(m_pFilename, filename);
}

bool CGameScene::Update(float deltaTime)
{
	CGameobject* obj = NULL;
	for (int i = 0; i < m_cameraList.GetNumObjects(); i++)
	{
		obj = m_cameraList.GetGameObject(i);
		if (obj)
			obj->Update(deltaTime);
 	}
	for (int i = 0; i < m_lightList.GetNumObjects(); i++)
	{
		obj = m_lightList.GetGameObject(i);
		if (obj)
			obj->Update(deltaTime);
	}
	for (int i = 0; i < m_objectList.GetNumObjects(); i++)
	{
		obj = m_objectList.GetGameObject(i);
		if (obj)
			obj->Update(deltaTime);
	}

	return true;
}

bool CGameScene::Render(CD3DGraphics& gfx)
{
	CGameobject* obj = NULL;
	for (int i = 0; i <  m_objectList.GetNumObjects(); i++)
	{
		obj = m_objectList.GetGameObject(i);
		if (obj)
			obj->Render(gfx);
	}

	return true;
}

int CGameScene::AddGameobject(char* filename)
{
	return m_objectList.AddObjectFromFile(filename);
}

int CGameScene::AddLight(char* filename)
{
	return m_lightList.AddObjectFromFile(filename);
}

int CGameScene::AddCamera(char * filename)
{
	return m_cameraList.AddObjectFromFile(filename);
}

void CGameScene::DeleteLight(int index)
{
	m_lightList.DeleteObject(index);
}

void CGameScene::DeleteGameobject(int index)
{
	m_objectList.DeleteObject(index);
}

void CGameScene::DeleteCamera(int index)
{
	m_cameraList.DeleteObject(index);
}

void CGameScene::SetSceneName(const char * name)
{
	STRING::Copy(m_pName, name);
}

CGameobjectList& CGameScene::GetGameobjectList()
{
	return m_objectList;
}

CGameobject * CGameScene::GetGameobject(int index)
{
	return m_objectList.GetGameObject(index);
}

CGameobject * CGameScene::GetGameobject(char * name)
{
	return m_objectList.GetGameObject(name);
}

CGameobjectList& CGameScene::GetLightList()
{
	return m_lightList;
}

CGameobjectList& CGameScene::GetCameraList()
{
	return m_cameraList;
}

void CGameScene::ResetGamobjectComponentsParent()
{
	for (int i = 0; i < m_objectList.GetNumObjects(); i++)
	{
		CGameobject* obj = m_objectList.GetGameObject(i);
		if (obj)
			obj->SetComponentsParent();
	}
}