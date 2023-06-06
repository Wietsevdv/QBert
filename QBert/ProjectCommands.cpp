#include "ProjectCommands.h"
#include "GameObject.h"
#include "ProjectComponents.h"
#include "SceneManager.h"

#include <iostream>

void dae::JumpRightUpCommand::Execute(GameObject* pPlayer, void* /*pData*/)
{
	if (MovementComponent* pMovementComponent = dynamic_cast<MovementComponent*>(pPlayer->GetComponent<MovementComponent>()))
	{
		pMovementComponent->JumpRightUp();
	}
}

void dae::JumpLeftUpCommand::Execute(GameObject* pPlayer, void* /*pData*/)
{
	if (MovementComponent* pMovementComponent = dynamic_cast<MovementComponent*>(pPlayer->GetComponent<MovementComponent>()))
	{
		pMovementComponent->JumpLeftUp();
	}
}

void dae::JumpRightDownCommand::Execute(GameObject* pPlayer, void* /*pData*/)
{
	if (MovementComponent* pMovementComponent = dynamic_cast<MovementComponent*>(pPlayer->GetComponent<MovementComponent>()))
	{
		pMovementComponent->JumpRightDown();
	}
}

void dae::JumpLeftDownCommand::Execute(GameObject* pPlayer, void* /*pData*/)
{
	if (MovementComponent* pMovementComponent = dynamic_cast<MovementComponent*>(pPlayer->GetComponent<MovementComponent>()))
	{
		pMovementComponent->JumpLeftDown();
	}
}

void dae::ChangeSceneCommand::Execute(GameObject* /*pPlayer*/, void* pData)
{
	SceneManager::GetInstance().SetActiveScene("MainMenuScene");

	Sint32* pX = static_cast<Sint32*>(pData);
	Sint32* pY = pX + 1;

	std::cout << "Mouse X: " << *pX << std::endl << "Mouse Y: " << *pY << std::endl << std::endl;
}
