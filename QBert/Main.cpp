//#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Scene.h"
//
#include "GameObject.h"
#include "Services.h"

#include "SDL_mixer.h"
#include <thread>

#include <iostream>

#include "ProjectComponents.h"
#include "ProjectCommands.h"

using namespace dae;

void HandlePlayerOverlap()
{
	std::cout << "\n\nOMG I OVERLAPPED SOMETHING!!!\n\n";
}

void HandlePlayerOverlap222()
{
	std::cout << "\n\nOMG I OVERLAPPED SOMETHING!!!\n\n";
}

void load()
{
	// First scene created and automatically the active one
	auto& scene = SceneManager::GetInstance().CreateScene("StartScene");

	//BLOCK TOP
	std::shared_ptr<GameObject> blockGO = std::make_shared<GameObject>();
	TextureComponent* pBlockTexture = blockGO->AddComponent<TextureComponent>(blockGO.get());
	pBlockTexture->SetTexture("Block_Blue_1.png");

	TransformComponent* pBlockTransformComp = blockGO->GetComponent<TransformComponent>();
	pBlockTransformComp->SetLocalPosition(250.f, 300.f, 0.f);

	blockGO->AddComponent<CubeComponent>(blockGO.get());

	//collisionComp
	CollisionComponent* pCollisionComp = blockGO->AddComponent<CollisionComponent>(blockGO.get());

	glm::vec2 bottomLeft{ -4.f, -24.f };
	glm::vec2 bottomRight{ 4.f, -24.f };
	glm::vec2 topLeft{ -4.f, -26.f };
	glm::vec2 topRight{ 4.f, -26.f };

	pCollisionComp->SetPoints(bottomLeft, bottomRight, topLeft, topRight);

	scene.Add(blockGO);

	//BLOCK RIGHT
	blockGO = std::make_shared<GameObject>();
	pBlockTexture = blockGO->AddComponent<TextureComponent>(blockGO.get());
	pBlockTexture->SetTexture("Block_Blue_1.png");

	pBlockTransformComp = blockGO->GetComponent<TransformComponent>();
	pBlockTransformComp->SetLocalPosition(266.f, 324.f, 0.f);

	blockGO->AddComponent<CubeComponent>(blockGO.get());

	//collisionComp
	pCollisionComp = blockGO->AddComponent<CollisionComponent>(blockGO.get());
	pCollisionComp->SetPoints(bottomLeft, bottomRight, topLeft, topRight);

	scene.Add(blockGO);

	//BLOCK LEFT
	blockGO = std::make_shared<GameObject>();
	pBlockTexture = blockGO->AddComponent<TextureComponent>(blockGO.get());
	pBlockTexture->SetTexture("Block_Blue_1.png");

	pBlockTransformComp = blockGO->GetComponent<TransformComponent>();
	pBlockTransformComp->SetLocalPosition(234.f, 324.f, 0.f);

	blockGO->AddComponent<CubeComponent>(blockGO.get());

	//collisionComp
	pCollisionComp = blockGO->AddComponent<CollisionComponent>(blockGO.get());
	pCollisionComp->SetPoints(bottomLeft, bottomRight, topLeft, topRight);

	scene.Add(blockGO);

	//BLOCK LEFT
	blockGO = std::make_shared<GameObject>();
	pBlockTexture = blockGO->AddComponent<TextureComponent>(blockGO.get());
	pBlockTexture->SetTexture("Block_Blue_1.png");

	pBlockTransformComp = blockGO->GetComponent<TransformComponent>();
	pBlockTransformComp->SetLocalPosition(250.f, 348.f, 0.f);

	blockGO->AddComponent<CubeComponent>(blockGO.get());

	//collisionComp
	pCollisionComp = blockGO->AddComponent<CollisionComponent>(blockGO.get());
	pCollisionComp->SetPoints(bottomLeft, bottomRight, topLeft, topRight);

	scene.Add(blockGO);

	//Q-BERT
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>();

	TextureComponent* pTexture = go->AddComponent<TextureComponent>(go.get());
	pTexture->SetTexture("Q-BertLeftDownStand.png");

	go->AddComponent<ProjectComponent>(go.get());
	UIComponent* pUIcomp = go->AddComponent<UIComponent>(go.get());

	TransformComponent* pTransformComp = go->GetComponent<TransformComponent>();
	pTransformComp->SetLocalPosition(250.f, 50.f, 0.f);

	go->AddComponent<GravityComponent>(go.get());
	go->AddComponent<PlayerComponent>(go.get());

	PlayerCollisionComponent* pPlayerCollisionComponent = go->AddComponent<PlayerCollisionComponent>(go.get());
	bottomLeft = { -3.f, 0.f };
	bottomRight = { 3.f, 0.f };
	topLeft = { -3.f, -5.f };
	topRight = { 3.f, -5.f };
	pPlayerCollisionComponent->SetPoints(bottomLeft, bottomRight, topLeft, topRight);
	
	ControllerComponent* pQBertControllerComponent = go->AddComponent<ControllerComponent>(go.get());
	pQBertControllerComponent->BindButtonToCommand<JumpRightUpCommand>(ControllerButtons::DRight, ButtonState::pressed);
	pQBertControllerComponent->BindButtonToCommand<JumpLeftUpCommand>(ControllerButtons::DUp, ButtonState::pressed);
	pQBertControllerComponent->BindButtonToCommand<JumpRightDownCommand>(ControllerButtons::DDown, ButtonState::pressed);
	pQBertControllerComponent->BindButtonToCommand<JumpLeftDownCommand>(ControllerButtons::DLeft, ButtonState::pressed);

	scene.Add(go);

	Subject* pSubject = go->MakeSubject();
	pSubject->AddObserver(pUIcomp);
	pSubject->NotifyObservers(*go.get(), GameEvents::PlayerDied);

	//SOUND
	dae::ServiceLocator::RegisterSoundSystem(new dae::SDL_SoundSystem());
	dae::SoundSystem* pSS = dae::ServiceLocator::GetSoundSystem();
	
	pSS->Load(0, "fall.mp3", 16);
	pSS->Load(1, "jump.mp3", 16);

	std::cout << "\n\nPress 'v' to play a sound\n";

}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}