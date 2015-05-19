#include "Camera.h"


Camera::Camera(vec3 p_position, float p_FoV, float p_aspect, float p_zNear, float p_zFar)
{
	m_perspective = perspective(p_FoV, p_aspect, p_zNear, p_zFar);
	m_position = p_position;

	m_target = vec3(0, 0, 1);
	m_up = vec3(0, 1, 0);
}


Camera::~Camera()
{
}


mat4 Camera::GetProjection()
{
	return m_perspective;
}

mat4 Camera::GetWorldMatrix()
{
	return m_worldMatrix;
}

void Camera::UpdateCamera(vec3 p_position, vec3 p_target, vec3 p_up)
{
	m_worldMatrix = lookAt(p_position, p_position + p_target, p_up);
	
}
