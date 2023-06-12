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
#include "Services.h"

#include "SDL_mixer.h"
#include <thread>

#include <iostream>

#include "ProjectComponents.h"
#include "ProjectCommands.h"

#include "GameStateMachine.h"

#include "BinaryFileWriter.h"

using namespace dae;

void WriteBinaryFile();

void load()
{
	//WriteBinaryFile();

	//Create GameStateMachine
	std::shared_ptr<GameStateMachine> pGameStateMachine = std::make_shared<GameStateMachine>();
	StateMachineManager::GetInstance().AddStateMachine(pGameStateMachine);

	std::shared_ptr<MenuState> pMenu = std::make_shared<MenuState>();
	std::shared_ptr<LoadSoloState> pLoadSolo = std::make_shared<LoadSoloState>();

	pGameStateMachine->AddState(pMenu, true);
	pGameStateMachine->AddState(pLoadSolo);
	pGameStateMachine->AddTransition(std::make_shared<LoadSoloTransition>(pMenu.get(), pLoadSolo.get()));

	//SOUND
	dae::ServiceLocator::RegisterSoundSystem(new dae::SDL_SoundSystem());
	dae::SoundSystem* pSS = dae::ServiceLocator::GetSoundSystem();
	
	pSS->Load(0, "Sounds/fall.mp3", 16);
	pSS->Load(1, "Sounds/jump.wav", 16);
}

void WriteBinaryFile()
{
	BinaryFileWriter soloLevel01{ "Level01Solo.bin" };
	//---BLOCK 1---
	char nrOfPlayers{ 1 };
	soloLevel01.Write(&nrOfPlayers, sizeof(nrOfPlayers));

	float pos[2]{ 320.f, 50.f };
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));

	//---BLOCK 2---
	CubeComponent::CubeType cubeType = CubeComponent::CubeType::Normal;
	soloLevel01.Write(reinterpret_cast<char*>(&cubeType), sizeof(cubeType));

	//---round 1---
	std::string cubeTextureFile{ "Cube01_Blue.png" };
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1); //+1 to also write the null terminator
	cubeTextureFile = "Cube01_Red.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);
	cubeTextureFile = "Cube01_Yellow.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);

	char nrOfDiscs{ 2 };
	soloLevel01.Write(&nrOfDiscs, sizeof(nrOfDiscs));
	pos[0] = 240.f;
	pos[1] = 272.f;
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));
	pos[0] = 400.f;
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));

	//---round 2---
	cubeTextureFile = "Cube02_Blue.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);
	cubeTextureFile = "Cube02_Green.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);
	cubeTextureFile = "Cube02_Yellow.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);

	nrOfDiscs = 2;
	soloLevel01.Write(&nrOfDiscs, sizeof(nrOfDiscs));
	pos[0] = 224.f;
	pos[1] = 296.f;
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));
	pos[0] = 432.f;
	pos[1] = 320.f;
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));

	//---round 3---
	cubeTextureFile = "Cube03_Black.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);
	cubeTextureFile = "Cube03_Blue.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);
	cubeTextureFile = "Cube03_White.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);

	nrOfDiscs = 2;
	soloLevel01.Write(&nrOfDiscs, sizeof(nrOfDiscs));
	pos[0] = 256.f;
	pos[1] = 248.f;
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));
	pos[0] = 416.f;
	pos[1] = 296.f;
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));

	//---round 4---
	cubeTextureFile = "Cube04_Blue.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);
	cubeTextureFile = "Cube04_Yellow.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);
	cubeTextureFile = "Cube04_Purple.png";
	soloLevel01.Write(cubeTextureFile.c_str(), cubeTextureFile.size() + 1);

	nrOfDiscs = 2;
	soloLevel01.Write(&nrOfDiscs, sizeof(nrOfDiscs));
	pos[0] = 304.f;
	pos[1] = 268.f;
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));
	pos[0] = 432.f;
	pos[1] = 320.f;
	soloLevel01.Write(reinterpret_cast<char*>(&pos), sizeof(pos));
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}