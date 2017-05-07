#pragma once

#include <memory>
#include <map>
#include <string>

#include "OpenGL\OGLTexture.h"
#include "OpenGL\OGLShader.h"
#include "Mesh.h"

class ResourceManager
{
private:
	static ResourceManager* m_singleton;

	std::map<std::string, std::shared_ptr<OGLTexture>> m_textures;
	std::map<std::string, std::shared_ptr<OGLShaderProgram>> m_shaders;
	std::map<std::string, std::shared_ptr<Mesh>> m_meshes;

	ResourceManager(){};
	~ResourceManager(){};

	void LoadTextures();
	void LoadShaders();
	void LoadMeshes();

public:
	

	static void Initialize();
	static ResourceManager* GetInstance();
	static void Destroy();

	std::shared_ptr<OGLTexture> GetTexture(std::string name);
	std::shared_ptr<OGLShaderProgram> GetShader(std::string name);
	std::shared_ptr<Mesh> GetMesh(std::string name);
};

