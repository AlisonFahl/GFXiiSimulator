#include "Scene.h"

using namespace std;

std::shared_ptr<Camera> Scene::s_camera;

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Update(float dt)
{
	for (auto actor : m_actors)
	{
		actor->Update(dt);
	}
}

void Scene::Render()
{
	for (auto actor : m_actors)
	{
		actor->Render();
	}
}
