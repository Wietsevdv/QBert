#include "ProjectCommands.h"
#include "GameObject.h"
#include "ProjectComponents.h"

#include <iostream>

void dae::JumpRightUpCommand::Execute(GameObject* pPlayer, void* /*pData*/)
{
	if (PlayerComponent* pPlayerComponent = dynamic_cast<PlayerComponent*>(pPlayer->GetComponent<PlayerComponent>()))
	{
		pPlayerComponent->JumpRightUp();
	}
}

void dae::JumpLeftUpCommand::Execute(GameObject* pPlayer, void* /*pData*/)
{
	if (PlayerComponent* pPlayerComponent = dynamic_cast<PlayerComponent*>(pPlayer->GetComponent<PlayerComponent>()))
	{
		pPlayerComponent->JumpLeftUp();
	}
}

void dae::JumpRightDownCommand::Execute(GameObject* pPlayer, void* /*pData*/)
{
	if (PlayerComponent* pPlayerComponent = dynamic_cast<PlayerComponent*>(pPlayer->GetComponent<PlayerComponent>()))
	{
		pPlayerComponent->JumpRightDown();
	}
}

void dae::JumpLeftDownCommand::Execute(GameObject* pPlayer, void* /*pData*/)
{
	if (PlayerComponent* pPlayerComponent = dynamic_cast<PlayerComponent*>(pPlayer->GetComponent<PlayerComponent>()))
	{
		pPlayerComponent->JumpLeftDown();
	}
}
