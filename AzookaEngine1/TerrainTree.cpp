#include "TerrainTree.h"

TerrainTree::TerrainTree( int _maxVerticesPerLeaf)
{
	maxVerticesPerLeaf = _maxVerticesPerLeaf;
	//CreateTree(&fullTerrainBuffer,&terrainTreeRoot);
}

TerrainTree::TerrainTree()
{

}

TerrainTree::~TerrainTree(void)
{
}


std::vector<int> CreateTerrainIndices(int width, int height)
{
	std::vector<int> indices;
	indices.resize((height-1)*(width-1)*6);
	int index = 0;
	int vertex = 0;
	for (int x = 0; x < width-1; x++)
	{
		for (int z = 0; z < height-1; z++)
		{		
			//int vertex = x*width + z;
			indices[index]= vertex;
			indices[index+1]= vertex+1;
			indices[index+2]= vertex+height;
			index +=3;
			indices[index]= vertex+1;
			indices[index+1]= vertex+height+1;
			indices[index+2]= vertex+height;
			index +=3;
			vertex++;
		} 
		vertex++;
	}
	indices.shrink_to_fit();
	return indices;
}

void TerrainTree::CreateTree(std::vector<std::vector<Vertex>>* currentTerrainBuffer, TerrainNode* terrainNode)
{
	float high = currentTerrainBuffer->at(0)[0].m_position[1];
	float low = currentTerrainBuffer->at(0)[0].m_position[1];
	for (int i = 0; i < currentTerrainBuffer->size(); i++)
	{
		for (int j = 0; j < currentTerrainBuffer->at(0).size(); j++)
		{
			if (currentTerrainBuffer->at(i)[j].m_position[1] > high)
			{
				high = currentTerrainBuffer->at(i)[j].m_position[1];
			}
			else if (currentTerrainBuffer->at(i)[j].m_position[1] < low)
			{
				low = currentTerrainBuffer->at(i)[j].m_position[1];
			}
		}
	}

	int width = currentTerrainBuffer->size();
	int height = currentTerrainBuffer->at(width-1).size();
	//lower near left 1
	terrainNode->boxCorners[0] = glm::vec3(	currentTerrainBuffer->at(0).at(0).m_position[0],	low,						
		currentTerrainBuffer->at(0).at(0).m_position[2]);
	////upper far right 2					//use for fancy book method. For myzooka, use all corners below and comment this one out
	//terrainNode ->boxCorners[1] = glm::vec3(currentTerrainBuffer->at(width-1).at(height-1).position[0], high,
	//									currentTerrainBuffer->at(width-1).at(height-1).position[2]);

	//lower near right 2
	terrainNode->boxCorners[1] = glm::vec3(	currentTerrainBuffer->at(width-1).at(height-1).m_position[0],low,
		currentTerrainBuffer->at(0).at(0).m_position[2]);

	//upper near right 3
	terrainNode->boxCorners[2] = glm::vec3(	currentTerrainBuffer->at(width-1).at(height-1).m_position[0],high,
		currentTerrainBuffer->at(0).at(0).m_position[2]);

	//upper near left 4
	terrainNode->boxCorners[3] = glm::vec3(	currentTerrainBuffer->at(0).at(0).m_position[0],	high,						
		currentTerrainBuffer->at(0).at(0).m_position[2]);
	//last vertex m_position

	//lower far left 5
	terrainNode->boxCorners[4] = glm::vec3(	currentTerrainBuffer->at(0).at(0).m_position[0],	low,						
		currentTerrainBuffer->at(width-1).at(height-1).m_position[2]);

	//lower far right 6
	terrainNode->boxCorners[5] = glm::vec3(	currentTerrainBuffer->at(width-1).at(height-1).m_position[0],low,
		currentTerrainBuffer->at(width-1).at(height-1).m_position[2]);

	//upper far right 7
	terrainNode->boxCorners[6] = glm::vec3(	currentTerrainBuffer->at(width-1).at(height-1).m_position[0],high,
		currentTerrainBuffer->at(width-1).at(height-1).m_position[2]);

	//upper far left 8
	terrainNode->boxCorners[7] = glm::vec3(	currentTerrainBuffer->at(0).at(0).m_position[0],	high,						
		currentTerrainBuffer->at(width-1).at(height-1).m_position[2]);




	terrainNode->totalVertexCount = width*height;

	if(width*height > maxVerticesPerLeaf)
	{

		std::vector<std::vector<Vertex>> subBuffers[4];
		terrainNode->isLeaf = false;
		int widthSize = width/2 + 1;
		int heightSize = height/2 +1;
		int indexX = 0;
		int indexZ = 0;
		for (int i = 0; i < 4; i++)
		{			
			if (i == 2)
			{
				indexX = 0;
				indexZ = heightSize-1;
			}
			for (int x = 0; x < widthSize; x++)
			{
				if (indexX < width)
				{
					std::vector<Vertex> temp;
					subBuffers[i].push_back(temp);
				}
				for (int z = 0; z < heightSize; z++)
				{
					if(indexX < width && indexZ < height)
						subBuffers[i][x].push_back(currentTerrainBuffer->at(indexX).at(indexZ));

					indexZ++;
				}								
				indexZ -=heightSize;				
				indexX++;


			}

			indexX--;
			subBuffers[i].shrink_to_fit();

			terrainNode->children[i] = new TerrainNode();
			CreateTree(&subBuffers[i], terrainNode->children[i]);

		}
	}
	else
	{
		vector<Vertex> t_leafVertices;
		terrainNode->isLeaf = true;
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				t_leafVertices.push_back(currentTerrainBuffer->at(i).at(j)); 
			}
		}

		std::vector<int> indices = CreateTerrainIndices(currentTerrainBuffer->size(), currentTerrainBuffer->at(0).size());
		int size = indices.size();
		terrainNode->indexCount = indices.size();
		if(terrainNode->indexCount > 0)
		{
			terrainNode->mesh = new Mesh(t_leafVertices, indices);
		}
	}

	int lineIndices[] = 
	{
		0,1,1,2,2,3,3,0, 
		4,5,5,6,6,7,7,4,
		0,4,1,5,2,6,3,7
	};

	vector<int> t_lineIndicesVector;
	for (int i = 0; i < sizeof(lineIndices)/4; i++)
	{
		t_lineIndicesVector.push_back(lineIndices[i]);
	}

	vector<Vertex> t_debugCorners;
	for (int i = 0; i < 8; i++)
	{
		t_debugCorners.push_back(Vertex(terrainNode->boxCorners[i], vec2(0,0),vec3(0,0,0)));
	}

	terrainNode->debugMesh = new Mesh(t_debugCorners, t_lineIndicesVector);


}

