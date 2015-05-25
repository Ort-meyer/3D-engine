#pragma once
#include <map>
#include <iostream>
#include <GLEW/glew.h>
#include <string>
#include <vector>
#include "Display.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Globals.h"
#include "Player.h"
#include "Terrain.h"
#include "Reflection.h"
#include "Light.h"

using namespace std;
using namespace glm;
enum MeshType {MESH_BTH, MESH_MIRROR, MESH_FRUSTUM, MESH_COUNT};
enum GraphicState {NORMAL, SHADOW, REFLECTION};
enum ShaderType{SHADER_SHADOW, SHADER_TERRAIN, SHADER_BASIC_INSTANCED, SHADER_BASIC, SHADER_DEBUG_FRUSTUM, SHADER_DEBUG_BASIC, SHADER_COUNT};
class GraphicHandler
{
public:
	GraphicHandler();
	GraphicHandler(mat4 p_playerPerspectiveMatrix);
	~GraphicHandler(void);
	void Update(mat4 p_cameraMatrix);
	void UpdateReflection(mat4 p_worldMatrix, vec3 p_playerPosition);
	void ChangeState(GraphicState p_state);
	void AddNewMesh( MeshType p_meshType, string p_fileName);
	void AddNewMesh( MeshType p_meshType, vector<Vertex> p_vertices, vector<int> p_indices);
	void DrawInstanced(MeshType p_meshType, ShaderType p_shaderType, vector<mat4> p_worldPositions);
	void DrawTerrain();
	void UpdateSunDebug(float p_sunMovement);
	void UpdateFrustumDebug(mat4(p_frustumMovement));
	mat4 GetSunWorldMatrix();

private:
	Mesh* m_drawObjects[MESH_COUNT+1];
	ShaderProgram* m_shaderPrograms[SHADER_COUNT+1];
	mat4 m_playerPerspectiveMatrix;
	mat4 m_cameraMatrix;
	void InitShaders();
	void CreateMirrorMesh();
	mat4 m_frustumWorldMatrix;

	GraphicState m_state;
	Terrain* m_terrain;
	Light* m_sun;
	Reflection* m_reflection;
	Reflection* m_shadow;
	Texture* m_bthTexture;
	Texture* m_terrainTexture;
};

