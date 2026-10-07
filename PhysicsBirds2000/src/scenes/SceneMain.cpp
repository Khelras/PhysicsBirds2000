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

#include <iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "myproject/scenes/SceneMain.h"
#include "myproject/core/Settings.h"

namespace
{
	// Box2D works in Metres whereas SFML works in Pixels
	constexpr float PIXELS_PER_METRE = 30.0f;
	constexpr float CIRCLE_RADIUS_PX = 50.0f;
	constexpr float FIXED_TIME_STEP = 1.0f / 60.0f;
	constexpr int VELOCITY_ITERATIONS = 8;
	constexpr int POSITION_ITERATIONS = 3;

	float toMetres(float pixels) { return pixels / PIXELS_PER_METRE; }
	float toPixels(float metres) { return metres * PIXELS_PER_METRE; }
}

SceneMain::SceneMain()
{
	// Register the Commands
	this->registerCommands();

	// Create the Physics World
	this->createPhysicsWorld();
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
			std::cout << "Space Key Pressed in Context of the Main Scene!" << std::endl;

			// Launch the Ball upwards (Negative Y is up)
			this->m_ballBody->ApplyLinearImpulseToCenter(b2Vec2(0.0f, -30.0f), true);
		}
	});
	// -- //
}

void SceneMain::createPhysicsWorld()
{
	// Width and Height of the Window in Pixels
	float windowW = static_cast<float>(Settings::getInstance().windowWidth);
	float windowH = static_cast<float>(Settings::getInstance().windowHeight);

	// Gravity points down the Y Axis (SFML's Y Axis already points down the screen)
	this->m_world = std::make_unique<b2World>(b2Vec2(0.0f, 9.8f));

	// -- Window Edges (One Static Body with four Edge Fixtures) -- //
	b2BodyDef wallDef;
	wallDef.type = b2_staticBody;
	wallDef.position.Set(0.0f, 0.0f);
	this->m_wallBody = this->m_world->CreateBody(&wallDef);

	float w = toMetres(windowW);
	float h = toMetres(windowH);

	b2Vec2 topLeft(0.0f, 0.0f);
	b2Vec2 topRight(w, 0.0f);
	b2Vec2 bottomRight(w, h);
	b2Vec2 bottomLeft(0.0f, h);

	b2EdgeShape edge;

	edge.SetTwoSided(topLeft, topRight);       // Top
	this->m_wallBody->CreateFixture(&edge, 0.0f);
	edge.SetTwoSided(topRight, bottomRight);   // Right
	this->m_wallBody->CreateFixture(&edge, 0.0f);
	edge.SetTwoSided(bottomRight, bottomLeft); // Bottom
	this->m_wallBody->CreateFixture(&edge, 0.0f);
	edge.SetTwoSided(bottomLeft, topLeft);     // Left
	this->m_wallBody->CreateFixture(&edge, 0.0f);
	// -- //

	// -- Ball (Dynamic Body with a Circle Fixture) -- //
	b2BodyDef ballDef;
	ballDef.type = b2_dynamicBody;
	ballDef.position.Set(toMetres(windowW / 2.0f), toMetres(windowH / 4.0f));
	this->m_ballBody = this->m_world->CreateBody(&ballDef);

	b2CircleShape circleShape;
	circleShape.m_radius = toMetres(CIRCLE_RADIUS_PX);

	b2FixtureDef ballFixture;
	ballFixture.shape = &circleShape;
	ballFixture.density = 1.0f;
	ballFixture.friction = 0.3f;
	ballFixture.restitution = 0.8f; // Bounciness (0 = no bounce, 1 = perfect bounce)
	this->m_ballBody->CreateFixture(&ballFixture);

	// Give the Ball a small sideways push so it does not just bounce straight up and down
	this->m_ballBody->SetLinearVelocity(b2Vec2(6.0f, 0.0f));
	// -- //
}

void SceneMain::update(StateContext ctx)
{
	// Clamp the given Delta Time so any lag spikes does NOT cause a huge physics jump
	float deltaTime = ctx.dt;
	if (deltaTime > 0.25f) deltaTime = 0.25f;

	// Step the Physics World at a Fixed Time Step for a stable and consistent simulation
	this->m_accumulator += deltaTime;
	while (this->m_accumulator >= FIXED_TIME_STEP)
	{
		this->m_world->Step(FIXED_TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
		this->m_accumulator -= FIXED_TIME_STEP;
	}
}

void SceneMain::render(StateContext ctx)
{
	// -- TEMPORARY: Draw a simple Green Circle Shape to the Window -- //
	b2Vec2 ballPosition = this->m_ballBody->GetPosition();
	sf::Vector2f centerPosition(toPixels(ballPosition.x), toPixels(ballPosition.y));

	sf::CircleShape circle(CIRCLE_RADIUS_PX);
	circle.setOrigin(circle.getGeometricCenter());
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(centerPosition);
	ctx.window->draw(circle);
	// -- //
}