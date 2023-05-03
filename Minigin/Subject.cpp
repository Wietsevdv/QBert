#include "Subject.h"

void dae::Subject::NotifyObservers(const GameObject& gameObject, GameEvents gameEvent)
{
	for (auto observer : m_Observers)
	{
		observer->Notify(gameObject, gameEvent);
	}
}
