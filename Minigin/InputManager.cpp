#include <SDL.h>
#include "InputManager.h"
#include "XInputController.h"

#include <iostream>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../3rdParty/imgui-1.89.4/backends/imgui_impl_sdl2.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) 
		{
			return false;
		}
		if (e.type == SDL_KEYDOWN) 
		{
			if (e.key.keysym.sym == SDLK_s)
			{
				glm::vec3 vector{ 0, 1, 0 };
				GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
				m_ButtonCommandmap.find(ControllerButtons::DDown)->second.get()->Execute(pOwner, &vector);
			}
			if (e.key.keysym.sym == SDLK_w)
			{
				glm::vec3 vector{ 0, -1, 0 };
				GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
				m_ButtonCommandmap.find(ControllerButtons::DUp)->second.get()->Execute(pOwner, &vector);
			}
			if (e.key.keysym.sym == SDLK_a)
			{
				glm::vec3 vector{ -1, 0, 0 };
				GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
				m_ButtonCommandmap.find(ControllerButtons::DLeft)->second.get()->Execute(pOwner, &vector);
			}
			if (e.key.keysym.sym == SDLK_d)
			{
				glm::vec3 vector{ 1, 0, 0 };
				GameObject* pOwner = m_pControllerComponents[1]->GetOwner();
				m_ButtonCommandmap.find(ControllerButtons::DRight)->second.get()->Execute(pOwner, &vector);
			}
		}
		if (e.type == SDL_KEYUP)
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
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			
		}
		// ImGui
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	//-----XINPUT-----			THIS DOES NOT ALLOW NEW KEYS TO BE BOUND TO DIFFERENT ACTIONS. THERE SHOULD NOT BE MORE LOGIC HERE THAN: CHECK IF PRESSED -> EXECUTE
	for (int i{}; i < static_cast<int>(m_pControllerComponents.size()); ++i)
	{
		const XInputController* pController = m_pControllerComponents[i]->GetController();
		GameObject* pOwner = m_pControllerComponents[i]->GetOwner();

		//dying logic
		if (pController->WentDownThisFrame(ControllerButtons::B))
			m_ButtonCommandmap.find(ControllerButtons::B)->second.get()->Execute(pOwner, nullptr);

		//score increase logic
		if (pController->WentDownThisFrame(ControllerButtons::X))
			m_ButtonCommandmap.find(ControllerButtons::X)->second.get()->Execute(pOwner, nullptr);

		//movement logic
		bool moveDown = pController->WentDownThisFrame(ControllerButtons::DDown);
		bool moveUp = pController->WentDownThisFrame(ControllerButtons::DUp);
		bool moveLeft = pController->WentDownThisFrame(ControllerButtons::DLeft);
		bool moveRight = pController->WentDownThisFrame(ControllerButtons::DRight);

		bool stopMoveDown = pController->WentUpThisFrame(ControllerButtons::DDown);
		bool stopMoveUp = pController->WentUpThisFrame(ControllerButtons::DUp);
		bool stopMoveLeft = pController->WentUpThisFrame(ControllerButtons::DLeft);
		bool stopMoveRight = pController->WentUpThisFrame(ControllerButtons::DRight);

		//start movement
		if (moveDown)
		{
			glm::vec3 vector{ 0, 1, 0 };
			m_ButtonCommandmap.find(ControllerButtons::DDown)->second.get()->Execute(pOwner, &vector);
		}
		if (moveUp)
		{
			glm::vec3 vector{ 0, -1, 0 };
			m_ButtonCommandmap.find(ControllerButtons::DUp)->second.get()->Execute(pOwner, &vector);
		}
		if (moveLeft)
		{
			glm::vec3 vector{ -1, 0, 0 };
			m_ButtonCommandmap.find(ControllerButtons::DLeft)->second.get()->Execute(pOwner, &vector);
		}
		if (moveRight)
		{
			glm::vec3 vector{ 1, 0, 0 };
			m_ButtonCommandmap.find(ControllerButtons::DRight)->second.get()->Execute(pOwner, &vector);
		}

		//stop movement
		if (stopMoveDown)
		{
			glm::vec3 vector{ 0, -1, 0 };
			m_ButtonCommandmap.find(ControllerButtons::DDown)->second.get()->Execute(pOwner, &vector);
		}
		if (stopMoveUp)
		{
			glm::vec3 vector{ 0, 1, 0 };
			m_ButtonCommandmap.find(ControllerButtons::DUp)->second.get()->Execute(pOwner, &vector);
		}
		if (stopMoveLeft)
		{
			glm::vec3 vector{ 1, 0, 0 };
			m_ButtonCommandmap.find(ControllerButtons::DLeft)->second.get()->Execute(pOwner, &vector);
		}
		if (stopMoveRight)
		{
			glm::vec3 vector{ -1, 0, 0 };
			m_ButtonCommandmap.find(ControllerButtons::DRight)->second.get()->Execute(pOwner, &vector);
		}
	}

	return true;
}

void dae::InputManager::AddController(ControllerComponent* pControllerComp)
{
	m_pControllerComponents.emplace_back(pControllerComp);
}

void dae::InputManager::BindButtonToCommand(ControllerButtons button, std::unique_ptr<Command> pCommand)
{
	if (m_ButtonCommandmap.find(button) != m_ButtonCommandmap.end())
	{
		std::cout << "Add unbind logic\n";
		return;
	}
	m_ButtonCommandmap.insert(std::pair<ControllerButtons, std::unique_ptr<Command>>(button, std::move(pCommand)));
}
