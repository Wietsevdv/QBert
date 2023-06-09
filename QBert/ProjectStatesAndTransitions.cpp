#include "ProjectStatesAndTransitions.h"
#include "SceneManager.h"
#include "ProjectComponents.h"

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

void dae::MainGameState::Notify(const GameObject& gameObject, GameEvents /*event*/)
{
	if (CubeComponent* pCubeComponent = gameObject.GetComponent<CubeComponent>())
	{
		if (pCubeComponent->GetCubeState() == CubeComponent::CubeState::Completed)
		{
			m_CubeCompletions[pCubeComponent] = true;

			for (auto pCube : m_CubeCompletions)
			{
				if (pCube.second == false)
					return;

				//all cubes are in completed state
				//advance to next round or level
				//use a state for this
			}
		}
		else
			m_CubeCompletions[pCubeComponent] = false;
	}
}

void dae::MainGameState::AddCube(CubeComponent* pCube)
{
	pCube->GetOwner()->GetSubject()->AddObserver(this);
	m_CubeCompletions[pCube] = false;
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

bool dae::LoadSoloGameTransition::Check(const float)
{
	return false;
}

bool dae::LoadCoopGameTransition::Check(const float)
{
	return false;
}

bool dae::LoadVSGameTransition::Check(const float)
{
	return false;
}
