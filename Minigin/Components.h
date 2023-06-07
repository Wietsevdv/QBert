#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>

#include <SDL_ttf.h>
#include <glm/glm.hpp>

#include "ResourceManager.h"
#include "InputManager.h"
#include "XInputController.h"
#include "Subject.h"

namespace dae
{
	class GameObject;
	class Font;
	class Texture2D;

	class BaseComponent
	{
	public:
		virtual ~BaseComponent() {};
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		void SetDead() { m_IsDead = true; }
		bool GetIsDead() const { return m_IsDead; }

		virtual void Update(const float deltaT) = 0;
		virtual void LateUpdate(const float deltaT) = 0;

		GameObject* GetOwner() const { return m_pGameObject; }

	private:
		GameObject* m_pGameObject;
		bool m_IsDead = false;

	protected:
		explicit BaseComponent(GameObject* pGameObject) : m_pGameObject(pGameObject) {}
	};

	class TransformComponent final : public BaseComponent
	{
	public:
		TransformComponent(GameObject* pGameObject)
			: BaseComponent(pGameObject)
			, m_LocalPosition()
			, m_WorldPosition()
			, m_PositionIsDirty(false)
			, m_pCollisionComponent(nullptr)
		{};

		virtual ~TransformComponent() {};
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

		void SetLocalPosition(const glm::vec3& localPos);
		void SetLocalPosition(float x, float y, float z);

		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec3& GetWorldPosition();

		void SetPositionDirty() { m_PositionIsDirty = true; }

		//BaseComp because Collisioncomp is not yet defined
		void SetCollisionComponent(BaseComponent* pCollisionComp) { m_pCollisionComponent = pCollisionComp; }

	private:
		glm::vec3 m_LocalPosition;
		glm::vec3 m_WorldPosition;

		bool m_PositionIsDirty;

		void UpdateWorldPosition();

		//BaseComp because Collisioncomp is not yet defined
		BaseComponent* m_pCollisionComponent;
	};

	class RenderComponent final : public BaseComponent
	{
	public:
		RenderComponent(GameObject* pGameObject);

		virtual ~RenderComponent() {};
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

		void SetTexture(std::shared_ptr<Texture2D> pTexture) { m_pTexture = pTexture; }
		void Render(float x, float y);

		void SetLayer(int newLayer);
		int GetLayer() const { return m_Layer; }

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		int m_Layer;
	};

	class TextureComponent final : public BaseComponent
	{
	public:
		TextureComponent(GameObject* pGameObject);

		virtual ~TextureComponent() {};
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

		void SetTexture(const std::string& fileName);

		glm::ivec2 GetTextureSize() const;

	private:
		std::shared_ptr<Texture2D> m_pTexture;

		//QUICK ACCESS (no ownership)
		RenderComponent* m_pRenderComponent;
	};

	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(GameObject* pGameObject);

		virtual ~TextComponent() {};
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		virtual void Update(const float deltaT) override;
		virtual void LateUpdate(const float) override {};

		void SetText(const std::string& text);
		void SetColor(Uint8 r, Uint8 g, Uint8 b);
		void SetSize(int size);

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		SDL_Color m_Color;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTextTexture;

		//QUICK ACCESS (no ownership)
		RenderComponent* m_pRenderComponent;
	};

	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(GameObject* pGameObject);

		virtual ~FPSComponent() {};
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		virtual void Update(const float) override;
		virtual void LateUpdate(const float) override {};

	private:
		int m_NrOfFrames;
		float m_TimePassed;
		int m_FPS;

		//QUICK ACCESS (no ownership)
		TextComponent* m_pTextComponent;
	};

	class ControllerComponent final : public BaseComponent
	{
	public:
		ControllerComponent(GameObject* pGameObject);

		virtual ~ControllerComponent() {};
		ControllerComponent(const ControllerComponent& other) = delete;
		ControllerComponent(ControllerComponent&& other) = delete;
		ControllerComponent& operator=(const ControllerComponent& other) = delete;
		ControllerComponent& operator=(ControllerComponent&& other) = delete;

		virtual void Update(const float dt) override;
		virtual void LateUpdate(const float) override {};

		XInputController* GetController() const { return m_pController.get(); }

	    template <class Command>
		void BindButtonToCommand(GameObject* pGameObject, ControllerButtons button, ButtonState state);

	private:
		std::unique_ptr<XInputController> m_pController{};
	};

	template <class Command>
	inline void ControllerComponent::BindButtonToCommand(GameObject* pGameObject, ControllerButtons button, ButtonState state)
	{
		InputManager::GetInstance().BindControllerButtonToCommand<Command>(pGameObject, this, button, state);
	}

	//default collision is a small cube at on top of gameObject
	class CollisionComponent : public BaseComponent
	{
	public:
		CollisionComponent(GameObject* pGameObject);

		virtual ~CollisionComponent() {};
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(CollisionComponent&& other) = delete;

		virtual void Update(const float) override;
		virtual void LateUpdate(const float) override {};

		void SetPoints(glm::vec2& leftBottom, glm::vec2& rightBottom, glm::vec2& leftTop, glm::vec2& rightTop, bool areWorldPositions = false);
		void AddDisplacement(const glm::vec2& displacement);

		void SetOverlapCallbackFunction(std::function<void(GameObject* pOtherObject)> pOverlapFunction) { m_pCallbackFunction = pOverlapFunction; }

		bool IsOverlapping(const glm::vec2& leftBottom, const glm::vec2& rightBottom, const glm::vec2& leftTop, const glm::vec2& rightTop) const;

	private:
		glm::vec2 m_Points[4];
		static std::vector<CollisionComponent*> m_pAllCollisionComponents;

		std::function<void(GameObject* pOtherObject)> m_pCallbackFunction;

		TransformComponent* m_pTransformComponent;
	};

	class UIComponent : public BaseComponent, public Observer
	{
	public:
		UIComponent(GameObject* pGameObject);

		virtual ~UIComponent() {};
		UIComponent(const UIComponent& other) = delete;
		UIComponent(UIComponent&& other) = delete;
		UIComponent& operator=(const UIComponent& other) = delete;
		UIComponent& operator=(UIComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

		virtual void SetTexture(const std::string& fileName);
		void SetText(const std::string& text);

		virtual void Notify(const GameObject& gameObject, GameEvents event) override;

		const TextureComponent* GetTextureComponent() const { return m_pTextureComponent; }

	private:
		TextureComponent* m_pTextureComponent;
		TextComponent* m_pTextComponent;
	};

	class MenuButtonComponent : public UIComponent
	{
	public:
		MenuButtonComponent(GameObject* pGameObject);

		virtual ~MenuButtonComponent() {};
		MenuButtonComponent(const MenuButtonComponent& other) = delete;
		MenuButtonComponent(MenuButtonComponent&& other) = delete;
		MenuButtonComponent& operator=(const MenuButtonComponent& other) = delete;
		MenuButtonComponent& operator=(MenuButtonComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

		virtual void SetTexture(const std::string& fileName) override;

		virtual void Notify(const GameObject&, GameEvents) override {};

		void Click(void* pData) const;

	private:
		glm::vec2 m_Points[4];
		TransformComponent* m_pTransformComponent;
	};
}