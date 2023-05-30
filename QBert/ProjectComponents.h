#pragma once
#include "Components.h"

namespace dae
{
	class ProjectComponent final : public BaseComponent
	{
	public:
		ProjectComponent(GameObject* pGameObject) : BaseComponent(pGameObject) {};

		virtual ~ProjectComponent() {};
		ProjectComponent(const ProjectComponent& other) = delete;
		ProjectComponent(ProjectComponent&& other) = delete;
		ProjectComponent& operator=(const ProjectComponent& other) = delete;
		ProjectComponent& operator=(ProjectComponent&& other) = delete;

		virtual void Update(const float) override;
		virtual void LateUpdate() override {};
	};

}