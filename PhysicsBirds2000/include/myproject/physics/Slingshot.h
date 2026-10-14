/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Slingshot.h
Description : Declares the Slingshot Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include "myproject/physics/PhysicsObject.h"
#include "myproject/physics/BirdProjectile.h"

class Slingshot : public PhysicsObject
{
private:
    // -- Slingshot Properties -- //
    BirdProjectile* m_bird = nullptr;
    b2DistanceJoint* m_joint = nullptr;

    b2Vec2 m_localAnchor;

    bool m_dragging = false;
    float m_launchTime = 0.0f;
    b2Vec2 m_dragTarget = b2Vec2(0.0f, 0.0f);

    float m_maxPullDistance = 2.0f;
    float m_restLength = 0.1f;
    // -- //

public:
    /// <summary>
    ///     Constructor.
    /// </summary>
    /// 
    /// <param name="sprite">Pointer to sprite representing this slightshot physics object.</param>
    /// <param name="size">Size of the slightshot physics object.</param>
    /// <param name="position">Position of this slightshot physics object.</param>
    /// <param name="physicsWorld">Pointer to the physics world.</param>
    Slingshot(std::shared_ptr<sf::Sprite> sprite, b2Vec2 size, b2Vec2 position, b2World* physicsWorld);

    /// <summary>
    ///     Destructor.
    /// </summary>
    ~Slingshot() override;

    /// <summary>
    ///     Load a bird projectile onto the slingshot.
    /// </summary>
    /// 
    /// <param name="bird">Reference to the bird projectile.</param>
    void loadBird(BirdProjectile& bird);

    /// <summary>
    ///     Begin dragging the bird projectile and the slingshot.
    /// </summary>
    /// 
    /// <param name="mousePosition">2D position vector of the mouse.</param>
    /// 
    /// <returns>
    ///     True if the bird projectile is being dragged. False otherwise.
    /// </returns>
    bool beginDrag(b2Vec2 mousePosition);

    /// <summary>
    ///     Drag the bird projectile to the given mouse position.
    /// </summary>
    /// 
    /// <param name="mousePosition">2D position vector of the mouse.</param>
    void dragTo(b2Vec2 mousePosition);

    /// <summary>
    ///     Apply mouse-dragging forces before the physics world steps.
    /// </summary>
    void applyDragForce();

    /// <summary>
    ///     Release and launch the bird projectile.
    /// </summary>
    void release();

    /// <summary>
    ///     Call after the Box2D world has finished it's step process.
    /// </summary>
    /// 
    /// <param name="deltaTime">Delta time as a float.</param>
    void update(float deltaTime);

    /// <summary>
    ///     Draw the band of the slingshot.
    /// </summary>
    /// 
    /// <param name="window">Reference to the SFML render window.</param>
    void drawBand(sf::RenderWindow& window) const;

// PUBLIC HELPER FUNCTIONS
public:
    /// <returns>
    ///     The anchor point position of the slingshot distance joint as a 2D vector.
    /// </returns>
    b2Vec2 getAnchorPosition() const;

// PRIVATE HELPER FUNCTIONS
private:
    /// <summary>
    ///     Destroy the joint.
    /// </summary>
    void destroyJoint();
};
