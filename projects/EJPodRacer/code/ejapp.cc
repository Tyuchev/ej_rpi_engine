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
#include <queue>

using namespace Display;
using namespace Render;

namespace Game
{

constexpr int AVG_FPS_SAMPLES = 1000;
float fps;
float minFps = 50000.0f;
float maxFps = 0.0f;
std::queue<float> fpsQueue;
float avgFpsSum;
double dt;
HighscoreSystem scoreSystem;
// Set in game with tab.
bool debugMode = false;
Input::Keyboard* kbd;
// Collision Tracker
std::vector<std::tuple<Physics::ColliderId, Physics::RaycastPayload>> collisionList;

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
    kbd = Input::GetDefaultKeyboard();

    Render::LightServer::CreatePointLight(glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, 0.0f);

    StartGame();

    std::clock_t c_start = std::clock();
    dt = 0.01667f;

    // game loop
    while (this->window->IsOpen())
	{
        auto timeStart = std::chrono::steady_clock::now();
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        this->window->Update();
        this->RunGame();
        if (kbd->pressed[Input::Key::Code::Escape])
            this->Exit();

        // Execute the entire rendering pipeline
        RenderDevice::Render(this->window, dt);

        // transfer new frame to window
        this->window->SwapBuffers();

        auto timeEnd = std::chrono::steady_clock::now();
        dt = std::min(0.04, std::chrono::duration<double>(timeEnd - timeStart).count());
	}
}

void
EJApp::DoDeath()
{
    this->SetGameState(GameState::PreDeath);
}

void PrintState(GameState state)
{
    switch(state)
    {
    case GameState::Start:
        printf("Start\n");
        return;
	case GameState::Game:
        printf("Game\n");
        return;
    case GameState::NewHigh:
        printf("NewHigh\n");
        return;
	case GameState::PreDeath:
        printf("PreDeath\n");
        return;
    case GameState::Death:
        printf("Death\n");
        return;
    default:
        printf("Unknown state: %i\n", (int)state);
    }
}

float
GetAvgFps()
{
    return avgFpsSum/fpsQueue.size();
}

void
EJApp::RunGame()
{
    if (racer == nullptr || mapgen == nullptr)
    {
        return;
    }
    fps = 1 / dt;
    fpsQueue.push(fps);
    avgFpsSum += fps;
    maxFps = glm::max(maxFps, fps);
    minFps = glm::min(minFps, fps);
    if (fpsQueue.size() > AVG_FPS_SAMPLES)
    {
        float last = fpsQueue.front();
        fpsQueue.pop();
        avgFpsSum -= last;
    }
    Render::RenderDevice::SetPlayerPos(racer->position);

    if (kbd->pressed[Input::Key::Code::F])
    {
        minFps = 50000.0f;
        maxFps = 0.0f;
    }
    if (kbd->pressed[Input::Key::Code::Tab])
    {
        debugMode = !debugMode;
    }

    // Check state.
    UpdateState();
    //PrintState(gameState);
    //printf("%f\n", stateTime);

    if (kbd->pressed[Input::Key::Code::R])
    {
        //ShaderResource::ReloadShaders();
        DoDeath();
    }

    // Road turn test.
    if (gameState == GameState::Game && stateTime > 5.0f)
    {
        // TODO: Fix bug where road snaps after new game.
        static float t = 0.0f;
        const float TURN_SPEED = 0.05f;
        t += dt;
        if (t >= (2 * 3.14159)/TURN_SPEED) {
            t = 0.0f;
        }
        Render::RenderDevice::SetRoadTurnFactor(sin(t * TURN_SPEED));
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
    racer->Update(dt);
    racer->CheckCollisions(collisionList);

    // Store all drawcalls in the render device

    if (gameState != GameState::PreDeath)
    {
        RenderDevice::Draw(racer->model, racer->transform * glm::scale(glm::vec3(1.2f)), racer->position, true);
    }


    // TODO: Test performance on pi with these commented.
    mapgen->Generate();
    mapgen->Draw();
}

void
EJApp::StartGame()
{
    this->SetGameState(GameState::Start);
    scoreSystem.Load("score.txt");
    RenderDevice::SetRoadTurnFactor(0.0f);
    racer = std::make_unique<PodRacer>(PodRacer());
    racer->AddOnDeathCallback([this]{this->DoDeath();});
    racer->Init();
    racer->model = LoadModel("assets/system/podracer.glb");
    racer->position = glm::vec3(0.0f, 1.0f, 0.0f);
    mapgen = std::make_unique<Mapgen>(Mapgen());
    mapgen->Init();
    mapgen->SetPlayer(racer.get());
}

void
EJApp::EndGame()
{
    racer.reset();
    mapgen.reset();
    scoreSystem.Save();
}

void
EJApp::RestartGame()
{
    EndGame();
    StartGame();
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

void
EJApp::SetGameState(GameState state)
{
    static GameState prevState = this->gameState;
    if (prevState != state && state != this->gameState) {
        stateTime = 0.0f;
        this->gameState = state;
    }
}

void
EJApp::UpdateState()
{
    if (!debugMode) {
        switch (gameState)
        {
        case GameState::Start:
            if (stateTime >= 2.0f)
            {
                SetGameState(GameState::Game);
            }
            racer->controlScheme = ControlScheme::NoControls;
            break;
        case GameState::PreDeath:
            if (stateTime >= 3.0f)
            {
                SetGameState(GameState::Death);
            }
            racer->controlScheme = ControlScheme::NoControls;
            break;
        case GameState::Game:
            racer->controlScheme = ControlScheme::NewControls;
            break;
        case GameState::Death:
            if (stateTime > 3.0f)
            {
                RestartGame();
            }
            break;
        default:
            break;
        }
    }
    else {
        racer->controlScheme = ControlScheme::DebugControls;
    }
    
    stateTime += dt;
}

//------------------------------------------------------------------------------
/**
*/
void
EJApp::RenderNanoVG(NVGcontext* vg)
{
    nvgSave(vg);

    NVGcolor inMenuColor = nvgRGBA(255, 255, 255, 255);
    NVGcolor inGameColor = nvgRGBA(255, 255, 255, 255);
    NVGcolor backgroundColor = nvgRGBA(0, 0, 0, 255);
    int width;
    int height;
    window->GetSize(width, height);

    std::string fpsText = std::format("Fps: {}, Min: {}, Max: {}, Avg: {}", fps, minFps, maxFps, GetAvgFps());
    GUI::DrawLabel(vg, fpsText.c_str(), 16.0f, 10.0f, 10.0f, 100.0f, 30.0f, inGameColor);

    if (gameState == GameState::Start) {
        std::string startText;
        if (stateTime < 1.0f) {
            startText = "Get ready...";
        }
        else if (stateTime < 2.0f) {
            startText = "Set...";
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
    else if (gameState == GameState::PreDeath)
    {
        if (stateTime < 3.0f)
        {
            const float size = 500.0f;
            const float x = width / 2.0f - size / 2.0;
            const float y = height / 2.0f - size / 2.0 - 50.0f;
            GUI::DrawExplosion(vg, x, y, size, size, dt, 3.0);
        }
    }
    else if (gameState == GameState::Death) {
        GUI::DrawFilledBox(vg, 0.0f, 0.0f, (float)width, (float)height, backgroundColor);
        GUI::DrawLabel(vg, "Game Over", 35.0f, width / 2.0f - 100.0f, height / 2.0f - 50.0f, 100.0f, 50.0f, inMenuColor);
    }

    nvgRestore(vg);
}

} // namespace Game
