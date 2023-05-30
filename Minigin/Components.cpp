#include "Components.h"
#include "GameObject.h"
#include "Renderer.h"

#include "Font.h"
#include "Texture2D.h"
#include "XInputController.h"

#include "iostream"

void dae::RenderComponent::Render(float x, float y)
{
	Renderer::GetInstance().RenderTexture(*m_pTexture.get(), x, y);
}

dae::TextureComponent::TextureComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
{
	if (!pGameObject->IsComponentAdded<RenderComponent>())
		m_pRenderComponent = pGameObject->AddComponent<RenderComponent>(pGameObject);
	else
		m_pRenderComponent = pGameObject->GetComponent<RenderComponent>();
}

void dae::TextureComponent::SetTexture(const std::string& fileName)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName);
	
	if (m_pRenderComponent)
	{
		m_pRenderComponent->SetTexture(m_pTexture);
	}
	else
		std::cout << "RenderComponent is nullptr\n";
}

dae::TextComponent::TextComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_NeedsUpdate(true)
	, m_Text(" ") //empty start string will cause error
	, m_Color({ 255, 255, 255 }) //default white
	, m_pFont(dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36))
	, m_pTextTexture(nullptr)
{
	if (!pGameObject->IsComponentAdded<RenderComponent>())
		m_pRenderComponent = pGameObject->AddComponent<RenderComponent>(pGameObject);
	else
		m_pRenderComponent = pGameObject->GetComponent<RenderComponent>();
}

void dae::TextComponent::Update(const float)
{
	if (m_NeedsUpdate)
	{
		const auto surface = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		if (surface == nullptr)
		{
			std::cout << "Could not render text\n";
			return;
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface);
		if (texture == nullptr)
		{
			std::cout << "Could not create texture from surface";
			return;
		}
		
		SDL_FreeSurface(surface);
		m_pTextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;

		if (m_pRenderComponent)
		{
			m_pRenderComponent->SetTexture(m_pTextTexture);
		}
		else
			std::cout << "RenderComponent is nullptr\n";
	}
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
	m_Color.r = r;
	m_Color.g = g;
	m_Color.b = b;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetSize(int size)
{
	m_pFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", size);
	m_NeedsUpdate = true;
}

dae::FPSComponent::FPSComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_NrOfFrames{ 0 }
	, m_TimePassed{ 0 }
	, m_FPS{ 0 }
{
	if (!pGameObject->IsComponentAdded<TextComponent>())
		m_pTextComponent = pGameObject->AddComponent<TextComponent>(pGameObject);
	else
		m_pTextComponent = pGameObject->GetComponent<TextComponent>();
}

void dae::FPSComponent::Update(const float deltaT)
{
	++m_NrOfFrames;
	m_TimePassed += deltaT;

	if (m_NrOfFrames >= 10 && m_TimePassed >= 0.05f)
	{
		m_FPS = int(m_NrOfFrames / (m_TimePassed));

		m_NrOfFrames = 0;
		m_TimePassed = 0;

		//update textComp
		if (m_pTextComponent)
		{
			m_pTextComponent->SetText(std::to_string(m_FPS) + " FPS");
		}
	}
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& localPos)
{
	m_LocalPosition = localPos;
	m_PositionIsDirty = true;

	GameObject* pParent = GetOwner()->GetParent();
	if (pParent)
		pParent->SetChildrenPosDirty();
}

void dae::TransformComponent::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;

	m_PositionIsDirty = true;

	int nrOfChildren = GetOwner()->GetNrOfChildren();
	if (nrOfChildren > 0)
		GetOwner()->SetChildrenPosDirty();
}

void dae::TransformComponent::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		auto pOwnerParent = GetOwner()->GetParent();
		if (pOwnerParent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = pOwnerParent->GetWorldPosition() + m_LocalPosition;
	}
	m_PositionIsDirty = false;
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

dae::CirclingComponent::CirclingComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_Angle(0)
{
	if (!pGameObject->IsComponentAdded<TransformComponent>())
		m_pTransformComponent = pGameObject->AddComponent<TransformComponent>(pGameObject);
	else
		m_pTransformComponent = pGameObject->GetComponent<TransformComponent>();
}

void dae::CirclingComponent::Update(const float deltaT)
{
	m_Angle += m_RotationSpeed * deltaT;

	float x = m_Radius * cos(m_Angle);
	float y = m_Radius * sin(m_Angle);

	m_pTransformComponent->SetLocalPosition(x, y, 0.f);
}

dae::ControllerComponent::ControllerComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_pController{ std::make_unique<XInputController>(this) }
{
}

void dae::ControllerComponent::Update(const float)
{
	m_pController->Update();
}

dae::MovementComponent::MovementComponent(GameObject* pGameObject)
	: BaseComponent(pGameObject)
	, m_Speed{ 50.f }
	, m_Direction{ glm::vec3{0.f, 0.f, 0.f} }
{
	if (!pGameObject->IsComponentAdded<TransformComponent>())
		m_pTransformComponent = pGameObject->AddComponent<TransformComponent>(pGameObject);
	else
		m_pTransformComponent = pGameObject->GetComponent<TransformComponent>();
}

void dae::MovementComponent::Update(const float deltaT)
{
	glm::vec3 direction = m_Direction;
	const float length = glm::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0)
	{
		direction.x /= length;
		direction.y /= length;
	}

	if (m_pTransformComponent)
	{
		const glm::vec3& newPos = m_pTransformComponent->GetLocalPosition() + direction * m_Speed * deltaT;
		m_pTransformComponent->SetLocalPosition(newPos);
	}
	else
		std::cout << "TransformComponent is nullptr\n";
}

void dae::MovementComponent::AddDirection(const glm::vec3& direction)
{
	m_Direction += direction;
}

void dae::MovementComponent::SetDirection(const glm::vec3& direction)
{
	m_Direction = direction;
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

void dae::UIComponent::Notify(const GameObject& gameObject, GameEvents event)
{
	gameObject;
	event;

	std::cout << "\nUI component notified\n";
}
