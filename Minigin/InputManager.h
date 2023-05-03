#pragma once
#include "Singleton.h"
#include "Components.h"
#include "Commands.h"

#include <vector>
#include <map>
#include <memory>

namespace dae
{
	class ControllerComponent;

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		void AddController(ControllerComponent* pControllerComp);
		int GetNrOfControllers() const { return static_cast<int>(m_pControllerComponents.size()); }

		void BindButtonToCommand(ControllerButtons button, std::unique_ptr<Command> pCommand);

	private:
		std::vector<ControllerComponent*> m_pControllerComponents;
		std::map<ControllerButtons, std::unique_ptr<Command>> m_ButtonCommandmap;
	};

}
