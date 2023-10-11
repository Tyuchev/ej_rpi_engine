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
#include "guihelper.h"
#include "highscore.h"

using namespace Display;
using namespace Render;

namespace Game
{

float fps;
double dt;
HighscoreSystem scoreSystem = HighscoreSystem("score.txt");
// Set in game with tab.
bool debugMode = false;
Input::Keyboard* kbd;
// Collision Tracker
std::vector<std::tuple<Physics::ColliderId, Physics::RaycastPayload>> collisionList;
PodRacer racer;
Mapgen mapgen;

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
    
    StartGame();

    std::clock_t c_start = std::clock();
    dt = 0.01667f;

    // game loop
    while (this->window->IsOpen())
	{
        this->RunGame();
        if (kbd->pressed[Input::Key::Code::Escape])
            this->Exit();
	}
}

void
EJApp::RunGame()
{
    fps = 1 / dt;
    Render::RenderDevice::SetPlayerPos(racer.position);
    auto timeStart = std::chrono::steady_clock::now();
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    this->window->Update();

    if (kbd->pressed[Input::Key::Code::Tab])
    {
        debugMode = !debugMode;
    }

    // Check state.
    if (!debugMode) {
        switch (gameState)
        {
        case GameState::Start:
            racer.controlScheme = ControlScheme::NoControls;
            break;
        case GameState::Game:
            racer.controlScheme = ControlScheme::NewControls;
            break;
        }
    }
    else {
        racer.controlScheme = ControlScheme::DebugControls;
    }

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

    // Temp score system.
    if (gameState == GameState::Game)
    {
        static float t = 0.0f;
        t += dt;
        if (t > 1.0f)
        {
            scoreSystem.currentScore++;
            t = 0.0f;
        }
    }

    racer.Update(dt);
    racer.CheckCollisions(collisionList);

    // Store all drawcalls in the render device

    RenderDevice::Draw(racer.model, racer.transform, racer.position, true);


    mapgen.Generate();
    mapgen.Draw();

    // Execute the entire rendering pipeline
    RenderDevice::Render(this->window, dt);

    // transfer new frame to window
    this->window->SwapBuffers();

    auto timeEnd = std::chrono::steady_clock::now();
    dt = std::min(0.04, std::chrono::duration<double>(timeEnd - timeStart).count());

}

void
EJApp::StartGame()
{
    kbd = Input::GetDefaultKeyboard();
    racer.Init();
    racer.model = LoadModel("assets/system/podracer.glb");
    racer.position = glm::vec3(0.0f, 1.0f, 0.0f);
    mapgen.Init();
    mapgen.SetPlayer(&racer);
}

void
EJApp::EndGame()
{
    scoreSystem.Save();
}

//------------------------------------------------------------------------------
/**
*/
void
EJApp::Exit()
{
    this->EndGame();
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
    static GameState prevState = this->gameState;
    static float stateTime = 0.0f;
    if (prevState != gameState) {
        stateTime = 0.0f;
        prevState = gameState;
    }
    else {
        stateTime += dt;
    }
    nvgSave(vg);

    NVGcolor inMenuColor = nvgRGBA(255, 255, 255, 255);
    NVGcolor inGameColor = nvgRGBA(255, 255, 255, 255);
    NVGcolor backgroundColor = nvgRGBA(0, 0, 0, 255);
    int width;
    int height;
    window->GetSize(width, height);

    std::string fpsText = "Fps: ";
    fpsText += std::to_string(fps);
    GUI::DrawLabel(vg, fpsText.c_str(), 16.0f, 10.0f, 10.0f, 100.0f, 30.0f, inGameColor);

    if (gameState == GameState::Start) {
        std::string startText;
        if (stateTime < 1.0f) {
            startText = "Get ready...";
        }
        else if (stateTime < 2.0f) {
            startText = "Set...";
        }
        else {
            gameState = GameState::Game;
        }
        GUI::DrawLabel(vg, startText.c_str(), 35.0, width / 2.0 - 100.0f, height / 2.0f - 50.0f, 100.0f, 50.0f, inGameColor);
    }

    else if (gameState == GameState::Game) {
        if (stateTime < 1.0f) {
            GUI::DrawLabel(vg, "Go!", 35.0, width / 2.0 - 100.0f, height / 2.0f - 50.0f, 100.0f, 50.0f, inGameColor);
        }
        std::string scoreText = "Current score: ";
        scoreText += std::to_string(scoreSystem.currentScore);
        GUI::DrawLabel(vg, scoreText.c_str(), 16.0f, width - 200.0f, 10.0f, 200.0f, 30.0f, inGameColor);

        std::string hiScoreText = "Previous high: ";
        hiScoreText += std::to_string(scoreSystem.previousHigh);
        GUI::DrawLabel(vg, hiScoreText.c_str(), 16.0f, width - 200.0f, 30.0f, 200.0f, 30.0f, inGameColor);
    }

    else if (gameState == GameState::Death) {
        GUI::DrawFilledBox(vg, 0.0f, 0.0f, (float)width, (float)height, backgroundColor);
        GUI::DrawLabel(vg, "Game Over", 35.0f, width / 2.0f - 100.0f, height / 2.0f - 50.0f, 100.0f, 50.0f, inMenuColor);
    }


    nvgRestore(vg);
}

} // namespace Game
