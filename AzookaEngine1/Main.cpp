#include <iostream>
#include <GLEW/glew.h>
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
#include "GraphicHandler.h"
#include "TestObject.h"
using namespace std;
std::vector<mat4> GetBthMatrices();

int main(int argc, char** argv)
{

	Display m_display(WIDTH, HEIGHT, "Hello World!");
	UserCmd m_userCmd;
	Player* m_player = new Player(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0));
	TestObject m_testObject(vec3(0, 5, 0));
	GraphicHandler m_graphicHandler = GraphicHandler(m_player->GetPerspectiveMatrix());

	m_graphicHandler.AddNewMesh(MESH_BTH,"./objs/bth.obj");

	float t_testCounter = 0;


	mat4 t_mirrorTrans;
	t_mirrorTrans = translate(t_mirrorTrans, vec3(30, -30, -150)); //Something is horribly wronk!
	mat4 t_mirrorRot;
	t_mirrorRot = rotate(t_mirrorRot, 180.0f, vec3(0, 1, 0));
	vector<mat4> t_mirrorWorldMatrix;
	t_mirrorWorldMatrix.push_back(t_mirrorRot * inverse(t_mirrorTrans));
	m_graphicHandler.UpdateSunDebug(210);
	while (!m_display.IsClosed())
	{
		/////UPDATE
		//get user inputs
		m_display.Update(m_userCmd);
		//player movements
		m_player->Update(&m_userCmd);
		//update the player matrix for the graphics handler
		m_graphicHandler.Update(m_player->GetCameraMatrix());
		m_graphicHandler.UpdateFrustumDebug(inverse(m_player->GetWorldMatrix()));//inverse(m_player->GetWorldMatrix())
		float right = 0;
		float forward = 0;
		if (m_userCmd.m_rightArrowPressed)
			right = 1;
		if (m_userCmd.m_leftArrowPressed)
			right = -1;

		if (m_userCmd.m_upArrowPressed)
			forward = 1;
		if (m_userCmd.m_downArrowPressed)
			forward = -1;
		m_testObject.Update(right, forward);




		/////DRAW
		m_display.Clear(vec4(0, 1, 0.5, 1));
		mat4 t_cameraMatrix;


		////DRAW EVERYTHING TO BE SHADOWED
		m_graphicHandler.ChangeState(SHADOW);
		vector<mat4> t_bthObjMatrices = GetBthMatrices();
		t_bthObjMatrices.push_back(m_graphicHandler.GetSunWorldMatrix() * scale(vec3(0.05)));
		m_graphicHandler.DrawInstanced(MESH_BTH, SHADER_SHADOW, GetBthMatrices());
		m_testObject.Draw(&m_graphicHandler);



		////DRAW EVERYTHYING TO BE REFLECTED
		m_graphicHandler.ChangeState(REFLECTION);
		m_graphicHandler.DrawTerrain();
		m_testObject.Draw(&m_graphicHandler);

		///Draw BTH obj
		m_graphicHandler.DrawInstanced(MESH_BTH, SHADER_BASIC_INSTANCED, t_bthObjMatrices);



		///DRAW EVERYTHING FROM PLAYER'S PERSPECTIVE
		m_graphicHandler.ChangeState(NORMAL);
		m_graphicHandler.DrawTerrain();
		m_testObject.Draw(&m_graphicHandler);
		m_graphicHandler.DrawInstanced(MESH_BTH, SHADER_BASIC_INSTANCED, t_bthObjMatrices);

		//Draw the reflection

		m_graphicHandler.UpdateReflection(t_mirrorRot * inverse(t_mirrorTrans),m_player->GetPosition());
		//Draw mirror
		m_graphicHandler.DrawInstanced(MESH_MIRROR, SHADER_BASIC, t_mirrorWorldMatrix);
	}
	return 0;
}

std::vector<mat4> GetBthMatrices()
{
	static float t_rotate = 0;
	t_rotate += 0.1;
	std::vector<mat4> bthObjects;
	//	bthObjects.resize(5);
	for (int i = 0; i < 2; i++)
	{
		float angle = i * 35;
		glm::vec3 position = glm::vec3(i * 25 + 50, 15, i*25 - 50);
		glm::mat4 translationMatrix;
		translationMatrix = glm::translate(translationMatrix, position);

		glm::mat4 rotationMatrix;
		rotationMatrix = glm::rotate(mat4(1), 90.0f, glm::vec3(1, 0, 0)) * glm::rotate(mat4(1), angle + t_rotate, glm::vec3(0, 1, 0));

		bthObjects.push_back(translationMatrix*rotationMatrix*scale(vec3(0.3,0.3,0.3)));

	}
	return bthObjects;
}
