#include "StateMachine.h"

void dae::StateMachine::Update(const float deltaT)
{
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
