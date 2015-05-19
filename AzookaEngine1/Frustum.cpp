#include "Frustum.h"
#include<glm\gtc\matrix_transform.hpp>


Frustum::Frustum(void)
{
}


Frustum::~Frustum(void)
{
}

void Frustum::Initialize(glm::mat4 perspective)
{
	vanillaCorners[0] = glm::vec4(-1, -1, -1, 1);
	vanillaCorners[1] = glm::vec4(-1, 1, -1, 1);
	vanillaCorners[2] = glm::vec4(1, 1, -1, 1);
	vanillaCorners[3] = glm::vec4(1, -1, -1, 1);

	vanillaCorners[4] = glm::vec4(-1, -1, 1, 1);
	vanillaCorners[5] = glm::vec4(-1, 1, 1, 1);
	vanillaCorners[6] = glm::vec4(1, 1, 1, 1);
	vanillaCorners[7] = glm::vec4(1, -1, 1, 1);



	perspective = glm::inverse(perspective);

	vector<Vertex> t_vertices;
	for (int i = 0; i < 8; i++)
	{
		vanillaCorners[i] = perspective*vanillaCorners[i];
		vanillaCorners[i].x /= vanillaCorners[i].w;
		vanillaCorners[i].y /= vanillaCorners[i].w;
		vanillaCorners[i].z /= vanillaCorners[i].w;
		vanillaCorners[i].w = 1.0f;
		t_vertices.push_back(Vertex(vec3(vanillaCorners[i].x, vanillaCorners[i].y, vanillaCorners[i].z), vec2(0, 0), vec3(0, 0, 0)));
	}
	//engine->CreateStaticDrawBuffer(vertexBufferHandle, vanillaCorners, GL_ARRAY_BUFFER, sizeof(vanillaCorners));

	vector<int> t_indices;
	int lineIndices[] =
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};
	for (int i = 0; i < sizeof(lineIndices)/4; i++)
	{
		t_indices.push_back(lineIndices[i]);
	}
	//engine->CreateStaticDrawBuffer(indexBufferHandle, lineIndices, GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices));

	m_mesh = new Mesh(t_vertices, t_indices);
}

void Frustum::UpdateFrustum(glm::mat4 transformation)
{

	for (int i = 0; i < 8; i++)
	{
		frustumCorners[i] = transformation*vanillaCorners[i];
		frustumCorners[i].x /= vanillaCorners[i].w;
		frustumCorners[i].y /= vanillaCorners[i].w;
		frustumCorners[i].z /= vanillaCorners[i].w;
		frustumCorners[i].w = 1.0f;
	}
}

void Frustum::Draw()
{
	m_mesh->DrawLines();
}

std::vector<Plane> Frustum::GetPlanes()
{
	std::vector<Plane> planes;
	planes.resize(6);
	glm::vec3 cornerPositions[8];
	for (int i = 0; i < 8; i++)
	{
		cornerPositions[i] = glm::vec3(frustumCorners[i].x, frustumCorners[i].y, frustumCorners[i].z);
	}
	/////Near
	glm::vec3 normal = glm::cross(cornerPositions[1] - cornerPositions[0], cornerPositions[3] - cornerPositions[0]);
	normal = glm::normalize(normal);
	float distance = normal.x*cornerPositions[0].x + normal.y*cornerPositions[0].y + normal.z*cornerPositions[0].z;
	distance = -distance;
	planes[0].normal = normal;
	planes[0].distance = distance;
	planes[0].name = "near";

	/////Far
	normal = -normal;
	normal = glm::normalize(normal);
	distance = normal.x*cornerPositions[5].x + normal.y*cornerPositions[5].y + normal.z*cornerPositions[5].z;
	distance = -distance;
	planes[1].normal = normal;
	planes[1].distance = distance;
	planes[1].name = "far";

	/////Right
	normal = glm::cross(cornerPositions[2] - cornerPositions[3], cornerPositions[7] - cornerPositions[3]);
	normal = glm::normalize(normal);
	distance = normal.x*cornerPositions[3].x + normal.y*cornerPositions[3].y + normal.z*cornerPositions[3].z;
	distance = -distance;
	planes[2].normal = normal;
	planes[2].distance = distance;
	planes[2].name = "right";

	/////Left
	normal = glm::cross(cornerPositions[4] - cornerPositions[0], cornerPositions[1] - cornerPositions[0]);
	normal = glm::normalize(normal);
	distance = normal.x*cornerPositions[1].x + normal.y*cornerPositions[1].y + normal.z*cornerPositions[1].z;
	distance = -distance;
	planes[3].normal = normal;
	planes[3].distance = distance;
	planes[3].name = "left";

	/////Top
	normal = glm::cross(cornerPositions[5] - cornerPositions[1], cornerPositions[2] - cornerPositions[1]);
	normal = glm::normalize(normal);
	distance = normal.x*cornerPositions[1].x + normal.y*cornerPositions[1].y + normal.z*cornerPositions[1].z;
	distance = -distance;
	planes[4].normal = normal;
	planes[4].distance = distance;
	planes[4].name = "top";

	/////Down
	normal = glm::cross(cornerPositions[3] - cornerPositions[0], cornerPositions[4] - cornerPositions[0]);
	normal = glm::normalize(normal);
	distance = normal.x*cornerPositions[1].x + normal.y*cornerPositions[1].y + normal.z*cornerPositions[1].z;
	distance = -distance;
	planes[5].normal = normal;
	planes[5].distance = distance;
	planes[5].name = "down";

	return planes;
}

