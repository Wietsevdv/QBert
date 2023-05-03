#pragma once
#include <vector>
#include "Observers.h"

namespace dae
{
	class GameObject;

	class Subject final
	{
	public:
		Subject() = default;
		virtual ~Subject() = default;
		Subject(const Subject& other) = delete;
		Subject(Subject&& other) = delete;
		Subject& operator=(const Subject& other) = delete;
		Subject& operator=(Subject&& other) = delete;

		void AddObserver(Observer* pObserver) { m_Observers.emplace_back(pObserver); } //add checks
		void RemoveObserver(Observer*) {}; //make

		void NotifyObservers(const GameObject& gameObject, GameEvents gameEvent);

	private:
		std::vector<Observer*> m_Observers;
	};

}