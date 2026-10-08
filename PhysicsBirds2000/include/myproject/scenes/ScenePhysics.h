/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : ScenePhysics.h
Description : Declares the ScenePhysics Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <memory>
#include "Scene.h"

// Forward Declarations
class b2World;

/// <summary>
///     The base physics scene which derives from the base scene class.
/// </summary>
class ScenePhysics : public Scene
{
protected:
    // -- Main Scene Properties -- //
	std::unique_ptr<b2World> m_physicsWorld;
    // -- //

public:
    /// <summary>
    ///     Constructor.
    /// </summary>
    ScenePhysics();

    /// <summary>
    ///     Destructor
    /// </summary>
    ~ScenePhysics();

    /// <summary>
    ///     Called in the constructor to register all Commands.
    /// </summary>
    void registerCommands() override;

    /// <summary>
    ///     Updates scene logic.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the ScenePhysics.</param>
    void update(StateContext ctx) override;

    /// <summary>
    ///     Renders scene contents to the Window.
    /// </summary>
    /// 
    /// <param name="ctx">Context information that be used by the ScenePhysics.</param>
    void render(StateContext ctx) override;

// PROTECTED HELPER FUNCTIONS
protected:
	/// <summary>
	///     Create the Box2D Physics World.
	/// </summary>
	virtual void createPhysicsWorld();
};