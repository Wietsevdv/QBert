#include "GameStateMachine.h"
#include "Scene.h"
#include "GameObject.h"
#include "ProjectComponents.h"
#include "ProjectCommands.h"
#include "BinaryFileReader.h"
#include <string>

dae::GameStateMachine::GameStateMachine() 
	: StateMachine{}
	, m_pPlayer{ nullptr }
{
}

void dae::GameStateMachine::AddState(std::shared_ptr<BaseState> pNewState, bool setActive)
{
	if (GameStateBase* pGameState = dynamic_cast<GameStateBase*>(pNewState.get()))
		pGameState->m_pStateMachine = this;

	StateMachine::AddState(pNewState, setActive);
}

dae::MenuState::MenuState()
	: GameStateBase{}
	, m_pSoloButton{ nullptr }
	, m_LoadSolo{ false }
{
}

void dae::MenuState::OnEnter()
{
	auto& mainMenuScene = SceneManager::GetInstance().CreateScene("MenuScene");

	m_pSoloButton = std::make_shared<GameObject>();
	m_pSoloButton->MakeSubject();
	m_pSoloButton->AddComponent<TransformComponent>(m_pSoloButton.get())->SetLocalPosition(320.f, 230.f, 0.f);
	m_pSoloButton->AddComponent<MenuButtonComponent>(m_pSoloButton.get())->SetTexture("UI/Solo.png");

	m_pSoloButton->GetSubject()->AddObserver(this);

	mainMenuScene.Add(m_pSoloButton);

	std::shared_ptr<SDL_Event> pPlayClickedEvent = std::make_shared<SDL_Event>();
	pPlayClickedEvent->type = SDL_MOUSEBUTTONDOWN;
	pPlayClickedEvent->button.button = SDL_BUTTON_LEFT;

	InputManager::GetInstance().BindSDLEventToCommand<ClickCommand>(m_pSoloButton.get(), pPlayClickedEvent);
}

void dae::MenuState::Notify(const GameObject& gameObject, void*)
{
	if (&gameObject == m_pSoloButton.get())
		m_LoadSolo = true;
}

dae::GameStateMachine* dae::GameStateBase::GetStateMachine()
{
	return m_pStateMachine;
}

dae::LoadSoloState::LoadSoloState()
	: GameStateBase{}
{
}

void dae::LoadSoloState::OnEnter()
{
	//std::ifstream soloLevel01{ "Level01Solo.bin", std::ios::binary };

	////block 1
	//char* nrOfPlayers{};
	//soloLevel01.read(nrOfPlayers, 0);
	//float position[2]{};
	//soloLevel01.read(reinterpret_cast<char*>(position), sizeof(position));

	////block 2
	//CubeComponent::CubeType* pCubeType{ nullptr };
	//soloLevel01.read(reinterpret_cast<char*>(pCubeType), sizeof(CubeComponent::CubeType));

	//std::vector<std::string> textures{};
	////round 1
	//for (int i{}; i < 3; ++i)
	//{
	//	std::string texture{};
	//	std::getline(soloLevel01, texture, '\0');
	//	textures.emplace_back(texture);
	//}

	auto& mainMenuScene = SceneManager::GetInstance().CreateScene("MainGameScene");

	//Q-BERT
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>();

	TextureComponent* pTexture = go->AddComponent<TextureComponent>(go.get());
	pTexture->SetTexture("Q-Bert/Q-BertLeftDownStand.png");

	TransformComponent* pTransformComp = go->GetComponent<TransformComponent>();
	pTransformComp->SetLocalPosition(320.f, 50.f, 0.f);

	go->GetComponent<RenderComponent>()->SetLayer(2);

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

	mainMenuScene.Add(go);

	LoadCubes(mainMenuScene);
}

void dae::LoadSoloState::LoadCubes(dae::Scene& scene)
{
	glm::vec3 position{ 320.f, 200.f, 0.f };

	glm::vec2 bottomLeft{ -4.f, -24.f };
	glm::vec2 bottomRight{ 4.f, -24.f };
	glm::vec2 topLeft{ -4.f, -26.f };
	glm::vec2 topRight{ 4.f, -26.f };

	glm::ivec2 textureSize{};

	//SWITCH THESE WITH WHAT YOU READ FROM FILE
	std::shared_ptr<dae::Texture2D> pStartTexture = ResourceManager::GetInstance().LoadTexture("Cubes/Cube01_Blue.png");
	std::shared_ptr<dae::Texture2D> pIntermediateTexture = ResourceManager::GetInstance().LoadTexture("Cubes/Cube01_Yellow.png");
	std::shared_ptr<dae::Texture2D> pGoalTexture = ResourceManager::GetInstance().LoadTexture("Cubes/Cube01_Red.png");

	for (int i{ 1 }; i < 8; ++i)
	{
		for (int j{}; j < i; ++j)
		{
			//load cube
			std::shared_ptr<GameObject> pCube = std::make_shared<GameObject>();
			pCube->MakeSubject();

			TextureComponent* pTextureComponent = pCube->AddComponent<TextureComponent>(pCube.get());
			pTextureComponent->SetTexture(pStartTexture);
			textureSize = pTextureComponent->GetTextureSize();

			pCube->GetComponent<TransformComponent>()->SetLocalPosition(position);
			pCube->AddComponent<CollisionComponent>(pCube.get())->SetPoints(bottomLeft, bottomRight, topLeft, topRight);

			CubeComponent* pCubeComponent = pCube->AddComponent<CubeComponent>(pCube.get());

			//SWITCH THIS WITH ONE READ FROM FILE
			pCubeComponent->SetCubeType(dae::CubeComponent::CubeType::SwitchBack);

			pCubeComponent->SetFirstTexture(pStartTexture);
			pCubeComponent->SetSecondTexture(pIntermediateTexture);
			pCubeComponent->SetThirdTexture(pGoalTexture);

			pCubeComponent->SetStartTexture(1);
			pCubeComponent->SetIntermediateTexture(2);
			pCubeComponent->SetGoalTexture(3);

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

bool dae::LoadSoloTransition::Check(float)
{
	if (MenuState* pMenu = dynamic_cast<MenuState*>(GetFrom()))
	{
		if (pMenu->GetLoadSolo())
			return true;
	}
	
	return false;
}