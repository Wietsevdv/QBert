#include "ProjectComponents.h"
#include "GameObject.h"
#include <iostream>

void dae::ProjectComponent::Update(const float)
{
	static bool isPrinted = false;
	if (isPrinted == false)
	{
		isPrinted = true;
		std::cout << "ProjectComponent reached\n";
	}
}

dae::MovementComponent::MovementComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_Velocity{ glm::vec3{0.f, 0.f, 0.f} }
{
	if (!pGameObject->IsComponentAdded<TransformComponent>())
		m_pTransformComponent = pGameObject->AddComponent<TransformComponent>(pGameObject);
	else
		m_pTransformComponent = pGameObject->GetComponent<TransformComponent>();
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

void dae::MovementComponent::Land()
{
	m_Velocity = glm::vec3{ 0.f, 0.f, 0.f };
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
};

void dae::PlayerCollisionComponent::HandleOverlap(GameObject* pOtherObject)
{
	if (pOtherObject->IsComponentAdded<CubeComponent>())
	{
		m_pMovementComponent->Land();

		if (m_pGravityComponent)
			m_pGravityComponent->SetGravityOn(false);
	}
	else
		std::cout << "That ain't a box\n";
}
