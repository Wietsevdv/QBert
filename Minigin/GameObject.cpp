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

void dae::GameObject::LateUpdate(const float deltaT)
{
	for (auto pComponent : m_pComponents)
	{
		pComponent->LateUpdate(deltaT);
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

void dae::GameObject::SetParent(GameObject* pParent)
{
	if (pParent == nullptr)
		m_pTransformComponent->SetLocalPosition(GetWorldPosition());
	else
	{
		m_pTransformComponent->SetLocalPosition(GetLocalPosition() - pParent->GetWorldPosition());
		//SetPositionDirty();
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
