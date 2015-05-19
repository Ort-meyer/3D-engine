#include "Light.h"


Light::Light()
{
}


Light::~Light()
{
}

void Light::Initialize(mat4 p_lightBox, vec3 p_position, vec3 p_direction)
{
	m_worldMatrix = lookAt(p_position, p_direction + p_position, vec3(0, 0, 1));
	m_direction = p_direction;
	m_lightBoxMatrix = p_lightBox;
	m_color = vec3(1, 1, 1);
	m_strenght = 1;
}

void Light::InitializeSun(mat4 p_lightBox, vec3 p_position, vec3 p_lookAt, vec3 p_rotationAxis)
{
	m_worldMatrix = lookAt(p_position, p_lookAt, p_rotationAxis);
	m_lightBoxMatrix = p_lightBox;
	m_lookAt = p_lookAt;
	m_rotateAxis = p_rotationAxis;
	m_color = vec3(1);
	m_strenght = 1;
	m_direction = p_lookAt - p_position;
}

void Light::Update()
{

}

void Light::UpdateSun(float p_rotationValue)
{
	mat4 t_rotationMatrix = rotate(mat4(1), p_rotationValue, m_rotateAxis);
	vec4 t_direction4 = (t_rotationMatrix*vec4(m_direction.x, m_direction.y, m_direction.z, 0));
	m_direction.x = t_direction4.x;
	m_direction.y = t_direction4.y;
	m_direction.z = t_direction4.z;
	m_worldMatrix = lookAt(m_lookAt + m_direction, m_lookAt, m_rotateAxis);
}

mat4 Light::GetLightCameraMatrix()
{
	return m_lightBoxMatrix*m_worldMatrix;
}
vec3 Light::GetColor()
{
	return m_color * m_strenght;
}

vec3 Light::GetDirection()
{
	return normalize(m_direction);
}

mat4 Light::GetWorldMatrix()
{
	return inverse(m_worldMatrix);
}