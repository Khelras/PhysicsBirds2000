/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : SceneMain.h
Description : Declares the SceneMain Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <memory>
#include <SFML/System/Clock.hpp>
#include "Scene.h"

// Forward Declarations
class b2World;
class b2Body;
class Slingshot;
class Bird;

/// <summary>
///     The main scene which derives from the base scene class.
/// </summary>
class SceneMain : public Scene
{
protected:
    // -- Main Scene Properties -- //
    std::unique_ptr<b2World> m_world; // Box2D World for Physics Simulation
    b2Body* m_wallBody; // Box2D Body for the Wall

    std::unique_ptr<Slingshot> m_slingshot;
    std::vector<std::unique_ptr<Bird>> m_birds;
    Bird* m_currentBird = nullptr;
    int m_birdsRemaining = 0;
    bool m_wasMouseDown = false;

    sf::Clock m_clock;
    float m_accumulator = 0.0f;
    // -- //

public:
    /// <summary>
    ///     Constructor.
    /// </summary>
    SceneMain();

    /// <summary>
    ///     Destructor
    /// </summary>
    ~SceneMain();

    /// <summary>
    ///     Called in the constructor to register all Commands.
    /// </summary>
    void registerCommands() override;

    /// <summary>
    ///     Updates scene logic.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the SceneMain.</param>
    void update(StateContext ctx) override;

    /// <summary>
    ///     Renders scene contents to the Window.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the SceneMain.</param>
    void render(StateContext ctx) override;

// PRIVATE HELPER FUNCTIONS
private:
    /// <summary>
    ///     Creates the Box2D World and the static window edge Body.
    /// </summary>
    void createPhysicsWorld();

    /// <summary>
    ///     Creates the next Bird in the Slingshot, if any remain.
    /// </summary>
    void loadNextBird();

    /// <summary>
    ///     Removes all Birds and reloads the full set.
    /// </summary>
    void resetBirds();

    /// <summary>
    ///     Reads the Mouse and drives the grab / drag / launch logic.
    /// </summary>
    void handleSlingshotInput(StateContext ctx);
};