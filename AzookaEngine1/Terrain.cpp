#include "Terrain.h"
#include "Globals.h"
#include <fstream>


Terrain::Terrain(void)
{
	m_frustumCuller = new FrustumCuller();
}


Terrain::~Terrain(void)
{
}

void Terrain::Initialize(mat4 p_frustumMatrix)
{
	m_frustum = new Frustum();
	m_frustum->Initialize(p_frustumMatrix);
	terrainTree = TerrainTree(MAX_VERTICES_PER_LEAF);
}

void Terrain::CreateTerrain()
{
	float scale = MAP_SCALE;
	ReadHeightMap("testHeightMap.bmp");

	std::vector<std::vector<Vertex>> verticesTerrain;
	verticesTerrain.resize(m_mapWidth);
	for (int x = 0; x < m_mapWidth; x++)
	{
		for (int z = 0; z < m_mapHeight; z++)
		{
			//unsigned int index = x*MAP_WIDTH + z;			//for height map
			Vertex tempVertex;
			tempVertex.m_position[0] = x*scale;
			float height = m_heightMap[z][x].x;
			if (height != 25)
			{
				int derp = 0;
			}
			tempVertex.m_position[1] = height;
			tempVertex.m_position[2] = -z*scale;
			float t1 = 1 - ((float)x - (float)(m_mapWidth)) / (float)-m_mapWidth;
			float t2 = 1 - ((float)z - (float)(m_mapHeight)) / (float)-m_mapHeight;
			tempVertex.m_texCoord[0] = t1; //FEL!!!
			tempVertex.m_texCoord[1] = t2;
			tempVertex.m_normal = vec3(0, 1, 0);
			verticesTerrain[x].push_back(tempVertex);
		}
	}
	for (int x = 0; x < verticesTerrain.size(); x++)
	{
		for (int z = 0; z < verticesTerrain[0].size(); z++)
		{
			verticesTerrain[x][z].m_normal = CalculateNormal(x, z, &verticesTerrain);
		}
	}



	terrainTree.CreateTree(&verticesTerrain, &terrainTree.terrainTreeRoot);
}

TerrainNode* Terrain::GetTreeRoot()
{
	return &terrainTree.terrainTreeRoot;
}

void Terrain::ReadHeightMap(string p_fileName)
{


	FILE* file = fopen(p_fileName.c_str(), "rb");
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, file);

	//get width and height
	m_mapWidth = *(int*)&header[18];
	m_mapHeight = *(int*)&header[22];

	int row_padded = (m_mapWidth * 3 + 3) & (~3);//silly bmp thing
	unsigned char* data = new unsigned char[row_padded];

	m_heightMap.resize(m_mapHeight);
	for (int i = 0; i < m_mapHeight; i++)
	{

		m_heightMap[i].resize(m_mapWidth);
		fread(data, sizeof(unsigned char), row_padded, file);
		int heightIterator = 0;
		for (int j = 0; j < m_mapWidth * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)

			unsigned char tmp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = tmp;

			float heightScale = 0.1;
			int r = (int)data[j] * heightScale;
			int b = (int)data[j + 1] * heightScale;
			int g = (int)data[j + 2] * heightScale;
			m_heightMap[i][heightIterator] = vec3(r, g, b);
			heightIterator++;

		}
	}
	fclose(file);

	BoxFilterHeightMap();

}

void Terrain::BoxFilterHeightMap()
{
	vector< vector<vec3> > postFilterHeightMap = m_heightMap;


	for (int i = 0; i < postFilterHeightMap.size(); i++)
	{
		for (int j = 0; j < postFilterHeightMap[0].size(); j++)
		{
			postFilterHeightMap[i][j].x = SampleHeightBox(i, j);
		}
	}

	m_heightMap = postFilterHeightMap;
}

float Terrain::SampleHeightBox(int i, int j)
{
	float avrage = 0.0f;
	float sampleCount = 0.0f;

	for (int m = i - 1; m <= i + 1; m++)
	{

		for (int n = j - 1; n <= j + 1; n++)
		{
			if (InBoundsOfHeightMap(m, n))
			{
				avrage += m_heightMap[m][n].x;
				sampleCount += 1.0f;
			}
		}
	}
	return avrage / sampleCount;
}

bool Terrain::InBoundsOfHeightMap(int m, int n)
{
	if (m < m_heightMap.size() && m >= 0)
	{
		if (n < m_heightMap[0].size() && n >= 0)
		{
			return true;
		}
	}

	return false;
}


vec3 Terrain::CalculateNormal(int i, int j, vector<vector<Vertex>>* vertices)
{
	vec3 returnNormal;

	//for (int m = i - 1; m <= i+1; m++)
	//{
	//	for (int n = j - 1; n <= j+1; n++)
	//	{
	//		if (InBoundsOfMap(m, n) && InBoundsOfMap(m + 1, n + 1))
	//		{

	//		}


	for (int m = i - 1; m <= i; m++)
	{
		for (int n = j - 1; n <= j; n++)
		{
			int mapSizeX = vertices->size()-1;
			int mapSizeY = vertices->at(0).size()-1;
			if (m >= 0 && m <= mapSizeX && n >= 0 && n <= mapSizeY
				&& m + 1 >= 0 && m + 1 <= mapSizeX && n + 1 >= 0 && n + 1 <= mapSizeY)
			{
				if (m != i - 1 && n != j - 1)
				{
					vec3 test = vertices->at(0)[0].m_position;
					vec3 temp1 = vertices->at(m + 1).at(n).m_position - vertices->at(m).at(n).m_position;
					vec3 temp2 = vertices->at(m).at(n + 1).m_position - vertices->at(m).at(n).m_position;
					returnNormal += cross(temp1, temp2);
				}
				if (m != i + 1 && n != j + 1)
				{
					vec3 temp1 = vertices->at(m + 1).at(n).m_position - vertices->at(m).at(n + 1).m_position;
					vec3 temp2 = vertices->at(m + 1).at(n + 1).m_position - vertices->at(m).at(n + 1).m_position;
					returnNormal += cross(temp1, temp2);
				}
			}

		}
	}

	return normalize(returnNormal);
}

void Terrain::Draw(mat4 p_cullMatrix)
{

}

void Terrain::DrawDebugBoxes()
{
	vector<TerrainNode*> t_nodesToBeDrawn = m_frustumCuller->DebugNodesToBeDrawn(&m_frustum->GetPlanes(), GetTreeRoot());
	for (int i = 0; i < t_nodesToBeDrawn.size(); i++)
	{
		t_nodesToBeDrawn.at(i)->debugMesh->DrawLines();
	}
}
void Terrain::DrawDebugFrustum()
{
	m_frustum->Draw();
}
void Terrain::DrawTerrain()
{
	vector<TerrainNode*> t_nodesToBeDrawn = m_frustumCuller->NodesToBeDrawn(&m_frustum->GetPlanes(), GetTreeRoot());
	for (int i = 0; i < t_nodesToBeDrawn.size(); i++)
	{
		t_nodesToBeDrawn.at(i)->mesh->Draw();
	}
}

void Terrain::UpdateFrustum(mat4 p_cullWorldMatrix)
{
	m_frustum->UpdateFrustum(p_cullWorldMatrix);
}