#include "Observers.h"
#include "GameObject.h"

void dae::LivesDisplay::Notify(const GameObject& gameObject, GameEvents gameEvent)
{
	switch (gameEvent)
	{
	case dae::PlayerDied:
		PlayerComponent* pPlayerComponent = gameObject.GetComponent<PlayerComponent>();
		if (m_pLivesText && pPlayerComponent)
		{
			int nrOfLivesLeft = pPlayerComponent->GetNrOfLives();
			m_pLivesText->SetText("Lives: " + std::to_string(nrOfLivesLeft));
		}
		break;
	}
}

void dae::ScoreDisplay::Notify(const GameObject& gameObject, GameEvents gameEvent)
{
	switch (gameEvent)
	{
	case dae::PlayerScoreIncreased:
		PlayerComponent* pPlayerComponent = gameObject.GetComponent<PlayerComponent>();
		if (m_pScoreText && pPlayerComponent)
		{
			int score = pPlayerComponent->GetScore();
			m_pScoreText->SetText("Score: " + std::to_string(score));
		}
		break;
	}
}

void dae::Achievements::Notify(const GameObject& gameObject, GameEvents gameEvent)
{
	switch (gameEvent)
	{
	case dae::PlayerScoreIncreased:
		PlayerComponent* pPlayerComponent = gameObject.GetComponent<PlayerComponent>();
		if (pPlayerComponent)
		{
			//if (!m_pSteamAchievements->IsAchievementSet("ACH_WIN_ONE_GAME") && pPlayerComponent->GetScore() >= 500)
			//	m_pSteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
		}
		break;
	}
}