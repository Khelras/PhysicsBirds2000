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
#include "myproject/physics/BirdProjectile.h"

// Forward Declarations
class b2World;
class PhysicsContactListener;
class Slingshot;

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
    sf::Texture m_pigTexture;
    sf::Texture m_plankTexture;

    // Sprites
    std::shared_ptr<sf::Sprite> m_groundSprite;
    std::shared_ptr<sf::Sprite> m_slingshotSprite;
    std::shared_ptr<sf::Sprite> m_redBirdSprite;
    std::shared_ptr<sf::Sprite> m_yellowBirdSprite;
    std::shared_ptr<sf::Sprite> m_greenBirdSprite;
    std::shared_ptr<sf::Sprite> m_pigSprite;
    std::shared_ptr<sf::Sprite> m_plankSprite;

    // Physics
    std::unique_ptr<b2World> m_physicsWorld;
    std::unique_ptr<PhysicsContactListener> m_contactListener;
	std::vector<std::unique_ptr<PhysicsObject>> m_physicsObjects;
    std::unique_ptr<Slingshot> m_slingshot;

    bool m_isLevelSettling = true;
    float m_levelSettleTime = 0.0f;
    float m_maxLevelSettleTime = 3.0f;

    bool m_isLeftClicking = false;

    BirdProjectile* m_activeBird = nullptr;
    std::vector<BirdType> m_birdStack;

    float m_birdQueueOffsetX = 1.125f; // Distance behind the slingshot to the first waiting bird
    float m_birdQueueSpacing = 0.9f; // Distance between waiting birds
    float m_birdQueueY = 14.125f; // Resting height (ground top is 14, bird radius is 0.5)

    float m_birdFlightTime = 0.0f;
    float m_birdStillTime = 0.0f;
    float m_minFlightTime = 1.0f;
    float m_maxFlightTime = 8.0f;
    float m_requiredStillTime = 1.0f;
    float m_stillSpeed = 0.2f;

    std::vector<PhysicsObject*> m_enemies; // Non-Owning
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

    /// <summary>
    ///     Whether the level is still settling after loading (impacts are ignored while true).
    /// </summary>
    inline bool isLevelSettling() const { return this->m_isLevelSettling; };

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
    ///     Update the level settling state after loading.
    ///     This state ends once everything is at rest.
    /// </summary>
    void updateLevelSettling(float timeStep);

    /// <summary>
    ///     Create the slingshot and load its initial projectile.
    /// </summary>
    void createSlingshot();

    /// <summary>
	///     Create a plank physics object at the given position and rotation.
    /// </summary>
    /// 
    /// <param name="position">2D position vector to spawn the plank.</param>
    /// <param name="rotation">The plank's rotation.</param>
    /// <param name="health">Total health of the plank (default to 20).</param>
    void createPlank(b2Vec2 position, sf::Angle rotation, float health = 20.0f);

    /// <summary>
    ///     Create a enemy pig physics object at the given position.
    /// </summary>
    /// 
    /// <param name="position">2D position vector to spawn the enemy pig.</param>
    /// <param name="health">Total health of the enemy pig (default to 10).</param>
    void createEnemyPig(b2Vec2 position, float health = 10);

    /// <summary>
    ///     Load the next bird projectile onto the slingshot.
    /// </summary>
    void loadNextBird();

    /// <summary>
    ///     Get the sprite of the given bird type.
    /// </summary>
    /// 
    /// <param name="type">The type of bird.</param>
    std::shared_ptr<sf::Sprite> getBirdSprite(BirdType type) const;

    /// <summary>
    /// Draw the birds waiting to be launched in a line behind the slingshot (visual only).
    /// </summary>
    /// <param name="window">Reference to the render window target.</param>
    void drawBirdQueue(sf::RenderWindow& window) const;

    /// <summary>
    ///     Check whether the current bird projectile in flight has settled.
    ///     If so, load the next bird projectile onto the slingshot.
    /// </summary>
    void updateBirdLoading(float timeStep);

    /// <summary>
    ///     Check how many enemies remain in the scene and log the level progress.
    /// </summary>
    void checkEnemiesRemaining();

    /// <summary>
    ///     Destroy the current physics world and rebuild this level.
    /// </summary>
    virtual void resetLevel();

// PROTECTED HELPER FOR THE GREEN BIRD
protected:
    // Non-owning list of extra Bird Projectiles spawned.
    std::vector<PhysicsObject*> m_extraBirds;
    float m_splitAngle = 15.0f; // Degrees between the split Bird Projectiles trajectories

    /// <summary>
    ///     Split the given bird into three birds with different trajectories.
    /// </summary>
    /// 
    /// <param name="bird">The bird that is splitting.</param>
    void splitBird(BirdProjectile& bird);

    /// <summary>
    ///     Check whether a bird body has come to rest, left the play area, or fallen asleep.
    /// </summary>
    /// 
    /// <param name="birdBody">The bird's body.</param>
    bool isBirdAtRest(b2Body* birdBody) const;
};