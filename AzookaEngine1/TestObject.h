#pragma once
#include "GraphicHandler.h"


using namespace glm;
class TestObject
{
public:
	TestObject();
	TestObject(vec3 p_position);
	~TestObject();


	void Update(float p_right, float p_forward);
	void Draw(GraphicHandler* p_graphicsHandler);
	mat4 GetWorldMatrix();

private:
	float m_movement;
	vec3 m_up;
	vec3 m_position;
	vec3 m_lookDirection;
	vec3 m_moveDirection;
	vec3 m_scale;
	mat4 m_worldMatrix;
};

