/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : SceneMain.cpp
Description : Defines the SceneMain Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <cmath>
#include <iostream>
#include "myproject/scenes/SceneMain.h"
#include "myproject/core/Settings.h"
#include "myproject/physics/PhysicsLibrary.h"
#include "myproject/physics/Slingshot.h"
#include "myproject/physics/Bird.h"

namespace
{
	constexpr float FIXED_TIME_STEP = 1.0f / 60.0f;
	constexpr int VELOCITY_ITERATIONS = 8;
	constexpr int POSITION_ITERATIONS = 3;

	constexpr float BIRD_RADIUS_PX = 20.0f;
	constexpr int BIRDS_PER_LEVEL = 3;
	constexpr float GRAB_RADIUS_FACTOR = 1.5f; // Forgiving click area around the Bird
	constexpr float MIN_PULL_PX = 10.0f; // Pulls shorter than this cancel the shot
	constexpr float MAX_PULL_PX = 120.0f;
	constexpr float LAUNCH_POWER = 4.5f; // (m/s) of launch speed per Metre pulled back

	float distanceBetween(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return std::hypot(a.x - b.x, a.y - b.y);
	}
}

SceneMain::SceneMain()
{
	// Register the Commands
	this->registerCommands();

	// Create the Physics World and the Window Edges
	this->createPhysicsWorld();

	// Create the Slingshot, then load the first Bird into it
	float windowH = static_cast<float>(Settings::getInstance().windowHeight);
	float windowW = static_cast<float>(Settings::getInstance().windowWidth);
	sf::Vector2f anchor(windowW * 0.15f, windowH - 200.0f);
	this->m_slingshot = std::make_unique<Slingshot>(anchor, MAX_PULL_PX, LAUNCH_POWER);

	this->resetBirds();
}

SceneMain::~SceneMain()
{}

void SceneMain::registerCommands()
{
	// -- Space Key Pressed -- //
	this->m_commands.push_back({
		// Execution Criteria
		[](const sf::Event& event)
		{
			// First check if the Event was a Key Press, then check if the Key was the Space Key
			if (const auto* key = event.getIf<sf::Event::KeyPressed>())
			{
				return key->code == sf::Keyboard::Key::Space;
			}

			// Otherwise, the event does not match the criteria
			return false;
		},
		// Command Action
		[this](const CommandContext& ctx)
		{
			// DEBUG
			std::cout << "Space Key Pressed: Resetting the Birds!" << std::endl;

			// Reset all the Birds
			this->resetBirds();
		}
	});
	// -- //
}

void SceneMain::update(StateContext ctx)
{
	// Get the time since the last frame, clamped so a long hitch does not cause a huge physics jump
	float deltaTime = this->m_clock.restart().asSeconds();
	if (deltaTime > 0.25f)
	{
		deltaTime = 0.25f;
	}

	// Handle the Slingshot Input before stepping the Physics
	this->handleSlingshotInput(ctx);

	// Step the Physics World at a Fixed Time Step for stable, consistent simulation
	this->m_accumulator += deltaTime;
	while (this->m_accumulator >= FIXED_TIME_STEP)
	{
		this->m_world->Step(FIXED_TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
		this->m_accumulator -= FIXED_TIME_STEP;
	}

	// Update every Bird (tracks which ones have come to rest)
	for (auto& bird : this->m_birds)
	{
		bird->update(deltaTime);
	}

	// Once the current Bird is Spent, load the next one
	if (this->m_currentBird != nullptr && this->m_currentBird->getState() == BirdState::Spent)
	{
		if (this->m_birdsRemaining > 0)
		{
			this->loadNextBird();
		}
		else
		{
			// DEBUG: The Lose Condition check will be added here later
			std::cout << "Out of Birds!" << std::endl;
			this->m_currentBird = nullptr;
		}
	}
}

void SceneMain::render(StateContext ctx)
{
	// -- Slingshot (Bands stretch to the Bird while it is held, otherwise rest at the anchor) -- //
	sf::Vector2f bandEnd = this->m_slingshot->getAnchorPx();
	if (this->m_currentBird != nullptr)
	{
		BirdState state = this->m_currentBird->getState();
		if (state == BirdState::Waiting || state == BirdState::Dragging)
		{
			bandEnd = this->m_currentBird->getPositionPx();
		}
	}
	this->m_slingshot->render(*ctx.window, bandEnd);
	// -- //

	// -- Birds -- //
	for (const auto& bird : this->m_birds)
	{
		bird->render(*ctx.window);
	}
	// -- //
}

void SceneMain::createPhysicsWorld()
{
	float windowW = static_cast<float>(Settings::getInstance().windowWidth);
	float windowH = static_cast<float>(Settings::getInstance().windowHeight);

	this->m_world = std::make_unique<b2World>(b2Vec2(0.0f, 9.8f));

	// -- Window Edges (One Static Body with four Edge Fixtures) -- //
	b2BodyDef wallDef;
	wallDef.type = b2_staticBody;
	wallDef.position.Set(0.0f, 0.0f);
	this->m_wallBody = this->m_world->CreateBody(&wallDef);

	float w = PhysicsLibrary::toMetres(windowW);
	float h = PhysicsLibrary::toMetres(windowH);

	b2Vec2 topLeft(0.0f, 0.0f);
	b2Vec2 topRight(w, 0.0f);
	b2Vec2 bottomRight(w, h);
	b2Vec2 bottomLeft(0.0f, h);

	b2EdgeShape edge;

	edge.SetTwoSided(topLeft, topRight);
	this->m_wallBody->CreateFixture(&edge, 0.0f);
	edge.SetTwoSided(topRight, bottomRight);
	this->m_wallBody->CreateFixture(&edge, 0.0f);
	edge.SetTwoSided(bottomRight, bottomLeft);
	this->m_wallBody->CreateFixture(&edge, 0.0f);
	edge.SetTwoSided(bottomLeft, topLeft);
	this->m_wallBody->CreateFixture(&edge, 0.0f);
	// -- //
}

void SceneMain::loadNextBird()
{
	if (this->m_birdsRemaining <= 0)
	{
		this->m_currentBird = nullptr;
		return;
	}

	this->m_birds.push_back(
		std::make_unique<Bird>(*this->m_world, this->m_slingshot->getAnchorPx(), BIRD_RADIUS_PX));
	this->m_currentBird = this->m_birds.back().get();
	--this->m_birdsRemaining;
}

void SceneMain::resetBirds()
{
	// Destroying the Birds also removes their Bodies from the World
	this->m_currentBird = nullptr;
	this->m_birds.clear();
	this->m_birdsRemaining = BIRDS_PER_LEVEL;
	this->loadNextBird();
}

void SceneMain::handleSlingshotInput(StateContext ctx)
{
	// -- Read the Mouse (only counts when the Window has focus) -- //
	bool mouseDown = ctx.window->hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	sf::Vector2f mousePx = ctx.window->mapPixelToCoords(sf::Mouse::getPosition(*ctx.window));
	// -- //

	if (this->m_currentBird != nullptr)
	{
		BirdState state = this->m_currentBird->getState();

		if (mouseDown && !this->m_wasMouseDown && state == BirdState::Waiting)
		{
			// -- Mouse just pressed: grab the Bird if the click was close enough -- //
			float grabRadius = this->m_currentBird->getRadiusPx() * GRAB_RADIUS_FACTOR;
			if (distanceBetween(mousePx, this->m_currentBird->getPositionPx()) <= grabRadius)
			{
				this->m_currentBird->beginDrag();
			}
			// -- //
		}
		else if (mouseDown && state == BirdState::Dragging)
		{
			// -- Mouse held: pull the Bird back, limited by the maximum pull distance -- //
			this->m_currentBird->setHeldPosition(this->m_slingshot->clampPull(mousePx));
			// -- //
		}
		else if (!mouseDown && state == BirdState::Dragging)
		{
			// -- Mouse released: launch the Bird, or cancel if barely pulled back -- //
			sf::Vector2f birdPos = this->m_currentBird->getPositionPx();

			if (distanceBetween(birdPos, this->m_slingshot->getAnchorPx()) < MIN_PULL_PX)
			{
				this->m_currentBird->setHeldPosition(this->m_slingshot->getAnchorPx());
				this->m_currentBird->cancelDrag();
			}
			else
			{
				this->m_currentBird->launch(this->m_slingshot->getLaunchVelocity(birdPos));
			}
			// -- //
		}
	}

	this->m_wasMouseDown = mouseDown;
}