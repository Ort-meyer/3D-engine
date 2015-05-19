#pragma once
#include <vector>

using namespace std;

static struct UserCmd
{
	vector<char> m_keysPressed;
	float m_xMove;
	float m_yMove;
	bool m_kKeyPressed;
	bool m_mKeyPressed;
	bool m_frustumControllToggled;
	bool m_lightControllToggle;
	bool m_rightArrowPressed;
	bool m_leftArrowPressed;
	bool m_upArrowPressed;
	bool m_downArrowPressed;
};