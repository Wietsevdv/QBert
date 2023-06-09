//#include <SDL.h>
//
//#if _DEBUG
//// ReSharper disable once CppUnusedIncludeDirective
//#if __has_include(<vld.h>)
//#include <vld.h>
//#endif
//#endif
//
//#include "Minigin.h"
//#include "SceneManager.h"
//#include "ResourceManager.h"
//#include "InputManager.h"
//#include "Scene.h"
//
//#include "GameObject.h"
//#include "Components.h"
//#include "Commands.h"
//#include "Observers.h"
//
//void load()
//{
//	std::cout << "\n------CONTROLS------\n";
//	std::cout << "Controller B to lose a life\n";
//	std::cout << "Controller X to increase score\n";
//	std::cout << "No keyboard support\n";
//	std::cout << "You can set achievements again without error. But I still added a check assuming its more performant\n";
//	std::cout << "\nconnecting 2 controllers made steam set the controllers to spacewars and input doesn't work anymore\n";
//	std::cout << "Shift+tab and selecting standard 360 controller for both fixes this\n\n";
//
//	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
//
//	//BACKGROUND
//	auto go = std::make_shared<dae::GameObject>();
//	auto pTextureComponent = go->AddComponent<dae::TextureComponent>(go.get());
//	pTextureComponent->SetTexture("background.tga");
//	scene.Add(go);
//
//	//FPS
//	go = std::make_shared<dae::GameObject>();
//	auto pTextComponent = go->AddComponent<dae::TextComponent>(go.get());
//	pTextComponent->SetSize(25);
//	pTextComponent->SetColor(255, 255, 0);
//	go->AddComponent<dae::FPSComponent>(go.get());
//	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(10, 10, 0);
//	scene.Add(go);
//
//	//Adding snake first so a single connected controller is set to snake
//	//------Snake------
//	// 
//	//---Snake_LiveDisplay---
//	auto pSnakeLiveDisplay = std::make_shared<dae::GameObject>();
//	auto QSnakeLiveTextComp = pSnakeLiveDisplay->AddComponent<dae::TextComponent>(pSnakeLiveDisplay.get());
//	pSnakeLiveDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(5.f, 350.f, 0.f);
//
//	QSnakeLiveTextComp->SetColor(0, 255, 0);
//	QSnakeLiveTextComp->SetText("Lives: 3");
//	QSnakeLiveTextComp->SetSize(20);
//
//	scene.Add(pSnakeLiveDisplay);
//
//	//---Snake_PointDisplay---
//	auto pSnakeScoreDisplay = std::make_shared<dae::GameObject>();
//	auto pSnakeScoreTextComp = pSnakeScoreDisplay->AddComponent<dae::TextComponent>(pSnakeScoreDisplay.get());
//	pSnakeScoreDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(5.f, 370.f, 0.f);
//
//	pSnakeScoreTextComp->SetColor(0, 255, 0);
//	pSnakeScoreTextComp->SetText("Score: 0");
//	pSnakeScoreTextComp->SetSize(20);
//
//	scene.Add(pSnakeScoreDisplay);
//
//	//---Snake_Character---
//	auto goSnake = std::make_shared<dae::GameObject>();
//
//	auto pTransformComponent = goSnake->GetComponent<dae::TransformComponent>();
//	goSnake->AddComponent<dae::PlayerComponent>(goSnake.get());
//	pTextureComponent = goSnake->AddComponent<dae::TextureComponent>(goSnake.get());
//	goSnake->AddComponent<dae::ControllerComponent>(goSnake.get());
//	auto pSnakeMovementComponent = goSnake->AddComponent<dae::MovementComponent>(goSnake.get());
//
//	pTextureComponent->SetTexture("Snake.png");
//	pTransformComponent->SetLocalPosition(270, 350, 0);
//
//	//---observers---
//	auto pSnakeLifeObserver = std::make_shared<dae::LivesDisplay>(QSnakeLiveTextComp);
//	scene.Add(pSnakeLifeObserver);
//	auto pSnakeScoreObserver = std::make_shared<dae::ScoreDisplay>(pSnakeScoreTextComp);
//	scene.Add(pSnakeScoreObserver);
//
//	dae::Subject* pSnakeSubject = goSnake->MakeSubject();
//	pSnakeSubject->AddObserver(pSnakeLifeObserver.get());
//	pSnakeSubject->AddObserver(pSnakeScoreObserver.get());
//
//	scene.Add(goSnake);
//
//	//------Q-Bert------
//	// 
//	//---Q-Bert_LiveDisplay---
//	auto pQBertLiveDisplay = std::make_shared<dae::GameObject>();
//	auto pQBertLiveTextComp = pQBertLiveDisplay->AddComponent<dae::TextComponent>(pQBertLiveDisplay.get());
//	pQBertLiveDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(5.f, 300.f, 0.f);
//
//	pQBertLiveTextComp->SetColor(255, 255, 0);
//	pQBertLiveTextComp->SetText("Lives: 3");
//	pQBertLiveTextComp->SetSize(20);
//
//	scene.Add(pQBertLiveDisplay);
//
//	//---Q-Bert_PointDisplay---
//	auto pQBertScoreDisplay = std::make_shared<dae::GameObject>();
//	auto pQBertScoreTextComp = pQBertScoreDisplay->AddComponent<dae::TextComponent>(pQBertScoreDisplay.get());
//	pQBertScoreDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(5.f, 320.f, 0.f);
//
//	pQBertScoreTextComp->SetColor(255, 255, 0);
//	pQBertScoreTextComp->SetText("Score: 0");
//	pQBertScoreTextComp->SetSize(20);
//
//	scene.Add(pQBertScoreDisplay);
//
//	//---Q-Bert_Character---
//	auto goQBert = std::make_shared<dae::GameObject>();
//
//	pTransformComponent = goQBert->GetComponent<dae::TransformComponent>();
//	goQBert->AddComponent<dae::PlayerComponent>(goQBert.get());
//	pTextureComponent = goQBert->AddComponent<dae::TextureComponent>(goQBert.get());
//	goQBert->AddComponent<dae::ControllerComponent>(goQBert.get());
//	auto pQBertMovementComponent = goQBert->AddComponent<dae::MovementComponent>(goQBert.get());
//
//	pTextureComponent->SetTexture("Q-Bert.png");
//	pTransformComponent->SetLocalPosition(300, 350, 0);
//	pQBertMovementComponent->SetSpeed(100.f);
//
//	//---Observers---
//	auto pQBertLifeObserver = std::make_shared<dae::LivesDisplay>(pQBertLiveTextComp);
//	auto pQBertScoreObserver = std::make_shared<dae::ScoreDisplay>(pQBertScoreTextComp);
//	//auto pQBertAchievementsObserver = std::make_shared<dae::Achievements>(g_pSteamAchievements);
//	scene.Add(pQBertLifeObserver);
//	scene.Add(pQBertScoreObserver);
//	//scene.Add(pQBertAchievementsObserver);
//
//	dae::Subject* pQBertSubject = goQBert->MakeSubject();
//	pQBertSubject->AddObserver(pQBertLifeObserver.get());
//	pQBertSubject->AddObserver(pQBertScoreObserver.get());
//	//pQBertSubject->AddObserver(pQBertAchievementsObserver.get());
//
//	scene.Add(goQBert);
//
//	//double snake movement to qbert
//	pSnakeMovementComponent->SetSpeed(pQBertMovementComponent->GetSpeed() * 2);
//
//	//------INPUT------
//	dae::InputManager& inputManager = dae::InputManager::GetInstance();
//	inputManager.BindButtonToCommand(dae::ControllerButtons::DDown, std::make_unique<dae::AddDirectionCommand>());
//	inputManager.BindButtonToCommand(dae::ControllerButtons::DUp, std::make_unique<dae::AddDirectionCommand>());
//	inputManager.BindButtonToCommand(dae::ControllerButtons::DLeft, std::make_unique<dae::AddDirectionCommand>());
//	inputManager.BindButtonToCommand(dae::ControllerButtons::DRight, std::make_unique<dae::AddDirectionCommand>());
//
//	inputManager.BindButtonToCommand(dae::ControllerButtons::B, std::make_unique<dae::LoseLifePointCommand>());
//	inputManager.BindButtonToCommand(dae::ControllerButtons::X, std::make_unique<dae::IncreaseScoreCommand>());
//}
//
//int main(int, char*[]) {
//	dae::Minigin engine("../Data/");
//	engine.Run(load);
//
//    return 0;
//}