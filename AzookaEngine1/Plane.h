#pragma once
#include <string>
#include <glm\glm.hpp>

struct Plane
{
	glm::vec3 normal;
	float distance; // D
	std::string name;
};