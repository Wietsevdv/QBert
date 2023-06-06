#include "Components.h"
#include "GameObject.h"
#include "Renderer.h"

#include "Font.h"
#include "Texture2D.h"
#include "XInputController.h"
#include "Scene.h"

#include "iostream"

void dae::TransformComponent::SetLocalPosition(const glm::vec3& localPos)
{
	SetLocalPosition(localPos.x, localPos.y, localPos.z);
}

void dae::TransformComponent::SetLocalPosition(float x, float y, float z)
{
	CollisionComponent* pCollisionComp = dynamic_cast<CollisionComponent*>(m_pCollisionComponent);
	if (pCollisionComp)
	{
		const glm::vec3 oldPos = m_LocalPosition;
		m_LocalPosition = { x, y, z };

		const glm::vec3 displacement = m_LocalPosition - oldPos;
		const glm::vec2 displacement2D = { displacement.x, displacement.y };
		pCollisionComp->AddDisplacement(displacement2D);
	}
	else
	{
		m_LocalPosition = { x, y, z };
	}

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

dae::RenderComponent::RenderComponent(GameObject* pGameObject)
	: BaseComponent{ pGameObject }
	, m_Layer{ 1 }
{
}

void dae::RenderComponent::Render(float x, float y)
{
	glm::ivec2 textureSize = m_pTexture->GetSize();
	float renderPosX = x - textureSize.x / 2.f;
	float renderPosY = y - textureSize.y;
	Renderer::GetInstance().RenderTexture(*m_pTexture.get(), renderPosX, renderPosY);
}

void dae::RenderComponent::SetLayer(int newLayer)
{
	m_Layer = newLayer;
	SceneManager::GetInstance().GetActiveScene()->LayerGameObjects();
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

std::vector<dae::CollisionComponent*> dae::CollisionComponent::m_pAllCollisionComponents;

dae::CollisionComponent::CollisionComponent(GameObject* pGameObject)
	: BaseComponent{ pGameObject }
	, m_pCallbackFunction{ nullptr }
{
	m_pAllCollisionComponents.emplace_back(this);

	if (!pGameObject->IsComponentAdded<TransformComponent>())
		m_pTransformComponent = pGameObject->AddComponent<TransformComponent>(pGameObject);
	else
		m_pTransformComponent = pGameObject->GetComponent<TransformComponent>();

	m_pTransformComponent->SetCollisionComponent(this);

	const glm::vec3& worldPos = m_pTransformComponent->GetWorldPosition();
	const glm::vec2 worldPos2D = { worldPos.x, worldPos.y };

	m_Points[0] = worldPos2D + glm::vec2{ -10.f, 0.f };
	m_Points[1] = worldPos2D + glm::vec2{ 10.f, 0.f };
	m_Points[2] = worldPos2D + glm::vec2{ -10.f, -10.f };
	m_Points[3] = worldPos2D + glm::vec2{ 10.f, -10.f };
}

void dae::CollisionComponent::Update(const float)
{
	//write here the logic for where you check if you're overlapping other collision components by calling IsOverlapping(...) on every other collision component
	if (m_pCallbackFunction)
	{
		GameObject* pOwner = GetOwner();
		glm::vec2& bottomLeft = m_Points[0];
		glm::vec2& bottomRight = m_Points[1];
		glm::vec2& topLeft = m_Points[2];
		glm::vec2& topRight = m_Points[3];

		for (auto pCollisionComp : m_pAllCollisionComponents)
		{
			if (pOwner == pCollisionComp->GetOwner()) //skip yourself
				continue;

			if (pCollisionComp->IsOverlapping(bottomLeft, bottomRight, topLeft, topRight))
				m_pCallbackFunction(pCollisionComp->GetOwner());
		}
	}
}

void dae::CollisionComponent::SetPoints(glm::vec2& leftBottom, glm::vec2& rightBottom, glm::vec2& leftTop, glm::vec2& rightTop, bool areWorldPositions)
{
	if (areWorldPositions)
	{
		m_Points[0] = leftBottom;
		m_Points[1] = rightBottom;
		m_Points[2] = leftTop;
		m_Points[3] = rightTop;
	}
	else
	{
		const glm::vec3& worldPos = m_pTransformComponent->GetWorldPosition();
		const glm::vec2 worldPos2D = { worldPos.x, worldPos.y };

		m_Points[0] = worldPos2D + leftBottom;
		m_Points[1] = worldPos2D + rightBottom;
		m_Points[2] = worldPos2D + leftTop;
		m_Points[3] = worldPos2D + rightTop;
	}
}

void dae::CollisionComponent::AddDisplacement(const glm::vec2& displacement)
{
	for (int i{}; i < 4; ++i)
	{
		m_Points[i] += displacement;
	}
}

bool dae::CollisionComponent::IsOverlapping(const glm::vec2& leftBottom, const glm::vec2& rightBottom, const glm::vec2&, const glm::vec2& rightTop) const
{
	if (rightBottom.x < m_Points[0].x || leftBottom.x > m_Points[1].x) // projections of boxes on x-axis do not overlap
		return false;

	//opposite comparison because y-axis points downwards
	if (rightTop.y > m_Points[0].y || rightBottom.y < m_Points[3].y) // projections of boxes on y-axis do not overlap
		return false;

	//projections on both axis overlap. This means the boxes overlap in the level.
	return true;
}
