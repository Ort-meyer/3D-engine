#pragma once
#include <glm/glm.hpp>
#include<glm/gtx/transform.hpp>
using namespace glm;
class Transform
{
public:
	Transform();
	Transform(vec3 p_position, vec3 p_rotation, vec3 p_scale);
	~Transform();


	vec3 GetPosition();
	vec3 GetRotation();
	vec3 GetScale();

	void AlterPosition(vec3 p_change);
	void AlterRotation(vec3 p_change);
	void AlterScale(vec3 p_change);

	void SetPosition(vec3 p_position);
	void SetRotation(vec3 p_rotation);
	void SetScale(vec3 p_scale);

	mat4 GetWorldMatrix();

private:
	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;
};

