#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Observer;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAllGameObjects();

		void Add(std::shared_ptr<Observer> observer);
		void Remove(std::shared_ptr<Observer> object);
		void RemoveAllObservers();

		void Update(const float deltaT);
		void LateUpdate();
		void Render() const;

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

		static unsigned int m_idCounter; 
	};

}
