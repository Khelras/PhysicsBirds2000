/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : StateMain.h
Description : Declares the StateMain Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include "State.h"

// Forward Declarations
class SceneManager;

/// <summary>
///		The Main State class.
/// </summary>
class StateMain : public State
{
private:
	// -- Managers -- //
	SceneManager* m_sceneManager;
	// -- //

	// -- Main State Properties -- //
	
	// -- //

public:
	/// <summary>
	///		Constructor.
	/// </summary>
	StateMain();

	/// <summary>
	///		Destructor.
	/// </summary>
	~StateMain();

	/// <summary>
	///     Called in the constructor to register all Commands.
	/// </summary>
	void registerCommands() override;

	/// <summary>
	///     Check the given event against all Commands and their execution criterias.
	/// </summary>
	/// 
	/// <param name="event">The raw SFML event.</param>
	/// <param name="ctx">Context information provided for command execution.</param>
	void handleEvent(const sf::Event& event, CommandContext& ctx) override;

	/// <summary>
	///     Updates state logic.
	/// </summary>
	/// 
	/// <param name="ctx">Context information that be used by the State.</param>
	void update(StateContext ctx) override;

	/// <summary>
	///     Renders state contents to the Window.
	/// </summary>
	/// 
	/// <param name="ctx">Context information that be used by the State.</param>
	void render(StateContext ctx) override;

	/// <summary>
	///     Return the background color of the state.
	/// </summary>
	/// 
	/// <returns>Background color</returns>
	sf::Color getBackgroundColor() override;
};