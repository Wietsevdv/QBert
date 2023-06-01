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
		virtual void LateUpdate(const float) override {};
	};

	class MovementComponent final : public BaseComponent
	{
	public:
		MovementComponent(GameObject* pGameObject);

		virtual ~MovementComponent() {};
		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float deltaT) override;

		//Don't multiply your direction with deltaT. This is done when adding the complete velocity to the gameObject in LateUpdate()
		void Add(const glm::vec3& direction) { m_Velocity += direction; }
		void Land();

	private:
		glm::vec3 m_Velocity;

		//QUICK ACCESS (no ownership)
		TransformComponent* m_pTransformComponent;
	};

	class PlayerComponent final : public BaseComponent
	{
	public:
		PlayerComponent(GameObject* pGameObject)
			: BaseComponent(pGameObject)
			, m_NrOfLives{ 3 }
			, m_Score{ 0 }
		{};

		virtual ~PlayerComponent() {};
		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

		int GetNrOfLives() const { return m_NrOfLives; }
		int GetScore() const { return m_Score; }

		void LoseLifePoint();
		void IncreaseScore(int amount);

	private:
		int m_NrOfLives;
		int m_Score;
	};

	class GravityComponent final : public BaseComponent
	{
	public:
		GravityComponent(GameObject* pGameObject);

		virtual ~GravityComponent() {};
		GravityComponent(const GravityComponent& other) = delete;
		GravityComponent(GravityComponent&& other) = delete;
		GravityComponent& operator=(const GravityComponent& other) = delete;
		GravityComponent& operator=(GravityComponent&& other) = delete;

		virtual void Update(const float) override;
		virtual void LateUpdate(const float) override {};

		void SetDirection(const glm::vec3& direction) { m_Direction = direction; }

		void SetGravityOn(bool gravityOn = true) { m_On = gravityOn; }

	private:
		glm::vec3 m_Direction;

		bool m_On;

		//QUICK ACCESS (no ownership)
		MovementComponent* m_pMovementComponent;
	};

	class PlayerCollisionComponent final : public CollisionComponent
	{
	public:
		PlayerCollisionComponent(GameObject* pGameObject);

		virtual ~PlayerCollisionComponent() {};
		PlayerCollisionComponent(const PlayerCollisionComponent& other) = delete;
		PlayerCollisionComponent(PlayerCollisionComponent&& other) = delete;
		PlayerCollisionComponent& operator=(const PlayerCollisionComponent& other) = delete;
		PlayerCollisionComponent& operator=(PlayerCollisionComponent&& other) = delete;

		virtual void Update(const float deltaT) override { CollisionComponent::Update(deltaT); } //DOES THIS WORK WITHOUT OVERWRITING???
		virtual void LateUpdate(const float) override {};

		void HandleOverlap(GameObject* pOtherObject);

	private:
		MovementComponent* m_pMovementComponent;
		GravityComponent* m_pGravityComponent;
	};

	class CubeComponent final : public BaseComponent
	{
	public:
		CubeComponent(GameObject* pGameObject)
			: BaseComponent{ pGameObject }
		{
		};

		virtual ~CubeComponent() {};
		CubeComponent(const CubeComponent& other) = delete;
		CubeComponent(CubeComponent&& other) = delete;
		CubeComponent& operator=(const CubeComponent& other) = delete;
		CubeComponent& operator=(CubeComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

	private:
	};
}