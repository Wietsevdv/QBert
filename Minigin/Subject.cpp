#include "Subject.h"

void dae::Subject::NotifyObservers(const GameObject& gameObject, void* pData)
{
	for (auto observer : m_Observers)
	{
		observer->Notify(gameObject, pData);
	}
}
