#include "ProjectCommands.h"
#include "GameObject.h"
#include "ProjectComponents.h"

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
