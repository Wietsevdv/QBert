#pragma once
#include "StateMachine.h"
#include "Observers.h"
#include "GameObject.h"

namespace dae
{
	class StartMenuState final : public BaseState, public Observer
	{
	public:
		StartMenuState(GameObject* pGameObject);

		virtual ~StartMenuState() = default;
		StartMenuState(const StartMenuState& other) = delete;
		StartMenuState(StartMenuState&& other) = delete;
		StartMenuState operator=(const StartMenuState& other) = delete;
		StartMenuState operator=(StartMenuState&& other) = delete;

		virtual void OnEnter();
		virtual void Update(const float) {};
		virtual void OnLeave() {};

		virtual void Notify(const GameObject& gameObject, GameEvents event) override;

		bool IsPlayClicked() const { return m_PlayClicked; }

	private:
		bool m_PlayClicked;
	};

	class MainGameState final : public BaseState, public Observer
	{
	public:
		MainGameState(GameObject* pGameObject);

		virtual ~MainGameState() = default;
		MainGameState(const MainGameState& other) = delete;
		MainGameState(MainGameState&& other) = delete;
		MainGameState operator=(const MainGameState& other) = delete;
		MainGameState operator=(MainGameState&& other) = delete;

		virtual void OnEnter();
		virtual void Update(const float) {};
		virtual void OnLeave() {};

		virtual void Notify(const GameObject& gameObject, GameEvents event) override;
	};

	class StartGameTransition final : public BaseTransition
	{
	public:
		StartGameTransition(BaseState* pFrom, BaseState* pTo);

		virtual ~StartGameTransition() = default;
		StartGameTransition(const StartGameTransition& other) = delete;
		StartGameTransition(StartGameTransition&& other) = delete;
		StartGameTransition operator=(const StartGameTransition& other) = delete;
		StartGameTransition operator=(StartGameTransition&& other) = delete;

		virtual bool Check(const float) override;
	};

}