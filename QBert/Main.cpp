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

#include "BinaryFileWriter.h"

using namespace dae;

void LoadCubes(dae::Scene& scene, MainGameState* pMainGameState);

void WriteBinaryFile();

void load()
{
	WriteBinaryFile();

	// First scene created and automatically the active one
	auto& scene = SceneManager::GetInstance().CreateScene("MainGameScene");

	//-----------------------MAIN MENU SCENE--------------------------------
	auto& mainMenuScene = SceneManager::GetInstance().CreateScene("StartMenuScene");

	auto playButton = std::make_shared<GameObject>();
	playButton->MakeSubject();
	playButton->AddComponent<TransformComponent>(playButton.get())->SetLocalPosition(320.f, 230.f, 0.f);
	playButton->AddComponent<MenuButtonComponent>(playButton.get())->SetTexture("UI/Solo.png");

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

	LoadCubes(scene, mgState.get());

	//Q-BERT
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>();

	TextureComponent* pTexture = go->AddComponent<TextureComponent>(go.get());
	pTexture->SetTexture("Q-Bert/Q-BertLeftDownStand.png");

	UIComponent* pUIcomp = go->AddComponent<UIComponent>(go.get());

	TransformComponent* pTransformComp = go->GetComponent<TransformComponent>();
	pTransformComp->SetLocalPosition(320.f, 50.f, 0.f);

	go->AddComponent<GravityComponent>(go.get());
	go->AddComponent<PlayerComponent>(go.get());

	PlayerCollisionComponent* pPlayerCollisionComponent = go->AddComponent<PlayerCollisionComponent>(go.get());
	glm::vec2 bottomLeft = { -3.f, 0.f };
	glm::vec2 bottomRight = { 3.f, 0.f };
	glm::vec2 topLeft = { -3.f, -5.f };
	glm::vec2 topRight = { 3.f, -5.f };
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
	
	pSS->Load(0, "Sounds/fall.mp3", 16);
	pSS->Load(1, "Sounds/jump.wav", 16);
}

void LoadCubes(dae::Scene& scene, MainGameState* pMainGameState)
{
	const std::string blockName{ "Cubes/Cube_Blue_1.png" };
	glm::vec3 position{ 320.f, 200.f, 0.f };

	glm::vec2 bottomLeft{ -4.f, -24.f };
	glm::vec2 bottomRight{ 4.f, -24.f };
	glm::vec2 topLeft{ -4.f, -26.f };
	glm::vec2 topRight{ 4.f, -26.f };

	glm::ivec2 textureSize{};

	std::shared_ptr<dae::Texture2D> pNormalTexture = ResourceManager::GetInstance().LoadTexture("Cubes/Cube_Blue_1.png");
	std::shared_ptr<dae::Texture2D> pIntermediateTexture = ResourceManager::GetInstance().LoadTexture("Cubes/Cube_Green_1.png");
	std::shared_ptr<dae::Texture2D> pGoalTexture = ResourceManager::GetInstance().LoadTexture("Cubes/Cube_Yellow_1.png");

	for (int i{ 1 }; i < 8; ++i)
	{
		for (int j{}; j < i; ++j)
		{
			//load cube
			std::shared_ptr<GameObject> pCube = std::make_shared<GameObject>();
			pCube->MakeSubject();

			TextureComponent* pTextureComponent = pCube->AddComponent<TextureComponent>(pCube.get());
			pTextureComponent->SetTexture(blockName);
			textureSize = pTextureComponent->GetTextureSize();

			pCube->GetComponent<TransformComponent>()->SetLocalPosition(position);
			pCube->AddComponent<CollisionComponent>(pCube.get())->SetPoints(bottomLeft, bottomRight, topLeft, topRight);

			CubeComponent* pCubeComponent = pCube->AddComponent<CubeComponent>(pCube.get());
			pCubeComponent->SetCubeType(dae::CubeComponent::CubeType::SwitchBack);
			pCubeComponent->SetFirstTexture(pNormalTexture);
			pCubeComponent->SetSecondTexture(pIntermediateTexture);
			pCubeComponent->SetThirdTexture(pGoalTexture);

			pMainGameState->AddCube(pCubeComponent);

			if (j == 0)
				pCubeComponent->SetIsLeftEdgeCube();
			if (!(j + 1 < i))
				pCubeComponent->SetIsRightEdgeCube();
			if (i == 7)
				pCubeComponent->SetIsBottomEdgeCube();

			scene.Add(pCube);

			//move position for next cube
			if (i == 1)
				position.x -= textureSize.x * 0.5f;
			else if (!(j + 1 < i))
				position.x -= textureSize.x * (j + 0.5f);
			else
				position.x += textureSize.x;
		}
		position.y += textureSize.y * 0.75f;
	}
}

void WriteBinaryFile()
{
	BinaryFileWriter soloLevel01{ "Level01Solo" };
	//---BLOCK 1---
	char nrOfPlayers{ 1 };
	soloLevel01.Write(&nrOfPlayers, sizeof(char));

	float pos[2]{ 320.f, 50.f };
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));

	//---BLOCK 2---
	CubeComponent::CubeType cubeType = CubeComponent::CubeType::Normal;
	soloLevel01.Write(reinterpret_cast<char*>(cubeType), sizeof(cubeType));

	//round 1
	
	//round 2
	
	//round 3
	
	//round 4
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}