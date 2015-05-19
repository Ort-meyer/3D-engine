#pragma once

#include<glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

class Camera
{
public:
	Camera(vec3 p_position, float p_FoV, float p_aspect, float p_zNear, float p_zFar);
	~Camera();

	mat4 GetProjection();
	mat4 GetWorldMatrix();

	void UpdateCamera(vec3 p_position, vec3 p_target, vec3 p_up);

private:
	mat4 m_perspective;
	mat4 m_worldMatrix;

	vec3 m_position;
	vec3 m_target;
	vec3 m_up;
};

