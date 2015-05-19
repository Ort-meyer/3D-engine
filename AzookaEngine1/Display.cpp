#include "Display.h"
#include <GLEW/glew.h>
#include <iostream>
#include "Globals.h"


Display::Display(int p_width, int p_height, const string& p_windowTitle)
{

	//probably silly to do here. Also, better just get everything
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_ShowCursor(0);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(p_windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_width, p_height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum t_status = glewInit();
	if (t_status != GLEW_OK)
	{
		cout << "Glew failed to initialize";
	}

	m_isClosed = false;
	glEnable(GL_DEPTH_TEST);
	if (DEBUGPOLYGONMODE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	m_windowWidth = p_width;
	m_windowHeight = p_height; 

}


Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::Clear(vec4 p_clearColor)
{
	glClearColor(p_clearColor.r, p_clearColor.g, p_clearColor.b, p_clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Display::Update(UserCmd& o_userCmd)
{
	SDL_GL_SwapWindow(m_window);
	SDL_Event t_event;
	//SDL_PumpEvents();
	m_keyState = SDL_GetKeyboardState(NULL); 
	o_userCmd.m_keysPressed.clear();
	o_userCmd.m_xMove = 0;
	o_userCmd.m_yMove = 0;
	if (m_keyState[SDL_SCANCODE_W])
	{
		o_userCmd.m_keysPressed.push_back('w');
	}
	if (m_keyState[SDL_SCANCODE_S])
	{
		o_userCmd.m_keysPressed.push_back('s');
	}
	if (m_keyState[SDL_SCANCODE_A])
	{
		o_userCmd.m_keysPressed.push_back('a');
	}
	if (m_keyState[SDL_SCANCODE_D])
	{
		o_userCmd.m_keysPressed.push_back('d');
	}
	if (m_keyState[SDL_SCANCODE_LEFT])
	{
		o_userCmd.m_leftArrowPressed = true;
	}
	else
	{
		o_userCmd.m_leftArrowPressed = false;
	}
	if (m_keyState[SDL_SCANCODE_RIGHT])
	{
		o_userCmd.m_rightArrowPressed = true;
	}
	else
	{
		o_userCmd.m_rightArrowPressed = false;
	}
	if (m_keyState[SDL_SCANCODE_UP])
	{
		o_userCmd.m_upArrowPressed = true;
	}
	else
	{
		o_userCmd.m_upArrowPressed = false;
	}
	if (m_keyState[SDL_SCANCODE_DOWN])
	{
		o_userCmd.m_downArrowPressed = true;
	}
	else
	{
		o_userCmd.m_downArrowPressed = false;
	}
	if (m_keyState[SDL_SCANCODE_K])
	{
		o_userCmd.m_kKeyPressed = true;
	}
	else
	{
		o_userCmd.m_kKeyPressed = false;
	}
	if (m_keyState[SDL_SCANCODE_M])
	{
		o_userCmd.m_mKeyPressed = true;
	}
	else
	{
		o_userCmd.m_mKeyPressed = false;
	}

	while (SDL_PollEvent(&t_event))
	{
		if (t_event.type == SDL_QUIT)
			m_isClosed = true;

		if (t_event.type == SDL_QUIT)
		{
			//quiting!
		}
		if (t_event.type == SDL_MOUSEMOTION)
		{
			if(t_event.motion.x != m_windowWidth/2 || t_event.motion.y != m_windowHeight/2)
			{
				o_userCmd.m_xMove = t_event.motion.xrel;
				o_userCmd.m_yMove = t_event.motion.yrel;
				SDL_WarpMouseInWindow(0, m_windowWidth/2, m_windowHeight/2);
				//cout<< "xmove" << o_userCmd.m_xMove <<endl;
				//cout << "ymove" << o_userCmd.m_yMove << endl;
			}
		}
		if (t_event.type == SDL_KEYDOWN)
		{
			if (t_event.key.keysym.sym == 'f')
			{
				o_userCmd.m_frustumControllToggled = !o_userCmd.m_frustumControllToggled;
			}
			if (t_event.key.keysym.sym == 'l')
			{
				o_userCmd.m_lightControllToggle = !o_userCmd.m_lightControllToggle;
			}
		}
	}

}


bool Display::IsClosed()
{
	return m_isClosed;
}