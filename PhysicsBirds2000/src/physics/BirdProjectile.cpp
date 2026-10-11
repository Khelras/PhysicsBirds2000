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
	: PhysicsObject(b2Shape::e_circle, sprite, b2Vec2(1.0f, 1.0f), position, sf::degrees(0.0f), b2_kinematicBody, physicsWorld)
{
    this->m_body->SetFixedRotation(true);
    this->m_body->SetBullet(true);
    
    // Bird has 500 health
    this->m_health = 500.0f;
}

BirdProjectile::~BirdProjectile()
{}

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
    // Bird Projectile is now a Dynamic Body
    this->m_body->SetType(b2_dynamicBody);
    this->m_body->SetGravityScale(0.0f); // No gravity during initial launch
    this->m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
    this->m_body->SetAwake(true);

    // Bird Projectile in a Launching State
    this->m_state = BirdProjectile::State::Launching;
}

void BirdProjectile::finishLaunch()
{
    this->m_body->SetGravityScale(1.0f);
    this->m_state = BirdProjectile::State::Flying;
}
