#include "ProjectComponents.h"
#include "GameObject.h"
#include "Services.h"
#include <iostream>

dae::MovementComponent::MovementComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_Velocity{ glm::vec3{0.f, 0.f, 0.f} }
	, m_pCubeComponentLanded { nullptr }
	, m_IsFallingToDeath{ false }
{
	if (!pGameObject->IsComponentAdded<TransformComponent>())
		m_pTransformComponent = pGameObject->AddComponent<TransformComponent>(pGameObject);
	else
		m_pTransformComponent = pGameObject->GetComponent<TransformComponent>();

	if (!pGameObject->IsComponentAdded<TextureComponent>())
		m_pTextureComponent = pGameObject->AddComponent<TextureComponent>(pGameObject);
	else
		m_pTextureComponent = pGameObject->GetComponent<TextureComponent>();

	if (!pGameObject->IsComponentAdded<RenderComponent>())
		m_pRenderComponent = pGameObject->AddComponent<RenderComponent>(pGameObject);
	else
		m_pRenderComponent = pGameObject->GetComponent<RenderComponent>();
}

void dae::MovementComponent::LateUpdate(const float deltaT)
{
	if (m_pTransformComponent)
	{
		const glm::vec3 newPos = m_pTransformComponent->GetLocalPosition() + m_Velocity * deltaT;
		m_pTransformComponent->SetLocalPosition(newPos);
	}
	else
		std::cout << "MovementComponent: transformComponent is nullptr\n";
}

void dae::MovementComponent::Land(CubeComponent* pCubeComponent)
{
	m_Velocity = glm::vec3{ 0.f, 0.f, 0.f };

	if (m_pCubeComponentLanded == pCubeComponent)
		return;

	m_pCubeComponentLanded = pCubeComponent;

	dae::SoundSystem* pSS = dae::ServiceLocator::GetSoundSystem();
	pSS->Play(1);

	//Add logic to change the texture into the correct standing texture
}

void dae::MovementComponent::JumpRightUp()
{
	if (m_Velocity.y == 0.f)
	{
		//Maybe attach the gameObject as a child to the block he lands on. That way you can access the block through GetParent().
		//Remove as child after jumping ofcourse

		m_Velocity += glm::vec3{ 40.f, -340.f, 0.f };

		m_pTextureComponent->SetTexture("Q-BertRightUpJump.png");

		if (m_pCubeComponentLanded->IsRightEdgeCube())
		{
			FallOff();
			m_pRenderComponent->SetLayer(0);
		}
	}
}

void dae::MovementComponent::JumpLeftUp()
{
	if (m_Velocity.y == 0.f)
	{
		m_Velocity += glm::vec3{ -40.f, -340.f, 0.f };

		m_pTextureComponent->SetTexture("Q-BertLeftUpJump.png");

		if (m_pCubeComponentLanded->IsLeftEdgeCube())
		{
			FallOff();
			m_pRenderComponent->SetLayer(0);
		}

	}
}

void dae::MovementComponent::JumpRightDown()
{
	if (m_Velocity.y == 0.f)
	{
		m_Velocity += glm::vec3{ 40.f, -230.f, 0.f };

		m_pTextureComponent->SetTexture("Q-BertRightDownJump.png");

		if (m_pCubeComponentLanded->IsBottomEdgeCube())
			FallOff();
	}
}

void dae::MovementComponent::JumpLeftDown()
{
	if (m_Velocity.y == 0.f)
	{
		m_Velocity += glm::vec3{ -40.f, -230.f, 0.f };

		m_pTextureComponent->SetTexture("Q-BertLeftDownJump.png");

		if (m_pCubeComponentLanded->IsBottomEdgeCube())
			FallOff();
	}
}

void dae::MovementComponent::FallOff()
{
	m_IsFallingToDeath = true;

	dae::SoundSystem* pSS = dae::ServiceLocator::GetSoundSystem();
	pSS->Play(0);
}

dae::PlayerComponent::PlayerComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_NrOfLives{ 3 }
	, m_Score{ 0 }
{
}

void dae::PlayerComponent::LoseLifePoint()
{
	if (m_NrOfLives > 0)
	{
		--m_NrOfLives;

		GameObject* pOwner = GetOwner();
		pOwner->GetSubject()->NotifyObservers(*pOwner, GameEvents::PlayerDied);
	}
}

void dae::PlayerComponent::IncreaseScore(int amount)
{
	m_Score += amount;

	GameObject* pOwner = GetOwner();
	pOwner->GetSubject()->NotifyObservers(*pOwner, GameEvents::PlayerScoreIncreased);
}

dae::GravityComponent::GravityComponent(GameObject* pGameObject)
	: BaseComponent{ pGameObject }
	, m_Direction{ glm::vec3{0.f, 9.81f, 0.f} }
	, m_On{ true }
{
	if (!pGameObject->IsComponentAdded<MovementComponent>())
		m_pMovementComponent = pGameObject->AddComponent<MovementComponent>(pGameObject);
	else
		m_pMovementComponent = pGameObject->GetComponent<MovementComponent>();
}

void dae::GravityComponent::Update(const float)
{
	if (m_On)
	{
		const glm::vec3 gravity = m_Direction;
		m_pMovementComponent->Add(gravity);
	}
}

dae::PlayerCollisionComponent::PlayerCollisionComponent(GameObject* pGameObject)
	: CollisionComponent{ pGameObject }
{
	CollisionComponent::SetOverlapCallbackFunction(std::function<void(GameObject * pOtherObject)>{ std::bind(&PlayerCollisionComponent::HandleOverlap, this, std::placeholders::_1) }); //bind necessary for passing member functions

	if (!pGameObject->IsComponentAdded<MovementComponent>())
		m_pMovementComponent = pGameObject->AddComponent<MovementComponent>(pGameObject);
	else
		m_pMovementComponent = pGameObject->GetComponent<MovementComponent>();

	if (!pGameObject->IsComponentAdded<GravityComponent>())
		m_pGravityComponent = nullptr;
	else
		m_pGravityComponent = pGameObject->GetComponent<GravityComponent>();

	if (!pGameObject->IsComponentAdded<TransformComponent>())
		m_pTransformComponent = pGameObject->AddComponent<TransformComponent>(pGameObject);
	else
		m_pTransformComponent = pGameObject->GetComponent<TransformComponent>();
}

void dae::PlayerCollisionComponent::HandleOverlap(GameObject* pOtherObject)
{
	if (CubeComponent* pCubeComponent = pOtherObject->GetComponent<CubeComponent>())
	{
		const glm::vec3& velocity = m_pMovementComponent->GetVelocity();
		if (velocity.y > 0.f && !m_pMovementComponent->IsFallingToDeath())
		{
			m_pMovementComponent->Land(pCubeComponent);

			//fix Q*Bert to stand in the middle
			const glm::vec3& position = m_pTransformComponent->GetLocalPosition();
			glm::vec3 newPosition = { pOtherObject->GetComponent<TransformComponent>()->GetLocalPosition().x, position.y, 0.f };
			m_pTransformComponent->SetLocalPosition(newPosition);
		}
	}
	else
		std::cout << "That ain't a box\n";
}
