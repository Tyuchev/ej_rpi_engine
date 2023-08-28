#include "config.h"
#include <GL/glew.h>
#include "particlesystem.h"
#include "shaderresource.h"
#include <algorithm>

namespace Render
{
	void ParticleSystem::Initialize()
	{
        return;
        auto vs = Render::ShaderResource::LoadShader(Render::ShaderResource::ShaderType::VERTEXSHADER, "shd/vs_particles_bufstorage.vs");
        auto fs = Render::ShaderResource::LoadShader(Render::ShaderResource::ShaderType::FRAGMENTSHADER, "shd/fs_particles_bufstorage.fs");
        this->particleShaderId = Render::ShaderResource::CompileShaderProgram({ vs, fs });
        auto cs = Render::ShaderResource::LoadShader(Render::ShaderResource::ShaderType::COMPUTESHADER, "shd/cs_particle_sim_bufstorage.comp");
        this->particleSimComputeShaderId = Render::ShaderResource::CompileShaderProgram({ cs });
        glGenBuffers(1, &this->emitterBlockUBO);
	}
    
    ParticleEmitter::ParticleEmitter(uint32_t numParticles)
    {
        return;
        data.numParticles = numParticles;
        glGenBuffers(2, this->bufPositions);
        glGenBuffers(2, this->bufVelocities);
        glGenBuffers(2, this->bufColors);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufPositions[0]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, this->data.numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufPositions[1]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, this->data.numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufVelocities[0]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, this->data.numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufVelocities[1]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, this->data.numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufColors[0]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, this->data.numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->bufColors[1]);
        glBufferData(GL_SHADER_STORAGE_BUFFER, this->data.numParticles * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    }

    ParticleEmitter::~ParticleEmitter()
    {
        return;
        glDeleteBuffers(2, this->bufPositions);
        glDeleteBuffers(2, this->bufVelocities);
        glDeleteBuffers(2, this->bufColors);
    }
}