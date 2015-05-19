#pragma once

#include<SDL2/SDL.h>
#include <glm/glm.hpp>
#include<string>
#include <iostream>
#include "UserCmd.h"

using namespace std;
using namespace glm;

class Display
{
public:
	Display(int p_width, int p_height, const string& p_windowTitle);
	~Display();

	void Clear(vec4 p_clearColor);
	void Update(UserCmd& o_userCmd);
	bool IsClosed();

private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	const Uint8* m_keyState;
	int m_windowWidth;
	int m_windowHeight; 

	bool m_isClosed;
};

