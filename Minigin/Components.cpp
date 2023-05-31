#include "Components.h"
#include "GameObject.h"
#include "Renderer.h"

#include "Font.h"
#include "Texture2D.h"
#include "XInputController.h"

#include "iostream"

void dae::RenderComponent::Render(float x, float y)
{
	glm::ivec2 textureSize = m_pTexture->GetSize();
	float renderPosX = x - textureSize.x / 2.f;
	float renderPosY = y - textureSize.y;
	Renderer::GetInstance().RenderTexture(*m_pTexture.get(), renderPosX, renderPosY);
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
	SetLocalPosition(localPos.x, localPos.y, localPos.z);
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

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

void dae::TransformComponent::UpdateWorldPosition()
{
	auto pOwnerParent = GetOwner()->GetParent();
	if (pOwnerParent == nullptr)
		m_WorldPosition = m_LocalPosition;
	else
		m_WorldPosition = pOwnerParent->GetWorldPosition() + m_LocalPosition;

	m_PositionIsDirty = false;
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

void dae::UIComponent::Notify(const GameObject& gameObject, GameEvents event)
{
	gameObject;
	event;

	std::cout << "\nUI component notified\n";
}
