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
class PhysicsContactListener;

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
    sf::Texture m_plankTexture;

    // Sprites
    std::shared_ptr<sf::Sprite> m_groundSprite;
    std::shared_ptr<sf::Sprite> m_redBirdSprite;
    std::shared_ptr<sf::Sprite> m_yellowBirdSprite;
    std::shared_ptr<sf::Sprite> m_plankSprite;

    // Physics
    std::unique_ptr<b2World> m_physicsWorld;
    std::unique_ptr<PhysicsContactListener> m_contactListener;
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
    ///     Create the Box2D Physics World.
    /// </summary>
    virtual void createPhysicsWorld();

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

    /// <returns>
	///     Vector of Physics Objects in the Scene.
    /// </returns>
    std::vector<PhysicsObject*> getPhysicsObjects();

// PROTECTED HELPER FUNCTIONS
protected:
    /// <summary>
    ///     Load the assets, including textures and sprites.
    /// </summary>
    virtual void loadAssets();

    /// <summary>
	///     Create a plank physics object at the given position and rotation.
    /// </summary>
    /// 
    /// <param name="position">2D position vector to spawn the plank.</param>
    /// <param name="rotation">The plank's rotation.</param>
    /// <param name="health">Total health of the plank (default to 50).</param>
    virtual void createPlank(b2Vec2 position, sf::Angle rotation, float health = 50.0f);
};