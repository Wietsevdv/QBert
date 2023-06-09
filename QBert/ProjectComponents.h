#pragma once
#include "Components.h"

namespace dae
{
	class CubeComponent final : public BaseComponent
	{
	public:
		enum class CubeType
		{
			Normal,
			Intermediate,
			SwitchBack
		};

		enum class CubeState
		{
			Normal,
			Intermediate,
			Completed
		};

		CubeComponent(GameObject* pGameObject);

		virtual ~CubeComponent() {};
		CubeComponent(const CubeComponent& other) = delete;
		CubeComponent(CubeComponent&& other) = delete;
		CubeComponent& operator=(const CubeComponent& other) = delete;
		CubeComponent& operator=(CubeComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

		CubeState GetCubeState() const { return m_CubeState; }
		void SetCubeType(CubeType type) { m_CubeType = type; }
		void SetNormalTexture(std::shared_ptr<Texture2D> pNormalTexture) { m_pNormalTexture = pNormalTexture; }
		void SetIntermediateTexture(std::shared_ptr<Texture2D> pIntermediateTexture) { m_pIntermediateTexture = pIntermediateTexture; }
		void SetGoalTexture(std::shared_ptr<Texture2D> pGoalTexture) { m_pGoalTexture = pGoalTexture; }

		void Hit(GameObject* pGameObject);

		void SetIsBottomEdgeCube() { m_IsBottomEdgeCube = true; }
		void SetIsRightEdgeCube() { m_IsRightEdgeCube = true; }
		void SetIsLeftEdgeCube() { m_IsLeftEdgeCube = true; }

		bool IsBottomEdgeCube() const { return m_IsBottomEdgeCube; }
		bool IsRightEdgeCube() const { return m_IsRightEdgeCube; }
		bool IsLeftEdgeCube() const { return m_IsLeftEdgeCube; }

	private:
		CubeType m_CubeType;
		CubeState m_CubeState;

		std::shared_ptr<Texture2D> m_pNormalTexture;
		std::shared_ptr<Texture2D> m_pIntermediateTexture;
		std::shared_ptr<Texture2D> m_pGoalTexture;

		bool m_IsBottomEdgeCube;
		bool m_IsRightEdgeCube;
		bool m_IsLeftEdgeCube;

		TextureComponent* m_pTextureComponent;
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
		void Land(CubeComponent* pCubeComponent);

		const glm::vec3& GetVelocity() const { return m_Velocity; }

		void JumpRightUp();
		void JumpLeftUp();
		void JumpRightDown();
		void JumpLeftDown();

		bool IsFallingToDeath() const { return m_IsFallingToDeath; }

	private:
		glm::vec3 m_Velocity;
		bool m_IsFallingToDeath;
		bool m_DontTriggerLandHit;

		void FallOff();

		//QUICK ACCESS (no ownership)
		TransformComponent* m_pTransformComponent;
		CubeComponent* m_pCubeComponentLanded;
		TextureComponent* m_pTextureComponent;
		RenderComponent* m_pRenderComponent;

		const std::string m_JumpRightUp;
		const std::string m_JumpLeftUp;
		const std::string m_JumpRightDown;
		const std::string m_JumpLeftDown;
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

	private:
		glm::vec3 m_Direction;

		bool m_On;

		//QUICK ACCESS (no ownership)
		MovementComponent* m_pMovementComponent;
	};

	class PlayerComponent final : public BaseComponent
	{
	public:
		PlayerComponent(GameObject* pGameObject);

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

	class PlayerCollisionComponent final : public CollisionComponent
	{
	public:
		PlayerCollisionComponent(GameObject* pGameObject);

		virtual ~PlayerCollisionComponent() {};
		PlayerCollisionComponent(const PlayerCollisionComponent& other) = delete;
		PlayerCollisionComponent(PlayerCollisionComponent&& other) = delete;
		PlayerCollisionComponent& operator=(const PlayerCollisionComponent& other) = delete;
		PlayerCollisionComponent& operator=(PlayerCollisionComponent&& other) = delete;

		//virtual void Update(const float deltaT) override { CollisionComponent::Update(deltaT); } //DOES THIS WORK WITHOUT OVERWRITING???
		//virtual void LateUpdate(const float) override {};

		void HandleOverlap(GameObject* pOtherObject);

	private:
		MovementComponent* m_pMovementComponent;
		GravityComponent* m_pGravityComponent;
		TransformComponent* m_pTransformComponent;
	};
}