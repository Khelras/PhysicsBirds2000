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
#include "myproject/physics/PhysicsContactListener.h"

ScenePhysics::ScenePhysics()
{
	// Register the Commands
	this->registerCommands();

	// Load the Assets
	this->loadAssets();

	// Create the Physics World
	this->createPhysicsWorld();
}

ScenePhysics::~ScenePhysics()
{}

void ScenePhysics::registerCommands()
{

}

void ScenePhysics::update(StateContext ctx)
{
	// Physics World Step
	this->m_physicsWorld->Step(1.0f / 60.0f, 8, 3);

	// Remove Physics Objects that are marked for destruction
	for (auto it = this->m_physicsObjects.begin(); it != this->m_physicsObjects.end();)
	{
		if ((*it)->isMarkedForDestroy())
			it = this->m_physicsObjects.erase(it);
		else
			it++;
	}
}

void ScenePhysics::render(StateContext ctx)
{
	for (auto& physicsObject : this->m_physicsObjects)
	{
		physicsObject->draw(*ctx.window);
	}
}

std::vector<PhysicsObject*> ScenePhysics::getPhysicsObjects()
{
	std::vector<PhysicsObject*> physicsObjects;
	for (auto& physicsObject : this->m_physicsObjects)
	{
		physicsObjects.push_back(physicsObject.get());
	}

	return physicsObjects;
}

void ScenePhysics::loadAssets()
{
	// Load the Textures
	if (this->m_groundTexture.loadFromFile("assets/textures/ground.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/ground.png'" << std::endl;
	if (this->m_redBirdTexture.loadFromFile("assets/textures/red_bird.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/red_bird.png'" << std::endl;
	if (this->m_yellowBirdTexture.loadFromFile("assets/textures/yellow_bird.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/yellow_bird.png'" << std::endl;
	if (this->m_greenBirdTexture.loadFromFile("assets/textures/green_bird.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/green_bird.png'" << std::endl;

	// Create the Sprites
	this->m_groundSprite = std::make_shared<sf::Sprite>(this->m_groundTexture);
	this->m_redBirdSprite = std::make_shared<sf::Sprite>(this->m_redBirdTexture);
	this->m_yellowBirdSprite = std::make_shared<sf::Sprite>(this->m_yellowBirdTexture);
	this->m_greenBirdSprite = std::make_shared<sf::Sprite>(this->m_greenBirdTexture);
}

void ScenePhysics::createPhysicsWorld()
{
	// Physics World
	this->m_physicsWorld = std::make_unique<b2World>(b2Vec2(0.0f, 9.81f));
	this->m_contactListener = std::make_unique<PhysicsContactListener>(this->m_physicsWorld.get(), this);
	this->m_physicsWorld->SetContactListener(this->m_contactListener.get());

	// Ground Object
	this->m_physicsObjects.emplace_back(
		std::make_unique<PhysicsObject>(
			b2Shape::e_polygon,
			this->m_groundSprite,
			b2Vec2(30.0f, 2.0f),
			b2Vec2(15.0f, 15.0f),
			sf::degrees(0.0f),
			b2BodyType::b2_staticBody,
			this->m_physicsWorld.get()
		)
	);

	// Red Bird Physics Object
	this->m_physicsObjects.emplace_back(
		std::make_unique<PhysicsObject>(
			b2Shape::e_circle,
			this->m_redBirdSprite,
			b2Vec2(1.0f, 1.0f),
			b2Vec2(5.0f, 0.0f),
			sf::degrees(0.0f),
			b2BodyType::b2_dynamicBody,
			this->m_physicsWorld.get()
		)
	);
}