#pragma once
#include <vector>
#include <map>
#include <memory>

namespace dae
{
	class BaseState
	{
	public:
		BaseState() = default;
		virtual ~BaseState() = default;
		BaseState(const BaseState& other) = delete;
		BaseState(BaseState&& other) = delete;
		BaseState operator=(const BaseState& other) = delete;
		BaseState operator=(BaseState&& other) = delete;

		virtual void OnEnter() = 0;
		virtual void Update(const float) = 0;
		virtual void OnLeave() = 0;
	};

	class BaseTransition
	{
	public:
		BaseTransition(BaseState* pFrom, BaseState* pTo)
			: m_pFrom{ pFrom }
			, m_pTo{ pTo }
		{
		};

		virtual ~BaseTransition() = default;
		BaseTransition(const BaseTransition& other) = delete;
		BaseTransition(BaseTransition&& other) = delete;
		BaseTransition operator=(const BaseTransition& other) = delete;
		BaseTransition operator=(BaseTransition&& other) = delete;

		virtual bool Check(const float) = 0;

		BaseState* GetFrom() const { return m_pFrom; }
		BaseState* GetTo() const { return m_pTo; }

	private:
		BaseState* m_pFrom;
		BaseState* m_pTo;
	};

	class StateMachine
	{
	public:
		StateMachine() = default;
		~StateMachine() = default;
		StateMachine(const StateMachine& other) = delete;
		StateMachine(StateMachine&& other) = delete;
		StateMachine operator=(const StateMachine& other) = delete;
		StateMachine operator=(StateMachine&& other) = delete;

		void AddState(std::shared_ptr<BaseState> pNewState) { m_States.emplace_back(pNewState); }
		void AddTransition(std::shared_ptr<BaseTransition> pNewTransition) { m_Transitions[pNewTransition->GetFrom()].emplace_back(pNewTransition); }

		void Update(const float deltaT);

	private:
		BaseState* m_pActiveState;

		std::vector<std::shared_ptr<BaseState>> m_States;
		std::map<BaseState*, std::vector<std::shared_ptr<BaseTransition>>> m_Transitions;
	};

}