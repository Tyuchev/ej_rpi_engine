//------------------------------------------------------------------------------
//  @file gamepad.cc
//  @copyright (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "gamepad.h"
#include "GLFW/glfw3.h"
namespace Input
{

Gamepad::Code
Gamepad::FromGLFW(int32 key)
{
    // Not yet implemented.
    return Gamepad::Code::InvalidKey;
}

void
PrintAxes(glm::vec2 ls)
{
    printf("{x: %f, y: %f}\n", ls.x, ls.y);
}

void
Gamepad::Update()
{
    int count;
    const float* axes = glfwGetJoystickAxes(id, &count);
    if (count >= 2)
    {
        leftStickValues.x = axes[0];
        leftStickValues.y = axes[1];
    }
    //PrintAxes(leftStickValues);
}


} // namespace Input
