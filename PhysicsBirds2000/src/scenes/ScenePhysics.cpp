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
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "myproject/scenes/ScenePhysics.h"
#include "myproject/core/Settings.h"
#include "myproject/physics/PhysicsLibrary.h"
#include "myproject/physics/PhysicsContactListener.h"
#include "myproject/physics/Slingshot.h"
#include "myproject/physics/BirdProjectile.h"

ScenePhysics::ScenePhysics()
{
	// Register the Commands
	this->registerCommands();

	// Load the Assets
	this->loadAssets();
}

ScenePhysics::~ScenePhysics()
{
	this->clearPhysicsWorld();
}

void ScenePhysics::registerCommands()
{
	// -- Left Mouse Button Pressed -- //
    this->m_commands.push_back({
        // Execution Criteria
        [](const sf::Event& event)
        {
			// First check if the Event was a Mouse Button Press, then check if the Mouse Button was the Left Mouse Button
            if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
            {
				return mouse->button == sf::Mouse::Button::Left;
            }

            // Otherwise, the event does not match the criteria.
            return false;
        },

        // Command Action
        [this](const CommandContext& ctx)
        {
			// Convert the Mouse Position to World Coordinates.
			const sf::Vector2f mousePosition = ctx.window->mapPixelToCoords(sf::Mouse::getPosition(*ctx.window));

			// Convert Pixels to Box2D Metres.
			const b2Vec2 physicsPosition(
				mousePosition.x / pl::sizeScale,
				mousePosition.y / pl::sizeScale
			);

			// Only track a drag if the player clicked the bird.
			this->m_isLeftClicking = this->m_slingshot->beginDrag(physicsPosition);

			std::cout << (this->m_isLeftClicking ? "mouse dragging" : "not dragging") << std::endl;
        }
    });
    // -- //

	// -- Left Mouse Button Release -- //
    this->m_commands.push_back({
        // Execution Criteria
        [](const sf::Event& event)
        {
			// First check if the Event was a Mouse Button Release, then check if the Mouse Button was the Left Mouse Button
            if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>())
            {
				return mouse->button == sf::Mouse::Button::Left;
            }

            // Otherwise, the event does not match the criteria.
            return false;
        },

        // Command Action
		[this](const CommandContext& ctx)
		{
			// Convert the Mouse Position to World Coordinates.
			const sf::Vector2f mousePosition = ctx.window->mapPixelToCoords(sf::Mouse::getPosition(*ctx.window));

			// Convert Pixels to Box2D Metres.
			const b2Vec2 physicsPosition(
				mousePosition.x / pl::sizeScale,
				mousePosition.y / pl::sizeScale
			);

			// Update the final target, then release the bird.
			this->m_slingshot->dragTo(physicsPosition);
			this->m_slingshot->release();

			this->m_isLeftClicking = false;
		}
    });
    // -- //

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

            // Otherwise, the event does not match the criteria.
            return false;
        },

        // Command Action
        [this](const CommandContext& ctx)
        {
            // DEBUG
            std::cout << "Space Key pressed, resetting the current level." << std::endl;

            // Reset the Current Level
            this->resetLevel();
        }
    });
    // -- //
}

void ScenePhysics::handleEvent(const sf::Event& event, const CommandContext& ctx)
{
	// Call from the Base Class
	Scene::handleEvent(event, ctx);

	// Extend Functionality
	// Ensure the Window and Slingshot exist.
	if (ctx.window == nullptr || this->m_slingshot == nullptr)
	{
		return;
	}

	// -- Mouse Moved -- //
	if (const auto* mouseMove =
		event.getIf<sf::Event::MouseMoved>())
	{
		if (this->m_isLeftClicking == true)
		{
			// Convert the Mouse Position to World Coordinates.
			const sf::Vector2f mousePosition =
				ctx.window->mapPixelToCoords(mouseMove->position);

			// Convert Pixels to Box2D Metres.
			const b2Vec2 physicsPosition(
				mousePosition.x / pl::sizeScale,
				mousePosition.y / pl::sizeScale
			);

			// Update the Slingshot's Drag Target.
			this->m_slingshot->dragTo(physicsPosition);
		}

		return;
	}
	// -- //

	// -- Window Focus Lost -- //
	if (event.is<sf::Event::FocusLost>())
	{
		// Avoid a stuck drag if the player leaves the window.
		if (this->m_isLeftClicking == true)
		{
			this->m_slingshot->release();
		}

		this->m_isLeftClicking = false;
	}
	// -- //
}

void ScenePhysics::createPhysicsWorld()
{
	// Safely remove any previous world before rebuilding
	this->clearPhysicsWorld();

	// Create the Physics World
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
	)->setHealth(1.0, true);

	// Create the Slingshot
	this->createSlingshot();
}

void ScenePhysics::update(StateContext ctx)
{
	// Ensure the Physics World exists.
	if (this->m_physicsWorld == nullptr)
	{
		return;
	}

	// Time Step at 60FPS
	constexpr float timeStep = 1.0f / 60.0f;

	// Apply Slingshot Dragging Forces.
	if (this->m_slingshot != nullptr)
	{
		this->m_slingshot->applyDragForce();
		this->m_slingshot->update(timeStep);
	}

	// Check if the next bird should be loaded.
	this->updateBirdLoading(timeStep);

	// Physics World Step
	this->m_physicsWorld->Step(timeStep, 8, 3);

	// Update the Slingshot after the Physics World Step.
	if (this->m_slingshot != nullptr)
	{
		this->m_slingshot->update(timeStep);
	}

	// Remove Physics Objects that are marked for destruction
	for (auto it = this->m_physicsObjects.begin(); it != this->m_physicsObjects.end();)
	{
		if ((*it)->isMarkedForDestroy())
		{
			it = this->m_physicsObjects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void ScenePhysics::render(StateContext ctx)
{
	// Draw the Slingshot and its Band.
	if (this->m_slingshot != nullptr)
	{
		this->m_slingshot->draw(*ctx.window);
		this->m_slingshot->drawBand(*ctx.window);
	}

	// Draw the Scene's Physics Objects.
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

void ScenePhysics::clearPhysicsWorld()
{
	// Reset Bird Loading State
	this->m_activeBird = nullptr;
	this->m_birdsRemaining = 0;
	this->m_birdFlightTime = 0.0f;
	this->m_birdStillTime = 0.0f;

	// Reset Input State
	this->m_isLeftClicking = false;

	// Stop callbacks into the scene during cleanup.
	if (this->m_physicsWorld != nullptr)
	{
		this->m_physicsWorld->SetContactListener(nullptr);
	}

	// Destroy the slingshot's joint before its connected bodies.
	this->m_slingshot.reset();

	// PhysicsObject destructors still need a valid world.
	this->m_physicsObjects.clear();

	// Destroy the world after all owned physics objects.
	this->m_physicsWorld.reset();
	this->m_contactListener.reset();
}

void ScenePhysics::loadAssets()
{
	// Load the Textures
	if (this->m_groundTexture.loadFromFile("assets/textures/ground.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/ground.png'" << std::endl;
	//if (this->m_slingshotTexture.loadFromFile("assets/textures/slingshot.png") == false)
	//	std::cerr << "Error loading texture from 'assets/textures/slingshot.png'" << std::endl;
	if (this->m_redBirdTexture.loadFromFile("assets/textures/red_bird.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/red_bird.png'" << std::endl;
	if (this->m_yellowBirdTexture.loadFromFile("assets/textures/yellow_bird.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/yellow_bird.png'" << std::endl;
	if (this->m_greenBirdTexture.loadFromFile("assets/textures/green_bird.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/green_bird.png'" << std::endl;
	if (this->m_plankTexture.loadFromFile("assets/textures/plank.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/plank.png'" << std::endl;

	// -- TESTING SLINGSHOT IMAGE TEXTURE -- //
	const sf::Image slingshotImage(sf::Vector2u(32u, 128u), sf::Color(130, 75, 35));
	if (this->m_slingshotTexture.loadFromImage(slingshotImage) == false)
		throw std::runtime_error("Error creating the slingshot placeholder texture.");
	// -- //

	// Create the Sprites
	this->m_slingshotSprite = std::make_shared<sf::Sprite>(this->m_slingshotTexture);
	this->m_groundSprite = std::make_shared<sf::Sprite>(this->m_groundTexture);
	this->m_redBirdSprite = std::make_shared<sf::Sprite>(this->m_redBirdTexture);
	this->m_yellowBirdSprite = std::make_shared<sf::Sprite>(this->m_yellowBirdTexture);
	this->m_plankSprite = std::make_shared<sf::Sprite>(this->m_plankTexture);
}

void ScenePhysics::createPlank(b2Vec2 position, sf::Angle rotation, float health)
{
	// Create the Plank Physics Object
	this->m_physicsObjects.emplace_back(
		std::make_unique<PhysicsObject>(
			b2Shape::e_polygon,
			this->m_plankSprite,
			b2Vec2(0.5f, 3.0f),
			position,
			rotation,
			b2BodyType::b2_dynamicBody,
			this->m_physicsWorld.get()
		)
	)->setHealth(health);
}

void ScenePhysics::createSlingshot()
{
	// Slingshot Physics Object
	this->m_slingshot = std::make_unique<Slingshot>(
		this->m_slingshotSprite,
		b2Vec2(0.6f, 3.0f),
		b2Vec2(5.0f, 12.5f),
		this->m_physicsWorld.get()
	);

	// Start with a full set of bird projectiles and load the first one.
	this->m_birdsRemaining = this->m_maxBirds;
	this->loadNextBird();
}

void ScenePhysics::resetLevel()
{
	// Simply rebuild the level
	this->createPhysicsWorld();
}

void ScenePhysics::loadNextBird()
{
	// Check if there are any birds left to load.
	if (this->m_birdsRemaining <= 0)
	{
		// DEBUG
		std::cout << "No birds remaining." << std::endl;

		this->m_activeBird = nullptr;
		return;
	}

	// Bird Projectile
	auto bird = std::make_unique<BirdProjectile>(
		this->m_redBirdSprite,
		this->m_slingshot->getAnchorPosition(),
		this->m_physicsWorld.get()
	);

	// Attach the bird using the distance joint.
	this->m_slingshot->loadBird(*bird);

	// Track the new bird, the scene owns it and the slingshot only borrows it.
	this->m_activeBird = bird.get();
	this->m_physicsObjects.emplace_back(std::move(bird));

	// Update the Bird Loading State
	this->m_birdsRemaining--;
	this->m_birdFlightTime = 0.0f;
	this->m_birdStillTime = 0.0f;

	// DEBUG
	std::cout << "Bird loaded. Birds remaining after this one: " << this->m_birdsRemaining << std::endl;
}

void ScenePhysics::updateBirdLoading(float timeStep)
{
	// Only track a bird that has actually been launched.
	if (this->m_activeBird == nullptr || this->m_slingshot == nullptr) return;
	if (this->m_activeBird->getState() != BirdProjectile::State::Flying) return;

	this->m_birdFlightTime += timeStep;

	b2Body* birdBody = this->m_activeBird->getBody();
	const b2Vec2 birdPosition = birdBody->GetPosition();
	const float birdSpeed = birdBody->GetLinearVelocity().Length();

	// Track how long the bird has been nearly still.
	if (birdSpeed < this->m_stillSpeed)
	{
		this->m_birdStillTime += timeStep;
	}
	else
	{
		this->m_birdStillTime = 0.0f;
	}

	// Settled: nearly still for long enough, or Box2D put the body to sleep.
	const bool hasSettled = this->m_birdFlightTime >= this->m_minFlightTime &&
		(this->m_birdStillTime >= this->m_requiredStillTime || birdBody->IsAwake() == false);

	// Out of Bounds: the bird has left the play area.
	const bool isOutOfBounds = birdPosition.x < -5.0f || birdPosition.x > 35.0f || birdPosition.y > 25.0f;

	// Safety Timer: the bird has been in flight for too long.
	const bool hasTimedOut = this->m_birdFlightTime >= this->m_maxFlightTime;

	if (hasSettled == true || isOutOfBounds == true || hasTimedOut == true)
	{
		// The spent bird stays in the scene, so only stop tracking it.
		this->m_activeBird = nullptr;

		// Load the next bird.
		this->loadNextBird();
	}
}