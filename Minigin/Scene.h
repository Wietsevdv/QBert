#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Observer;
	class RenderComponent;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, bool setActive);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAllGameObjects();

		void Add(std::shared_ptr<Observer> observer);
		void Remove(std::shared_ptr<Observer> object);
		void RemoveAllObservers();

		void Update(const float deltaT);
		void LateUpdate(const float deltaT);
		void Render() const;

		void AddRenderComponent(RenderComponent* pRenderComponent);

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};
		std::vector <std::shared_ptr<Observer>> m_Observers{};

		std::vector<RenderComponent*> m_RenderComponents;

		static unsigned int m_idCounter; 
	};

}
