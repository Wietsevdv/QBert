#pragma once
#include <memory>
#include <list>
#include "Components.h"
#include "Subject.h"
#include <iostream>

namespace dae
{
	class GameObject final
	{
	public:
		void Update(const float deltaT);
		void LateUpdate(const float deltaT);
		void Render() const;

		const glm::vec3 GetLocalPosition() const { return m_pTransformComponent->GetLocalPosition(); }
		const glm::vec3 GetWorldPosition() const {	return m_pTransformComponent->GetWorldPosition(); }

		template <class component> component* AddComponent(GameObject* pGo);
		template <class component> void RemoveComponent();
		template <class component> component* GetComponent() const;
		template <class component> bool IsComponentAdded() const;

		void SetParent(GameObject* pParent);
		GameObject* GetParent() { return m_pParent; }

		void AddChild(GameObject* pChild) { m_pChildren.push_back(pChild); }
		void RemoveChild(GameObject* pChild) { m_pChildren.remove(pChild); }
		const std::list<GameObject*>& GetChildren() const { return m_pChildren; }
		void SetChildrenPosDirty();
		int GetNrOfChildren() const { return static_cast<int>(m_pChildren.size()); }

		Subject* MakeSubject();
		//check for nullptr
		Subject* GetSubject() const { return m_pSubject.get(); }
		
		void SetDead() { m_IsDead = true; }
		bool GetIsDead() const { return m_IsDead; }

		int GetLayer() const;

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		GameObject* m_pParent{ nullptr };
		
		std::list<std::shared_ptr<BaseComponent>> m_pComponents;
		std::list<GameObject*> m_pChildren;

		TransformComponent* m_pTransformComponent{ nullptr }; //for quick access. don't delete. it's also in m_pComponents
		RenderComponent* m_pRenderComponent{ nullptr }; //for quick access. don't delete. it's also in m_pComponents

		std::unique_ptr<Subject> m_pSubject{ nullptr };

		bool m_IsDead = false;

		void SetPositionDirty() { m_pTransformComponent->SetPositionDirty(); }
	};

	template<class component>
	inline component* GameObject::AddComponent(GameObject* pGo)
	{
		if (IsComponentAdded<component>())
		{
			std::cout << "Component already added\n";
			return GetComponent<component>();
		}

		//Check whether passed template is inherited from BaseComponent
		std::shared_ptr<component> pComponent = std::make_shared<component>(pGo);
		if (dynamic_cast<BaseComponent*>(pComponent.get()))
		{
			if (dynamic_cast<TextureComponent*>(pComponent.get()))
			{
				if (IsComponentAdded<TextComponent>())
				{
					std::cout << "A gameObject cannot have both a texture and text component\n";
					return nullptr;
				}

				m_pComponents.push_back(pComponent);
				return dynamic_cast<component*>(pComponent.get());
			}
			else if (dynamic_cast<TextComponent*>(pComponent.get()))
			{
				if (IsComponentAdded<TextureComponent>())
				{
					std::cout << "A gameObject cannot have both a texture and text component\n";
					return nullptr;
				}

				m_pComponents.push_back(pComponent);
				return dynamic_cast<component*>(pComponent.get());
			}

			m_pComponents.push_back(pComponent);
			return pComponent.get();
		}

		std::cout << "new component could not be cast to Component class\n";
		return nullptr;
	}

	template<class component>
	inline void GameObject::RemoveComponent()
	{
		//no check for correct template necessary here
		for (auto pComponent : m_pComponents)
		{
			component* pCastComponent = dynamic_cast<component*>(pComponent.get());
			if (pCastComponent != nullptr)
			{
				dynamic_cast<BaseComponent*>(pComponent)->SetDead();
			}
		}
	}

	template<class component>
	inline component* GameObject::GetComponent() const
	{
		//no check for correct template necessary here
		for (auto pComponent : m_pComponents)
		{
			component* pCastComponent = dynamic_cast<component*>(pComponent.get());
			if (pCastComponent != nullptr && !pComponent->GetIsDead())
			{
				return pCastComponent;
			}
		}

		std::cout << "Component not found\n";
		return nullptr;
	}

	template<class component>
	inline bool GameObject::IsComponentAdded() const
	{
		//no check for correct template necessary here
		for (auto pComponent : m_pComponents)
		{
			component* pCastComponent = dynamic_cast<component*>(pComponent.get());
			if (pCastComponent != nullptr)
			{
				return true;
			}
		}
		return false;
	}
}
