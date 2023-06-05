#pragma once
#include "Singleton.h"
#include "Commands.h"
#include "XInputController.h"

#include <vector>
#include <map>
#include <memory>

#include "SDL_keycode.h"

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
		struct boundSDLKey
		{
			int playerIndex;
			std::shared_ptr<SDL_Keycode> pKey;
			ButtonState buttonState;
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
		void BindSDLKeyToCommand(int playerIdx, std::shared_ptr<SDL_Keycode> pKey, ButtonState state);

		template <class Command>
		void BindControllerButtonToCommand(int controllerIdx, ControllerButtons button, ButtonState state);

	private:
		std::vector<ControllerComponent*> m_pControllerComponents;

		std::vector<boundSDLKey> m_BoundSDLKeys;
		std::vector<boundControllerButton> m_BoundControllerButtons;

		std::vector<std::unique_ptr<Command>> m_pCommands;

		bool CheckBoundKeys();
		void CheckBoundButtons();
	};



	template <class Command>
	inline void InputManager::BindSDLKeyToCommand(int playerIdx, std::shared_ptr<SDL_Keycode> pKey, ButtonState state)
	{
		for (boundSDLKey& boundKey : m_BoundSDLKeys)
		{
			if (dynamic_cast<Command*>(boundKey.pCommand))
			{
				m_BoundSDLKeys.emplace_back(playerIdx, pKey, state, boundKey.pCommand);
				return;
			}
		}
		for (boundControllerButton& boundButton : m_BoundControllerButtons)
		{
			if (dynamic_cast<Command*>(boundButton.pCommand.get()))
			{
				m_BoundSDLKeys.emplace_back(playerIdx, pKey, state, boundButton.pCommand);
				return;
			}
		}

		m_BoundSDLKeys.emplace_back(playerIdx, pKey, state, std::make_shared<Command>());
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
		for (boundSDLKey& boundKey : m_BoundSDLKeys)
		{
			if (dynamic_cast<Command*>(boundKey.pCommand.get()))
			{
				m_BoundControllerButtons.emplace_back(controllerIdx, button, state, boundKey.pCommand);
				return;
			}
		}

		m_BoundControllerButtons.emplace_back(controllerIdx, button, state, std::make_shared<Command>());
	}
}
