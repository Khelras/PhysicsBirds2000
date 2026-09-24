/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : StateManager.cpp
Description : Defines the StateManager Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <iostream>
#include "myproject/managers/StateManager.h"
#include "myproject/core/Command.h"
#include "myproject/states/State.h"
#include "myproject/states/StateMain.h"

StateManager::StateManager()
{
	// Register States
	this->m_stateRegistry.emplace("MainState", std::make_shared<StateMain>());

	// Start with the Main State
	this->m_stateStack.push(this->m_stateRegistry.at("MainState"));
}

void StateManager::handleEvent(const sf::Event& event, CommandContext& ctx)
{
	// Ensure the State Stack is NOT empty
	if (this->m_stateStack.empty()) return;

	// Send the Event to the current Active State for State-Related Processing.
	this->m_stateStack.top()->handleEvent(event, ctx);
}

void StateManager::update(StateContext ctx)
{
	// Ensure the State Stack is NOT empty
	if (this->m_stateStack.empty()) return;

	// Call the Update method of the current Active State for State-Related Updating.
	this->m_stateStack.top()->update(ctx);
}

void StateManager::render(StateContext ctx)
{
	// Ensure the State Stack is NOT empty
	if (this->m_stateStack.empty()) return;

	// Call the Render method of the current Active State for State-Related Rendering.
	this->m_stateStack.top()->render(ctx);
}

std::weak_ptr<State> StateManager::getActiveState()
{
	// Check if the State Stack is empty
	if (this->m_stateStack.empty())
	{
		// Return Empty Weak Pointer
		return std::weak_ptr<State>();
	}

	// Return a Weak Pointer to the current Active State (Top of the Stack)
	return std::weak_ptr<State>(this->m_stateStack.top());
}

void StateManager::goToState(const std::string& stateName)
{
	// Check if the requested state exists in the registry
	if (this->m_stateRegistry.find(stateName) != this->m_stateRegistry.end())
	{
		// If there is a current Active State, call its onExit() method for cleanup
		if (!this->m_stateStack.empty())
		{
			this->m_stateStack.top()->onExit();
			this->m_stateStack.pop();
		}

		// Push the new state onto the stack and call its onEnter() method for initialization
		this->m_stateStack.push(this->m_stateRegistry.at(stateName));
		this->m_stateStack.top()->onEnter();
	} 
	else
	{
		// DEBUG
		std::cerr << "State '" << stateName << "' not found in the state registry!" << std::endl;
	}
}
