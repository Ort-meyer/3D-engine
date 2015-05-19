#pragma once
#include <vector>
#include "TerrainTree.h"
#include "FrustumCuller.h"
#include "Frustum.h"

class Terrain
{

public:
	Terrain(void);
	~Terrain(void);

	void Initialize(mat4 p_frustumMatrix);
	void CreateTerrain();
	TerrainNode* GetTreeRoot();

	void ReadHeightMap(string p_fileName);
	void BoxFilterHeightMap();
	float SampleHeightBox(int i, int j);
	bool InBoundsOfHeightMap(int m, int n);

	vec3 CalculateNormal(int i, int j, vector<vector<Vertex>>* vertices);


	void Draw(mat4 p_cullMatrix);

	void DrawDebugBoxes();
	void DrawDebugFrustum();
	void DrawTerrain();

	void UpdateFrustum(mat4 p_cullWorldMatrix);
private:
	TerrainTree terrainTree;
	FrustumCuller* m_frustumCuller;
	Frustum* m_frustum;
	int indexCountTerrain;
	vector<vector<vec3>> m_heightMap;

	int m_mapWidth;
	int m_mapHeight;

};

