/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : BirdProjectile.cpp
Description : Defines the BirdProjectile Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include "myproject/physics/BirdProjectile.h"

BirdProjectile::BirdProjectile(std::shared_ptr<sf::Sprite> sprite, b2Vec2 position, b2World* physicsWorld)
	: PhysicsObject(b2Shape::e_circle, sprite, b2Vec2(0.75f, 0.75f), position, sf::degrees(0.0f), b2_kinematicBody, physicsWorld)
{
    this->m_body->SetFixedRotation(true);
    this->m_body->SetBullet(true);
    this->m_body->SetGravityScale(0.0f);
    
    // Bird has 500 health
    this->m_isIndustructible = true; // Prevents the Slingshot from damaging the bird
    this->m_health = 500.0f;
}

BirdProjectile::~BirdProjectile()
{}

void BirdProjectile::prepareForLoading(b2Vec2 position)
{
    // Set to a Loaded State
    this->m_state = BirdProjectile::State::Loaded;

    // Set the Bird Projectile to be a Dynamic Body and ready for Launching
    this->getBody()->SetType(b2_dynamicBody);
    this->getBody()->SetGravityScale(0.0f);
    this->getBody()->SetTransform(position, 0.0f);
    this->getBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    this->getBody()->SetAngularVelocity(0.0f);
    this->getBody()->SetAwake(true);
}

void BirdProjectile::beginDrag()
{
    this->m_state = BirdProjectile::State::Dragging;
}

void BirdProjectile::moveTo(b2Vec2 position)
{
    // Ensure in a dragging state
    if (m_state != BirdProjectile::State::Dragging) return;

    // Move the Bird Projectile
    this->m_body->SetTransform(position, 0.0f);
    this->m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    this->m_body->SetAngularVelocity(0.0f);
}

void BirdProjectile::beginLaunch()
{
    // Bird Projectile in a Launching State
    this->m_state = BirdProjectile::State::Launching;

    // No gravity during initial launch
    this->getBody()->SetGravityScale(0.0f);

    // Launch energy comes from the distance-joint spring,
    // rather than any velocity left over from dragging.
    this->getBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    this->getBody()->SetAngularVelocity(0.0f);
    this->getBody()->SetAwake(true);
}

void BirdProjectile::finishLaunch()
{
    this->m_body->SetGravityScale(1.0f);
    this->m_state = BirdProjectile::State::Flying;
}

bool BirdProjectile::activateAbility()
{
    // Abilities can only be used once and only while the bird is in flight
    if (this->m_state != State::Flying || this->m_hasUsedAbility == true) return false;

    // Check there is an ability to execute
    if (this->m_ability == nullptr) return false;

    // Use the Ability
    this->m_hasUsedAbility = true;
    this->m_ability(*this);
    return true;
}
