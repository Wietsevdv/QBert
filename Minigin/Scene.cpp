#include "Scene.h"
#include "GameObject.h"
#include "Observers.h"

#include <map>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Add(std::shared_ptr<Observer> observer)
{
	m_Observers.emplace_back(std::move(observer));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::Remove(std::shared_ptr<Observer> observer)
{
	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void Scene::RemoveAllGameObjects()
{
	m_objects.clear();
}

void Scene::RemoveAllObservers()
{
	m_Observers.clear();
}

void Scene::Update(const float deltaT)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaT);
	}
}

void dae::Scene::LateUpdate(const float deltaT)
{
	for (auto& object : m_objects)
	{
		object->LateUpdate(deltaT);
	}

	//delete dead gameObjects
	for (auto& object : m_objects)
	{
		if (object->GetIsDead())
			Remove(object);
	}
}

void Scene::Render() const
{
	//Get all rendercomponents in order of their layer
	std::map<int, std::vector<RenderComponent*>> layeredComponents;
	layeredComponents[0].reserve(10);
	layeredComponents[1].reserve(50);
	layeredComponents[1].reserve(10);

	for (RenderComponent* rc : m_RenderComponents)
	{
		layeredComponents[rc->GetLayer()].push_back(rc);
	}

	//Render them
	for (auto layer : layeredComponents)
	{
		for (auto rc : layer.second)
		{
			rc->GetOwner()->Render(); //call render on owner because owner passes location data
		}
	}
}

void dae::Scene::AddRenderComponent(RenderComponent* pRenderComponent)
{
	m_RenderComponents.push_back(pRenderComponent);
}