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

namespace Game
{

enum class GameState {
    Start,
	Game,
    NewHigh,
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
private:

	/// show some ui things
	void RenderUI();
	/// render some nanovg stuff
	void RenderNanoVG(NVGcontext* vg);

	Display::Window* window;
	GameState gameState = GameState::Start;
	std::unique_ptr<PodRacer> racer;
	//PodRacer* racer;
	std::unique_ptr<Mapgen> mapgen;
	//Mapgen* mapgen;
};
} // namespace Game