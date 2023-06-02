#include <SDL.h>
#include "InputManager.h"
#include "Components.h"
#include "Services.h"

#include <iostream>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../3rdParty/imgui-1.89.4/backends/imgui_impl_sdl2.h"

void dae::InputManager::AddController(ControllerComponent* pControllerComp)
{
	m_pControllerComponents.emplace_back(pControllerComp);
}

int dae::InputManager::GetNrOfControllers() const
{ 
	return static_cast<int>(m_pControllerComponents.size()); 
}

bool dae::InputManager::ProcessInput()
{
	SoundSystem* pSS = ServiceLocator::GetSoundSystem();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) 
		{
			return false;
		}
		if (e.type == SDL_KEYDOWN) 
		{
			//if (e.key.keysym.sym == SDLK_s)
			//{
			//	glm::vec3 vector{ 0, 1, 0 };
			//	GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
			//	m_ButtonCommandmap.find(ControllerButtons::DDown)->second.get()->Execute(pOwner, &vector);
			//}
			//if (e.key.keysym.sym == SDLK_w)
			//{
			//	glm::vec3 vector{ 0, -1, 0 };
			//	GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
			//	m_ButtonCommandmap.find(ControllerButtons::DUp)->second.get()->Execute(pOwner, &vector);
			//}
			//if (e.key.keysym.sym == SDLK_a)
			//{
			//	glm::vec3 vector{ -1, 0, 0 };
			//	GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
			//	m_ButtonCommandmap.find(ControllerButtons::DLeft)->second.get()->Execute(pOwner, &vector);
			//}
			//if (e.key.keysym.sym == SDLK_d)
			//{
			//	glm::vec3 vector{ 1, 0, 0 };
			//	GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
			//	m_ButtonCommandmap.find(ControllerButtons::DRight)->second.get()->Execute(pOwner, &vector);
			//}
			if (e.key.keysym.sym == SDLK_v)
			{
				pSS->Play(0);
			}
		}
		/*if (e.type == SDL_KEYUP)
		{
			if (e.key.keysym.sym == SDLK_s)
			{
				glm::vec3 vector{ 0, -1, 0 };
				GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
				m_ButtonCommandmap.find(ControllerButtons::DDown)->second.get()->Execute(pOwner, &vector);
			}
			if (e.key.keysym.sym == SDLK_w)
			{
				glm::vec3 vector{ 0, 1, 0 };
				GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
				m_ButtonCommandmap.find(ControllerButtons::DUp)->second.get()->Execute(pOwner, &vector);
			}
			if (e.key.keysym.sym == SDLK_a)
			{
				glm::vec3 vector{ 1, 0, 0 };
				GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
				m_ButtonCommandmap.find(ControllerButtons::DLeft)->second.get()->Execute(pOwner, &vector);
			}
			if (e.key.keysym.sym == SDLK_d)
			{
				glm::vec3 vector{ -1, 0, 0 };
				GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
				m_ButtonCommandmap.find(ControllerButtons::DRight)->second.get()->Execute(pOwner, &vector);
			}
		}*/
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			
		}
		// ImGui
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	CheckBoundButtons();

	return true;
}

void dae::InputManager::CheckBoundButtons()
{
	for (auto boundButton : m_BoundControllerButtons)
	{
		switch (boundButton.buttonState)
		{
			case ButtonState::pressed:
			{
				if (m_pControllerComponents[boundButton.controllerIndex]->GetController()->WentDownThisFrame(boundButton.button))
				{
					boundButton.pCommand->Execute(m_pControllerComponents[boundButton.controllerIndex]->GetOwner(), nullptr);
				}
			}
			break;
			case ButtonState::released:
			{
				if (m_pControllerComponents[boundButton.controllerIndex]->GetController()->WentUpThisFrame(boundButton.button))
				{
					boundButton.pCommand->Execute(m_pControllerComponents[boundButton.controllerIndex]->GetOwner(), nullptr);
				}
			}
			break;
			case ButtonState::held:
			{
				if (m_pControllerComponents[boundButton.controllerIndex]->GetController()->IsPressed(boundButton.button))
				{
					boundButton.pCommand->Execute(m_pControllerComponents[boundButton.controllerIndex]->GetOwner(), nullptr);
				}
			}
			break;
		}
	}
}