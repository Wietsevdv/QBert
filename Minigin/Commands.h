#pragma once

namespace dae
{
	class GameObject;

	class Command abstract
	{
	public:
		virtual ~Command() = default;
		virtual void Execute(GameObject* pGameObject, void* pData) = 0;
	};

	class ClickCommand final : public Command
	{
	public:
		virtual ~ClickCommand() = default;
		virtual void Execute(GameObject* pGameObject, void* pData) override;
	};
}