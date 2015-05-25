#include "TestObject.h"
#include "MathHelper.h"


TestObject::TestObject()
{

}

TestObject::TestObject(vec3 p_position)
{
	m_position = p_position;
	m_up = vec3(0, 1, 0);
	m_lookDirection = vec3(0, 0, 1);
	m_moveDirection = vec3(0, 0, 1);
	float scale = 0.1;
	m_scale = vec3(scale, scale, scale);
	m_movement = 0;

}


TestObject::~TestObject()
{
}

void TestObject::Update(float p_right, float p_forward)
{
	p_forward *= -1;
	float moveSpeed = 0.5;
	float rotationSpeed = 1;
	float moveRotationSpeed = 1;



	//rotation around y-axis (changes direction the object moves)
	if (p_right != 0)
	{
		m_moveDirection = GetXYZ(vec4(m_moveDirection, 0)*rotate(p_right*rotationSpeed, vec3(0, 1, 0)));
		m_lookDirection = GetXYZ(vec4(m_lookDirection, 0)*rotate(p_right*rotationSpeed, vec3(0, 1, 0)));
		m_up = GetXYZ(vec4(m_up, 0)*rotate(p_right*rotationSpeed, vec3(0, 1, 0)));
	}

	//movement forward
	if (p_forward != 0)
	{
		m_position += m_moveDirection*moveSpeed*p_forward;

		//rotation when moving forwards (the roll)
		vec3 right = cross(m_lookDirection, m_up);
		m_lookDirection = GetXYZ(vec4(m_lookDirection, 0)*rotate(p_forward*moveRotationSpeed, right));
		m_up = GetXYZ(vec4(m_up, 0)*rotate(p_forward*moveRotationSpeed, right));

	}



	m_worldMatrix = inverse(lookAt(m_position, m_lookDirection + m_position, m_up))*scale(m_scale);


}

void TestObject::Draw(GraphicHandler* p_graphicsHandler)
{
	vector<mat4> matrix;
	matrix.push_back(m_worldMatrix);
	p_graphicsHandler->DrawInstanced(MESH_BTH, SHADER_BASIC_INSTANCED, matrix);
}

