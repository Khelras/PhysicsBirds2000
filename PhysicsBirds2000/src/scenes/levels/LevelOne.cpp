/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : LevelOne.cpp
Description : Defines the LevelOne Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include "myproject/scenes/levels/LevelOne.h"
#include "myproject/managers/SceneManager.h"

LevelOne::LevelOne()
{}

LevelOne::~LevelOne()
{}

void LevelOne::createPhysicsWorld()
{
	// Call from the base class
	ScenePhysics::createPhysicsWorld();

	// Extend Functionality
	// Create Planks and Concretes
    this->createConcrete(b2Vec2(12.25f, 12.375f), sf::degrees(0.0f));
    this->createPlank(b2Vec2(13.75f, 12.375f), sf::degrees(0.0f));
    this->createPlank(b2Vec2(13.0f, 11.0625f), sf::degrees(90.0f));

    this->createPlank(b2Vec2(12.25f, 9.75f), sf::degrees(0.0f));
    this->createConcrete(b2Vec2(13.75f, 9.75f), sf::degrees(0.0f));
    this->createPlank(b2Vec2(13.0f, 8.4375f), sf::degrees(90.0f));

    // Create the Lever (Revolute Joint)
    this->createLever(b2Vec2(13.0f, 14.125f));

	this->createConcrete(b2Vec2(18.25f, 13.375f), sf::degrees(0.0f));
	this->createPlank(b2Vec2(19.75f, 13.375f), sf::degrees(0.0f));
	this->createPlank(b2Vec2(19.0f, 12.0625f), sf::degrees(90.0f));

	this->createPlank(b2Vec2(18.25f, 10.75f), sf::degrees(0.0f));
	this->createConcrete(b2Vec2(19.75f, 10.75f), sf::degrees(0.0f));
	this->createPlank(b2Vec2(19.0f, 9.4375f), sf::degrees(90.0f));

	// Create Enemies
	this->createEnemyPig(b2Vec2(19.0f, 14.125f));
	this->createEnemyPig(b2Vec2(19.0f, 11.5f));
	this->createEnemyPig(b2Vec2(13.125f, 10.0f));
}

void LevelOne::goToNextLevel(StateContext ctx)
{
    ctx.sceneManager->goToScene("LevelTwo");
}

void LevelOne::createLever(b2Vec2 position)
{
    // Fulcrum (static), the lever pivots on top of this.
    auto fulcrum = std::make_unique<PhysicsObject>(
        b2Shape::e_polygon,
        this->m_plankSprite,
        b2Vec2(0.45f, 0.75f),
        position,
        sf::degrees(0.0f),
        b2BodyType::b2_staticBody,
        this->m_physicsWorld.get()
    );

    fulcrum->setHealth(1.0f, true);
    b2Body* fulcrumBody = fulcrum->getBody();
    this->m_physicsObjects.emplace_back(std::move(fulcrum));

    // Lever (dynamic), a plank lying flat on top of the fulcrum.
    auto lever = std::make_unique<PhysicsObject>(
        b2Shape::e_polygon,
        this->m_plankSprite,
        b2Vec2(0.375f, 2.25f),
        position + b2Vec2(0.0f, -0.5625f),
        sf::degrees(90.0f),
        b2BodyType::b2_dynamicBody,
        this->m_physicsWorld.get()
    );

    lever->setHealth(1.0f, true);
    b2Body* leverBody = lever->getBody();
    this->m_physicsObjects.emplace_back(std::move(lever));

    // Revolute Joint, hinged at the lever's centre.
    b2RevoluteJointDef jointDef;
    jointDef.Initialize(fulcrumBody, leverBody, leverBody->GetWorldCenter());
    jointDef.enableLimit = true;
    jointDef.lowerAngle = -0.5f;
    jointDef.upperAngle = 0.0f;
    this->m_physicsWorld->CreateJoint(&jointDef);
}
