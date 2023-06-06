#pragma once
#include <map>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name, bool setActive = true);

		void Update(const float deltaT);
		void LateUpdate(const float deltaT);
		void Render();

		Scene* GetActiveScene() const { return m_pActiveScene; }

		void SetActiveScene(const std::string& name);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::map<const std::string, std::shared_ptr<Scene>> m_scenes;
		Scene* m_pActiveScene{ nullptr };
	};
}
