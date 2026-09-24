/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : State.h
Description : Declares and Defines the State Base Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "myproject/core/Command.h"

/// <summary>
///		A struct to hold any necessary context information for the update and draw loops of the State.
///		This can be extended as needed to include relevant data that may be required by the State.
/// </summary>
struct StateContext
{
    /// <summary>
	///     Pointer to the SFML RenderWindow, which can be used by the State to interact with the window.
    /// </summary>
    sf::RenderWindow* window;

    /// <summary>
    ///     Delta Time for calculations relative to time.
    /// </summary>
    float dt;
};





/// <summary>
///     Base State class representing a self-contained state of the application.
///     A State groups together elements such as objects, UI, logic, and assets relevant to that state of the application.
///     Each State defines their own update and rendering loop based on their group of elements.
/// </summary>
class State
{
protected:
    // -- State Properties -- //
    std::vector<Command> m_commands;
	sf::Color m_backgroundColor = sf::Color::Black; // Default to a Black Background Color
    // -- //

public:
    /// <summary>
    ///     Default Constructor.
    /// </summary>
    State() = default;

    /// <summary>
    ///     Virtual Destructor for safe polymorphic destruction.
    /// </summary>
    virtual ~State() = default;

    /// <summary>
    ///     Called when the state is first created or entered.
    ///     Use for initialization.
    /// </summary>
    virtual void onEnter() {};

    /// <summary>
    ///     Called when the State is being exited or replaced.
    ///     Use for cleanup upon exiting.
    /// </summary>
    virtual void onExit() {};

    /// <summary>
    ///     Called in the constructor to register all Commands.
    /// </summary>
    virtual void registerCommands() = 0 ;

    /// <summary>
    ///     Check the given event against all Commands and their execution criterias.
    /// </summary>
    /// 
    /// <param name="event">The raw SFML event.</param>
    /// <param name="ctx">Context information provided for command execution.</param>
    virtual inline void handleEvent(const sf::Event& event, CommandContext& ctx) 
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
    }


    /// <summary>
    ///     Updates state logic.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the State.</param>
    virtual void update(StateContext ctx) = 0;

    /// <summary>
    ///     Renders state contents to the Window.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the State.</param>
    virtual void render(StateContext ctx) = 0;

    /// <summary>
    ///     Return the background color of the state.
    /// </summary>
    /// 
    /// <returns>Background color</returns>
    virtual sf::Color getBackgroundColor() { return m_backgroundColor; };
};

