#include "SceneManager.h"
#include "Scene.h"

#include <iostream>

void dae::SceneManager::Update(const float deltaT)
{
	m_pActiveScene->Update(deltaT);
}

void dae::SceneManager::LateUpdate(const float deltaT)
{
	m_pActiveScene->LateUpdate(deltaT);
}

void dae::SceneManager::Render()
{
	m_pActiveScene->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name, bool setActive)
{
	if (m_scenes.contains(name))
	{
		std::cout << "scene already exists\n";
		return *m_scenes[name].get();
	}

	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes[name] = scene;

	if (setActive)
		m_pActiveScene = scene.get();

	return *scene;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	if (m_scenes.contains(name))
		m_pActiveScene = m_scenes[name].get();
	else
		std::cout << "That scene does not exist\n";
}
