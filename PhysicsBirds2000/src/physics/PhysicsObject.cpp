/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : PhysicsObject.cpp
Description : Defines the PhysicsObject Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include "myproject/physics/PhysicsObject.h"

PhysicsObject::PhysicsObject(b2Shape::Type shapeType, sf::Sprite* sprite, b2Vec2 size,
	b2Vec2 position,sf::Angle rotation, b2BodyType bodyType, b2World* physicsWorld)
{
	this->m_sprite = sprite;
	this->m_size = size;

	this->m_physicsWorld = physicsWorld;

	// Body Defition of this Physics Object
	b2BodyDef bodyDef;
	bodyDef.position = position;
	bodyDef.angle = rotation.asRadians();
	bodyDef.type = bodyType;
	this->m_body = this->m_physicsWorld->CreateBody(&bodyDef);

	// Geometric and Physical Definitions (Fixture) of this Physics Object
	b2FixtureDef fixtureDef;

	// Set the Shape of the Fixture based on the provided Shape Type
	b2PolygonShape polygonShape;
	b2PolygonShape circleShape;
	switch (shapeType)
	{
		case (b2Shape::Type::e_polygon):
		{
			polygonShape.SetAsBox(size.x / 2.0f, size.y / 2.0f);
			fixtureDef.shape = &polygonShape;
		} break;

		case (b2Shape::Type::e_circle):
		{
			circleShape.m_radius = size.x / 2.0f;
			fixtureDef.shape = &circleShape;
		} break;
	}

	// Create the Fixture for this Physics Object
	fixtureDef.friction = 0.5f;
	fixtureDef.density = 1.0f;
	fixtureDef.restitution = 0.3f;
	this->m_body->CreateFixture(&fixtureDef);;
}

PhysicsObject::~PhysicsObject()
{
	this->m_physicsWorld->DestroyBody(this->m_body);
}

b2Body* PhysicsObject::getBody() const
{
	return this->m_body;
}

void PhysicsObject::draw(sf::RenderWindow& window) const
{}
