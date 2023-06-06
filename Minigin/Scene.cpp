#include "Scene.h"
#include "GameObject.h"
#include "Observers.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) 
	: m_name(name) 
	, m_LayerGameObjects{ true }
{}

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

	//add functionality to only do this check every time a RenderComp changed its layer. Simple dirtyflag can be used
	if (m_LayerGameObjects)
	{
		m_LayeredGameObjects.clear();
		for (auto& go : m_objects)
		{
			m_LayeredGameObjects[go->GetLayer()].emplace_back(go.get());
		}

		m_LayerGameObjects = false;
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
	for (auto& goL : m_LayeredGameObjects)
	{
		for (auto& go : goL.second)
		{
			go->Render();
		}
	}
}