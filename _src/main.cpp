#include <Windows.h>

#include "CSystem.h"
#include "CSourceAnalyser.h"
#include "Helper.h"

#include <iostream>
using namespace std;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
#ifdef _DEBUG
	AllocConsole();
	freopen("CONIN$", "r",stdin);
	freopen("CONOUT$", "w",stdout);
	freopen("CONOUT$", "w",stderr);
#endif

	CSourceAnalyser m_src;
	m_src.GetInfo();
	m_src.PrintData();

	////////////////////////////////////////////////////////////////////////
	//Initialization
	cout << "Initializing System...\n...\n";
	CSystem* mainSystem = new CSystem;
	if(!mainSystem->Init(hInstance,lpCmdLine))
	{
		MessageBox(NULL, "Failed To Initialize The Game System!","Error!",
			MB_OK | MB_ICONEXCLAMATION );
		cout << "\n" <<"Failed To Initialize The Game System!\n";
		return -1;
	}
	//**********************************************************************
	//Program Loop
	cout << "Beginning Main Program Loop...\n======================================\n\n";
	int result = mainSystem->Run();
	cout << "\n======================================\n...\nMain Program Loop Has Ended...\n\n";

	//Cleanup
	cout << " --Deleting Message Pipeline...\n";
	MEMORY::SafeDelete(CSystem::g_MessagePipeline);
	cout << " --Deleting Graphics Initialization Pump...\n";
	MEMORY::SafeDelete(CSystem::g_gfxInitPump);
	cout << " --Deleting Resource Manager...\n";
	MEMORY::SafeDelete(CSystem::g_resourceManager);

	cout << " --Cleaning up System...\n";
	MEMORY::SafeDelete(mainSystem);

#ifdef _DEBUG
 	system("pause");
#endif

	return result;
}