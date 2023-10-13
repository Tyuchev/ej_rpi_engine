//------------------------------------------------------------------------------
//  @file inputserver.cc
//  @copyright (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "inputserver.h"
#include "GLFW/glfw3.h"
#include <cstdio>

namespace Input
{

struct HIDState
{
	Keyboard keyboard;
	Mouse mouse;
	std::vector<Gamepad*> gamepads;
};

static HIDState* hid = nullptr;

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::Create()
{
	if (hid == nullptr)
		hid = new HIDState();

	hid->gamepads = std::vector<Gamepad*>(16);
	for (int i = 0; i < 16; i++)
	{
		Gamepad* gamepad = nullptr;
		if (glfwJoystickPresent(i))
		{
			printf("[Input] Found controller id: %i\n", i);
			gamepad = new Gamepad();
			gamepad->id = i;
		}
		hid->gamepads[i] = gamepad;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::BeginFrame()
{
	for (int i = 0; i < Key::Code::NumKeyCodes; i++)
	{
		if (hid->keyboard.released[i])
			hid->keyboard.held[i] = false;
		
		hid->keyboard.pressed[i] = false;
		hid->keyboard.released[i] = false;
	}

	for (int i = 0; i < Mouse::Button::NumMouseButtons; i++)
	{
		if (hid->mouse.released[i])
			hid->mouse.held[i] = false;

		hid->mouse.pressed[i] = false;
		hid->mouse.released[i] = false;
	}

	hid->mouse.delta = glm::vec2(0);
	hid->mouse.previousPosition = hid->mouse.position;
	for (int i = 0; i < 16; i++)
	{
		Gamepad* gamepad = hid->gamepads[i];
		if (gamepad != nullptr)
		{
			gamepad->Update();
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::HandleKeyEvent(int32 key, int32 scancode, int32 action, int32 mods)
{
	if (action == GLFW_PRESS)
	{
		Key::Code code = Key::FromGLFW(key);
		hid->keyboard.pressed[code] = true;
		hid->keyboard.held[code] = true;

		// Special case buttons:
		if (code == Key::Code::LeftControl || code == Key::Code::RightControl)
		{
			hid->keyboard.pressed[Key::Code::Control] = true;
			hid->keyboard.held[Key::Code::Control] = true;
		}
		else if (code == Key::Code::LeftShift || code == Key::Code::RightShift)
		{
			hid->keyboard.pressed[Key::Code::Shift] = true;
			hid->keyboard.held[Key::Code::Shift] = true;
		}
		else if (code == Key::Code::LeftAlt || code == Key::Code::RightAlt)
		{
			hid->keyboard.pressed[Key::Code::Alt] = true;
			hid->keyboard.held[Key::Code::Alt] = true;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		Key::Code code = Key::FromGLFW(key);
		hid->keyboard.released[code] = true;

		// Special case buttons:
		if (code == Key::Code::LeftControl || code == Key::Code::RightControl)
		{
			hid->keyboard.released[Key::Code::Control] = true;
		}
		else if (code == Key::Code::LeftShift || code == Key::Code::RightShift)
		{
			hid->keyboard.released[Key::Code::Shift] = true;
		}
		else if (code == Key::Code::LeftAlt || code == Key::Code::RightAlt)
		{
			hid->keyboard.released[Key::Code::Alt] = true;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::HandleMousePressEvent(int32 button, int32 action, int32 mods)
{
	assert(button < Mouse::Button::NumMouseButtons);
	if (action == GLFW_PRESS)
	{
		hid->mouse.pressed[button] = true;
		hid->mouse.held[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		hid->mouse.released[button] = true;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::HandleMouseMoveEvent(double x, double y)
{
	hid->mouse.position = glm::vec2(x, y);
	hid->mouse.delta = hid->mouse.position - hid->mouse.previousPosition;
}

//------------------------------------------------------------------------------
/**
*/
Keyboard*
GetDefaultKeyboard()
{
	assert(hid != nullptr);
	return &hid->keyboard;
}

//------------------------------------------------------------------------------
/**
*/
Mouse*
GetDefaultMouse()
{
	assert(hid != nullptr);
	return &hid->mouse;
}

//------------------------------------------------------------------------------
/**
*/
Gamepad*
GetGamepad(int id)
{
	assert(hid != nullptr);
	assert(id >= 0 && id < hid->gamepads.size());
	return hid->gamepads[id];
}

void
AddGamepad(int id)
{
	assert(hid != nullptr);
	assert(id >= 0 && id < hid->gamepads.size());
	Gamepad* gamepad = new Gamepad();
	gamepad->id = id;
	hid->gamepads[id] = gamepad;
}

void
RemoveGamepad(int id)
{
	assert(hid != nullptr);
	assert(id >= 0 && id < hid->gamepads.size());
	delete hid->gamepads[id];
}

void
InputHandler::HandleJoystickEvent(int32 jid, int32 event)
{
	if (event == GLFW_CONNECTED)
	{
		AddGamepad(jid);
		printf("[Input] Gamepad added!\n");
	}
	else if (event == GLFW_DISCONNECTED)
	{
		RemoveGamepad(jid);
		printf("[Input] Gamepad removed!\n");
	}
}

} // namespace Input
