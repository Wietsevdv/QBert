#pragma once
#include "Singleton.h"
#include "Commands.h"
#include "XInputController.h"

#include <vector>
#include <map>
#include <memory>

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
	public:
		bool ProcessInput();

		void AddController(ControllerComponent* pControllerComp);
		int GetNrOfControllers() const;

		template <class Command>
		void BindControllerButtonToCommand(int controllerIdx, ControllerButtons button, ButtonState state);

	private:
		std::vector<ControllerComponent*> m_pControllerComponents;

		struct boundControllerButton
		{
			int controllerIndex;
			ControllerButtons button;
			ButtonState buttonState;
			std::shared_ptr<Command> pCommand;
		};
		std::vector<boundControllerButton> m_BoundControllerButtons;

		std::vector<std::unique_ptr<Command>> m_pCommands;

		void CheckBoundButtons();
	};

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

		m_BoundControllerButtons.emplace_back(controllerIdx, button, state, std::make_shared<Command>());
	}
}
