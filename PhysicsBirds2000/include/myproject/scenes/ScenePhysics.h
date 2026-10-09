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
#include <vector>
#include "myproject/scenes/Scene.h"
#include "myproject/physics/PhysicsObject.h"

// Forward Declarations
class b2World;

/// <summary>
///     The base physics scene which derives from the base scene class.
/// </summary>
class ScenePhysics : public Scene
{
protected:
    // -- Physics Scene Properties -- //
    // Textures
    sf::Texture m_groundTexture;
    sf::Texture m_redBirdTexture;
    sf::Texture m_yellowBirdTexture;
    sf::Texture m_greenBirdTexture;

    // Sprites
    std::shared_ptr<sf::Sprite> m_groundSprite;
    std::shared_ptr<sf::Sprite> m_redBirdSprite;
    std::shared_ptr<sf::Sprite> m_yellowBirdSprite;
    std::shared_ptr<sf::Sprite> m_greenBirdSprite;

    // Physics
    std::unique_ptr<b2World> m_physicsWorld;
	std::vector<std::unique_ptr<PhysicsObject>> m_physicsObjects;
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
    ///     Load the assets, including textures and sprites.
    /// </summary>
    virtual void loadAssets();

	/// <summary>
	///     Create the Box2D Physics World.
	/// </summary>
	virtual void createPhysicsWorld();
};