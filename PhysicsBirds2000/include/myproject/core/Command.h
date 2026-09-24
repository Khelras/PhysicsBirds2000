/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Command.h
Description : Declares and Defines the Command Class Interface and Template
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

// Forward Declarations
class StateManager;
class SceneManager;

/// <summary>
///		A struct to hold any necessary context information for Command execution.
///		This can be extended as needed to include relevant data that may be required by the Command.
/// </summary>
struct CommandContext
{
	/// <summary>
	///		Pointer to the SFML RenderWindow, which can be used by the Command to interact with the window.
	/// </summary>
	sf::RenderWindow* window;

	/// <summary>
	///		Reference to the StateManager, which can be used by the Command to interact with the state management system.
	/// </summary>
	StateManager* stateManager;

	/// <summary>
	///		Reference to the SceneManager, which can be used by the Command to interact with the scene managment system.
	/// </summary>
	SceneManager* sceneManager;
};

/// <summary>
///		Command class that links a raw SFML event to an action.
/// </summary>
class Command
{
private:
	// -- Command Properties -- //
	std::function<bool(const sf::Event&)> m_match;
	std::function<void(const CommandContext&)> m_action;
	// -- //

public:
	/// <summary>
	///		Constructs a Command given an event and a callable object and essentially
	///		linking the the two together.
	/// </summary>
	/// 
	/// <param name="match">A function, lambda, or functor that defines the execution criteria of this Command.</param>
	/// <param name="action">A function, lambda, or functor that defines the action to execute.</param>
	Command(std::function<bool(const sf::Event&)> match, std::function<void(const CommandContext&)> action)
		: m_match(match), m_action(std::move(action))
	{};

	/// <summary>
	///		Default Destructor.
	/// </summary>
	~Command() = default;

	/// <summary>
	///		Checks whether the given SFML event matches the execution criteria to execute this Command.
	/// </summary>
	/// 
	/// <param name="event">A constant reference to an SFML event to be checked against the exectuion criteria.</param>
	/// 
	/// <returns>
	///		True if the event matches the execution criteria and the Command should execute; false otherwise.
	/// </returns>
	inline bool match(const sf::Event& event)
	{
		return this->m_match(event);
	}

	/// <summary>
	///		Executes the stored callable.
	/// </summary>
	inline void operator()(const CommandContext& ctx)
	{
		this->m_action(ctx);
	}
};