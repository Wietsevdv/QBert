#pragma once

namespace dae
{
	class GameObject;

	class Command abstract
	{
	public:
		virtual ~Command() = default;
		virtual void Execute(GameObject* pPlayer, void* pData) = 0;
	};

	class AddDirectionCommand final : public Command
	{
		virtual void Execute(GameObject* pPlayer, void* pData) override;
	};

	class LoseLifePointCommand final : public Command
	{
		virtual void Execute(GameObject* pPlayer, void* pData) override;
	};

	class IncreaseScoreCommand final : public Command
	{
		virtual void Execute(GameObject* pPlayer, void* pData) override;
	};
}