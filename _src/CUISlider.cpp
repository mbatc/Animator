#include "CUISlider.h"
#include "CSystem.h"
#include "CVector2.h"

CUISlider::CUISlider()
	:
	CUIControl()
{}

CUISlider::CUISlider(CUIMenu* menu)
	:
	CUIControl(menu)
{}

CUISlider::CUISlider(CUIControl* control)
	:
	CUIControl(control)
{}

void CUISlider::SetSliderPosition(float p)
{
	m_slider_position = p;
}

void CUISlider::SetMaxValue(float v)
{
	m_max_value = v;
}

void CUISlider::SetValue(float v)
{
	m_slider_position = v / m_max_value;
}

float CUISlider::GetSliderPosition()
{
	return m_slider_position;
}

float CUISlider::GetSliderValue()
{
	return m_slider_position * m_max_value;
}

float CUISlider::GetMaxValue()
{
	return m_max_value;
}

void CUISlider::OnInit(RECT r, UINT id, char* active, char* inactive)
{
	CUIControl::OnInit(r, id, active, inactive);
	m_slider_position = 0.0f;
	m_max_value = 1.0f;
	slider_x = 0;
	slider_y = 0;
	m_bMoveSlider = false;

	m_bar = CSystem::m_gfx.DEFAULTSPRITE;
	m_slider = CSystem::m_gfx.DEFAULTSPRITE;

	m_slider.Resize(15, (GetRect().bottom - GetRect().top)/2);
	m_bar.Resize(GetRect().right - GetRect().left, 2);

	m_slider.SetColor(0.8f, 0.8f, 0.8f, 1.0f);
	m_bar.SetColor(0.1f, 0.1f, 0.1f, 1.0f);

	GetSprite(true).SetColor(0.0f, 0.0f, 0.0f, 0.0f);
	GetSprite(false).SetColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void CUISlider::OnRender()
{
	m_slider.SetPosition(slider_x - m_slider.GetWidth()/2, slider_y);
	m_bar.SetPosition(GetGlobalX(), GetGlobalY() + GetHeight()/2 - m_bar.GetHeight()/2);

	SYSTEM::RenderSprite(m_bar, 8);
	SYSTEM::RenderSprite(m_slider, 8);
}

void CUISlider::OnUpdate()
{
	if (m_bMoveSlider)
	{
		if (MOUSE::IsLeftMouseDown())
		{
			int mx = MOUSE::GetMouseX();
			slider_x = mx;

			if (slider_x < GetGlobalX())
				slider_x = GetGlobalX();
			if (slider_x > GetGlobalX() + GetWidth())
				slider_x = GetGlobalX() + GetWidth();

			m_slider_position = (float)(slider_x - GetGlobalX()) / (float)GetWidth();
		}
		else
			m_bMoveSlider = false;
	}

	if (m_slider_position > 1.0f) m_slider_position = 1.0f;
	if (m_slider_position < 0.0f) m_slider_position = 0.0f;

	slider_x = GetGlobalX() + m_slider_position * GetWidth();
	slider_y = GetGlobalY() + GetHeight()/4;
}

void CUISlider::OnPressed()
{
	int mx = MOUSE::GetMouseX();
	int my = MOUSE::GetMouseY();

	RECT s;
	s.top = slider_y;
	s.bottom = slider_y + m_slider.GetHeight();
	s.left = slider_x - m_slider.GetWidth();
	s.right = slider_x + m_slider.GetWidth();

	if (mx > s.left && mx < s.right && my > s.top && my < s.bottom)
	{
		m_bMoveSlider = true;
	}
}