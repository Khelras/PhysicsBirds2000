/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Bird.cpp
Description : Defines the Bird Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <cmath>
#include "myproject/physics/Bird.h"
#include "myproject/physics/PhysicsLibrary.h"

namespace
{
    constexpr float REST_SPEED = 0.3f; // Metres per second
    constexpr float REST_TIME = 2.0f; // Seconds below REST_SPEED before the Bird is Spent
    constexpr float MAX_FLIGHT_TIME = 10.0f; // Safety timeout in Seconds
    constexpr float MAX_SPRING_TIME = 1.0f; // Safety to always release the Spring after this long
}

Bird::Bird(b2World& world, const sf::Vector2f& startPx, float radiusPx)
    : m_world(&world), m_body(nullptr), m_radiusPx(radiusPx), m_state(BirdState::Waiting), m_color(sf::Color::Red), m_restTimer(0.0f), m_flightTimer(0.0f)
{
    this->m_springJoint = nullptr;

    // Kinematic Body so Gravity does not affect the Bird while it sits in the Slingshot
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(PhysicsLibrary::toMetres(startPx.x), PhysicsLibrary::toMetres(startPx.y));
    bodyDef.bullet = true; // Fast Birds should not tunnel through thin objects
    this->m_body = this->m_world->CreateBody(&bodyDef);

	// Mass and shape are defined by the Fixture, so we create a Circle Fixture for the Bird
    b2CircleShape circleShape;
    circleShape.m_radius = PhysicsLibrary::toMetres(this->m_radiusPx);

	// Geometric and physical definition of the Bird
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution = 0.3f;
    this->m_body->CreateFixture(&fixtureDef);
}

Bird::~Bird()
{
    // The World outlives the Bird, so it is safe to remove the Body here
    if (this->m_body != nullptr)
    {
        this->m_world->DestroyBody(this->m_body);
        this->m_body = nullptr;
    }
}

void Bird::setHeldPosition(const sf::Vector2f& positionPx)
{
    this->m_body->SetTransform(
        b2Vec2(PhysicsLibrary::toMetres(positionPx.x), PhysicsLibrary::toMetres(positionPx.y)), 0.0f);
}

void Bird::beginDrag()
{
    if (this->m_state == BirdState::Waiting)
    {
        this->m_state = BirdState::Dragging;
    }
}

void Bird::cancelDrag()
{
    if (this->m_state == BirdState::Dragging)
    {
        this->m_state = BirdState::Waiting;
    }
}

void Bird::launchWithSpring(b2Body& anchorBody, float frequencyHz, float dampingRatio)
{
    if (this->m_state != BirdState::Dragging)
    {
        return;
    }

    // Work out the launch direction from the pulled-back position
    this->m_anchorM = anchorBody.GetPosition();
    b2Vec2 pull = this->m_anchorM - this->m_body->GetPosition();
    pull.Normalize(); // Normalizes in place (returns the old length)
    this->m_launchDir = pull;

    // The Body must be Dynamic before the Spring is created so its mass is known
    this->m_body->SetType(b2_dynamicBody);
    this->m_body->SetAwake(true);

    // Create the Spring (Soft Distance Joint) with a rest length of 0
    b2DistanceJointDef jointDef;
    jointDef.bodyA = &anchorBody;
    jointDef.bodyB = this->m_body;
    jointDef.localAnchorA.SetZero();
    jointDef.localAnchorB.SetZero();
    jointDef.length = 0.0f;
    jointDef.minLength = 0.0f;
    jointDef.maxLength = FLT_MAX;
    jointDef.collideConnected = false;

    // Convert Frequency and Damping Ratio into the Stiffness and Damping Box2D needs
    b2LinearStiffness(jointDef.stiffness, jointDef.damping, frequencyHz, dampingRatio, &anchorBody, this->m_body);
    this->m_springJoint = this->m_world->CreateJoint(&jointDef);

    this->m_flightTimer = 0.0f;
    this->m_state = BirdState::Launching;
}

void Bird::update(float deltaTime)
{
    if (this->m_state != BirdState::Flying)
    {
        return;
    }

    this->m_flightTimer += deltaTime;

    // Count how long the Bird has been (nearly) stationary
    if (this->m_body->GetLinearVelocity().Length() < REST_SPEED)
    {
        this->m_restTimer += deltaTime;
    }
    else
    {
        this->m_restTimer = 0.0f;
    }

    if (this->m_restTimer >= REST_TIME || this->m_flightTimer >= MAX_FLIGHT_TIME)
    {
        this->m_state = BirdState::Spent;
    }
}

void Bird::fixedUpdate()
{
    if (this->m_state != BirdState::Launching)
    {
        return;
    }

    // Release the Spring once the Bird has passed the anchor (or after the safety timeout)
    b2Vec2 offset = this->m_body->GetPosition() - this->m_anchorM;
    bool passedAnchor = b2Dot(offset, this->m_launchDir) >= 0.0f;
    this->m_flightTimer += 1.0f / 60.0f;
    if (passedAnchor || this->m_flightTimer >= MAX_SPRING_TIME)
    {
        this->releaseSpring();
        this->m_flightTimer = 0.0f;
        this->m_state = BirdState::Flying;
    }
}

void Bird::render(sf::RenderWindow& window) const
{
    sf::CircleShape circle(this->m_radiusPx);
    circle.setOrigin({ this->m_radiusPx, this->m_radiusPx });
    circle.setPosition(this->getPositionPx());
    circle.setRotation(sf::radians(this->m_body->GetAngle()));
    circle.setFillColor(this->m_color);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(2.0f);
    window.draw(circle);
}

sf::Vector2f Bird::getPositionPx() const
{
    b2Vec2 position = this->m_body->GetPosition();
    return sf::Vector2f(PhysicsLibrary::toPixels(position.x), PhysicsLibrary::toPixels(position.y));
}

void Bird::releaseSpring()
{
    if (this->m_springJoint != nullptr)
    {
        this->m_world->DestroyJoint(this->m_springJoint);
        this->m_springJoint = nullptr;
    }
}
