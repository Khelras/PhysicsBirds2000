/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Slingshot.cpp
Description : Defines the Slingshot Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include "myproject/physics/Slingshot.h"

Slingshot::Slingshot(std::shared_ptr<sf::Sprite> sprite, b2Vec2 size, b2Vec2 position, b2World* physicsWorld)
    : PhysicsObject(b2Shape::e_polygon, sprite, size, position, sf::degrees(0.0f), b2_staticBody, physicsWorld)
{
	this->m_localAnchor = b2Vec2(0.0f, -size.y / 2.0f);

    // Industructable
    this->m_isIndustructible = true;
    this->m_health = 1.0f;

    // The placeholder is visual, rather than an obstacle.
    // Or else the launched bird could hit the post.
    for (b2Fixture* fixture = this->m_body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
    {
        fixture->SetSensor(true);
    }
}

Slingshot::~Slingshot()
{
    this->destroyJoint();
}

void Slingshot::loadBird(BirdProjectile& bird)
{
    // Destroy the previous Joint
    this->destroyJoint();

    // Set the newly loaded Bird Projectile
    this->m_bird = &bird;
    this->m_dragging = false;
    this->m_launchTime = 0.0f;

    // The Position (the launch point) to load the Bird Projectile
    const b2Vec2 loadedPosition = this->getAnchorPosition() + b2Vec2(-this->m_restLength, 0.0f);

    // Prepare the Bird Projectile for loading
    this->m_bird->prepareForLoading(loadedPosition);
    this->m_dragTarget = loadedPosition;

    // -- Distance Joint Definition -- //
    b2DistanceJointDef jointDef;

    jointDef.bodyA = this->getBody();
    jointDef.bodyB = this->m_bird->getBody();

    jointDef.localAnchorA = this->m_localAnchor;
    jointDef.localAnchorB.Set(0.0f, 0.0f);

    jointDef.length = this->m_restLength;
    jointDef.minLength = 0.01f;
    jointDef.maxLength = this->m_maxPullDistance;

    // Disable the spring while the Bird Projectile is in 'Loaded' or 'Dragging' States.
    jointDef.stiffness = 0.0f;
    jointDef.damping = 0.0f;

    jointDef.collideConnected = false;
    // -- //

    this->m_joint = static_cast<b2DistanceJoint*>(this->m_physicsWorld->CreateJoint(&jointDef));
}

bool Slingshot::beginDrag(b2Vec2 mousePosition)
{
    // Ensure a Bird Projectile is loaded onto the Slingshot
    if (this->m_bird == nullptr || this->m_bird->getState() != BirdProjectile::State::Loaded)
    {
        return false;
    }

    // Offset point from the Mouse Cursor Position to the Bird Projectile
    const b2Vec2 offset = mousePosition - this->m_bird->getBody()->GetPosition();

    // Ensure the offset point is within a click radius
    const float clickRadius = 0.5f;
    if (offset.LengthSquared() > clickRadius * clickRadius)
    {
        return false;
    }

    this->m_dragging = true;
    this->m_bird->beginDrag();
    this->dragTo(mousePosition);
    return true;
}

void Slingshot::dragTo(b2Vec2 mousePosition)
{
    // Ensure in a 'Dragging' State and a Bird Projectile is loaded
    if (this->m_dragging == false || this->m_bird == nullptr)
    {
        return;
    }

    this->m_dragTarget = mousePosition;

    // Restrict to only pull backwards (to the left of the Slingshot)
    // The Distance Joint will control the maximum pull distance
    const b2Vec2 anchorPosition = this->getAnchorPosition();
    if (this->m_dragTarget.x > anchorPosition.x)
    {
        this->m_dragTarget.x = anchorPosition.x;
    }
}

void Slingshot::applyDragForce()
{
    // Ensure in a 'Dragging' State and a Bird Projectile is loaded
    if (this->m_dragging == false || this->m_bird == nullptr)
    {
        return;
    }

    b2Body* birdBody = this->m_bird->getBody();

    const b2Vec2 positionError = this->m_dragTarget - birdBody->GetPosition();
    const b2Vec2 velocity = birdBody->GetLinearVelocity();

    // Pull towards the cursor and damp unwanted movement
    const float positionGain = 100.0f;
    const float velocityGain = 20.0f;

    // The Drag Force
    b2Vec2 acceleration = positionGain * positionError - velocityGain * velocity;

    // Prevent extreme cursor positions from producing huge forces
    const float maximumAcceleration = 100.0f;
    const float accelerationLength = acceleration.Length();
    if (accelerationLength > maximumAcceleration)
    {
        acceleration *= maximumAcceleration / accelerationLength;
    }

    // Apply the Drag Force
    birdBody->ApplyForceToCenter(birdBody->GetMass() * acceleration, true);
}

void Slingshot::release()
{
    // Ensure in a 'Dragging' State and a Bird Projectile is loaded
    if (this->m_dragging == false || this->m_bird == nullptr || this->m_joint == nullptr)
    {
        return;
    }

    // Release the Bird Projectile and start the launching the Bird Projectile
    this->m_dragging = false;
    this->m_launchTime = 0.0f;
    this->m_bird->beginLaunch();

    // Calculate spring values using the masses of the connected bodies.
    float stiffness, damping = 0.0f;
    b2LinearStiffness(
        stiffness, // Sets the Stiffness
        damping, // Sets the Damping
        1.67f, // Frequency in Hz
        0.10f, // Damping Ratio
        this->getBody(),
        this->m_bird->getBody()
    );

    // Enable the spring on the already-existing distance joint.
    this->m_joint->SetStiffness(stiffness);
    this->m_joint->SetDamping(damping);
}

void Slingshot::update(float deltaTime)
{
    // Ensure a Bird Projectile is loaded
    if (this->m_joint == nullptr || this->m_bird == nullptr || this->m_bird->getState() != BirdProjectile::State::Launching)
    {
        return;
    }

    // Increase the Launch Timer
    this->m_launchTime += deltaTime;

    // Detach near the anchor, or after a safety timeout.
    const b2Vec2 offset = this->m_bird->getBody()->GetPosition() - this->getAnchorPosition();
    const float releaseDistance = 0.3f;
    if (offset.LengthSquared() <= releaseDistance * releaseDistance || this->m_launchTime >= 0.35f)
    {
        this->destroyJoint();
        this->m_bird->finishLaunch();

        this->m_bird = nullptr;
    }
}

void Slingshot::drawBand(sf::RenderWindow & window) const
{

}

b2Vec2 Slingshot::getAnchorPosition() const
{
    return this->m_body->GetWorldPoint(this->m_localAnchor);
}

void Slingshot::destroyJoint()
{
    if (this->m_joint != nullptr)
    {
        this->m_physicsWorld->DestroyJoint(m_joint);
        this->m_joint = nullptr;
    }
}