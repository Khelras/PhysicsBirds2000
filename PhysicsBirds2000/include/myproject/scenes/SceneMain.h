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

/// <summary>
///     The main scene which derives from the base scene class.
/// </summary>
class SceneMain : public Scene
{
protected:
    // -- Main Scene Properties -- //
    std::unique_ptr<b2World> m_world; // Box2D World for Physics Simulation
    b2Body* m_ballBody; // Box2D Body for the Ball
    b2Body* m_wallBody; // Box2D Body for the Wall
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
    ///     Creates the Box2D World, the ball Body and the static window edge Body.
    /// </summary>
    void createPhysicsWorld();


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
};