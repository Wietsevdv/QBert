#include "ProjectStatesAndTransitions.h"
#include "SceneManager.h"

#include <iostream>

dae::StartMenuState::StartMenuState(GameObject* pGameObject)
	: BaseState{ pGameObject }
	, m_PlayClicked{ false }
{
	pGameObject->GetSubject()->AddObserver(this);
}

void dae::StartMenuState::OnEnter()
{
	SceneManager::GetInstance().SetActiveScene("StartMenuScene");
}

void dae::StartMenuState::Notify(const GameObject& /*gameObject*/, GameEvents /*event*/)
{
	m_PlayClicked = true;
}

dae::MainGameState::MainGameState(GameObject* pGameObject)
	: BaseState{ pGameObject }
{
}

void dae::MainGameState::OnEnter()
{
	SceneManager::GetInstance().SetActiveScene("MainGameScene");
}

void dae::MainGameState::Notify(const GameObject& /*gameObject*/, GameEvents /*event*/)
{
}

dae::StartGameTransition::StartGameTransition(BaseState* pFrom, BaseState* pTo)
	: BaseTransition{ pFrom, pTo }
{
}

bool dae::StartGameTransition::Check(const float)
{
	if (GetFrom())
	{
		if (const StartMenuState* sm = dynamic_cast<StartMenuState*>(GetFrom()))
		{
			if (sm->IsPlayClicked())
				return true;
		}
	}
	return false;
}
