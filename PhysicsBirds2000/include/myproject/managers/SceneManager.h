/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : SceneManager.h
Description : Declares the SceneManager Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <map>
#include <stack>
#include <memory>

// Forward Declarations
struct CommandContext;
struct StateContext;

// Scenes
class Scene;

/// <summary>
///		SceneManager class is responsible for managing various scenes and 
/// </summary>
class SceneManager
{
private:
	// -- Scene Manager Properties -- //
	std::map<std::string, std::shared_ptr<Scene>> m_sceneRegistry;
	std::shared_ptr<Scene> m_currentScene;
	// -- //

public:
	/// <summary>
	///		Constructs the Scene Manager class and registers all the scenes (pre-defined).
	/// </summary>
	SceneManager();

	/// <summary>
	///		Default destructor.
	/// </summary>
	~SceneManager() = default;

	/// <summary>
	///		Handle the given event in relation to the current active scene.
	/// </summary>
	/// 
	/// <param name="event">The raw SFML event.</param>
	/// <param name="ctx">Context information provided for any command execution.</param>
	void handleEvent(const sf::Event& event, CommandContext& ctx);

	/// <summary>
	///		Update loop for the Scene Manager
	/// </summary>
	/// 
	/// <param name="ctx">Context information that is provided from the StateManager and is used by the Scene.</param>
	void update(StateContext ctx);

	/// <summary>
	///     Renders the current active scene and its contents to the Window.
	/// </summary>
	/// 
	/// <param name="ctx">Context information that is provided from the StateManager and is used by the Scene.</param>
	void render(StateContext ctx);

	/// <summary>
	///		Get the current active scene.
	/// </summary>
	/// 
	/// <returns>The current active scene as a weak pointer.</returns>
	std::weak_ptr<Scene> getCurrentScene();

	/// <summary>
	///		Transition to another scene by a given name.	
	/// </summary>
	/// 
	/// <param name="sceneName"The name of the scene to switch to.></param>
	void goToScene(const std::string& sceneName);
};
