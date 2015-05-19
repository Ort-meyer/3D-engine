#include "Transform.h"


Transform::Transform()
{
	m_position = vec3();
	m_rotation = vec3();
	m_scale = vec3(1, 1, 1);
}

Transform::Transform(vec3 p_position, vec3 p_rotation, vec3 p_scale)
{
	m_position = p_position;
	m_rotation = p_rotation;
	m_scale = p_scale;
}

Transform::~Transform()
{
}


vec3 Transform::GetPosition()
{
	return m_position;
}

vec3 Transform::GetRotation()
{
	return m_rotation;
}

vec3 Transform::GetScale()
{
	return m_scale;
}


void Transform::AlterPosition(vec3 p_change)
{
	m_position += p_change;
}
void Transform::AlterRotation(vec3 p_change)
{
	m_rotation += p_change;
}
void Transform::AlterScale(vec3 p_change)
{
	m_scale += p_change;
}

void Transform::SetPosition(vec3 p_position)
{
	m_position = p_position;
}

void Transform::SetRotation(vec3 p_rotation)
{
	m_rotation = p_rotation;
}

void Transform::SetScale(vec3 p_scale)
{
	m_scale = p_scale;
}

mat4 Transform::GetWorldMatrix()
{
	mat4 t_translationMatrix = translate(m_position);
	//really nub way of doing this
	mat4 t_rotationXMatrix = rotate(m_rotation.x, vec3(1, 0, 0));
	mat4 t_rotationYMatrix = rotate(m_rotation.y, vec3(0, 1, 0));
	mat4 t_rotationZMatrix = rotate(m_rotation.z, vec3(0, 0, 1));
	mat4 t_scaleMatrix = scale(m_scale);

	mat4 t_fullRotationMatrix = t_rotationZMatrix * t_rotationYMatrix * t_rotationXMatrix;

	return t_translationMatrix * t_fullRotationMatrix * t_scaleMatrix;


}