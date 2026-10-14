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

LevelOne::LevelOne()
{}

LevelOne::~LevelOne()
{}

void LevelOne::createPhysicsWorld()
{
	// Call from the base class
	ScenePhysics::createPhysicsWorld();

	// Extend Functionality
	// Create Planks
	this->createPlank(b2Vec2(15.0f, 12.0f), sf::degrees(0.0f));
	this->createPlank(b2Vec2(16.0f, 10.0f), sf::degrees(90.0f));
	this->createPlank(b2Vec2(17.0f, 12.0f), sf::degrees(0.0f));

	this->createPlank(b2Vec2(15.0f, 8.0f), sf::degrees(0.0f));
	this->createPlank(b2Vec2(16.0f, 6.0f), sf::degrees(90.0f));
	this->createPlank(b2Vec2(17.0f, 8.0f), sf::degrees(0.0f));

	// Create Enemy Piggies
	this->createEnemyPig(b2Vec2(16.0f, 13.4f));
	this->createEnemyPig(b2Vec2(16.0f, 9.2f));
}
