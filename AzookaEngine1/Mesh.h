#pragma once
#include "GraphicsStructs.h"
#include "vector"
#include "obj_loader.h"
#include <GLEW/glew.h>

using namespace std;

class Mesh
{
public:
	Mesh(vector<Vertex> p_vertices, vector<int> p_indices);
	Mesh(string p_fileName);

	~Mesh();


	void Draw();
	void DrawLines();
	void DrawInstances(int p_numberOfInstances);

private:
	//the different kinds of buffers
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		
		NUM_BUFFERS
	};

	void InitMesh(IndexedModel& p_model);

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	int m_drawCount;
	int m_indexCount;
};

