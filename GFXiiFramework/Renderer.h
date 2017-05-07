#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "OpenGL/OGLTexture.h"
#include "OpenGL/OGLShader.h"
#include "Mesh.h"
#include "UniformSetter.h"

class Renderer
{
private:
	std::vector<std::shared_ptr<OGLTexture>> m_textures;
	std::shared_ptr<OGLShaderProgram> m_shaderProgram;
	std::shared_ptr<Mesh> m_mesh;
	std::function<void(std::shared_ptr<OGLShaderProgram>)> m_uniformSetter;

public:
	Renderer(std::vector<std::shared_ptr<OGLTexture>> textures, 
			 std::shared_ptr<OGLShaderProgram> shaderProgram,
			 std::shared_ptr<Mesh> mesh,
			 std::function<void(std::shared_ptr<OGLShaderProgram>)> uniformSetter/*std::shared_ptr<UniformSetter> uniformSetter*/);
	virtual ~Renderer();

	void Render();
};

