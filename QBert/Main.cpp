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

using namespace dae;

void load()
{
	// First scene created and automatically the active one
	auto& scene = SceneManager::GetInstance().CreateScene("StartScene");

	//Q-BERT
	std::shared_ptr<GameObject> go = std::make_shared<GameObject>();
	
	TextureComponent* pTexture = go->AddComponent<TextureComponent>(go.get());
	pTexture->SetTexture("Q-Bert.png");

	go->AddComponent<ProjectComponent>(go.get());
	UIComponent* pUIcomp = go->AddComponent<UIComponent>(go.get());

	TransformComponent* pTransformComp = go->GetComponent<TransformComponent>();
	pTransformComp->SetLocalPosition(50.f, 50.f, 0.f);

	//go->AddComponent<GravityComponent>(go.get());

	Subject* pSubject = go->MakeSubject();
	pSubject->AddObserver(pUIcomp);

	scene.Add(go);

	pSubject->NotifyObservers(*go.get(), GameEvents::PlayerDied);

	//BLOCK
	std::shared_ptr<GameObject> blockGO = std::make_shared<GameObject>();
	TextureComponent* pBlockTexture = blockGO->AddComponent<TextureComponent>(blockGO.get());
	pBlockTexture->SetTexture("Block_Blue_1.png");

	TransformComponent* pBlockTransformComp = blockGO->GetComponent<TransformComponent>();
	pBlockTransformComp->SetLocalPosition(50.f, 70.f, 0.f);

	scene.Add(blockGO);

	//SOUND
	dae::ServiceLocator::RegisterSoundSystem(new dae::SDL_SoundSystem());
	dae::SoundSystem* pSS = dae::ServiceLocator::GetSoundSystem();
	
	pSS->Load(0, "fall.mp3", 16);

	std::cout << "\n\nPress 'v' to play a sound\n";
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}