#include "ProjectComponents.h"
#include "GameObject.h"
#include "Services.h"
#include <iostream>

dae::CubeComponent::CubeComponent(GameObject* pGameObject)
	: BaseComponent{ pGameObject }
	, m_CubeType{ CubeType::Normal }
	, m_CubeState{ CubeState::Normal }
	, m_pFirstTexture{ nullptr }
	, m_pSecondTexture{ nullptr }
	, m_pThirdTexture{ nullptr }
	, m_IsBottomEdgeCube{ false }
	, m_IsRightEdgeCube{ false }
	, m_IsLeftEdgeCube{ false }
{
	if (!pGameObject->IsComponentAdded<TextureComponent>())
		m_pTextureComponent = pGameObject->AddComponent<TextureComponent>(pGameObject);
	else
		m_pTextureComponent = pGameObject->GetComponent<TextureComponent>();
}

void dae::CubeComponent::SetStartTexture(int textureNr)
{
	switch (textureNr)
	{
		case 1:
			m_pStartTexture = m_pFirstTexture;
		break;
		case 2:
			m_pStartTexture = m_pSecondTexture;
		break;
		case 3:
			m_pStartTexture = m_pThirdTexture;
		break;
	}
}

void dae::CubeComponent::SetIntermediateTexture(int textureNr)
{
	switch (textureNr)
	{
	case 1:
		m_pIntermediateTexture = m_pFirstTexture;
		break;
	case 2:
		m_pIntermediateTexture = m_pSecondTexture;
		break;
	case 3:
		m_pIntermediateTexture = m_pThirdTexture;
		break;
	}
}

void dae::CubeComponent::SetGoalTexture(int textureNr)
{
	switch (textureNr)
	{
	case 1:
		m_pGoalTexture = m_pFirstTexture;
		break;
	case 2:
		m_pGoalTexture = m_pSecondTexture;
		break;
	case 3:
		m_pGoalTexture = m_pThirdTexture;
		break;
	}
}

void dae::CubeComponent::Hit(GameObject* pGameObject)
{
	switch (m_CubeState)
	{
		case CubeState::Normal:
		{
			if (pGameObject->IsComponentAdded<PlayerComponent>())
			{
				if (m_CubeType == CubeType::Normal || m_CubeType == CubeType::SwitchBack)
				{
					m_pTextureComponent->SetTexture(m_pGoalTexture);
					m_CubeState = CubeState::Completed;

					GetOwner()->GetSubject()->NotifyObservers(*GetOwner(), nullptr);
				}
				else
				{
					m_pTextureComponent->SetTexture(m_pIntermediateTexture);
					m_CubeState = CubeState::Intermediate;
				}
			}
		}
		break;
		case CubeState::Intermediate:
		{
			//change color immediately if hit by player. Intermediate state can only be achieved when the cube is that type so no need to check
			if (pGameObject->IsComponentAdded<PlayerComponent>())
			{
				m_pTextureComponent->SetTexture(m_pGoalTexture);
				m_CubeState = CubeState::Completed;

				GetOwner()->GetSubject()->NotifyObservers(*GetOwner(), nullptr);
			}
		}
		break;
		case CubeState::Completed:
		{
			if (m_CubeType == CubeType::Normal || m_CubeType == CubeType::Intermediate)
				return;
			//switchback
			if (pGameObject->IsComponentAdded<PlayerComponent>())
			{
				m_pTextureComponent->SetTexture(m_pStartTexture);
				m_CubeState = CubeState::Normal;

				GetOwner()->GetSubject()->NotifyObservers(*GetOwner(), nullptr);
			}
		}
		break;
	}
}

dae::MovementComponent::MovementComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_Velocity{ glm::vec3{0.f, 0.f, 0.f} }
	, m_pCubeComponentLanded { nullptr }
	, m_IsFallingToDeath{ false }
	, m_DontTriggerLandHit{ true }
	, m_JumpRightUp{ "Q-Bert/Q-BertRightUpJump.png" }
	, m_JumpLeftUp{ "Q-Bert/Q-BertLeftUpJump.png" }
	, m_JumpRightDown{ "Q-Bert/Q-BertRightDownJump.png" }
	, m_JumpLeftDown{ "Q-Bert/Q-BertLeftDownJump.png" }
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

	if (m_pCubeComponentLanded != pCubeComponent)
	{
		m_pCubeComponentLanded = pCubeComponent;
		if (m_DontTriggerLandHit)
			return;
		pCubeComponent->Hit(GetOwner());

		dae::SoundSystem* pSS = dae::ServiceLocator::GetSoundSystem();
		pSS->Play(1);
	}

	//Add logic to change the texture into the correct standing texture
}

void dae::MovementComponent::JumpRightUp()
{
	if (m_Velocity.y == 0.f)
	{
		m_DontTriggerLandHit = false;

		m_Velocity += glm::vec3{ 40.f, -340.f, 0.f };

		m_pTextureComponent->SetTexture(m_JumpRightUp);

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
		m_DontTriggerLandHit = false;

		m_Velocity += glm::vec3{ -40.f, -340.f, 0.f };

		m_pTextureComponent->SetTexture(m_JumpLeftUp);

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
		m_DontTriggerLandHit = false;

		m_Velocity += glm::vec3{ 40.f, -230.f, 0.f };

		m_pTextureComponent->SetTexture(m_JumpRightDown);

		if (m_pCubeComponentLanded->IsBottomEdgeCube())
			FallOff();
	}
}

void dae::MovementComponent::JumpLeftDown()
{
	if (m_Velocity.y == 0.f)
	{
		m_DontTriggerLandHit = false;

		m_Velocity += glm::vec3{ -40.f, -230.f, 0.f };

		m_pTextureComponent->SetTexture(m_JumpLeftDown);

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
