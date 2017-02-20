#pragma once

#include "CUIControl.h"

class CUISlider : public CUIControl
{
public:
	CUISlider();
	CUISlider(CUIControl* parent);
	CUISlider(CUIMenu* menu);

	void SetSliderPosition(float p);
	void SetMaxValue(float v);
	void SetValue(float v);

	float GetSliderPosition();
	float GetSliderValue();
	float GetMaxValue();

	void OnInit(RECT r, UINT id, char* active = NULL, char* inactive = NULL);
	void OnRender();
	void OnUpdate();
	void OnPressed();

	bool IsActive() { return m_bMoveSlider; }
private:
	CSprite m_bar;
	CSprite m_slider;

	int slider_x, slider_y;
	bool m_bMoveSlider;

	float m_max_value;
	float m_slider_position;
};