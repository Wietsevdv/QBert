#include "StateMachineManager.h"
#include "StateMachine.h"

dae::StateMachine* dae::StateMachineManager::CreateStateMachine()
{
	std::shared_ptr<StateMachine> pStateMachine = std::make_shared<StateMachine>();
	m_StateMachines.emplace_back(pStateMachine);
	return pStateMachine.get();
}

void dae::StateMachineManager::Update(const float deltaT)
{
	for (auto stateMachine : m_StateMachines)
	{
		stateMachine->Update(deltaT);
	}
}
