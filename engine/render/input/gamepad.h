#pragma once
//------------------------------------------------------------------------------
/**
    @file

    @copyright
    (C) 2022 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
namespace Input
{

struct Gamepad
{
    // Not yet implemented.
    enum Code
    {
        NumKeyCodes,
        InvalidKey
    };

    void Update();
    static Gamepad::Code FromGLFW(int32 key);
    int id;
    glm::vec2 leftStickValues;
};

} // namespace Input
