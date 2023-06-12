#pragma once

//#ifndef OBSERVERS
//#define OBSERVERS

namespace dae
{
	class GameObject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const GameObject& gameObject, void* pData) = 0;
	};
}

//#endif // !OBSERVERS