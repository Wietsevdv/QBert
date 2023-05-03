#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Subject.h"

dae::GameObject::GameObject()
{
	m_pTransformComponent = AddComponent<dae::TransformComponent>(this);
	m_pRenderComponent = AddComponent<dae::RenderComponent>(this);
}

dae::GameObject::~GameObject()
{
	m_pComponents.clear();
};

void dae::GameObject::Update(const float deltaT)
{
	for (auto& pComponent : m_pComponents)
	{
		pComponent->Update(deltaT);
	}
}

void dae::GameObject::LateUpdate()
{
	for (auto pComponent : m_pComponents)
	{
		pComponent->LateUpdate();
	}

	//Remove and delete dead components
	for (auto pComponent : m_pComponents)
	{
		if (pComponent->GetIsDead())
		{
			m_pComponents.remove(pComponent);
		}
	}
}

void dae::GameObject::Render() const
{	
	if (m_pRenderComponent)
	{
		auto worldPosition = m_pTransformComponent->GetWorldPosition();
		m_pRenderComponent->Render(worldPosition.x, worldPosition.y);
	}
}

const glm::vec3 dae::GameObject::GetWorldPosition() const
{
	if (m_pParent)
		return m_pTransformComponent->GetWorldPosition();

	return m_pTransformComponent->GetLocalPosition();
}

void dae::GameObject::SetLocalPosition(const glm::vec3 localPos)
{
	SetLocalPosition(localPos.x, localPos.y, localPos.z);
}

void dae::GameObject::SetLocalPosition(float x, float y, float z)
{
	m_pTransformComponent->SetLocalPosition(x, y, z);
}

void dae::GameObject::SetParent(GameObject* pParent)
{
	if (pParent == nullptr)
		SetLocalPosition(GetWorldPosition());
	else
	{
		SetLocalPosition(GetLocalPosition() - pParent->GetWorldPosition());
		SetPositionDirty();
	}

	if (m_pParent)
		m_pParent->RemoveChild(this);
	m_pParent = pParent;
	if (m_pParent)
		m_pParent->AddChild(this);
}

void dae::GameObject::SetChildrenPosDirty()
{
	for (auto pChild : m_pChildren)
	{
		pChild->SetPositionDirty();
	}
}

dae::Subject* dae::GameObject::MakeSubject()
{
	m_pSubject = std::make_unique<Subject>();
	return m_pSubject.get();
}
