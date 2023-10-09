//------------------------------------------------------------------------------
// spacegameapp.cc
// (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "ejapp.h"
#include <cstring>
#include "imgui.h"
#include "render/renderdevice.h"
#include "render/shaderresource.h"
#include <vector>
#include "render/textureresource.h"
#include "render/model.h"
#include "render/cameramanager.h"
#include "render/lightserver.h"
#include "render/debugrender.h"
#include "core/random.h"
#include "render/input/inputserver.h"
#include "core/cvar.h"
#include "render/physics.h"
#include <chrono>
#include "podracer.h"
#include "mapgen.h"
#include "gameobject.h"

using namespace Display;
using namespace Render;

namespace Game
{

float fps;

//------------------------------------------------------------------------------
/**
*/
EJApp::EJApp()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
EJApp::~EJApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
EJApp::Open()
{
	App::Open();
	this->window = new Display::Window;
    this->window->SetSize(1280, 720);

    if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        RenderDevice::Init();

		// set ui rendering function
		this->window->SetUiRender([this]()
		{
			this->RenderUI();
		});
        this->window->SetNanoFunc([this](NVGcontext* vg)
        {
            this->RenderNanoVG(vg);
        });
        
        return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
EJApp::Run()
{
    int w;
    int h;
    this->window->GetSize(w, h);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), float(w) / float(h), 0.01f, 1000.f);
    Camera* cam = CameraManager::GetCamera(CAMERA_MAIN);
    cam->projection = projection;

    // Setup skybox
    std::vector<const char*> skybox
    {
        "assets/space_deprecated/bg.png",
        "assets/space_deprecated/bg.png",
        "assets/space_deprecated/bg.png",
        "assets/space_deprecated/bg.png",
        "assets/space_deprecated/bg.png",
        "assets/space_deprecated/bg.png"
    };
    TextureResourceId skyboxId = TextureResource::LoadCubemap("skybox", skybox, true);
    RenderDevice::SetSkybox(skyboxId);
    RenderDevice::SetRoadScale(TILE_SCALE);
    
    Input::Keyboard* kbd = Input::GetDefaultKeyboard();

    // Collision Tracker
    std::vector<std::tuple<Physics::ColliderId, Physics::RaycastPayload>> collisionList;

    //const int numLights = 4;
    //Render::PointLightId lights[numLights];
    //// Setup lights
    //for (int i = 0; i < numLights; i++)
    //{
        //glm::vec3 translation = glm::vec3(
            //Core::RandomFloatNTP() * 20.0f,
            //Core::RandomFloatNTP() * 20.0f,
            //Core::RandomFloatNTP() * 20.0f
        //);
        //glm::vec3 color = glm::vec3(
            //Core::RandomFloat(),
            //Core::RandomFloat(),
            //Core::RandomFloat()
        //);
        //lights[i] = Render::LightServer::CreatePointLight(translation, color, Core::RandomFloat() * 4.0f, 1.0f + (15 + Core::RandomFloat() * 10.0f));
    //}

    PodRacer racer;
    racer.model = LoadModel("assets/pod_racer/Models/GLTF format/craft_speederD.glb");
    racer.position = glm::vec3(0.0f, 1.0f, 0.0f);

    ModelId groundPlane = LoadModel("assets/pod_racer/Models/GLTF format/terrain.glb");
    glm::mat4 groundTransform = glm::scale(glm::mat4(1), glm::vec3(100.0f, 0.0f, 100.0f));

    std::clock_t c_start = std::clock();
    double dt = 0.01667f;

    Mapgen mapgen(&racer);


    // Debug draw tests
    //DebugDraw::ArrowStraight(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // +x is left of spawn origin
    // +y is up of spawn origin
    // +z is forward of spawn origin
    {
        //DebugDraw::ArrowStraight(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.0f, 1.0f)); // forward
        //DebugDraw::ArrowStraight(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)); // right
        //DebugDraw::ArrowStraight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)); // back
        //DebugDraw::ArrowStraight(glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // left
        //DebugDraw::ArrowStraight(glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // up
        //DebugDraw::ArrowStraight(glm::vec3(-15.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)); // down
    }
    //{
        //float step = 2.0f * 3.1415f / 40.0f;
        //for (int i = 0; i < 40; i++) {
            //glm::vec3 dir(sin(step*i), 0.0f, cos(step*i));
            //glm::vec3 pos = dir * 10.0f;
            //DebugDraw::ArrowStraightOffset(pos, dir);
            ////DebugDraw::ArrowStraight(glm::vec3(0.0f), dir);
            ////DebugDraw::ArrowStraight(glm::vec3(5.0f * i, 0.0f, 0.0f), dir);
        //}
    //}
    //{
        //float range = 10.0f;
        //float step = 1.0f;
        //for (float f = -range; f >= range; f += step) {
            //glm::vec3 dir(1.0f, 0.0f, f);
            //dir = glm::normalize(dir);
            ////DebugDraw::ArrowStraight(glm::vec3(0.0f), dir);
            //DebugDraw::ArrowStraight(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            
        //}
    //}

    // game loop
    while (this->window->IsOpen())
	{
        fps = 1 / dt;
        Render::RenderDevice::SetPlayerPos(racer.position);
        auto timeStart = std::chrono::steady_clock::now();
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
        
        this->window->Update();

        if (kbd->pressed[Input::Key::Code::R])
        {
            ShaderResource::ReloadShaders();
        }

        // Road turn test.
        {
            static float t = 0.0f;
            t += dt;
            if (t >= 2 * 3.14159) {
                t = 0.0f;
            }
            //Render::RenderDevice::SetRoadTurnFactor(sin(t));
        }

        racer.Update(dt);
        racer.CheckCollisions(collisionList);

        // Store all drawcalls in the render device

        RenderDevice::Draw(racer.model, racer.transform);
        //RenderDevice::Draw(groundPlane, groundTransform);
        groundTransform = glm::translate(glm::vec3(racer.position.x, TILE_HEIGHT, racer.position.z));
        groundTransform = glm::scale(groundTransform, glm::vec3(100.0f, 0.0f, 100.0f));


        mapgen.Generate();
        mapgen.Draw();

        // Execute the entire rendering pipeline
        RenderDevice::Render(this->window, dt);

        // transfer new frame to window
        this->window->SwapBuffers();

        auto timeEnd = std::chrono::steady_clock::now();
        dt = std::min(0.04, std::chrono::duration<double>(timeEnd - timeStart).count());

        //if (kbd->pressed[Input::Key::Code::R])
            //mapgen.Generate();

        if (kbd->pressed[Input::Key::Code::Escape])
            this->Exit();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
EJApp::Exit()
{
    this->window->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
EJApp::RenderUI()
{
	if (this->window->IsOpen())
	{
        

        //Debug::DispatchDebugTextDrawing();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
EJApp::RenderNanoVG(NVGcontext* vg)
{
    nvgSave(vg);

    nvgBeginPath(vg);
    nvgCircle(vg, 600, 100, 50);
    NVGpaint paint;
    paint = nvgLinearGradient(vg, 600, 100, 650, 150, nvgRGBA(255, 0, 0, 255), nvgRGBA(0, 255, 0, 255));
    nvgFillPaint(vg, paint);
    nvgFill(vg);



    // Header
    nvgBeginPath(vg);
    nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
    nvgStroke(vg);

    nvgFontSize(vg, 16.0f);
    nvgFontFace(vg, "sans");
    nvgFillColor(vg, nvgRGBA(255, 255, 255, 128));
    std::string fpsText = "Fps: ";
    fpsText += std::to_string(fps);
    nvgText(vg, 0, 30, fpsText.c_str(), NULL);

    nvgRestore(vg);
}

} // namespace Game
