#pragma once
#include <vector>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class StateMachine;

	class StateMachineManager final : public Singleton<StateMachineManager>
	{
	public:
		StateMachine* CreateStateMachine();
		void AddStateMachine(std::shared_ptr<StateMachine> pStateMachine) { m_StateMachines.emplace_back(pStateMachine); }

		void Update(const float deltaT);

	private:
		friend class Singleton<StateMachineManager>;
		StateMachineManager() = default;
		std::vector<std::shared_ptr<StateMachine>> m_StateMachines;
	};

}