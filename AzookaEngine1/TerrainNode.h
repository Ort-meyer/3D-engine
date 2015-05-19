#pragma once

#include <vector>
#include "Mesh.h"
#include <glm\vec3.hpp>

struct TerrainNode
{
	bool isLeaf;
	TerrainNode* children[4];
	//std::vector<TerrainVertex> vertices;
	std::vector<int> index;
	glm::vec3 boxCorners[8];
	//GLuint VBO;
	//GLuint indexBufferHandle;	
	//GLuint debugVertexBufferHandle;
	//GLuint debugIndexBufferHandle;
	int indexCount;
	int totalVertexCount;
	Mesh* mesh;
	Mesh* debugMesh;
	TerrainNode()
	{	
	}
};