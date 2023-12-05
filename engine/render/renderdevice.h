#pragma once
//------------------------------------------------------------------------------
/**
    RenderDevice

    (C) 2019 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "GL/glew.h"
#include <string>
#include <vector>
#include "render/window.h"
#include "resourceid.h"

namespace Render
{


class Grid;

typedef ResourceId MeshResourceId;

typedef ResourceId VertexShaderResourceId;
typedef ResourceId FragmentShaderResourceId;

typedef unsigned ImageId;
static const ImageId InvalidImageId = UINT_MAX;
typedef unsigned ModelId;

class RenderDevice
{
public:
    RenderDevice();
    static RenderDevice* Instance()
    {
        static RenderDevice instance;
        return &instance;
    }
public:
    RenderDevice(const RenderDevice&) = delete;
    void operator=(const RenderDevice&) = delete;

    static void Init();
    static void Draw(ModelId model, glm::mat4 localToWorld, glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f), bool isRoad = false);
    static void Render(Display::Window* wnd, float dt);
    static void SetSkybox(TextureResourceId tex);
    static void SetPlayerPos(const glm::vec3& pos);
    static void SetRoadScale(const float& scale);
    static void SetRoadTurnFactor(const float& factor);

private:
    GLuint forwardFrameBuffer;
    glm::vec3 playerPos;
    float roadScale = 1.0f;
    float roadTurnFactor = 0.0f;
    union RenderTargets
    {
        GLuint RT[2];
        struct
        {
            GLuint light;     // GL_RGBA32F linear HDR buffer
            GLuint normal;    // compressed screenspace normals, RG16F format
        };
    } renderTargets;
    GLuint depthStencilBuffer; // GL_DEPTH24_STENCIL8

    struct DrawCommand
    {
        ModelId modelId;
        glm::mat4 transform;
        glm::vec3 objectPos;
        bool isRoad;
    };

    std::vector<DrawCommand> drawCommands;

    void StaticShadowPass();
    void StaticGeometryPrepass();
    void StaticForwardPass();
    void SkyboxPass();
    void ParticlePass(float dt);
    void FinalizePass(Display::Window* wnd);

    unsigned int frameSizeW;
    unsigned int frameSizeH;
    TextureResourceId skybox = InvalidResourceId;
};

inline void RenderDevice::SetSkybox(TextureResourceId tex)
{
    Instance()->skybox = tex;
}

} // namespace Render
