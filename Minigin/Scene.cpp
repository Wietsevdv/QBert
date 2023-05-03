#include "Scene.h"
#include "GameObject.h"
#include "Observers.h"

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

void dae::Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
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
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

