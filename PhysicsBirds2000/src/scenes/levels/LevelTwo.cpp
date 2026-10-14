/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : LevelTwo.cpp
Description : Defines the LevelTwo Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <iostream>
#include "myproject/scenes/levels/LevelTwo.h"
#include "myproject/managers/SceneManager.h"

LevelTwo::LevelTwo()
{}

LevelTwo::~LevelTwo()
{}

void LevelTwo::createPhysicsWorld()
{
    // Call from the base class
    ScenePhysics::createPhysicsWorld();

    // Extend Functionality
    // Create the Welded Tee (Weld Joint)
    this->createWeldedTee(b2Vec2(10.0f, 13.375f));
    this->createWeldedTee(b2Vec2(15.0f, 13.375f));
    this->createWeldedTee(b2Vec2(20.0f, 13.375f));

    // Create Enemies
    this->createEnemyPig(b2Vec2(10.0f, 11.5f));
    this->createEnemyPig(b2Vec2(15.0f, 11.5f));
    this->createEnemyPig(b2Vec2(20.0f, 11.5f));
}

void LevelTwo::goToNextLevel(StateContext ctx)
{
    // DEBUG
    std::cout << "Player Wins!!!" << std::endl;
}

void LevelTwo::createWeldedTee(b2Vec2 position)
{
    // Post (standing) and Top Plank (lying flat across the top of the post).
    PhysicsObject* post = this->createPlank(position, sf::degrees(0.0f));
    PhysicsObject* top = this->createPlank(position + b2Vec2(0.0f, -1.3125f), sf::degrees(90.0f));

    // Weld Joint, fully rigid by default (no stiffness or damping set).
    b2WeldJointDef jointDef;
    jointDef.Initialize(post->getBody(), top->getBody(), top->getBody()->GetWorldCenter());
    this->m_physicsWorld->CreateJoint(&jointDef);
}
