#include "Renderer.h"
#include "GLEW/include/glew.h"


Renderer::Renderer(std::vector<std::shared_ptr<OGLTexture>> textures,
				   std::shared_ptr<OGLShaderProgram> shaderProgram,
				   std::shared_ptr<Mesh> mesh,
				   std::function<void(std::shared_ptr<OGLShaderProgram>)> uniformSetter)
{
	m_textures = textures;
	m_shaderProgram = shaderProgram;
	m_mesh = mesh;
	m_uniformSetter = uniformSetter;
}


Renderer::~Renderer()
{

}

void Renderer::Render()
{
	int tex_index = 0;
	for (auto texture : m_textures)
	{
		glActiveTexture(GL_TEXTURE0 + tex_index);
		glBindTexture(GL_TEXTURE_2D, texture->m_syshandle);

		tex_index++;
	}

	m_shaderProgram->ActivateShaderProgram();
	m_uniformSetter(m_shaderProgram);

	m_mesh->BindAttributes();

	m_mesh->Draw();
}
