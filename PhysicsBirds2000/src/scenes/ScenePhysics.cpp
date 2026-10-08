/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : ScenePhysics.cpp
Description : Defines the ScenePhysics Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "myproject/scenes/ScenePhysics.h"
#include "myproject/core/Settings.h"

ScenePhysics::ScenePhysics()
{
	// Register the Commands
	this->registerCommands();
}

ScenePhysics::~ScenePhysics()
{}

void ScenePhysics::registerCommands()
{

}

void ScenePhysics::update(StateContext ctx)
{}

void ScenePhysics::render(StateContext ctx)
{}

void ScenePhysics::createPhysicsWorld()
{
	// Default Physics World
	this->m_physicsWorld = std::make_unique<b2World>(b2Vec2(0.0f, 9.81f));
}
