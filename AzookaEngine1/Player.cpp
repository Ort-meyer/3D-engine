#include "Player.h"
#include "Globals.h"
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>


Player::Player(vec3 p_position, vec3 p_target, vec3 p_up)
{
	m_position = p_position;
	m_target = p_target;
	m_up = p_up;
	m_camera = new Camera(p_position, 70, (float)WIDTH/(float)HEIGHT, 0.01, 1000);

	m_turnSpeed = 0.01;
	m_movementSpeed = 1;
	m_anglex = 0;
	m_angley = 0;
}




Player::~Player(void)
{
}

void Player::Update(UserCmd* p_userCmd)
{
	////movement stuff

	vec3 t_right = cross(m_target, m_up);
	vec3 t_prevPos = m_position;
	for (int i = 0; i < p_userCmd->m_keysPressed.size(); i++)
	{
		if(p_userCmd->m_keysPressed.size()>0)
		{
			int retardbreakpoint = 0;
		}

		char t_current = p_userCmd->m_keysPressed.at(i);
		if(t_current == 'w')
			m_position+= m_movementSpeed*m_target;
		if(t_current == 's')
			m_position-= m_movementSpeed*m_target;
		if(t_current == 'a')
			m_position-= m_movementSpeed*t_right;
		if(t_current == 'd')
			m_position+= m_movementSpeed*t_right;
	}


	////rotation stuff
	if(p_userCmd->m_xMove!=0 || p_userCmd->m_yMove!=0)
	{
		float t_dx = -p_userCmd->m_xMove*m_turnSpeed;
		float t_dy = -p_userCmd->m_yMove*m_turnSpeed;

		vec4 t_up4 = vec4(normalize(m_up), 0);
		vec4 t_target4 = vec4(normalize(m_target), 0);
		vec4 t_right4 = vec4(normalize(t_right), 0);

		float t_dot = dot(m_target, m_up);
		cout <<t_dot<<endl;
		cout <<t_dy<<endl;
		if (dot(m_target, m_up)>0.95 && t_dy > 0)
		{
			t_dy = 0;
		}
		else if (dot(m_target, m_up)<-0.95 && t_dy < 0)
		{
			t_dy = 0;
		}
		//up-down rotation of target around right
		mat4 t_hrotation;
		t_hrotation= rotate(t_hrotation, t_dy, t_right);
		t_target4 = t_hrotation * t_target4;
		t_target4 = normalize(t_target4);



		//left-right rotation of target around up
		mat4 t_vrotation;
		t_vrotation = rotate(t_vrotation, t_dx, m_up);
		t_target4 = t_vrotation * t_target4;
		t_target4 = normalize(t_target4);
		m_target = vec3(t_target4.x,t_target4.y,t_target4.z);
		m_up = vec3(t_up4.x,t_up4.y,t_up4.z);



	}
	m_camera->UpdateCamera(m_position, m_target, m_up);
}

mat4 Player::GetWorldMatrix()
{
	return m_camera->GetWorldMatrix();
}

mat4 Player::GetPerspectiveMatrix()
{
	return m_camera->GetProjection();
}

mat4 Player::GetCameraMatrix()
{
	return m_camera->GetProjection()*m_camera->GetWorldMatrix();
}

vec3 Player::GetPosition()
{
	return m_position;
}