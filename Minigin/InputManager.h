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
			int playerIndex;
			std::shared_ptr<SDL_Event> pEvent;
			std::shared_ptr<Command> pCommand;
		};
		struct boundControllerButton
		{
			int controllerIndex;
			ControllerButtons button;
			ButtonState buttonState;
			std::shared_ptr<Command> pCommand;
		};

	public:
		bool ProcessInput();

		void AddController(ControllerComponent* pControllerComp);
		int GetNrOfControllers() const;

		template <class Command>
		void BindSDLEventToCommand(int playerIdx, std::shared_ptr<SDL_Event> pKey);

		template <class Command>
		void BindControllerButtonToCommand(int controllerIdx, ControllerButtons button, ButtonState state);

	private:
		std::vector<ControllerComponent*> m_pControllerComponents;

		std::vector<boundSDLEvent> m_BoundSDL_Events;
		std::vector<boundControllerButton> m_BoundControllerButtons;

		std::vector<std::unique_ptr<Command>> m_pCommands;

		bool CheckBoundEvents();
		void CheckBoundButtons();
	};



	template <class Command>
	inline void InputManager::BindSDLEventToCommand(int playerIdx, std::shared_ptr<SDL_Event> pKey)
	{
		for (boundSDLEvent& boundEvent : m_BoundSDL_Events)
		{
			if (dynamic_cast<Command*>(boundEvent.pCommand.get()))
			{
				m_BoundSDL_Events.emplace_back(playerIdx, pKey, boundEvent.pCommand);
				return;
			}
		}
		for (boundControllerButton& boundButton : m_BoundControllerButtons)
		{
			if (dynamic_cast<Command*>(boundButton.pCommand.get()))
			{
				m_BoundSDL_Events.emplace_back(playerIdx, pKey, boundButton.pCommand);
				return;
			}
		}

		m_BoundSDL_Events.emplace_back(playerIdx, pKey, std::make_shared<Command>());
	}

	template <class Command>
	inline void InputManager::BindControllerButtonToCommand(int controllerIdx, ControllerButtons button, ButtonState state)
	{
		for (boundControllerButton& boundButton : m_BoundControllerButtons)
		{
			if (dynamic_cast<Command*>(boundButton.pCommand.get()))
			{
				//already contains this command
				m_BoundControllerButtons.emplace_back(controllerIdx, button, state, boundButton.pCommand);
				return;
			}
		}
		for (boundSDLEvent& boundEvent : m_BoundSDL_Events)
		{
			if (dynamic_cast<Command*>(boundEvent.pCommand.get()))
			{
				m_BoundControllerButtons.emplace_back(controllerIdx, button, state, boundEvent.pCommand);
				return;
			}
		}

		m_BoundControllerButtons.emplace_back(controllerIdx, button, state, std::make_shared<Command>());
	}
}
