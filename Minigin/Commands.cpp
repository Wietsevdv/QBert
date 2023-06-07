#include "Commands.h"
#include "GameObject.h"

#include <iostream>

void dae::ClickCommand::Execute(GameObject* pGameObject, void* pData)
{
	if (MenuButtonComponent* pButton = pGameObject->GetComponent<MenuButtonComponent>())
	{
		pButton->Click(pData);
	}
}
