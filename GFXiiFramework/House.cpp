#include "House.h"
#include "GLEW\include\glew.h"
#include "ResourceManager.h"
#include "Farm.h"

using namespace std;
using namespace glm;

House::House(vec3 position, Farm* farm):
Actor()
{
	m_position = position;
	m_floorOffset = vec3(0, 0.7, 0);
	m_farm = farm;

	auto resourceManager = ResourceManager::GetInstance();

	std::vector<std::shared_ptr<OGLTexture>> textures;
	textures.push_back(resourceManager->GetTexture("house"));
	textures.push_back(resourceManager->GetTexture("houseNormal"));
	textures.push_back(resourceManager->GetTexture("houseSpecular"));

	m_renderer = make_unique<Renderer>(textures, resourceManager->GetShader("house"), resourceManager->GetMesh("house"), [this](std::shared_ptr<OGLShaderProgram> shader)
	{
		auto uniform_model = glGetUniformLocation(shader->GetProgramHandle(), "model");
		auto uniform_view = glGetUniformLocation(shader->GetProgramHandle(), "view");
		auto uniform_projection = glGetUniformLocation(shader->GetProgramHandle(), "projection");
		auto uniform_texture = glGetUniformLocation(shader->GetProgramHandle(), "texColour");
		auto uniform_normalMap = glGetUniformLocation(shader->GetProgramHandle(), "texNormal"); 
		auto uniform_eyePosition = glGetUniformLocation(shader->GetProgramHandle(), "eyePosition");
		auto uniform_specularMap = glGetUniformLocation(shader->GetProgramHandle(), "texSpecular");

		glUniform1i(uniform_texture, 0);
		glUniform1i(uniform_normalMap, 1);
		glUniform1i(uniform_specularMap, 2);
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)value_ptr((*(Scene::GetCamera()->GetViewMatrix()))));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, (float*)value_ptr(m_transform));
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetProjectionMatrix())));
		auto eyePosition = Scene::GetCamera()->GetCameraPosition();
		glUniform3f(uniform_eyePosition, eyePosition.x, eyePosition.y, eyePosition.z);
	});
}

House::~House()
{

}

void House::Update(float dt)
{
	m_transform = m_farm->GetTransformation() * translate(mat4(), m_position + m_floorOffset);
}

void House::Render()
{
	m_renderer->Render();
}