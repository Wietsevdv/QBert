#include "Commands.h"
#include "GameObject.h"

void dae::AddDirectionCommand::Execute(GameObject* /*pPlayer*/, void* /*pData*/)
{
	//glm::vec3* pDirection = static_cast<glm::vec3*>(pData);
	//pPlayer->GetComponent<MovementComponent>()->AddDirection(*pDirection);
}

void dae::LoseLifePointCommand::Execute(GameObject* /*pPlayer*/, void*)
{
	//pPlayer->GetComponent<PlayerComponent>()->LoseLifePoint();
}

void dae::IncreaseScoreCommand::Execute(GameObject* /*pPlayer*/, void*)
{
	//pPlayer->GetComponent<PlayerComponent>()->IncreaseScore(100);
}
