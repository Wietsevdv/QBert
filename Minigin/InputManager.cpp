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
	//ImGui_ImplSDL2_ProcessEvent(&e);

	CheckBoundButtons();
	return CheckBoundEvents();
}

bool dae::InputManager::CheckBoundEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		for (boundSDLEvent& boundEvent : m_BoundSDL_Events)
		{
			SDL_Event* pEvent = boundEvent.pEvent.get();
			if (pEvent->type == e.type && pEvent->key.keysym.sym == e.key.keysym.sym) //keyboard
			{
				boundEvent.pCommand->Execute(m_pControllerComponents[boundEvent.playerIndex]->GetOwner(), nullptr);
			}
			else if (pEvent->type == e.type && pEvent->button.button == e.button.button) //mouse
			{
				Sint32 mousePos[2]{ e.button.x, e.button.y };
	
				boundEvent.pCommand->Execute(m_pControllerComponents[boundEvent.playerIndex]->GetOwner(), mousePos);
			}
		}
	}

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