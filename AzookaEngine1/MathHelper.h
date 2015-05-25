#pragma once
#include<glm/glm.hpp>
using namespace glm;

vec3 GetXYZ(vec4 p_vec4)
{
	return vec3(p_vec4.x, p_vec4.y, p_vec4.z);
}