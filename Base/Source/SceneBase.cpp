#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//comg.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	
	meshList[GEO_UI] = MeshBuilder::GenerateQuad("UI", Color(0, 1, 0), 1.f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//bg.tga");

	meshList[GEO_PAUSEUI] = MeshBuilder::GenerateQuad("PauseUI", Color(0, 1, 0), 1.f);
	meshList[GEO_PAUSEUI]->textureID = LoadTGA("Image//pauseui.tga");

	meshList[GEO_PRINCESS] = MeshBuilder::GenerateQuad("Princess", Color(0, 0, 0), 1.f);
	meshList[GEO_PRINCESS]->textureID = LoadTGA("Image//princess.tga");

	meshList[GEO_HEALTHBAR] = MeshBuilder::GenerateQuad("Health Bar", Color(1, 1, 1), 1.f);
	meshList[GEO_HEALTHBAR]->textureID = LoadTGA("Image//healthbar.tga");

	// Character & background
    meshList[GEO_PLAYERHP] = MeshBuilder::GenerateQuad("Player HP", Color(0, 1, 0), 1.f);
	meshList[GEO_CHARACTER] = MeshBuilder::GenerateQuad("Character", Color(1, 0,1), 1.0f);
	//meshList[GEO_CHARACTER]->textureID = LoadTGA("Image//CharacterFrame0.tga");

    meshList[GEO_CHARACTER2] = MeshBuilder::GenerateQuad("Character", Color(1, 0, 1), 1.f);
	//meshList[GEO_CHARACTER2]->textureID = LoadTGA("Image//ICharacterFrame0.tga");

	meshList[GEO_PROJECTILE] = MeshBuilder::GenerateQuad("Character", Color(0, 0, 0), 1.f);
	meshList[GEO_PROJECTILE]->textureID = LoadTGA("Image//Bullet.tga");

	meshList[GEO_BACKGROUND] = MeshBuilder::Generate2DMesh("GEO_BACKGROUND", Color(1, 1, 1), 0.0f, 0.0f, 800.0f, 600.0f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//heaven.tga");

	meshList[GEO_EDITBACKGROUND] = MeshBuilder::GenerateQuad("GEO_EDITBACKGROUND", Color(1, 1, 1), 1.f);
	meshList[GEO_EDITBACKGROUND]->textureID = LoadTGA("Image//editbg.tga");

	//monster
	meshList[GEO_MONSTER] = MeshBuilder::GenerateQuad("Mario", Color(0, 0, 0), 1.0f);
	meshList[GEO_MONSTER]->textureID = LoadTGA("Image//mario.tga");
	meshList[GEO_MONSTER2] = MeshBuilder::GenerateQuad("Mario", Color(0, 0, 0), 1.f);
	meshList[GEO_MONSTER2]->textureID = LoadTGA("Image//mario2.tga");


    //panels
    meshList[GEO_B_PANEL] = MeshBuilder::GenerateQuad("Panel", Color(0, 1, 0), 1.f);
    meshList[GEO_B_PANEL]->textureID = LoadTGA("Image//panels//bPanel.tga");

    meshList[GEO_M_PANEL] = MeshBuilder::GenerateQuad("Panel", Color(0, 1, 0), 1.f);
    meshList[GEO_M_PANEL]->textureID = LoadTGA("Image//panels//mPanel.tga");

    meshList[GEO_T_PANEL] = MeshBuilder::GenerateQuad("Panel", Color(0, 1, 0), 1.f);
    meshList[GEO_T_PANEL]->textureID = LoadTGA("Image//panels//tPanel.tga");

    //platforms
    meshList[GEO_PLAT_NORMAL] = MeshBuilder::GenerateQuad("Normal platform", Color(0, 1, 0), 1.f);
    meshList[GEO_PLAT_NORMAL]->textureID = LoadTGA("Image//platform//normal.tga");

	meshList[GEO_CAVE] = MeshBuilder::GenerateQuad("Cave Platform", Color(1, 0, 0), 1.f);
	meshList[GEO_CAVE]->textureID = LoadTGA("Image//CaveTile.tga");

    meshList[GEO_PLAT_SELECTOR] = MeshBuilder::GenerateQuad("platform selector", Color(0, 1, 0), 1.f);
    meshList[GEO_PLAT_SELECTOR]->textureID = LoadTGA("Image//platform//selector//ss1.tga");

	meshList[GEO_CASTLE] = MeshBuilder::GenerateQuad("Castle Platform", Color(1, 1, 1), 1.f);
	meshList[GEO_CASTLE]->textureID = LoadTGA("Image//platform//castle.tga");

	meshList[GEO_CASTLECENTER] = MeshBuilder::GenerateQuad("Castle Center Platform", Color(1, 1, 1), 1.f);
	meshList[GEO_CASTLECENTER]->textureID = LoadTGA("Image//platform//castlecenter.tga");

	//Victory & death screen
	meshList[GEO_DEATHSCREEN] = MeshBuilder::GenerateQuad("DeathSCreen", Color(1, 1, 1), 1.f);
	meshList[GEO_DEATHSCREEN]->textureID = LoadTGA("Image//gameover.tga");

	meshList[GEO_VICTORY] = MeshBuilder::GenerateQuad("VictoryScreen", Color(1, 1, 1), 1.f);
	meshList[GEO_VICTORY]->textureID = LoadTGA("Image//victory.tga");

	// Buttons thing
	meshList[GEO_PLAY] = MeshBuilder::GenerateQuad("Play", Color(1, 1, 1), 1.f);
	meshList[GEO_PLAY]->textureID = LoadTGA("Image//buttons//play.tga");
	meshList[GEO_PLAYHOVER] = MeshBuilder::GenerateQuad("Play Hover ", Color(1, 1, 1), 1.f);
	meshList[GEO_PLAYHOVER]->textureID = LoadTGA("Image//buttons//playhover.tga");

	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("Menu", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//buttons//menu.tga");
	meshList[GEO_MENUHOVER] = MeshBuilder::GenerateQuad("Menu Hover ", Color(1, 1, 1), 1.f);
	meshList[GEO_MENUHOVER]->textureID = LoadTGA("Image//buttons//menuhover.tga");

	meshList[GEO_EDIT] = MeshBuilder::GenerateQuad("Edit", Color(1, 1, 1), 1.f);
	meshList[GEO_EDIT]->textureID = LoadTGA("Image//buttons//edit.tga");
	meshList[GEO_EDITHOVER] = MeshBuilder::GenerateQuad("Edit Hover But", Color(1, 1, 1), 1.f);
	meshList[GEO_EDITHOVER]->textureID = LoadTGA("Image//buttons//edithover.tga");

	meshList[GEO_LOAD] = MeshBuilder::GenerateQuad("Load", Color(1, 1, 1), 1.f);
	meshList[GEO_LOAD]->textureID = LoadTGA("Image//buttons//load.tga");
	meshList[GEO_LOADHOVER] = MeshBuilder::GenerateQuad("Load Hover But", Color(1, 1, 1), 1.f);
	meshList[GEO_LOADHOVER]->textureID = LoadTGA("Image//buttons//loadhover.tga");

	meshList[GEO_HIGHSCORE] = MeshBuilder::GenerateQuad("Highscore", Color(1, 1, 1), 1.f);
	meshList[GEO_HIGHSCORE]->textureID = LoadTGA("Image//buttons//highscore.tga");
	meshList[GEO_HIGHSCOREHOVER] = MeshBuilder::GenerateQuad("Highscore hover but", Color(1, 1, 1), 1.f);
	meshList[GEO_HIGHSCOREHOVER]->textureID = LoadTGA("Image//buttons//highscorehover.tga");

	meshList[GEO_SAVE] = MeshBuilder::GenerateQuad("save", Color(1, 1, 1), 1.f);
	meshList[GEO_SAVE]->textureID = LoadTGA("Image//buttons//save.tga");
	meshList[GEO_SAVEHOVER] = MeshBuilder::GenerateQuad("save hover ", Color(1, 1, 1), 1.f);
	meshList[GEO_SAVEHOVER]->textureID = LoadTGA("Image//buttons//savehover.tga");

	meshList[GEO_EXIT] = MeshBuilder::GenerateQuad("Exit", Color(1, 1, 1), 1.f);
	meshList[GEO_EXIT]->textureID = LoadTGA("Image//buttons//exit.tga");
	meshList[GEO_EXITHOVER] = MeshBuilder::GenerateQuad("Exit Hover ", Color(1, 1, 1), 1.f);
	meshList[GEO_EXITHOVER]->textureID = LoadTGA("Image//buttons//exithover.tga");

	meshList[GEO_RESTART] = MeshBuilder::GenerateQuad("restart", Color(1, 1, 1), 1.f);
	meshList[GEO_RESTART]->textureID = LoadTGA("Image//buttons//restart.tga");
	meshList[GEO_RESTARTHOVER] = MeshBuilder::GenerateQuad("restart hover", Color(1, 1, 1), 1.f);
	meshList[GEO_RESTARTHOVER]->textureID = LoadTGA("Image//buttons//restarthover.tga");

	meshList[GEO_RESUME] = MeshBuilder::GenerateQuad("resume", Color(1, 1, 1), 1.f);
	meshList[GEO_RESUME]->textureID = LoadTGA("Image//buttons//resume.tga");
	meshList[GEO_RESUMEHOVER] = MeshBuilder::GenerateQuad("resume hover ", Color(1, 1, 1), 1.f);
	meshList[GEO_RESUMEHOVER]->textureID = LoadTGA("Image//buttons//resumehover.tga");
	
    meshList[GEO_HOTSPOT] = MeshBuilder::GenerateQuad("hotspot ", Color(0.902f, 0.902f, 0.980f), 1.f);

	bLightEnabled = false;
}

void SceneBase::Update(double dt)
{
   // camera.Update(dt);
	//Keyboard Section
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);

	if(Application::IsKeyPressed('7'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('8'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	fps = (float)(1.f / dt);
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.7f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderModelOnScreen(Mesh *mesh, bool enableLight, Vector3 size, float x, float y, float z, Vector3 rotation)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -100, 70); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);

	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, z);
	modelStack.Scale(size.x, size.y, size.z);
	modelStack.Rotate(rotation.x, 1, 0, 0);
	modelStack.Rotate(rotation.y, 0, 1, 0);
	modelStack.Rotate(rotation.z, 0, 0, 1);

	RenderMesh(mesh, enableLight);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneBase::Render2DMesh(Mesh *mesh, bool enableLight, float size, float x, float y, bool rotate, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	if (rotate)
		modelStack.Rotate(0, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
