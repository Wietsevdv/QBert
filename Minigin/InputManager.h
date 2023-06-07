#pragma once
#include "Singleton.h"
#include "Commands.h"
#include "XInputController.h"

#include <vector>
#include <map>
#include <memory>

#include "SDL_events.h"

namespace dae
{
	class GameObject;
	class ControllerComponent;

	enum class ButtonState
	{
		pressed,
		released,
		held
	};

	class InputManager final : public Singleton<InputManager>
	{
		struct boundSDLEvent
		{
			GameObject* pGameObject;
			std::shared_ptr<SDL_Event> pEvent;
			std::shared_ptr<Command> pCommand;
		};
		struct boundControllerButton
		{
			GameObject* pGameObject;
			ControllerComponent* pControllerComponent;
			ControllerButtons button;
			ButtonState buttonState;
			std::shared_ptr<Command> pCommand;
		};

	public:
		bool ProcessInput();

		template <class Command>
		void BindSDLEventToCommand(GameObject* pGameObject, std::shared_ptr<SDL_Event> pEvent);

		template <class Command>
		void BindControllerButtonToCommand(GameObject* pGameObject, ControllerComponent* pControllerComponent, ControllerButtons button, ButtonState state);

	private:
		std::vector<boundSDLEvent> m_BoundSDL_Events;
		std::vector<boundControllerButton> m_BoundControllerButtons;

		std::vector<std::unique_ptr<Command>> m_pCommands;

		bool CheckBoundEvents();
		void CheckBoundButtons();
	};



	template <class Command>
	inline void InputManager::BindSDLEventToCommand(GameObject* pGameObject, std::shared_ptr<SDL_Event> pEvent)
	{
		for (boundSDLEvent& boundEvent : m_BoundSDL_Events)
		{
			if (dynamic_cast<Command*>(boundEvent.pCommand.get()))
			{
				m_BoundSDL_Events.emplace_back(pGameObject, pEvent, boundEvent.pCommand);
				return;
			}
		}
		for (boundControllerButton& boundButton : m_BoundControllerButtons)
		{
			if (dynamic_cast<Command*>(boundButton.pCommand.get()))
			{
				m_BoundSDL_Events.emplace_back(pGameObject, pEvent, boundButton.pCommand);
				return;
			}
		}

		m_BoundSDL_Events.emplace_back(pGameObject, pEvent, std::make_shared<Command>());
	}

	template <class Command>
	inline void InputManager::BindControllerButtonToCommand(GameObject* pGameObject, ControllerComponent* pControllerComponent, ControllerButtons button, ButtonState state)
	{
		for (boundControllerButton& boundButton : m_BoundControllerButtons)
		{
			if (dynamic_cast<Command*>(boundButton.pCommand.get()))
			{
				//already contains this command
				m_BoundControllerButtons.emplace_back(pGameObject, pControllerComponent, button, state, boundButton.pCommand);
				return;
			}
		}
		for (boundSDLEvent& boundEvent : m_BoundSDL_Events)
		{
			if (dynamic_cast<Command*>(boundEvent.pCommand.get()))
			{
				m_BoundControllerButtons.emplace_back(pGameObject, pControllerComponent, button, state, boundEvent.pCommand);
				return;
			}
		}

		m_BoundControllerButtons.emplace_back(pGameObject, pControllerComponent, button, state, std::make_shared<Command>());
	}
}
