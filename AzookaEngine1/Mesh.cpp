#include "Mesh.h"

Mesh::Mesh(string p_fileName)
{
	IndexedModel t_model = OBJModel(p_fileName).ToIndexedModel();
	InitMesh(t_model);
}

Mesh::Mesh(vector<Vertex> p_vertices, vector<int> p_indices)
{
	//m_drawCount = p_vertices.size();
	m_drawCount = p_indices.size();
	//m_indexCount = p_indices.size();

	IndexedModel t_model;

	for (int i = 0; i < p_vertices.size(); i++)
	{
		t_model.positions.push_back(p_vertices.at(i).m_position);
		t_model.texCoords.push_back(p_vertices.at(i).m_texCoord);
		t_model.normals.push_back(p_vertices.at(i).m_normal);
	}

	for (int i = 0; i < p_indices.size(); i++)
	{
		t_model.indices.push_back(p_indices.at(i));
	}


	//for (int i = 0; i < p_indices.size(); i++)
	//{
	//	t_indices.push_back(p_indices.at(i));
	//}


	InitMesh(t_model);

	//glGenVertexArrays(1, &m_vertexArrayObject);
	//glBindVertexArray(m_vertexArrayObject);

	//////buffer for positions
	////generate buffers and store data
	//glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	//glBufferData(GL_ARRAY_BUFFER, m_drawCount*sizeof(t_positions[0]), &t_positions[0], GL_STATIC_DRAW);
	////specify what data is stored
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	//////buffer for tex coords
	////generate buffers and store data
	//glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	//glBufferData(GL_ARRAY_BUFFER, m_drawCount*sizeof(t_texCoords[0]), &t_texCoords[0], GL_STATIC_DRAW);
	////specify what data is stored
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_drawCount*sizeof(int), &p_indices[0], GL_STATIC_DRAW);

	////clear vertex array
	//glBindVertexArray(0);
}

void Mesh::InitMesh(IndexedModel& p_model)
{
	m_drawCount = p_model.indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	////buffer for positions
	//generate buffers and store data
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, p_model.positions.size()*sizeof(p_model.positions[0]), &p_model.positions[0], GL_STATIC_DRAW);
	//specify what data is stored
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	////buffer for tex coords
	//generate buffers and store data
	//glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, p_model.positions.size()*sizeof(p_model.texCoords[0]), &p_model.texCoords[0], GL_STATIC_DRAW);
	//specify what data is stored
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//buffer for normals
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, p_model.positions.size()*sizeof(p_model.normals[0]), &p_model.normals[0], GL_STATIC_DRAW);
	//specify what data is stored
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_model.indices.size()*sizeof(int), &p_model.indices[0], GL_STATIC_DRAW);

	//clear vertex array
	glBindVertexArray(0);
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);

	//glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
	glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::DrawLines()
{
	glBindVertexArray(m_vertexArrayObject);

	//glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
	glDrawElements(GL_LINES, 8*3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::DrawInstances(int p_numberOfInstances)
{
	glBindVertexArray(m_vertexArrayObject);

	glDrawElementsInstanced(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0, p_numberOfInstances);

	glBindVertexArray(0);
}