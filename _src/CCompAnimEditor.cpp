#include "CCompAnimEditor.h"
#include "CSystem.h"
#include "SystemMessage.h"
#include "CAnimation.h"

#include "Helper.h"

CCompAnimEditor::CCompAnimEditor()
	:
	CComponent(CANIMEDITOR),
	m_pFilename(NULL)
{}

void CCompAnimEditor::InitUI()
{
	m_pivot.Init("Engine\\Resources\\Controls\\pivot.png",CSystem::m_gfx);
	m_pivot_a.Init("Engine\\Resources\\Controls\\pivot_a.png", CSystem::m_gfx);
	m_pivot.Resize(30, 30);
	m_pivot_a.Resize(30, 30);

	RECT r;
	r.left = 0;
	r.right = SYSTEM::GetWindowWidth()*0.2f;
	r.top = 0;
	r.bottom = SYSTEM::GetWindowHeight()*0.8f;

	m_pPivotMenu = CSystem::g_uiManager.AddMenu(CSystem::m_gfx, UI_PIVOT_MENU, r);

	r.left += r.right * 0.1f;
	r.right = r.right * 0.9f;
	r.top = r.bottom * 0.05f;
	r.bottom = r.bottom * 0.95f;
	m_pPivotList = (CUIList*)m_pPivotMenu->AddControl(UI_PIVOT_LIST, r, new CUIList());
	m_pPivotList->SetItemHeight(25);
	m_pPivotList->RenderBorder(true);
	for (int i = 0; i < NPIVOTS; i++)
	{
		if (i == CANIM::ERR || i == CANIM::SHOULDER)
			continue;
		CUIListItem* l = new CUIListItem;
		char* s = 0;
		STRING::Copy(s, GetAnimString((CANIM)i));

		l->SetItemName(s);
		l->SetData(NULL);

		m_pPivotList->AddListItem(l);
	}
	r.left = 0; r.right = SYSTEM::GetWindowWidth() * 0.2f; r.top = SYSTEM::GetWindowHeight() * 0.8f; r.bottom = SYSTEM::GetWindowHeight();

	int width = r.right; int height = r.bottom - r.top;
	m_pAnimProp = CSystem::g_uiManager.AddMenu(CSystem::m_gfx, UI_PROP_MENU,r);
	
	r.top = height * 0.05f; r.bottom = r.top + CSystem::g_textRenderer.GetActiveFont().GetFontHeight() + 4;
	r.left = width * 0.1f; r.right = r.right * 0.9f;
	CUIControl* c = m_pAnimProp->AddControl(UI_PROP_NAME, r, new CUITextIO());
	c->Clip(true);
	c->SetText("New Animation");
	
	r.top = r.bottom + 4; r.bottom += height * 0.3f; r.right = width * 0.7f;
	CUISlider* s= (CUISlider*)m_pAnimProp->AddControl(UI_PROP_TIME, r, new CUISlider());
	s->SetMaxValue(3.0f);
	s->SetSliderPosition(0.5f);

	r.left = r.right + 4; r.right = width*0.9f;
	c = m_pAnimProp->AddControl(UI_PROP_CURTIME, r, new CUITextIO());
	c->RenderBorder(true);
	c->Clip(true);

	r.top = r.bottom + 4; r.bottom += height*0.3f; r.left = width*0.1f; r.right = width* 0.7f;
	c = m_pAnimProp->AddControl(UI_PROP_PLAY, r, new CUIControl);
	c->SetText("PLAY");
	c->Clip(true);

	r.left = r.right + 4; r.right = width*0.9f;
	c = m_pAnimProp->AddControl(UI_PROP_LENGTH, r, new CUITextIO());
	c->RenderBorder(true);
	c->Clip(true);

	r.top = 0; r.bottom = 32; r.left = SYSTEM::GetWindowWidth() / 2 - 32; r.right = r.left + 32 * 2;
	m_pSaveMenu = CSystem::g_uiManager.AddMenu(CSystem::m_gfx, UI_SAVE_MENU, r);
	r.left = 0; r.right = 32;
	m_pSaveMenu->AddControl(UI_SAVE_BT, r, new CUIControl, "Engine\\Resources\\Controls\\save_a.png", "Engine\\Resources\\Controls\\save.png");
	r.left = 32; r.right = 64;
	m_pSaveMenu->AddControl(UI_LOAD_BT, r, new CUIControl, "Engine\\Resources\\Controls\\load_a.png", "Engine\\Resources\\Controls\\load.png");

	r.right = SYSTEM::GetWindowWidth();
	r.left = r.right *0.8f;
	r.top = 0.0f;
	r.bottom = SYSTEM::GetWindowHeight();
	m_pPivotProp = CSystem::g_uiManager.AddMenu(CSystem::m_gfx, UI_PIVPROP_MENU, r);

	width = r.right - r.left;
	height = r.bottom - r.top;

	r.bottom = r.bottom * 0.95f;
	r.top = r.bottom - CSystem::g_textRenderer.GetActiveFont().GetFontHeight() - 4;
	r.left = SYSTEM::GetWindowWidth() * 0.02f;
	r.right = SYSTEM::GetWindowWidth() * 0.18f;
	c = m_pPivotProp->AddControl(UI_DELETE_FRAME_BT, r, new CUIControl);
	c->SetText("DELETE KEYFRAME");

	r.bottom -= CSystem::g_textRenderer.GetActiveFont().GetFontHeight() + 8;
	r.top -= CSystem::g_textRenderer.GetActiveFont().GetFontHeight() + 8;
	c = m_pPivotProp->AddControl(UI_DUP_FRAME_BT, r, new CUIControl);
	c->SetText("DUPLICATE");


	r.bottom -= CSystem::g_textRenderer.GetActiveFont().GetFontHeight() + 8;
	r.top -= CSystem::g_textRenderer.GetActiveFont().GetFontHeight() + 8;
	c = m_pPivotProp->AddControl(UI_ADDKEYFRAME_BT, r, new CUIControl);
	c->SetText("ADD KEYFRAME");

	r.bottom -= CSystem::g_textRenderer.GetActiveFont().GetFontHeight() + 8;
	r.top -= CSystem::g_textRenderer.GetActiveFont().GetFontHeight() + 8;
	r.right = width*0.5f - 4;
	c = m_pPivotProp->AddControl(UI_KEYFRAME_TIME, r, new CUITextIO);
	c->SetText("");

	r.left = r.right + 4; r.right = SYSTEM::GetWindowWidth() * 0.18f;
	c = m_pPivotProp->AddControl(UI_KEYFRAME_ROT, r, new CUITextIO);
	c->SetText("");

	r.top = SYSTEM::GetWindowHeight() * 0.05f; r.bottom = SYSTEM::GetWindowHeight()*0.75f;
	r.left = SYSTEM::GetWindowWidth() * 0.02f;
	r.right = SYSTEM::GetWindowWidth() * 0.18f;
	m_pFramelist = (CUIList*)m_pPivotProp->AddControl(UI_KEYFRAME_LIST, r, new CUIList());
	m_pFramelist->SetItemHeight(25);
}

void CCompAnimEditor::Update(float deltaTime)
{
	bool bFollow = false;
	if (MOUSE::IsRightMousePressed())
	{
		for (int i = 0; i < NPIVOTS; i++)
		{
			if (i == CANIM::ERR || i == CANIM::SHOULDER)
				continue;

			CAnimPivot* p = m_pActor->GetPivot((CANIM)i);
			int mx = MOUSE::GetMouseX();
			int my = MOUSE::GetMouseY();
			int r = 15;

			if (mx > p->m_transformedPoint.x - r && mx < p->m_transformedPoint.x + r &&
				my > p->m_transformedPoint.y - r && my < p->m_transformedPoint.y + r)
			{
				m_pPivotList->SetActiveSelection(m_pPivotList->GetListItem(
					GetAnimString((CANIM)i)));
				break;
			}
		}
	}
	
	CUISlider* s = (CUISlider*)m_pAnimProp->GetControl(UI_PROP_TIME);
	CUITextIO* t = (CUITextIO*)m_pAnimProp->GetControl(UI_PROP_CURTIME);

	if (!m_bPreview)
	{
		if (t->IsActive())
		{
			if (t->GetText())
				s->SetValue(atof(t->GetText()));
			m_pController->UpdatePivot(true);
		}
		else
		{
			char temp[256] = { 0 };
			sprintf(temp, "%f", s->GetSliderValue());
			t->SetText(temp);
		}

		t = (CUITextIO*)m_pAnimProp->GetControl(UI_PROP_LENGTH);
		if (t->IsActive())
		{
			if (t->GetText())
				s->SetMaxValue(atof(t->GetText()));
		}
		else
		{
			char temp[256] = { 0 };
			sprintf(temp, "%f", s->GetMaxValue());
			t->SetText(temp);
		}
	}
	else
	{
		s->SetValue(m_pAnimation->GetTime());

		char temp[256] = { 0 };
		sprintf(temp, "%f", s->GetSliderValue());
		t->SetText(temp);
	}
	if (m_pFramelist->GetActiveSelection())
	{
		CANIM a = GetAnimPart(m_pPivotList->GetActiveSelection()->GetItemName());
		int index = m_pFramelist->GetActiveIndex();
		SKEYFRAME* kf = m_pAnimation->GetKeyframe(a, index);
		CUITextIO* t = (CUITextIO*)m_pPivotProp->GetControl(UI_KEYFRAME_ROT);

		if (t->IsActive())
		{
			if (kf)
				if (t->GetText())
					kf->m_rotation = atof(t->GetText()) * (M_PI / 180.0f);
				else
					kf->m_rotation = 0.0f;
		}
		else
		{
			char msg[256] = { 0 };
			if(kf)
				sprintf(msg, "%f", kf->m_rotation * (180.0f/M_PI));
			t->SetText(msg);
		}

		t = (CUITextIO*)m_pPivotProp->GetControl(UI_KEYFRAME_TIME);
		if (t->IsActive())
		{
			if (kf)
				if (t->GetText())
					kf->m_time = atof(t->GetText());
				else
					kf->m_time = 0.0f;
		}
		else
		{
			char msg[256] = { 0 };
			if(kf)
				sprintf(msg, "%f", kf->m_time);
			t->SetText(msg);
		}
	}
	else
	{
		if (m_pPivotList->GetActiveSelection())
		{
			CANIM a = GetAnimPart(m_pPivotList->GetActiveSelection()->GetItemName());
			float rot = m_pActor->GetPivot(a)->m_rotation;
			float time = m_pAnimation->GetTime();

			CUITextIO* t = (CUITextIO*)m_pPivotProp->GetControl(UI_KEYFRAME_ROT);
			if(!t->IsActive())
			{
				char msg[256] = { 0 };
				sprintf(msg, "%f", rot * (180.0f/M_PI));
				t->SetText(msg);
			}
			t = (CUITextIO*)m_pPivotProp->GetControl(UI_KEYFRAME_TIME);
			if (!t->IsActive())
			{
				char msg[256] = { 0 };
				sprintf(msg, "%f", time);
				t->SetText(msg);
			}
		}
	}


	if (m_pPivotList->GetActiveSelection())
	{
		m_pController->UpdatePivot(s->IsActive());

		CANIM a = GetAnimPart(m_pPivotList->GetActiveSelection()->GetItemName());
		CAnimPivot* p = m_pActor->GetPivot(a);

		if (MOUSE::IsLeftMousePressed())
		{
			int mx = MOUSE::GetMouseX();
			int my = MOUSE::GetMouseY();
			int r = 15;

			if (mx > p->m_transformedPoint.x - r && mx < p->m_transformedPoint.x + r &&
				my > p->m_transformedPoint.y - r && my < p->m_transformedPoint.y + r)
			{
				m_bRotate = true;
			}
		}
		else if (MOUSE::IsLeftMouseReleased())
		{
			m_bRotate = false;
		}
		else if (MOUSE::IsLeftMouseDown() && m_bRotate)
		{
			CVector2 center = p->m_parent->m_transformedPoint;
			CVector2 b(MOUSE::GetMouseX() - center.x, MOUSE::GetMouseY() - center.y);
			CVector2 c(MOUSE::GetMousePrevX() - center.x, MOUSE::GetMousePrevY() - center.y);
			CVector2 a(1.0f, 0.0f);
			if (b.Magnitude())
			{
				float r = acosf((a.y * b.y + a.x * b.x) / (a.Magnitude() * b.Magnitude()));
				float pr = acosf((a.x * c.x + a.y * c.y) / (a.Magnitude() * c.Magnitude()));
				if (b.y < 0)
					r = -r;
				if (c.y < 0)
					pr = -pr;

				float delta = r - pr;
				if (delta > M_PI)
				{
					do
					{
						delta -= 2 * M_PI;
					} while (delta > M_PI);
				}
				if (delta < -M_PI)
				{
					do
					{
						delta += 2 * M_PI;
					} while (delta < -M_PI);
				}
				p->m_rotation += delta;
			}
		}
	}
	else
		m_pController->UpdatePivot(true);

	if (!m_bPreview)
	{
		m_pAnimation->SetLength(s->GetMaxValue());
		m_pAnimation->SetTime(s->GetSliderValue());
		m_pController->SetSpeed(0.0f);
		m_pAnimProp->GetControl(UI_PROP_PLAY)->SetText("PLAY");
	}
	else
	{
		m_pController->UpdatePivot(true);
		m_pController->SetSpeed(1.0f);
		m_pAnimProp->GetControl(UI_PROP_PLAY)->SetText("PAUSE");

		if (m_pAnimation->GetTime() >= m_pAnimation->GetLength())
		{
			m_pAnimation->SetTime(0.0f);
			m_bPreview = false;
		}
	}
}

void CCompAnimEditor::Render(CD3DGraphics & gfx)
{
	CSprite* spr;

	for(int i = 0; i < NPIVOTS; i++)
	{
		spr = &m_pivot;
		if (m_pPivotList->GetActiveSelection())
		{
			if ((CANIM)i == GetAnimPart(m_pPivotList->GetActiveSelection()->GetItemName()))
			{
				spr = &m_pivot_a;
			}
		}
		spr->SetPosition(m_pActor->GetPivot((CANIM)i)->m_transformedPoint.x - 15,
			m_pActor->GetPivot((CANIM)i)->m_transformedPoint.y - 15);

		SYSTEM::RenderSprite(*spr, 8);
	}
}

void CCompAnimEditor::SetActor(CCompActorAnim * pActor)
{
	m_pActor = pActor;
}

void CCompAnimEditor::SetController(CCompAnimController * pController)
{
	m_pController = pController;
	m_pAnimation = pController->GetAnimation();
}

void CCompAnimEditor::Load()
{
	char* temp = OpenLoadDialog();

	if (!temp)
		return;

	MEMORY::SafeDeleteArr(m_pFilename);
	STRING::Copy(m_pFilename, temp);

	m_pAnimation->LoadAnimation(m_pFilename);

	char* ext = FILEH::GetFileExtension(m_pFilename);
	*ext = '\0';
	m_pAnimProp->GetControl(UI_PROP_NAME)->SetText(&(STRING::ScanFromEnd(m_pFilename, "\\")[1]));
	*ext = '.';
}

void CCompAnimEditor::Save()
{
	char* filename = 0;
	char* temp = m_pAnimProp->GetControl(UI_PROP_NAME)->GetText();
	if (temp)
	{
		STRING::Copy(filename, m_pAnimProp->GetControl(UI_PROP_NAME)->GetText());
		STRING::Append(filename, filename, ".anim");
	}

	if (!m_pFilename)
	{
		m_pFilename = OpenSaveDialog();
		if (!m_pFilename)
			return;
	}

	temp = STRING::ScanFromEnd(m_pFilename, "\\");
	
	if (strcmp(&temp[1], filename))
	{
		MEMORY::SafeDeleteArr(m_pFilename);
		m_pFilename = OpenSaveDialog();
		if (!m_pFilename)
			return;
	}

	FILE* pFile = fopen(m_pFilename, "w");

	if (!pFile)
		cout << "CCompAnimEditor: Failed to open animation save file: " << m_pFilename << endl;
	fprintf(pFile, " t %f", m_pAnimation->GetLength());
	for (int i = 0; i < m_pAnimation->GetTotalKeyframeCount(); i++)
	{
		SKEYFRAME f = *m_pAnimation->GetKeyframe(i);
		fprintf(pFile, " kf %s %f %f", GetAnimString(f.m_body), f.m_rotation, f.m_time);
	}

	fclose(pFile);

	char* ext = FILEH::GetFileExtension(m_pFilename);
	*ext = '\0';
	m_pAnimProp->GetControl(UI_PROP_NAME)->SetText(&(STRING::ScanFromEnd(m_pFilename, "\\")[1]));
	*ext = '.';
}

void CCompAnimEditor::Play()
{
	m_bPreview = !m_bPreview;

	if (m_bPreview)
		m_pAnimation->SetTime(0.0f);
}

void CCompAnimEditor::OnPivotChange()
{
	m_pFramelist->Clear();
	if (!m_pPivotList->GetActiveSelection())
		return;

	CANIM a = GetAnimPart(m_pPivotList->GetActiveSelection()->GetItemName());

	int nFrames = m_pAnimation->GetNKeyframe(a);
	SKEYFRAME* pFrames = new SKEYFRAME[nFrames];
	int*		pIndex = new int[nFrames];
	for (int i = 0, ii = 0; i < m_pAnimation->GetTotalKeyframeCount(); i++)
	{
		SKEYFRAME p = *m_pAnimation->GetKeyframe(i);

		if (p.m_body != a)
			continue;
		pFrames[ii] = p;
		pIndex[ii] = i;
		ii++;
	}

	for (int i = 0; i < nFrames; i++)
	{
		int newID = i;
		SKEYFRAME temp = pFrames[i];
		int index = pIndex[i];
		for (int ii = 0; ii < nFrames; ii++)
		{
			if (temp.m_time > pFrames[ii].m_time && newID < ii)
			{
				pFrames[newID] = pFrames[ii];
				pFrames[ii] = temp;
				pIndex[newID] = pIndex[ii];
				pIndex[ii] = index;
				newID = ii;
			}
		}
	}

	for (int i = 0; i < nFrames; i++)
	{
		CUIListItem* li = new CUIListItem;
		int* data = new int;
		*data = pIndex[i];
		char msg[256] = { 0 };
		sprintf(msg, "Frame t=%.2f", pFrames[i].m_time);
		li->SetItemName(msg);
		li->SetData((void*)data);

		m_pFramelist->AddListItem(li);
	}

	MEMORY::SafeDeleteArr(pFrames);
	MEMORY::SafeDeleteArr(pIndex);
}

void CCompAnimEditor::OnKeyframeChange()
{
	float t = 0.0f;
	float r = 0.0f;
	if (!m_pPivotList->GetActiveSelection())
		return;
	if (!m_pFramelist->GetActiveSelection())
		return;

	CANIM a = GetAnimPart(m_pPivotList->GetActiveSelection()->GetItemName());
	CUISlider* s = (CUISlider*)m_pAnimProp->GetControl(UI_PROP_TIME);

	CUITextIO* rot = (CUITextIO*)m_pPivotProp->GetControl(UI_KEYFRAME_ROT);
	CUITextIO* time = (CUITextIO*)m_pPivotProp->GetControl(UI_KEYFRAME_TIME);

	int i = *(int*)m_pFramelist->GetActiveSelection()->GetData();
	t = m_pAnimation->GetKeyframe(i)->m_time;
	r = m_pAnimation->GetKeyframe(i)->m_rotation;
	
	s->SetValue(t);
	m_pController->UpdatePivot(true);
	m_pAnimation->SetTime(t);
	m_pController->Update(0.0f);
	m_pController->UpdatePivot(false);

	char text[256] = { 0 };
	sprintf(text, "%f", r * (180.0f/M_PI));
	rot->SetText(text);

	sprintf(text, "%f", t);
	time->SetText(text);
}

void CCompAnimEditor::AddKeyframe()
{
	CUISlider* s = (CUISlider*)m_pAnimProp->GetControl(UI_PROP_TIME);

	for (int i = 0; i < NPIVOTS; i++)
	{
		if (m_pController->GetAnimRotation((CANIM)i, s->GetSliderValue()) != 
			m_pActor->GetPivot((CANIM)i)->m_rotation || 

			m_pAnimation->GetTime() != s->GetSliderValue())
		{
			SKEYFRAME kf;
			kf.m_rotation = m_pActor->GetPivot((CANIM)i)->m_rotation;
			kf.m_time = s->GetSliderValue();
			kf.m_body = (CANIM)i;

			m_pAnimation->AddKeyframe(kf);
		}
	}

	OnPivotChange();
}

void CCompAnimEditor::DeleteKeyframe()
{
	if (!m_pPivotList->GetActiveSelection())
		return;
	if (!m_pFramelist->GetActiveSelection())
		return;

	CANIM a = GetAnimPart(m_pPivotList->GetActiveSelection()->GetItemName());
	int index = m_pFramelist->GetActiveIndex();

	int kf_index = m_pAnimation->GetKeyframeIndex(m_pAnimation->GetKeyframe(a, index));

	m_pAnimation->DeleteKeyframe(kf_index);
	OnPivotChange();
}

void CCompAnimEditor::DuplicateKeyframe()
{
	if (!m_pFramelist->GetActiveSelection())
		return;
	SKEYFRAME kf = *m_pAnimation->GetKeyframe(*(int*)m_pFramelist->GetActiveSelection()->GetData());
	m_pAnimation->AddKeyframe(kf);

	OnPivotChange();
}

char * CCompAnimEditor::OpenSaveDialog()
{
	OPENFILENAME ofn;
	char* buffer = new char[260];
	buffer[0] = '\0';
	char*  initial_file = 0;
	if (m_pAnimProp->GetControl(UI_PROP_NAME)->GetText())
	{
		strcpy(buffer, m_pAnimProp->GetControl(UI_PROP_NAME)->GetText());
		int i = strlen(buffer);
		buffer[i] = '.';
		buffer[i + 1] = 'a';
		buffer[i + 2] = 'n';
		buffer[i + 3] = 'i';
		buffer[i + 4] = 'm';
		buffer[i + 5] = '\0';
	}

	HWND hWnd;
	HANDLE hf;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = SYSTEM::GetMainWindowHandle();
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = sizeof(char) * 260;
	ofn.lpstrFilter = "2D Animation\0*.anim\0All\0*.*\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = "Game Data\\Actors\\Animations\\";
	ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		if (!FILEH::GetFileExtension(buffer))
		{
			STRING::Append(buffer, buffer, ".anim");
		}
		else if(strcmp(FILEH::GetFileExtension(buffer),".anim"))
		{
			STRING::Append(buffer, buffer, ".anim");
		}
		return buffer;
	}

	cout << "CCompAnimEditor: Error saving animation - " << CommDlgExtendedError() << endl;

	return nullptr;
}

char * CCompAnimEditor::OpenLoadDialog()
{
	OPENFILENAME ofn;
	char* buffer = new char[260];
	buffer[0] = '\0';
	HWND hWnd;
	HANDLE hf;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = SYSTEM::GetMainWindowHandle();
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = sizeof(char) * 260;
	ofn.lpstrFilter = "2D Animation\0*.anim\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = "Game Data\\Actors\\Animations\\";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
		return buffer;
	return nullptr;
}

void CCompAnimEditor::UIMANAGER_MSG_PROC(UINT msg, UINT subMsg)
{
	switch (msg)
	{
	case UI_PROP_PLAY:
		if(subMsg == _SM_SUB_BT_PRESSED)
			Play();
		break;
	case UI_ADDKEYFRAME_BT:
		if (subMsg == _SM_SUB_BT_PRESSED)
			AddKeyframe();
		break;
	case UI_DELETE_FRAME_BT:
		if (subMsg == _SM_SUB_BT_PRESSED)
			DeleteKeyframe();
		break;
	case UI_SAVE_BT:
		if (subMsg == _SM_SUB_BT_PRESSED)
			Save();
		break;
	case UI_LOAD_BT:
		if (subMsg == _SM_SUB_BT_PRESSED)
			Load();
		break;
	case UI_PIVOT_LIST:
		if (subMsg == _SM_UI_SELECTION_CHANGE)
			OnPivotChange();
	case UI_KEYFRAME_LIST:
		if (subMsg == _SM_UI_SELECTION_CHANGE)
			OnKeyframeChange();
		break;
	}
}