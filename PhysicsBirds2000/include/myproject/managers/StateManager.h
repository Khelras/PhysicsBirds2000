/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : StateManager.h
Description : Declares the StateManager Class Functions and Properties.
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

// States
class State;
class StateMain;

/// <summary>
///		StateManager class is responsible for managing various states and 
/// </summary>
class StateManager
{
private:
	// -- State Manager Properties -- //
	std::map<std::string, std::shared_ptr<State>> m_stateRegistry;
	std::stack<std::shared_ptr<State>> m_stateStack;
	// -- //

public:
	/// <summary>
	///		Constructs the State Manager class and registers all the states (pre-defined).
	/// </summary>
	StateManager();

	/// <summary>
	///		Default destructor.
	/// </summary>
	~StateManager() = default;

	/// <summary>
	///		Handle the given event in relation to the current active state.
	/// </summary>
	/// 
	/// <param name="event">The raw SFML event.</param>
	/// <param name="ctx">Context information provided for any command execution.</param>
	void handleEvent(const sf::Event& event, CommandContext& ctx);

	/// <summary>
	///		Update loop for the State Manager
	/// </summary>
	/// 
	/// <param name="ctx">Context information that be used by the State.</param>
	void update(StateContext ctx);

	/// <summary>
	///     Renders the current active state and its contents to the Window.
	/// </summary>
	/// 
	/// <param name="ctx">Context information that be used by the State.</param>
	void render(StateContext ctx);

	/// <summary>
	///		Get the current active state.
	/// </summary>
	/// 
	/// <returns>The current active state as a weak pointer.</returns>
	std::weak_ptr<State> getActiveState();

	/// <summary>
	///		Transition to another state by a given name.	
	/// </summary>
	/// 
	/// <param name="stateName"The name of the state to switch to.></param>
	void goToState(const std::string& stateName);
};
