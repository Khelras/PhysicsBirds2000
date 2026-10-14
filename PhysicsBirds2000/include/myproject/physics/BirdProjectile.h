/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : BirdProjectile.h
Description : Declares the BirdProjectile Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include "myproject/physics/PhysicsObject.h"

/// <summary>
/// The types of bird projectiles, the values match the order they appear in the Slingshot queue.
/// </summary>
enum class BirdType : int
{
    Red = 1,
    Yellow = 2,
    Green = 3
};

/// <summary>
///     Bird Projectile class.
/// </summary>
class BirdProjectile : public PhysicsObject
{
public:
    /// <summary>
    ///     State of the bird projectile
    /// </summary>
    enum class State
    {
        Loaded,
        Dragging,
        Launching,
        Flying
    };

private:
    // -- Bird Projectile Properties -- //
    State m_state = State::Loaded;
    std::function<void(BirdProjectile&)> m_ability;
    bool m_hasUsedAbility = false;
    // -- //

public:
    /// <summary>
    ///     Constructor.
    /// </summary>
    /// 
    /// <param name="sprite">Pointer to sprite representing this bird projectile physics object.</param>
    /// <param name="position">Position of this bird projectile physics object.</param>
    /// <param name="physicsWorld">Pointer to the physics world.</param>
    BirdProjectile(std::shared_ptr<sf::Sprite> sprite, b2Vec2 position, b2World* physicsWorld);

    /// <summary>
    ///     Destructor.
    /// </summary>
    ~BirdProjectile() override;

    /// <summary>
    ///     Prepares the bird projectile physics object for loading onto the slingshot.
    /// </summary>
    /// 
    /// <param name="position">2D vector position of the anchor/launch point of the slingshot.</param>
    void prepareForLoading(b2Vec2 position);

    /// <summary>
    ///     Start dragging the bird projectile.
    /// </summary>
    void beginDrag();

    /// <summary>
    ///     Move the bird projectile to a given position.
    ///     This only works when the bird projectile is in a 'dragging' state.
    /// </summary>
    /// 
    /// <param name="position">2D position vector.</param>
    void moveTo(b2Vec2 position);

    /// <summary>
    ///     Initial launching of the bird projectile.
    /// </summary>
    void beginLaunch();

    /// <summary>
    ///     Completion of the launch of the bird projectile.
    /// </summary>
    void finishLaunch();

    /// <returns>
    ///     Tthe state of the bird projectile.
    /// </returns>
    State getState() const { return m_state; }

    /// <summary>
    ///     Set the ability of this bird projectile.
    /// </summary>
    /// 
    /// <param name="ability">A function, lambda, or functor taking this bird projectile as a parameter.</param>
    inline void setAbility(std::function<void(BirdProjectile&)> ability)
    {
        this->m_ability = std::move(ability);
    };

    /// <summary>
    ///     Activate the bird projectile's unique ability.
    ///     The ability can only be activated once and during flight.
    /// </summary>
    /// 
    /// <returns>
    ///     True if the ability activated successfully.
    ///     False otherwise.
    /// </returns>
    bool activateAbility();
};