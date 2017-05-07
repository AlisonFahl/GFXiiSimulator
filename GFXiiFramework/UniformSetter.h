#pragma once

#include "OpenGL/OGLShader.h"
#include <memory>

class UniformSetter
{
public:
	UniformSetter(){};
	virtual ~UniformSetter() {};

	virtual void SetUniforms(std::shared_ptr<OGLShaderProgram> shader) = 0;
};