#pragma once
#include <glm/glm.hpp>

using namespace glm;

struct Vertex
{
	vec3 m_position;
	vec2 m_texCoord;
	vec3 m_normal;
	Vertex(vec3 p_position, vec2 p_texCoord, vec3 p_normal)
	{
		m_position = p_position;
		m_texCoord = p_texCoord;
		m_normal = p_normal;
	}
	Vertex(){}
};