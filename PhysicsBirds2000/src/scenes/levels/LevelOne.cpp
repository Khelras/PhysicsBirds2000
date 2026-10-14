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
	// Create PLanks
	this->createPlank(b2Vec2(18.25f, 13.375f), sf::degrees(0.0f));
	this->createPlank(b2Vec2(19.75f, 13.375f), sf::degrees(0.0f));
	this->createPlank(b2Vec2(19.0f, 12.0625f), sf::degrees(90.0f));

	this->createPlank(b2Vec2(18.25f, 10.75f), sf::degrees(0.0f));
	this->createPlank(b2Vec2(19.75f, 10.75f), sf::degrees(0.0f));
	this->createPlank(b2Vec2(19.0f, 9.4375f), sf::degrees(90.0f));

	// Create Enemies
	this->createEnemyPig(b2Vec2(19.0f, 14.125f));
	this->createEnemyPig(b2Vec2(19.0f, 11.5f));
}
