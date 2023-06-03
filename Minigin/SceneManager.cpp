#include "SceneManager.h"
#include "Scene.h"

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
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);

	if (setActive)
		m_pActiveScene = scene.get();

	return *scene;
}
