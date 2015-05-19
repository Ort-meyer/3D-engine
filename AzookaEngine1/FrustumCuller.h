#pragma once

#include <vector>
#include <glm\glm.hpp>
#include "TerrainNode.h"
#include "Plane.h"

using namespace std;
using namespace glm;

class FrustumCuller
{
public:
	FrustumCuller(void);
	~FrustumCuller(void);

	vector<TerrainNode*> NodesToBeDrawn(std::vector<Plane>* frustumPlanes, TerrainNode* currentNode);
	vector<TerrainNode*> DebugNodesToBeDrawn(std::vector<Plane>* frustumPlanes, TerrainNode* currentNode);
private:
	bool BoxInsideFrustum(std::vector<Plane>* frustumPlanes, TerrainNode* currentNode);
	float PointVsPlaneCalculation(Plane* plane, glm::vec3 point);
	bool PointInsideFrustum(std::vector<Plane>* frustumPlanes, glm::vec3 point);
};

