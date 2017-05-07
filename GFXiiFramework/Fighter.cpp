#include "Fighter.h"

#include "ResourceManager.h"
#include "Scene.h"
#include "GLEW\include\glew.h"

using namespace std;
using namespace glm;

Fighter::Fighter():
Actor()
{
	m_rootScale = vec3(0.2f, 0.2f, 0.2f);
	m_rootTranslation = vec3(0, 150, 0);
	m_rootRotation = vec3(0, radians(-90.0f), 0);

	m_translation = translate(mat4(), m_rootTranslation);
	//m_rotation = mat4_cast(quat(m_rootRotation));
	m_scale = scale(mat4(), m_rootScale);

	m_movement = Movement{ 0 };
	velocity = 50;
	rotSpeed = 90;

	auto resourceManager = ResourceManager::GetInstance();
	vector<shared_ptr<OGLTexture>> textures;
	textures.push_back(resourceManager->GetTexture("house"));
	m_renderer = make_unique<Renderer>(textures, resourceManager->GetShader("basic"), resourceManager->GetMesh("plane"), [this](std::shared_ptr<OGLShaderProgram> shader)
	{
		auto uniform_model = glGetUniformLocation(shader->GetProgramHandle(), "model");
		auto uniform_view = glGetUniformLocation(shader->GetProgramHandle(), "view");
		auto uniform_projection = glGetUniformLocation(shader->GetProgramHandle(), "projection");
		auto uniform_texture = glGetUniformLocation(shader->GetProgramHandle(), "texColour");

		glUniform1i(uniform_texture, 0);
		glUniformMatrix4fv(uniform_view, 1, GL_FALSE, (float*)value_ptr((*(Scene::GetCamera()->GetViewMatrix()))));
		glUniformMatrix4fv(uniform_model, 1, GL_FALSE, (float*)value_ptr(m_transform));
		glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, (float*)value_ptr(*(Scene::GetCamera()->GetProjectionMatrix())));
	});
}


Fighter::~Fighter()
{

}

void Fighter::Update(float dt)
{
	vec3 tick_translation;
	vec3 tick_rotation;

	if (m_movement.goingForward)
	{
		tick_translation.z -= (velocity * dt);
	}

	if (m_movement.goingBackward)
	{
		tick_translation.z += (velocity * dt);
	}

	if (m_movement.goingLeft)
	{
		tick_translation.x -= (velocity * dt);
	}

	if (m_movement.goingRight)
	{
		tick_translation.x += (velocity * dt);
	}

	if (m_movement.goingDown)
	{
		tick_translation.y -= (velocity * dt);
	}

	if (m_movement.goingUp)
	{
		tick_translation.y += (velocity * dt);
	}

	if (m_movement.rollingLeft)
	{
		tick_rotation.z += rotSpeed * dt;
	}

	if (m_movement.rollingRight)
	{
		tick_rotation.z -= rotSpeed * dt;
	}

	if (m_movement.yawingLeft)
	{
		tick_rotation.y += rotSpeed * dt;
	}

	if (m_movement.yawingRight)
	{
		tick_rotation.y -= rotSpeed * dt;
	}

	if (m_movement.pitchingDown)
	{
		tick_rotation.x += rotSpeed * dt;
	}

	if (m_movement.pitchingUp)
	{
		tick_rotation.x -= rotSpeed * dt;
	}

	auto camera = Scene::GetCamera();
	vec3 cameraOffset(0, 7, 15);

	m_rotation = m_rotation * mat4_cast(quat(radians(tick_rotation)));

	tick_translation = vec3(m_rotation * vec4(tick_translation, 1));
	m_translation = translate(m_translation, tick_translation );

	m_transform = m_translation * (m_rotation * mat4_cast(quat(m_rootRotation))) * m_scale;

	m_position = vec3(column(m_translation, 3));

	camera->SetPosition(m_position + vec3(m_rotation * vec4(cameraOffset, 0)));
	camera->LookAtPoint(m_position);

	camera->UpdateViewMatrix();
}

void Fighter::Render()
{
	m_renderer->Render();
}

void Fighter::GoingForward(bool value)
{
	m_movement.goingForward = value;
}

void Fighter::GoingBackward(bool value)
{
	m_movement.goingBackward = value;
}

void Fighter::GoingLeft(bool value)
{
	m_movement.goingLeft = value;
}

void Fighter::GoingRight(bool value)
{
	m_movement.goingRight = value;
}

void Fighter::GoingUp(bool value)
{
	m_movement.goingUp = value;
}

void Fighter::GoingDown(bool value)
{
	m_movement.goingDown = value;
}

void Fighter::PitchingDown(bool value)
{
	m_movement.pitchingDown = value;
}

void Fighter::PitchingUp(bool value)
{
	m_movement.pitchingUp = value;
}

void Fighter::YawingLeft(bool value)
{
	m_movement.yawingLeft = value;
}

void Fighter::YawingRight(bool value)
{
	m_movement.yawingRight = value;
}

void Fighter::RollingLeft(bool value)
{
	m_movement.rollingLeft = value;
}
void Fighter::RollingRight(bool value)
{
	m_movement.rollingRight = value;
}