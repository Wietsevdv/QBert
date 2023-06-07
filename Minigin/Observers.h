#pragma once

//#ifndef OBSERVERS
//#define OBSERVERS

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
}

//#endif // !OBSERVERS