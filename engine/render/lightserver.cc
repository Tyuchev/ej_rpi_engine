//------------------------------------------------------------------------------
//  @file lightserver.cc
//  @copyright (C) 2021 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "GL/glew.h"
#include "lightserver.h"
#include "model.h"
#include "cameramanager.h"
#include "core/cvar.h"
#include "core/idpool.h"
#include "debugrender.h"
#include "core/random.h"

namespace Render
{
namespace LightServer
{

struct VisibleIndex
{
	int index;
};

enum LightType
{
	NaN = 1,
	Directional = 8
};

glm::vec3 globalLightDirection;
glm::vec3 globalLightColor;

static ModelId icoSphereModel;


constexpr GLuint maxTileProbes = 128;
static GLuint workGroupsX = 0;
static GLuint workGroupsY = 0;

static GLuint globalShadowMap;
static GLuint globalShadowFrameBuffer;
const unsigned int shadowMapSize = 1024;

static Core::CVar* r_draw_light_spheres = nullptr;
static Core::CVar* r_draw_light_sphere_id = nullptr;

//------------------------------------------------------------------------------
/**
*/
void
Initialize()
{
	globalLightDirection = glm::normalize(glm::vec3(-0.1f,-0.77735f,-0.27735f));
	globalLightColor = glm::vec3(1.0f,0.8f,0.8f) * 3.0f;

	// setup shadow pass
	glGenTextures(1, &globalShadowMap);
	glBindTexture(GL_TEXTURE_2D, globalShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, shadowMapSize, shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &globalShadowFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, globalShadowFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, globalShadowMap, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	// setup a shadow camera
	Render::CameraCreateInfo shadowCameraInfo;
	shadowCameraInfo.hash = CAMERA_SHADOW;
	shadowCameraInfo.projection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 500.0f);
	shadowCameraInfo.view = glm::lookAt(glm::vec3(-10.0f,75.0f, -20.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	LightServer::globalLightDirection = shadowCameraInfo.view[2];
	CameraManager::CreateCamera(shadowCameraInfo);
}

//------------------------------------------------------------------------------
/**
*/
void
UpdateWorkGroups(uint resolutionWidth, uint resolutionHeight)
{
	// Define work group sizes in x and y direction based off screen size and tile size (in pixels)
	const int TILE_SIZE = 32;
	workGroupsX = (resolutionWidth + (resolutionWidth % TILE_SIZE)) / TILE_SIZE;
	workGroupsY = (resolutionHeight + (resolutionHeight % TILE_SIZE)) / TILE_SIZE;

	size_t numberOfTiles = workGroupsX * workGroupsY;
}

//------------------------------------------------------------------------------
/**
*/
void
OnBeforeRender()
{
	float y = 70.0f;
	float z = -cos(glfwGetTime() * 0.04f) * 20;
	float x = -sin(glfwGetTime() * 0.02f) * 20;
	//Camera* shadowCamera = CameraManager::GetCamera(CAMERA_SHADOW);
	//shadowCamera->view = glm::lookAt(glm::vec3(x, 75.0f, z),
	//	glm::vec3(0.0f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 1.0f, 0.0f));
	//LightServer::globalLightDirection = shadowCamera->view[2];

}

//------------------------------------------------------------------------------
/**
*/

//------------------------------------------------------------------------------
/**
*/
GLuint GetWorkGroupsX()
{
	return workGroupsX;
}

//------------------------------------------------------------------------------
/**
*/
GLuint GetWorkGroupsY()
{
	return workGroupsY;
}

//------------------------------------------------------------------------------
/**
*/
void
Update(Render::ShaderProgramId pid)
{
	GLuint programHandle = ShaderResource::GetProgramHandle(pid);
	glUniform3fv(glGetUniformLocation(programHandle, "GlobalLightDirection"), 1, &globalLightDirection[0]);
	glUniform3fv(glGetUniformLocation(programHandle, "GlobalLightColor"), 1, &globalLightColor[0]);
}


//------------------------------------------------------------------------------
/**
*/
GLuint
GetGlobalShadowMapHandle()
{
	return globalShadowMap;
}

//------------------------------------------------------------------------------
/**
*/
GLuint
GetGlobalShadowFramebuffer()
{
	return globalShadowFrameBuffer;
}

//------------------------------------------------------------------------------
/**
*/
uint
GetShadowMapSize()
{
	return shadowMapSize;
}
		
} // namespace LightServer
} // namespace Render
