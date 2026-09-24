/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Scene.h
Description : Declares and Defines the Scene Base Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "myproject/core/Command.h"
#include "myproject/states/State.h"

/// <summary>
///     Base Scene class representing a self-contained scene of the application.
///     A Scene groups together elements such as objects, UI, logic, and assets relevant to that scene of the application.
///     Each Scene defines their own update and rendering loop based on their group of elements.
/// </summary>
class Scene
{
protected:
    // -- Scene Properties -- //
    std::vector<Command> m_commands;
    sf::Color m_backgroundColor = sf::Color::Black; // Default to Black Background Color
    // -- //

public:
    /// <summary>
    ///     Default Constructor.
    /// </summary>
    Scene() = default;

    /// <summary>
    ///     Virtual Destructor for safe polymorphic destruction.
    /// </summary>
    virtual ~Scene() = default;

    /// <summary>
    ///     Called when the scene is first created or entered.
    ///     Use for initialization.
    /// </summary>
    virtual void onEnter() {};

    /// <summary>
    ///     Called when the Scene is being exited or replaced.
    ///     Use for cleanup upon exiting.
    /// </summary>
    virtual void onExit() {};

    /// <summary>
    ///     Called in the constructor to register all Commands.
    /// </summary>
    virtual void registerCommands() = 0;

    /// <summary>
    ///     Check the given event against all Commands and their execution criterias.
    /// </summary>
    /// 
    /// <param name="event">The raw SFML event.</param>
    /// <param name="ctx">Context information provided for command execution.</param>
    virtual inline void handleEvent(const sf::Event& event, const CommandContext& ctx)
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
    ///     Updates scene logic.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the Scene.</param>
    virtual void update(StateContext ctx) = 0;

    /// <summary>
    ///     Renders scene contents to the Window.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the Scene.</param>
    virtual void render(StateContext ctx) = 0;

    /// <summary>
    ///     Return the background color of the scene.
    /// </summary>
    /// 
    /// <returns>Background color</returns>
    sf::Color getBackgroundColor() { return m_backgroundColor; };
};