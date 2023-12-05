#pragma once
//------------------------------------------------------------------------------
/**
	@file lightserver.h

	@copyright
	(C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "renderdevice.h"
#include <GL/glew.h>
#include "shaderresource.h"
#include <vector>

#define CAMERA_SHADOW uint('GSHW')

namespace Render
{

namespace LightServer
{

	extern glm::vec3 globalLightDirection;
	extern glm::vec3 globalLightColor;


	void Initialize();
	void UpdateWorkGroups(uint resolutionWidth, uint resolutionHeight);
	void OnBeforeRender();
	void Update(Render::ShaderProgramId pid);

	
	GLuint GetWorkGroupsX();
	GLuint GetWorkGroupsY();

	GLuint GetGlobalShadowMapHandle();
	GLuint GetGlobalShadowFramebuffer();
	uint GetShadowMapSize();

};
} // namespace Render
