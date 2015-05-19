#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace glm;
class Light
{
public:
	Light();
	~Light();
	void Initialize(mat4 p_lightBox, vec3 p_position, vec3 p_direction);
	void InitializeSun(mat4 p_lightBox, vec3 p_position, vec3 p_lookAt, vec3 p_axis);
	void Update();
	void UpdateSun(float p_rotationValue);
	mat4 GetLightCameraMatrix();
	vec3 GetColor();
	vec3 GetDirection();
	mat4 GetWorldMatrix();
private:
	mat4 m_worldMatrix;
	mat4 m_lightBoxMatrix;
	vec3 m_direction;
	vec3 m_color;
	vec3 m_rotateAxis;
	vec3 m_lookAt;
	vec3 m_position;
	float m_strenght;

};

