#pragma once
#include "StateMachine.h"
#include "Observers.h"
#include "GameObject.h"

namespace dae
{
	class CubeComponent;

	//---------------STATES---------------
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

		void AddCube(CubeComponent* pCube);

	private:
		std::map<CubeComponent*, bool> m_CubeCompletions{};
	};

	class LoadSoloGame final : public BaseState
	{
	public:
		LoadSoloGame(GameObject* pGameObject)
			: BaseState{ pGameObject }
		{
		}

		virtual ~LoadSoloGame() = default;
		LoadSoloGame(const LoadSoloGame& other) = delete;
		LoadSoloGame(LoadSoloGame&& other) = delete;
		LoadSoloGame operator=(const LoadSoloGame& other) = delete;
		LoadSoloGame operator=(LoadSoloGame&& other) = delete;

		virtual void OnEnter() {};
		virtual void Update(const float) {};
		virtual void OnLeave() {};
	};

	class LoadCoopGame final : public BaseState
	{
	public:
		LoadCoopGame(GameObject* pGameObject)
			: BaseState{ pGameObject }
		{
		}

		virtual ~LoadCoopGame() = default;
		LoadCoopGame(const LoadCoopGame& other) = delete;
		LoadCoopGame(LoadCoopGame&& other) = delete;
		LoadCoopGame operator=(const LoadCoopGame& other) = delete;
		LoadCoopGame operator=(LoadCoopGame&& other) = delete;

		virtual void OnEnter() {};
		virtual void Update(const float) {};
		virtual void OnLeave() {};
	};

	class LoadVSGame final : public BaseState
	{
	public:
		LoadVSGame(GameObject* pGameObject)
			: BaseState{ pGameObject }
		{
		}

		virtual ~LoadVSGame() = default;
		LoadVSGame(const LoadVSGame& other) = delete;
		LoadVSGame(LoadVSGame&& other) = delete;
		LoadVSGame operator=(const LoadVSGame& other) = delete;
		LoadVSGame operator=(LoadVSGame&& other) = delete;

		virtual void OnEnter() {};
		virtual void Update(const float) {};
		virtual void OnLeave() {};
	};

	//---------------TRANSITIONS---------------
	class StartGameTransition final : public BaseTransition
	{
	public:
		StartGameTransition(BaseState* pFrom, BaseState* pTo)
			: BaseTransition{ pFrom, pTo }
		{
		}

		virtual ~StartGameTransition() = default;
		StartGameTransition(const StartGameTransition& other) = delete;
		StartGameTransition(StartGameTransition&& other) = delete;
		StartGameTransition operator=(const StartGameTransition& other) = delete;
		StartGameTransition operator=(StartGameTransition&& other) = delete;

		virtual bool Check(const float) override;
	};

	class LoadSoloGameTransition final : public BaseTransition
	{
	public:
		LoadSoloGameTransition(BaseState* pFrom, BaseState* pTo)
			: BaseTransition{ pFrom, pTo }
		{
		}
		virtual ~LoadSoloGameTransition() = default;
		LoadSoloGameTransition(const LoadSoloGameTransition& other) = delete;
		LoadSoloGameTransition(LoadSoloGameTransition&& other) = delete;
		LoadSoloGameTransition operator=(const LoadSoloGameTransition& other) = delete;
		LoadSoloGameTransition operator=(LoadSoloGameTransition&& other) = delete;

		virtual bool Check(const float) override;
	};

	class LoadCoopGameTransition final : public BaseTransition
	{
	public:
		LoadCoopGameTransition(BaseState* pFrom, BaseState* pTo)
			: BaseTransition{ pFrom, pTo }
		{
		}
		virtual ~LoadCoopGameTransition() = default;
		LoadCoopGameTransition(const LoadCoopGameTransition& other) = delete;
		LoadCoopGameTransition(LoadCoopGameTransition&& other) = delete;
		LoadCoopGameTransition operator=(const LoadCoopGameTransition& other) = delete;
		LoadCoopGameTransition operator=(LoadCoopGameTransition&& other) = delete;

		virtual bool Check(const float) override;
	};

	class LoadVSGameTransition final : public BaseTransition
	{
	public:
		LoadVSGameTransition(BaseState* pFrom, BaseState* pTo)
			: BaseTransition{ pFrom, pTo }
		{
		}
		virtual ~LoadVSGameTransition() = default;
		LoadVSGameTransition(const LoadVSGameTransition& other) = delete;
		LoadVSGameTransition(LoadVSGameTransition&& other) = delete;
		LoadVSGameTransition operator=(const LoadVSGameTransition& other) = delete;
		LoadVSGameTransition operator=(LoadVSGameTransition&& other) = delete;

		virtual bool Check(const float) override;
	};
}