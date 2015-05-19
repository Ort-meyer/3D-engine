#include "FrustumCuller.h"


FrustumCuller::FrustumCuller(void)
{
}


FrustumCuller::~FrustumCuller(void)
{
}


vector<TerrainNode*> FrustumCuller::NodesToBeDrawn(std::vector<Plane>* frustumPlanes, TerrainNode* currentNode)
{
	std::vector<TerrainNode*> returnNodes;

	if (BoxInsideFrustum(frustumPlanes, currentNode))
	{
		if (currentNode->isLeaf)
		{

			returnNodes.push_back(currentNode);
			return returnNodes;
		}
		else
		{	
			for (int i = 0; i < 4; i++)
			{
				std::vector<TerrainNode*> childrenToBeDrawn = NodesToBeDrawn(frustumPlanes, currentNode->children[i]);
				returnNodes.insert(returnNodes.end(),childrenToBeDrawn.begin(), childrenToBeDrawn.end()); 
			}
			return returnNodes;
		}  
	}
	return returnNodes;
}

vector<TerrainNode*> FrustumCuller::DebugNodesToBeDrawn(std::vector<Plane>* frustumPlanes, TerrainNode* currentNode)
{
	std::vector<TerrainNode*> returnNodes;

	if (BoxInsideFrustum(frustumPlanes, currentNode))
	{
		returnNodes.push_back(currentNode);
		if (currentNode->isLeaf)
		{
			return returnNodes;
		}
		else
		{	
			for (int i = 0; i < 4; i++)
			{
				std::vector<TerrainNode*> childrenToBeDrawn = DebugNodesToBeDrawn(frustumPlanes, currentNode->children[i]);
				returnNodes.insert(returnNodes.end(),childrenToBeDrawn.begin(), childrenToBeDrawn.end()); 
			}
			return returnNodes;
		}  
	}
	return returnNodes;
}
bool FrustumCuller::BoxInsideFrustum(std::vector<Plane>* frustumPlanes, TerrainNode* currentNode)
{

	////////////fancy book method below
	//glm::vec3 boxCenter = (currentNode->boxCorners[1]+currentNode->boxCorners[0])/2.0f;
	//glm::vec3 radius = (currentNode->boxCorners[0]-currentNode->boxCorners[1])/2.0f;

	//for (int i = 0; i < 6; i++)
	//{
	//	float e = radius.x*glm::abs(frustumPlanes->at(i).normal.x) + radius.y*glm::abs(frustumPlanes->at(i).normal.y) + radius.z*glm::abs(frustumPlanes->at(i).normal.z);
	//	float s = glm::dot(boxCenter,frustumPlanes->at(i).normal) + frustumPlanes->at(i).distance;

	//	if (s - e > 0)
	//	{
	//		return false;
	//	}
	//}


	//////////working Myzooka method below
	for (int i = 0; i < 6; i++)
	{
		int numberOfPointsOutside = 0;
		for (int j = 0; j < 8; j++)
		{
			if (PointVsPlaneCalculation(&frustumPlanes->at(i), currentNode->boxCorners[j])<0)
			{
				numberOfPointsOutside++;
			}
		}
		if (numberOfPointsOutside >= 7)
		{
			return false;
		}
	}

	return true;
}

bool FrustumCuller::PointInsideFrustum(std::vector<Plane>* frustumPlanes, glm::vec3 point)
{

	if (PointVsPlaneCalculation(&frustumPlanes->at(0), point) >= 0 && PointVsPlaneCalculation(&frustumPlanes->at(1), point) >= 0
		&& PointVsPlaneCalculation(&frustumPlanes->at(2), point) >= 0 && PointVsPlaneCalculation(&frustumPlanes->at(3), point) >= 0
		&& PointVsPlaneCalculation(&frustumPlanes->at(4), point) >= 0 && PointVsPlaneCalculation(&frustumPlanes->at(5), point) >= 0)
	{
		return true;
	}
	return false;
}


float FrustumCuller::PointVsPlaneCalculation(Plane* plane, glm::vec3 point)
{
	plane->normal = glm::normalize(plane->normal);
	return point.x*plane->normal.x + point.y*plane->normal.y + point.z*plane->normal.z + plane->distance;
}
