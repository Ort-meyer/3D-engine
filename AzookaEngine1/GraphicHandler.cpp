#include "GraphicHandler.h"

GraphicHandler::GraphicHandler()
{}
GraphicHandler::GraphicHandler(mat4 p_playerPerspectiveMatrix)
{
	InitShaders();

	mat4 t_debugSmallerFrustumPerspective;
	t_debugSmallerFrustumPerspective = perspective( 40.0f, (float)WIDTH/(float)HEIGHT, 0.01f, 1000.0f);

	m_terrain = new Terrain();
	m_terrain->Initialize(p_playerPerspectiveMatrix);
	m_playerPerspectiveMatrix = p_playerPerspectiveMatrix;
	m_terrain->CreateTerrain();

	//Genesis 1:3
	m_sun = new Light();
	mat4 t_lightOrtho = ortho<float>(-50, 50, -50, 50, -50, 250);
	m_sun->InitializeSun(t_lightOrtho, vec3(50, 60, -50), vec3(50, 0, -50), vec3(1, 0, 0));

	//Initialize reflection
	m_reflection = new Reflection();
	m_reflection->Initialize(p_playerPerspectiveMatrix);
	//move it to the right position
	mat4 t_mirrorTrans;
	t_mirrorTrans = translate(t_mirrorTrans, vec3(30, -30, -150)); //Something is horribly wronk!
	mat4 t_mirrorRot;
	t_mirrorRot = rotate(t_mirrorRot, 180.0f, vec3(0, 1, 0));
	m_reflection->Update(t_mirrorTrans*t_mirrorRot);

	m_shadow = new Reflection();
	m_shadow->InitializeShadow();

	//Creating some meshes
	CreateMirrorMesh();
	m_bthTexture = new Texture(string("./textures/bth.png"));
	m_bthTexture->Bind(0);


}


GraphicHandler::~GraphicHandler(void)
{
}

void GraphicHandler::Update(mat4 p_cameraMatrix)	
{
	m_cameraMatrix = p_cameraMatrix;
}

void GraphicHandler::UpdateReflection(mat4 p_worldMatrix, vec3 p_playerPosition)
{
	vec4 t_refPosition4 = p_worldMatrix*vec4(0, 0, 0, 1);
	vec4 t_refNormal4 = p_worldMatrix*vec4(0, 0, -1, 0);
	vec4 t_lookDirection4 = t_refPosition4 - vec4(p_playerPosition.x, p_playerPosition.y, p_playerPosition.z, 1);

	vec3 t_refNormal;
	t_refNormal.z = t_refNormal4.z;
	t_refNormal.x = t_refNormal4.x;
	t_refNormal.y = t_refNormal4.y;

	vec3 t_lookDirection;
	t_lookDirection.x = t_lookDirection4.x;
	t_lookDirection.y = t_lookDirection4.y;
	t_lookDirection.z = t_lookDirection4.z;

	vec3 t_refDir = t_lookDirection - 2.0f * ((dot(t_lookDirection, t_refNormal) / (pow(length(t_refNormal), 2))*t_refNormal)); //reflection linea algebra magic

	vec3 t_refPosition;
	t_refPosition.z = t_refPosition4.z;
	t_refPosition.x = t_refPosition4.x;
	t_refPosition.y = t_refPosition4.y;

	m_reflection->Update(lookAt(t_refPosition, t_refPosition+t_refDir, vec3(0,1,0)));
	UpdateFrustumDebug(inverse(lookAt(t_refPosition, t_refPosition+t_refDir, vec3(0,1,0))));
}

void GraphicHandler::ChangeState(GraphicState p_state)
{
	switch (m_state)
	{
	case NORMAL:
		break;
	case SHADOW:
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_shadow->GetTexture());
		break;
	case REFLECTION:
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_reflection->GetTexture());
		break;
	default:
		break;
	}

	m_state = p_state;

	switch (m_state)
	{
	case NORMAL:
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		break;
	case SHADOW:
		m_shadow->BindFrameBuffer();
		break;
	case REFLECTION:
		m_reflection->BindFrameBuffer();
		break;
	default:
		break;
	}
}
void GraphicHandler::AddNewMesh(MeshType p_meshType,string p_fileName)	
{
	m_drawObjects[p_meshType] = new Mesh(p_fileName);
}
void GraphicHandler::AddNewMesh(MeshType p_meshType,vector<Vertex> p_vertices, vector<int> p_indices)		
{
	m_drawObjects[p_meshType] = new Mesh(p_vertices,p_indices);
}
void GraphicHandler::DrawInstanced(MeshType p_meshType, ShaderType p_shaderType, vector<mat4> p_worldPositions)	
{
	mat4 t_cameraMatrix;
	//set which camera to look from
	switch (m_state)
	{
	case SHADOW:
		t_cameraMatrix = m_sun->GetLightCameraMatrix();
		break;
	case NORMAL:
		t_cameraMatrix = m_cameraMatrix;
		break;
	case REFLECTION:
		t_cameraMatrix = m_reflection->GetReflectionCameraMatrix();
		break;
	}

	//draw from right shader
	switch (p_shaderType)
	{
	case SHADER_SHADOW:
		m_shaderPrograms[p_shaderType]->Bind(); 
		m_shaderPrograms[p_shaderType]->UseUniform(U_CAMERA_MATRIX, t_cameraMatrix);
		m_shaderPrograms[p_shaderType]->UseUniform(U_WORLD_MATRIX, p_worldPositions);
		m_drawObjects[p_meshType]->DrawInstances(p_worldPositions.size());
		break;
	case SHADER_TERRAIN:
		break;
	case SHADER_BASIC_INSTANCED:
			//For basic alpha blend, depends on the fact that the terrain and other thingies are drawn before
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		m_shaderPrograms[p_shaderType]->Bind();
		m_shaderPrograms[p_shaderType]->UseUniform(U_CAMERA_MATRIX, t_cameraMatrix);
		m_shaderPrograms[p_shaderType]->UseUniform(U_WORLD_MATRIX, p_worldPositions);
		m_shaderPrograms[p_shaderType]->UseUniform(U_LIGHT_DIRECTION,m_sun->GetDirection());
		m_shaderPrograms[p_shaderType]->UseUniform(U_TEXTURE_SAMPLER,0);
		m_drawObjects[p_meshType]->DrawInstances(p_worldPositions.size());
		glDisable(GL_BLEND);	
		//glDisable(GL_CULL_FACE);
		break;
	case SHADER_BASIC:
		m_shaderPrograms[p_shaderType]->Bind();
		m_shaderPrograms[p_shaderType]->UseUniform(U_TEXTURE_SAMPLER, 1);
		m_shaderPrograms[p_shaderType]->UseUniform(U_LIGHT_DIRECTION, m_sun->GetDirection());//vec3(1, -1, 1));
		m_shaderPrograms[p_shaderType]->UseUniform(U_CAMERA_MATRIX, t_cameraMatrix * p_worldPositions.at(0));//scale(vec3(0.01, 0.01, 0.01)));// m_mirrorRot * inverse(m_mirrorTrans));
		m_shaderPrograms[p_shaderType]->UseUniform(U_WORLD_MATRIX, mat4(1));
		m_drawObjects[p_meshType]->Draw();
		break;
	case SHADER_DEBUG_FRUSTUM:
		break;
	case SHADER_DEBUG_BASIC:

		break;
	case SHADER_COUNT:
		break;
	default:
		break;
	}
}
void GraphicHandler::DrawTerrain()												
{
	mat4 t_cameraMatrix;
	//set which camera to look from
	switch (m_state)
	{
	case SHADOW:
		t_cameraMatrix = m_sun->GetLightCameraMatrix();
		break;
	case NORMAL:
		t_cameraMatrix = m_cameraMatrix;
		break;
	case REFLECTION:
		t_cameraMatrix = m_reflection->GetReflectionCameraMatrix();
		break;
	}
	///Draw the acctual terrain
	m_shaderPrograms[SHADER_TERRAIN]->Bind();
	m_shaderPrograms[SHADER_TERRAIN]->UseUniform(U_CAMERA_MATRIX, t_cameraMatrix);
	m_shaderPrograms[SHADER_TERRAIN]->UseUniform(U_WORLD_MATRIX, mat4(1));
	m_shaderPrograms[SHADER_TERRAIN]->UseUniform(U_TEXTURE_SAMPLER, 0);
	m_shaderPrograms[SHADER_TERRAIN]->UseUniform(U_SHADOW_MAP, 2);
	m_shaderPrograms[SHADER_TERRAIN]->UseUniform(U_LIGHT_MATRIX, m_sun->GetLightCameraMatrix());
	m_shaderPrograms[SHADER_TERRAIN]->UseUniform(U_LIGHT_DIRECTION, m_sun->GetDirection());
	m_terrain->DrawTerrain();

	///Draw Debugg boxes
	m_shaderPrograms[SHADER_DEBUG_BASIC]->Bind();
	m_shaderPrograms[SHADER_DEBUG_BASIC]->UseUniform(U_CAMERA_MATRIX, t_cameraMatrix);
	//m_terrain->DrawDebugBoxes();

	///Draw Debugg frustum
	m_shaderPrograms[SHADER_DEBUG_FRUSTUM]->Bind();
	m_shaderPrograms[SHADER_DEBUG_FRUSTUM]->UseUniform(U_CAMERA_MATRIX, t_cameraMatrix);
	m_shaderPrograms[SHADER_DEBUG_FRUSTUM]->UseUniform(U_WORLD_MATRIX, m_frustumWorldMatrix);
	//m_terrain->DrawDebugFrustum();
}

void GraphicHandler::InitShaders()
{
	m_shaderPrograms[SHADER_BASIC_INSTANCED] = new ShaderProgram(string("./shaders/InstancedDrawShader"));
	m_shaderPrograms[SHADER_BASIC_INSTANCED]->AddUniform(U_CAMERA_MATRIX, "cameraMatrix");
	m_shaderPrograms[SHADER_BASIC_INSTANCED]->AddUniform(U_WORLD_MATRIX, "worldMatrix");
	m_shaderPrograms[SHADER_BASIC_INSTANCED]->AddUniform(U_LIGHT_DIRECTION, "lightDirection");
	m_shaderPrograms[SHADER_BASIC_INSTANCED]->AddUniform(U_TEXTURE_SAMPLER, "texture");

	m_shaderPrograms[SHADER_SHADOW] = new ShaderProgram(string("./shaders/ShadowMapShader"));
	m_shaderPrograms[SHADER_SHADOW]->AddUniform(U_CAMERA_MATRIX, "cameraMatrix");
	m_shaderPrograms[SHADER_SHADOW]->AddUniform(U_WORLD_MATRIX, "worldMatrix");


	m_shaderPrograms[SHADER_BASIC] = new ShaderProgram(string("./shaders/BasicShader"));
	m_shaderPrograms[SHADER_BASIC]->AddUniform(U_CAMERA_MATRIX, "cameraMatrix");
	m_shaderPrograms[SHADER_BASIC]->AddUniform(U_TEXTURE_SAMPLER, "diffuse");
	m_shaderPrograms[SHADER_BASIC]->AddUniform(U_LIGHT_DIRECTION, "lightDirection");
	m_shaderPrograms[SHADER_BASIC]->AddUniform(U_WORLD_MATRIX, "worldMatrix");

	m_shaderPrograms[SHADER_DEBUG_FRUSTUM] = new ShaderProgram(string("./shaders/frustumDebugShader"));
	m_shaderPrograms[SHADER_DEBUG_FRUSTUM]->AddUniform(U_CAMERA_MATRIX, "cameraMatrix");
	m_shaderPrograms[SHADER_DEBUG_FRUSTUM]->AddUniform(U_WORLD_MATRIX, "rotationMatrix");

	m_shaderPrograms[SHADER_DEBUG_BASIC] = new ShaderProgram(string("./shaders/debugShader"));
	m_shaderPrograms[SHADER_DEBUG_BASIC]->AddUniform(U_CAMERA_MATRIX, "cameraMatrix");

	m_shaderPrograms[SHADER_TERRAIN] = new ShaderProgram(string("./shaders/TerrainShader"));
	m_shaderPrograms[SHADER_TERRAIN]->AddUniform(U_CAMERA_MATRIX, "cameraMatrix");
	m_shaderPrograms[SHADER_TERRAIN]->AddUniform(U_WORLD_MATRIX, "worldMatrix");
	m_shaderPrograms[SHADER_TERRAIN]->AddUniform(U_TEXTURE_SAMPLER, "textureTerrain");
	m_shaderPrograms[SHADER_TERRAIN]->AddUniform(U_SHADOW_MAP, "shadowMap");
	m_shaderPrograms[SHADER_TERRAIN]->AddUniform(U_LIGHT_MATRIX, "lightMatrix");
	m_shaderPrograms[SHADER_TERRAIN]->AddUniform(U_LIGHT_DIRECTION, "lightDirection");
}

void GraphicHandler::CreateMirrorMesh()
{
	float mirrorbigness = 20;
	std::vector<Vertex> mirrorVertices;
	Vertex temp;
	temp.m_normal = glm::vec3(0, 0, -1);

	temp.m_position = glm::vec3(-mirrorbigness, -mirrorbigness, 0);
	temp.m_texCoord = glm::vec2(0, 0);
	mirrorVertices.push_back(temp);
	temp.m_position = glm::vec3(-mirrorbigness, mirrorbigness, 0);
	temp.m_texCoord = glm::vec2(0, 1);
	mirrorVertices.push_back(temp);
	temp.m_position = glm::vec3(mirrorbigness, mirrorbigness, 0);
	temp.m_texCoord = glm::vec2(1, 1);
	mirrorVertices.push_back(temp);
	temp.m_position = glm::vec3(mirrorbigness, -mirrorbigness, 0);
	temp.m_texCoord = glm::vec2(1, 0);
	mirrorVertices.push_back(temp);

	vector<int> t_indices;
	int lineIndices[] =
	{
		0,1,2,
		2,3,0
	};
	for (int i = 0; i < sizeof(lineIndices) / 4; i++)
	{
		t_indices.push_back(lineIndices[i]);
	}

	AddNewMesh(MESH_MIRROR, mirrorVertices, t_indices);
}

void GraphicHandler::UpdateSunDebug(float p_sunMovement)
{
	m_sun->UpdateSun(p_sunMovement);
}

void GraphicHandler::UpdateFrustumDebug(mat4 p_frustumMovement)
{
	m_terrain->UpdateFrustum(p_frustumMovement);
	m_frustumWorldMatrix = p_frustumMovement;
}

mat4 GraphicHandler::GetSunWorldMatrix()
{
	return m_sun->GetWorldMatrix();
}