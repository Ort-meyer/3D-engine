#pragma once
#include <glm\glm.hpp>
#include <vector>
#include "GraphicsStructs.h"
#include "TerrainNode.h"

class TerrainTree
{
private:
	int maxVerticesPerLeaf;
public:
	TerrainNode terrainTreeRoot;
	TerrainTree();
	TerrainTree(int _maxVerticesPerLeaf);
	~TerrainTree(void);
	void CreateTree(std::vector<std::vector<Vertex>>* currentTerrainBuffer, TerrainNode* terrainNode);
	//vector<vector<Vertex>> 1DTo2DConverter(vector<Vertex> listToTransform, int width);
};

