/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : StateMain.cpp
Description : Defines the StateMain Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "myproject/states/StateMain.h"
#include "myproject/managers/SceneManager.h"
#include "myproject/scenes/Scene.h"

StateMain::StateMain()
{
	// Scene Manager
	this->m_sceneManager = new SceneManager();

	// Register the Commands
	this->registerCommands();
}

StateMain::~StateMain()
{
	// Check if Scene Manager exists
	if (this->m_sceneManager != nullptr)
	{
		// Delete the Scene Manager
		delete (this->m_sceneManager);
		this->m_sceneManager = nullptr;
	}
}

void StateMain::registerCommands()
{
	// DISCLAIMER
	// REGISTERING COMMANDS IN THE CONTEXT OF A STATE WILL MAKE IT STATE GLOBAL
	// MEANING IT WILL BE AVAILABLE TO ALL SCENES UNDER THIS STATE
}

void StateMain::handleEvent(const sf::Event& event, CommandContext& ctx)
{
	// Loop through all Registered Commands and check if any of them match the given SFML Event
	for (auto& command : this->m_commands)
	{
		// Check if this Registered Command matches the given SFML Event
		if (command.match(event))
		{
			// Execute the Registered Command's action using the provided context
			command(ctx);
			return;
		}
	}

	// Otherwise, Send the Command down to the Scene
	ctx.sceneManager = this->m_sceneManager;
	this->m_sceneManager->handleEvent(event, ctx);
}

void StateMain::update(StateContext ctx)
{
	this->m_sceneManager->update(ctx);
}

void StateMain::render(StateContext ctx)
{
	this->m_sceneManager->render(ctx);
}

sf::Color StateMain::getBackgroundColor()
{
	// Check if there is a current active Scene
	if (this->m_sceneManager->getCurrentScene().lock() != nullptr)
	{
		// Return the Background Color of the current Active Scene
		return this->m_sceneManager->getCurrentScene().lock()->getBackgroundColor();
	}
	// There was no current active Scene
	else
	{
		// Return the Background Color of the State
		return this->m_backgroundColor;
	}
}
