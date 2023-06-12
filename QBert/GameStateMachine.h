#pragma once
#include <StateMachine.h>
#include "Observers.h"
#include "Scene.h"

namespace dae
{
	class GameObject;
	class CubeComponent;
	class GameStateMachine;

	class GameStateBase : public BaseState
	{
	public:
		GameStateBase()
			: BaseState{ nullptr }
			, m_pStateMachine{ nullptr }
		{
		};

		~GameStateBase() = default;
		GameStateBase(const GameStateBase& other) = delete;
		GameStateBase(GameStateBase&& other) = delete;
		GameStateBase operator=(const GameStateBase& other) = delete;
		GameStateBase operator=(GameStateBase&& other) = delete;

		virtual void OnEnter() = 0;
		virtual void Update(float) = 0;
		virtual void OnLeave() = 0;

	protected:
		GameStateMachine* GetStateMachine();

	private:
		friend class GameStateMachine;
		GameStateMachine* m_pStateMachine;
	};

	class MenuState final : public GameStateBase, public Observer
	{
	public:
		MenuState();

		~MenuState() = default;
		MenuState(const MenuState& other) = delete;
		MenuState(MenuState&& other) = delete;
		MenuState operator=(const MenuState& other) = delete;
		MenuState operator=(MenuState&& other) = delete;

		virtual void OnEnter();
		virtual void Update(float) {};
		virtual void OnLeave() {};

		virtual void Notify(const GameObject& gameObject, void*);

		bool GetLoadSolo() const { return m_LoadSolo; }

	private:
		std::shared_ptr<GameObject> m_pSoloButton;
		bool m_LoadSolo;
	};

	class LoadSoloState final : public GameStateBase
	{
	public:
		LoadSoloState();

		~LoadSoloState() = default;
		LoadSoloState(const LoadSoloState& other) = delete;
		LoadSoloState(LoadSoloState&& other) = delete;
		LoadSoloState operator=(const LoadSoloState& other) = delete;
		LoadSoloState operator=(LoadSoloState&& other) = delete;

		virtual void OnEnter();
		virtual void Update(float) {};
		virtual void OnLeave() {};

	private:
		void LoadCubes(dae::Scene& scene);
	};

	class LoadSoloTransition final : public BaseTransition
	{
	public:
		LoadSoloTransition(BaseState* pFrom, BaseState* pTo)
			: BaseTransition{ pFrom, pTo }
		{
		};

		~LoadSoloTransition() = default;
		LoadSoloTransition(const LoadSoloTransition& other) = delete;
		LoadSoloTransition(LoadSoloTransition&& other) = delete;
		LoadSoloTransition operator=(const LoadSoloTransition& other) = delete;
		LoadSoloTransition operator=(LoadSoloTransition&& other) = delete;

		virtual bool Check(float deltaT) override;
	};

	class GameStateMachine final : public StateMachine
	{
	public:
		GameStateMachine();

		~GameStateMachine() = default;
		GameStateMachine(const GameStateMachine& other) = delete;
		GameStateMachine(GameStateMachine&& other) = delete;
		GameStateMachine operator=(const GameStateMachine& other) = delete;
		GameStateMachine operator=(GameStateMachine&& other) = delete;

		void AddState(std::shared_ptr<BaseState> pNewState, bool setActive = false) override;

		void SetPlayer(GameObject* pPlayer) { m_pPlayer = pPlayer; }
		void AddCube(CubeComponent* pCube) { m_Cubes.emplace_back(pCube); }

		GameObject* GetPlayer() { return m_pPlayer; }
		std::vector<CubeComponent*>* GetCubes() { return &m_Cubes; }

	private:
		GameObject* m_pPlayer;
		std::vector<CubeComponent*> m_Cubes{};
	};

}