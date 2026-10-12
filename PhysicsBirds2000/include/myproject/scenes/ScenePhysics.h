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
class Slingshot;
class BirdProjectile;

/// <summary>
///     The base physics scene which derives from the base scene class.
/// </summary>
class ScenePhysics : public Scene
{
protected:
    // -- Physics Scene Properties -- //
    // Textures
    sf::Texture m_groundTexture;
    sf::Texture m_slingshotTexture;
    sf::Texture m_redBirdTexture;
    sf::Texture m_yellowBirdTexture;
    sf::Texture m_greenBirdTexture;
    sf::Texture m_plankTexture;

    // Sprites
    std::shared_ptr<sf::Sprite> m_groundSprite;
    std::shared_ptr<sf::Sprite> m_slingshotSprite;
    std::shared_ptr<sf::Sprite> m_redBirdSprite;
    std::shared_ptr<sf::Sprite> m_yellowBirdSprite;
    std::shared_ptr<sf::Sprite> m_plankSprite;

    // Physics
    std::unique_ptr<b2World> m_physicsWorld;
    std::unique_ptr<PhysicsContactListener> m_contactListener;
	std::vector<std::unique_ptr<PhysicsObject>> m_physicsObjects;
    std::unique_ptr<Slingshot> m_slingshot;

    bool m_isLeftClicking = false;

    BirdProjectile* m_activeBird = nullptr;
    int m_maxBirds = 3;
    int m_birdsRemaining = 0;

    float m_birdFlightTime = 0.0f;
    float m_birdStillTime = 0.0f;

    float m_minFlightTime = 1.0f;
    float m_maxFlightTime = 8.0f;
    float m_requiredStillTime = 1.0f;
    float m_stillSpeed = 0.3f;
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
    ///     Check the given event against all Commands and their execution criterias.
    /// </summary>
    /// 
    /// <param name="event">The raw SFML event.</param>
    /// <param name="ctx">Context information provided for command execution.</param>
    void handleEvent(const sf::Event& event, const CommandContext& ctx) override;

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
    ///     Safely remove joints, bodies, and the physics world.
    /// </summary>
    virtual void clearPhysicsWorld();

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
    /// <param name="health">Total health of the plank (default to 20).</param>
    void createPlank(b2Vec2 position, sf::Angle rotation, float health = 25.0f);

    /// <summary>
    ///     Create the slingshot and load its initial projectile.
    /// </summary>
    void createSlingshot();

    /// <summary>
    ///     Load the next bird projectile onto the slingshot.
    /// </summary>
    void loadNextBird();

    /// <summary>
    ///     Check whether the current bird projectile in flight has settled.
    ///     If so, load the next bird projectile onto the slingshot.
    /// </summary>
    void updateBirdLoading(float timeStep);

    /// <summary>
    ///     Destroy the current physics world and rebuild this level.
    /// </summary>
    virtual void resetLevel();
};