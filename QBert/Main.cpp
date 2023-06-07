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
#include "StateMachineManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "Services.h"

#include "SDL_mixer.h"
#include <thread>

#include <iostream>

#include "ProjectComponents.h"
#include "ProjectCommands.h"
#include "ProjectStatesAndTransitions.h"

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
	auto& scene = SceneManager::GetInstance().CreateScene("MainGameScene");

	//BLOCK TOP
	std::shared_ptr<GameObject> blockGO = std::make_shared<GameObject>();
	TextureComponent* pBlockTexture = blockGO->AddComponent<TextureComponent>(blockGO.get());
	pBlockTexture->SetTexture("Block_Blue_1.png");

	TransformComponent* pBlockTransformComp = blockGO->GetComponent<TransformComponent>();
	pBlockTransformComp->SetLocalPosition(320.f, 200.f, 0.f);

	CubeComponent* pCubeComponent = blockGO->AddComponent<CubeComponent>(blockGO.get());
	pCubeComponent->SetIsRightEdgeCube();
	pCubeComponent->SetIsLeftEdgeCube();

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
	pBlockTransformComp->SetLocalPosition(336.f, 224.f, 0.f);

	pCubeComponent = blockGO->AddComponent<CubeComponent>(blockGO.get());
	pCubeComponent->SetIsRightEdgeCube();

	//collisionComp
	pCollisionComp = blockGO->AddComponent<CollisionComponent>(blockGO.get());
	pCollisionComp->SetPoints(bottomLeft, bottomRight, topLeft, topRight);

	scene.Add(blockGO);

	//BLOCK LEFT
	blockGO = std::make_shared<GameObject>();
	pBlockTexture = blockGO->AddComponent<TextureComponent>(blockGO.get());
	pBlockTexture->SetTexture("Block_Blue_1.png");

	pBlockTransformComp = blockGO->GetComponent<TransformComponent>();
	pBlockTransformComp->SetLocalPosition(304.f, 224.f, 0.f);

	pCubeComponent = blockGO->AddComponent<CubeComponent>(blockGO.get());
	pCubeComponent->SetIsLeftEdgeCube();

	//collisionComp
	pCollisionComp = blockGO->AddComponent<CollisionComponent>(blockGO.get());
	pCollisionComp->SetPoints(bottomLeft, bottomRight, topLeft, topRight);

	scene.Add(blockGO);

	//BLOCK BOTTOM
	blockGO = std::make_shared<GameObject>();
	pBlockTexture = blockGO->AddComponent<TextureComponent>(blockGO.get());
	pBlockTexture->SetTexture("Block_Blue_1.png");

	pBlockTransformComp = blockGO->GetComponent<TransformComponent>();
	pBlockTransformComp->SetLocalPosition(320.f, 248.f, 0.f);

	pCubeComponent = blockGO->AddComponent<CubeComponent>(blockGO.get());
	pCubeComponent->SetIsBottomEdgeCube();

	//collisionComp
	pCollisionComp = blockGO->AddComponent<CollisionComponent>(blockGO.get());
	pCollisionComp->SetPoints(bottomLeft, bottomRight, topLeft, topRight);

	scene.Add(blockGO);

	//Q-BERT
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>();

	TextureComponent* pTexture = go->AddComponent<TextureComponent>(go.get());
	pTexture->SetTexture("Q-BertLeftDownStand.png");

	UIComponent* pUIcomp = go->AddComponent<UIComponent>(go.get());

	TransformComponent* pTransformComp = go->GetComponent<TransformComponent>();
	pTransformComp->SetLocalPosition(320.f, 50.f, 0.f);

	go->AddComponent<GravityComponent>(go.get());
	go->AddComponent<PlayerComponent>(go.get());

	PlayerCollisionComponent* pPlayerCollisionComponent = go->AddComponent<PlayerCollisionComponent>(go.get());
	bottomLeft = { -3.f, 0.f };
	bottomRight = { 3.f, 0.f };
	topLeft = { -3.f, -5.f };
	topRight = { 3.f, -5.f };
	pPlayerCollisionComponent->SetPoints(bottomLeft, bottomRight, topLeft, topRight);
	
	ControllerComponent* pQBertControllerComponent = go->AddComponent<ControllerComponent>(go.get());
	pQBertControllerComponent->BindButtonToCommand<JumpRightUpCommand>(go.get(), ControllerButtons::DRight, ButtonState::pressed);
	pQBertControllerComponent->BindButtonToCommand<JumpLeftUpCommand>(go.get(), ControllerButtons::DUp, ButtonState::pressed);
	pQBertControllerComponent->BindButtonToCommand<JumpRightDownCommand>(go.get(), ControllerButtons::DDown, ButtonState::pressed);
	pQBertControllerComponent->BindButtonToCommand<JumpLeftDownCommand>(go.get(), ControllerButtons::DLeft, ButtonState::pressed);

	scene.Add(go);

	Subject* pSubject = go->MakeSubject();
	pSubject->AddObserver(pUIcomp);
	pSubject->NotifyObservers(*go.get(), GameEvents::PlayerDied);

	//SOUND
	dae::ServiceLocator::RegisterSoundSystem(new dae::SDL_SoundSystem());
	dae::SoundSystem* pSS = dae::ServiceLocator::GetSoundSystem();
	
	pSS->Load(0, "fall.mp3", 16);
	pSS->Load(1, "jump.wav", 16);

	std::cout << "\n\nPress 'v' to play a sound\n";

	//-----------------------MAIN MENU SCENE--------------------------------
	auto& mainMenuScene = SceneManager::GetInstance().CreateScene("StartMenuScene");

	auto playButton = std::make_shared<GameObject>();
	playButton->MakeSubject();
	playButton->AddComponent<TransformComponent>(playButton.get())->SetLocalPosition(320.f, 230.f, 0.f);
	playButton->AddComponent<MenuButtonComponent>(playButton.get())->SetTexture("Play.png");

	mainMenuScene.Add(playButton);

	//---------------------GAMESTATE MACHINE--------------------------------
	StateMachine* gameStateMachine = StateMachineManager::GetInstance().CreateStateMachine();
	std::shared_ptr<StartMenuState> smState = std::make_shared<StartMenuState>(playButton.get());
	std::shared_ptr<MainGameState> mgState = std::make_shared<MainGameState>(nullptr);
	gameStateMachine->AddState(smState);
	gameStateMachine->AddState(mgState, false);
	gameStateMachine->AddTransition(std::make_shared<StartGameTransition>(smState.get(), mgState.get()));

	std::shared_ptr<SDL_Event> pPlayClickedEvent = std::make_shared<SDL_Event>();
	pPlayClickedEvent->type = SDL_MOUSEBUTTONDOWN;
	pPlayClickedEvent->button.button = SDL_BUTTON_LEFT;

	InputManager::GetInstance().BindSDLEventToCommand<ClickCommand>(playButton.get(), pPlayClickedEvent);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}