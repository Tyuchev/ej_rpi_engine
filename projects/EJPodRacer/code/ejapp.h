#pragma once
//------------------------------------------------------------------------------
/**
	Space game application

	(C) 20222 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "podracer.h"
#include "mapgen.h"
#include <memory>

namespace Game
{

enum class GameState {
    Start,
	Game,
    NewHigh,
	PreDeath,
    Death
};

class EJApp : public Core::App
{
public:
	/// constructor
	EJApp();
	/// destructor
	~EJApp();

	/// open app
	bool Open();
	/// run app
	void Run();
	/// exit app
	void Exit();

	void StartGame();
	void RunGame();
	void EndGame();
	void RestartGame();
	void SetGameState(GameState state);
	void DoDeath();
private:

	/// show some ui things
	void RenderUI();
	/// render some nanovg stuff
	void RenderNanoVG(NVGcontext* vg);
	void UpdateState();

	Display::Window* window;
	GameState gameState = GameState::Start;
	float stateTime = 0.0f;
	std::unique_ptr<PodRacer> racer;
	std::unique_ptr<Mapgen> mapgen;
};
} // namespace Game