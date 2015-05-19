#pragma once
#include "UserCmd.h"
#include "Camera.h"
#include "vector"
#include <glm\glm.hpp>

using namespace std;
using namespace glm;

class Player
{
public:
	Player(vec3 p_position, vec3 p_target, vec3 p_up);
	~Player(void);

	void Update(UserCmd* p_userCmd);
	mat4 GetWorldMatrix();
	mat4 GetPerspectiveMatrix();
	mat4 GetCameraMatrix();
	vec3 GetPosition();
private:
	Camera* m_camera;

	float m_movementSpeed;
	float m_turnSpeed;

	float m_anglex;
	float m_angley;

	vec3 m_position;
	vec3 m_target;
	vec3 m_up;
};

