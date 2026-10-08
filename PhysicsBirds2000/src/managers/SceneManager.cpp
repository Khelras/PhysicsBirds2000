/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : SceneManager.cpp
Description : Defines the SceneManager Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <iostream>

#include "myproject/managers/SceneManager.h"
#include "myproject/scenes/levels/LevelOne.h"
#include "myproject/scenes/levels/LevelTwo.h"

SceneManager::SceneManager()
{
	// Register Scenes
	this->m_sceneRegistry.emplace("LevelOne", std::make_shared<LevelOne>());
	this->m_sceneRegistry.emplace("LevelTwo", std::make_shared<LevelTwo>());

	// Start with the First Scene
	this->m_currentScene = this->m_sceneRegistry.at("LevelOne");
}

void SceneManager::handleEvent(const sf::Event& event, CommandContext& ctx)
{
	// Ensure there is a current Active Scene
	if (this->m_currentScene == nullptr) return;

	// Send the Event to the current Active Scene for Scene-Related Processing.
	this->m_currentScene->handleEvent(event, ctx);
}

void SceneManager::update(StateContext ctx)
{
	// Ensure there is a current Active Scene
	if (this->m_sceneRegistry.empty() == true || this->m_currentScene == nullptr) return;

	// Call the Update method of the current Active Scene for Scene-Related Updating.
	this->m_currentScene->update(ctx);
}

void SceneManager::render(StateContext ctx)
{
	// Ensure there is a current Active Scene
	if (this->m_sceneRegistry.empty() == true|| this->m_currentScene == nullptr) return;

	// Call the Render method of the current Active Scene for Scene-Related Rendering.
	this->m_currentScene->render(ctx);
}

std::weak_ptr<Scene> SceneManager::getCurrentScene()
{
	// Check if there is no Scenes in the Registry
	if (this->m_sceneRegistry.empty() == true || this->m_currentScene == nullptr)
	{
		// Return an Empty Weak Pointer
		return std::weak_ptr<Scene>();
	}

	// Return a Weak Pointer to the current Active Scene
	return std::weak_ptr<Scene>(this->m_currentScene);
}

void SceneManager::goToScene(const std::string& sceneName)
{
	// Check if the requested scene exists in the registry
	if (this->m_sceneRegistry.find(sceneName) != this->m_sceneRegistry.end())
	{
		// If there is a current Active Scene, call its onExit() method for cleanup
		if (this->m_currentScene != nullptr) this->m_currentScene->onExit();

		// Set the new current Active Scene call its onEnter() method for initialization
		this->m_currentScene = this->m_sceneRegistry.at(sceneName);
		this->m_currentScene->onEnter();
	}
	else
	{
		// DEBUG
		std::cerr << "Scene '" << sceneName << "' not found in the scene registry!" << std::endl;
	}
}
