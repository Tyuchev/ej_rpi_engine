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

using namespace Display;
using namespace Render;

namespace Game
{

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
    
    Input::Keyboard* kbd = Input::GetDefaultKeyboard();

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
    racer.model = LoadModel("assets/pod_racing/Models/GLTF format/craft_speederD.glb");

    std::clock_t c_start = std::clock();
    double dt = 0.01667f;

    Mapgen mapgen(&racer);

    // game loop
    while (this->window->IsOpen())
	{
        auto timeStart = std::chrono::steady_clock::now();
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
        
        this->window->Update();

        if (kbd->pressed[Input::Key::Code::End])
        {
            ShaderResource::ReloadShaders();
        }

        racer.Update(dt);
        racer.CheckCollisions();

        // Store all drawcalls in the render device

        RenderDevice::Draw(racer.model, racer.transform);

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
    nvgText(vg, 0, 30, "James' changes - Testing, testing... Everything seems to be in order.", NULL);

    nvgRestore(vg);
}

} // namespace Game
