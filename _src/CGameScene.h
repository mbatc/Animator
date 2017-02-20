#pragma once

#include "CGameobject.h"
#include "CGameobjectList.h"

class CGameScene
{
	struct GameobjectInfo
	{
		GameobjectInfo()
			:
			UniqueName(NULL),
			index(-1)
		{}
		char* UniqueName;
		int index;
	};
public:
	CGameScene();
	~CGameScene();

	void PrintSceneInfo();
	void SetSceneDataFile(char* filename);

	bool Update(float deltaTime);
	bool Render(CD3DGraphics& gfx);

	int AddGameobject(char* filename);

	int AddLight(char* filename);

	int AddCamera(char* filename);

	void DeleteLight(int index);
	void DeleteGameobject(int index);
	void DeleteCamera(int index);

	void SetSceneName(const char* name);
	char* GetName() { return m_pName; }

	CGameobjectList& GetGameobjectList();
	CGameobject* GetGameobject(int index);
	CGameobject* GetGameobject(char* UniqueName);
	int GetNumberOfGameobjects() { return m_objectList.GetNumObjects(); }

	CGameobjectList& GetLightList();
	int GetNumberOfLights() { return m_lightList.GetNumObjects(); }

	CGameobjectList& GetCameraList();
	int GetNumberOfCameras() { return m_cameraList.GetNumObjects(); }

	void ResetGamobjectComponentsParent();
private:
	char* m_pFilename;
	char* m_pName;

	CGameobjectList m_lightList;
	CGameobjectList m_cameraList;
	CGameobjectList m_objectList;
};