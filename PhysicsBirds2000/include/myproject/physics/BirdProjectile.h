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
    State m_state = State::Loaded;

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
    ///     Destructor
    /// </summary>
    ~BirdProjectile();

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

    // Override in specialised bird classes later.
    virtual void activateAbility() {}
};