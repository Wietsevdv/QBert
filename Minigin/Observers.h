#pragma once
#include "Components.h"

namespace dae
{
	class GameObject;

	enum GameEvents
	{
		PlayerSpawned,
		PlayerDied,
		PlayerScoreIncreased
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const GameObject& gameObject, GameEvents event) = 0;
	};

	class LivesDisplay final : public Observer
	{
	public:
		LivesDisplay(TextComponent* pLivesText)
			: m_pLivesText{ pLivesText }
		{};

		virtual ~LivesDisplay() = default;
		LivesDisplay(const LivesDisplay& other) = delete;
		LivesDisplay(LivesDisplay&& other) = delete;
		LivesDisplay& operator=(const LivesDisplay& other) = delete;
		LivesDisplay& operator=(LivesDisplay&& other) = delete;

		virtual void Notify(const GameObject& gameObject, GameEvents gameEvent) override;

	private:
		TextComponent* m_pLivesText;
	};

	class ScoreDisplay final : public Observer
	{
	public:
		ScoreDisplay(TextComponent* pScoreText)
			: m_pScoreText{ pScoreText }
		{};

		virtual ~ScoreDisplay() = default;
		ScoreDisplay(const ScoreDisplay& other) = delete;
		ScoreDisplay(ScoreDisplay&& other) = delete;
		ScoreDisplay& operator=(const ScoreDisplay& other) = delete;
		ScoreDisplay& operator=(ScoreDisplay&& other) = delete;

		virtual void Notify(const GameObject& gameObject, GameEvents gameEvent) override;

	private:
		TextComponent* m_pScoreText;
	};

	class CSteamAchievements;
	class Achievements final : public Observer
	{
	public:
		Achievements(CSteamAchievements* pSteamAchievements)
		: m_pSteamAchievements{ pSteamAchievements }
		{};

		virtual ~Achievements() = default;
		Achievements(const Achievements& other) = delete;
		Achievements(Achievements&& other) = delete;
		Achievements& operator=(const Achievements& other) = delete;
		Achievements& operator=(Achievements&& other) = delete;

		virtual void Notify(const GameObject& gameObject, GameEvents gameEvent) override;

	private:
		CSteamAchievements* m_pSteamAchievements;
	};
}