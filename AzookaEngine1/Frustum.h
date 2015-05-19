#pragma once
#include <glm\glm.hpp>
#include <vector>
#include "GraphicsStructs.h"
#include "Plane.h"
#include "Mesh.h"

using namespace std;
class Frustum
{
public:
	Frustum(void);
	~Frustum(void);
	void Initialize(glm::mat4 perspective);
	void UpdateFrustum(glm::mat4 transformation);
	std::vector<Plane> GetPlanes();
	void Draw();

private:
	glm::vec4 frustumCorners[8];
	glm::vec4 vanillaCorners[8];
	Mesh* m_mesh;
};

