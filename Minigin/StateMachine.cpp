#include "StateMachine.h"

#include <iostream>

void dae::StateMachine::AddState(std::shared_ptr<BaseState> pNewState, bool setActive)
{
	m_States.emplace_back(pNewState);

	if (setActive)
	{
		if (m_pActiveState)
			m_pActiveState->OnLeave();

		m_pActiveState = pNewState.get();
		m_pActiveState->OnEnter();
	}
}

void dae::StateMachine::Update(const float deltaT)
{
	if (m_pActiveState)
		m_pActiveState->Update(deltaT);

	for (auto pTransition : m_Transitions[m_pActiveState])
	{
		if (pTransition->Check(deltaT))
		{
			m_pActiveState->OnLeave();
			m_pActiveState = pTransition->GetTo();
			m_pActiveState->OnEnter();
		}
	}
}
