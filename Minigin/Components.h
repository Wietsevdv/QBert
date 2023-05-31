#pragma once

#include <memory>
#include <string>

#include <SDL_ttf.h>
#include <glm/glm.hpp>

#include "ResourceManager.h"
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

	private:
		glm::vec3 m_LocalPosition;
		glm::vec3 m_WorldPosition;

		bool m_PositionIsDirty;

		void UpdateWorldPosition();
	};

	class RenderComponent final : public BaseComponent
	{
	public:
		RenderComponent(GameObject* pGameObject) : BaseComponent(pGameObject) {};

		virtual ~RenderComponent() {};
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

		void SetTexture(std::shared_ptr<Texture2D> pTexture) { m_pTexture = pTexture; }
		void Render(float x, float y);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
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

	private:
		std::shared_ptr<Texture2D> m_pTexture;

		//QUICK ACCESS (no ownership)
		RenderComponent* m_pRenderComponent;
	};

	class TextComponent : public BaseComponent
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

	private:
		std::unique_ptr<XInputController> m_pController{};
	};

	class UIComponent final : public BaseComponent, public Observer
	{
	public:
		UIComponent(GameObject* pGameObject)
			: BaseComponent(pGameObject)
		{
		};

		virtual ~UIComponent() {};
		UIComponent(const UIComponent& other) = delete;
		UIComponent(UIComponent&& other) = delete;
		UIComponent& operator=(const UIComponent& other) = delete;
		UIComponent& operator=(UIComponent&& other) = delete;

		virtual void Update(const float) override {};
		virtual void LateUpdate(const float) override {};

		virtual void Notify(const GameObject& gameObject, GameEvents event) override;

	private:
		//keep pointers or reference to a texture and a text component. Give interface for setting a texture or a text which is then set on the rendercomponent.
		//writing functionality in Notify would make the UIComponent unusable.
		//instead make it a pure virtual base class. A project would inherit from this and add it's own functionality in its notify.
	};
}