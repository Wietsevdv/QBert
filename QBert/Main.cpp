//#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
//#include "SceneManager.h"
//#include "ResourceManager.h"
//#include "InputManager.h"
//#include "Scene.h"
//
//#include "GameObject.h"
//#include "Components.h"
//#include "Commands.h"
//#include "Observers.h"

void load()
{
}

int main(int, char* []) {

	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}