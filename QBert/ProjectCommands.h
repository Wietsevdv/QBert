#pragma once
#include "Commands.h"

namespace dae
{
	class JumpRightUpCommand final : public Command
	{
		virtual void Execute(GameObject* pGameObject, void* pData) override;
	};

	class JumpLeftUpCommand final : public Command
	{
		virtual void Execute(GameObject* pGameObject, void* pData) override;
	};

	class JumpRightDownCommand final : public Command
	{
		virtual void Execute(GameObject* pGameObject, void* pData) override;
	};

	class JumpLeftDownCommand final : public Command
	{
		virtual void Execute(GameObject* pGameObject, void* pData) override;
	};

	class ChangeSceneCommand final : public Command
	{
		virtual void Execute(GameObject* pGameObject, void* pData) override;
	};
}