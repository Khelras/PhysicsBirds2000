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
			// Ensure a Slingshot exists
			if (this->m_slingshot == nullptr) return;

			// Check if the Level is still settling
			if (this->m_isLevelSettling == true) return;

			// Convert the Mouse Position to World Coordinates.
			const sf::Vector2f mousePosition = ctx.window->mapPixelToCoords(sf::Mouse::getPosition(*ctx.window));

			// Convert Pixels to Box2D Metres.
			const b2Vec2 physicsPosition(
				mousePosition.x / pl::sizeScale,
				mousePosition.y / pl::sizeScale
			);

			// Only track a drag if the player clicked the bird.
			this->m_isLeftClicking = this->m_slingshot->beginDrag(physicsPosition);
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
			// Ensure a Slingshot exists
			if (this->m_slingshot == nullptr) return;

			// Only release if a drag was actually started
			if (this->m_isLeftClicking == false) return;

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
			// Check if there is an Active Bird Projectile
			if (this->m_activeBird != nullptr)
			{
				// Active Bird Projectile Ability
				if (this->m_activeBird->activateAbility() == true)
				{
					// DEBUG
					std::cout << "Space Key pressed, Bird Projectile ability activated." << std::endl;
				}
			}
        }
    });
    // -- //

	// -- R Key Pressed -- //
    this->m_commands.push_back({
        // Execution Criteria
        [](const sf::Event& event)
        {
			// First check if the Event was a Key Press, then check if the Key was the R Key
            if (const auto* key = event.getIf<sf::Event::KeyPressed>())
            {
                return key->code == sf::Keyboard::Key::R;
            }

            // Otherwise, the event does not match the criteria.
            return false;
        },

        // Command Action
        [this](const CommandContext& ctx)
        {
            // DEBUG
            std::cout << "R Key pressed, resetting the current level." << std::endl;

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
	this->m_physicsWorld = std::make_unique<b2World>(pl::gravity);
	this->m_contactListener = std::make_unique<PhysicsContactListener>(this->m_physicsWorld.get(), this);
	this->m_physicsWorld->SetContactListener(this->m_contactListener.get());

	// Ground Object
	this->m_physicsObjects.emplace_back(
		std::make_unique<PhysicsObject>(
			b2Shape::e_polygon,
			this->m_groundSprite,
			b2Vec2(28.5f, 1.5f), // Ground size
			b2Vec2(14.25f, 15.25f), // Ground position
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

	// Physics World Step
	this->m_physicsWorld->Step(timeStep, 8, 3);

	// Level Settling State Update
	this->updateLevelSettling(timeStep);

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
			// Remove from the Non-Owning Enemy Vector (if it is an Enemy)
			this->m_enemies.erase(
				std::remove(this->m_enemies.begin(), this->m_enemies.end(), it->get()),
				this->m_enemies.end()
			);

			// Destroy the Physics Object
			it = this->m_physicsObjects.erase(it);
		}
		else
		{
			it++;
		}
	}

	// Check if the next bird should be loaded.
	this->updateBirdLoading(timeStep);
}

void ScenePhysics::render(StateContext ctx)
{
	// Draw the Slingshot and its Band.
	if (this->m_slingshot != nullptr)
	{
		this->m_slingshot->draw(*ctx.window);
		this->m_slingshot->drawBand(*ctx.window);
	}

	// Draw the Birds waiting in line behind the Slingshot.
	this->drawBirdQueue(*ctx.window);

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
	this->m_birdStack.clear();
	this->m_birdFlightTime = 0.0f;
	this->m_birdStillTime = 0.0f;

	// Reset Input State
	this->m_isLeftClicking = false;

	// Stop callbacks into the scene during cleanup
	if (this->m_physicsWorld != nullptr)
	{
		this->m_physicsWorld->SetContactListener(nullptr);
	}

	// Delete the Slingshot's joint
	this->m_slingshot.reset();

	// Clear the Non-Owning Ememies Vector
	this->m_enemies.clear();

	// Delete and clear the Physics Object Vector
	this->m_physicsObjects.clear();

	// Reset the world after clearig all Physics Objects
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

	if (this->m_pigTexture.loadFromFile("assets/textures/pig.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/pig.png'" << std::endl;

	if (this->m_plankTexture.loadFromFile("assets/textures/plank.png") == false)
		std::cerr << "Error loading texture from 'assets/textures/plank.png'" << std::endl;

	// -- SLINGSHOT PLACEHOLDER IMAGE TEXTURE -- //
	const sf::Image slingshotImage(sf::Vector2u(32u, 128u), sf::Color(130, 75, 35));
	if (this->m_slingshotTexture.loadFromImage(slingshotImage) == false)
		throw std::runtime_error("Error creating the slingshot placeholder texture.");
	// -- //

	// Create the Sprites
	this->m_slingshotSprite = std::make_shared<sf::Sprite>(this->m_slingshotTexture);
	this->m_groundSprite = std::make_shared<sf::Sprite>(this->m_groundTexture);
	this->m_redBirdSprite = std::make_shared<sf::Sprite>(this->m_redBirdTexture);
	this->m_yellowBirdSprite = std::make_shared<sf::Sprite>(this->m_yellowBirdTexture);
	this->m_greenBirdSprite = std::make_shared<sf::Sprite>(this->m_greenBirdTexture);
	this->m_pigSprite = std::make_shared<sf::Sprite>(this->m_pigTexture);
	this->m_plankSprite = std::make_shared<sf::Sprite>(this->m_plankTexture);
}

void ScenePhysics::updateLevelSettling(float timeStep)
{
	// Ensure the Level is in a settling state
	if (this->m_isLevelSettling == false) return;

	this->m_levelSettleTime += timeStep;

	// Loop through every dynamic body and check if it has fallen asleep
	bool isAtRest = true;
	for (b2Body* body = this->m_physicsWorld->GetBodyList(); body != nullptr; body = body->GetNext())
	{
		// Check if this dynamic body is awake
		if (body->GetType() == b2BodyType::b2_dynamicBody && body->IsAwake() == true)
		{
			// If at least 1 dynamic body is awake then this check fails
			isAtRest = false;
			break;
		}
	}

	// Check if the level has settled (all dynamic bodies are at rest)
	if (isAtRest == true || this->m_levelSettleTime >= this->m_maxLevelSettleTime)
	{
		this->m_isLevelSettling = false;

		// DEBUG
		std::cout << "Level settled after " << this->m_levelSettleTime << " seconds." << std::endl;

		// Load the first bird now that the level has settled.
		this->loadNextBird();
	}
}

void ScenePhysics::createSlingshot()
{
	// Slingshot Physics Object
	this->m_slingshot = std::make_unique<Slingshot>(
		this->m_slingshotSprite,
		b2Vec2(0.45f, 2.25f),
		b2Vec2(3.75f, 13.375f),
		this->m_physicsWorld.get()
	);

	// Fill the Bird Projectile stack, the back of the stack is loaded first (Red -> Yellow -> Green).
	this->m_birdStack = { BirdType::Green, BirdType::Yellow, BirdType::Red };
}

void ScenePhysics::createPlank(b2Vec2 position, sf::Angle rotation, float health)
{
	// Create the Plank Physics Object
	this->m_physicsObjects.emplace_back(
		std::make_unique<PhysicsObject>(
			b2Shape::e_polygon,
			this->m_plankSprite,
			b2Vec2(0.375f, 2.25f),
			position,
			rotation,
			b2BodyType::b2_dynamicBody,
			this->m_physicsWorld.get()
		)
	)->setHealth(health);
}

void ScenePhysics::createEnemyPig(b2Vec2 position, float health)
{
	// Create the Enemy Physics Object
	auto enemy = std::make_unique<PhysicsObject>(
		b2Shape::e_circle,
		this->m_pigSprite,
		b2Vec2(0.8f, 0.75f),
		position,
		sf::degrees(0.0f),
		b2BodyType::b2_dynamicBody,
		this->m_physicsWorld.get()
	);

	enemy->setHealth(health);

	// Track the Enemy, the scene owns it and the list only borrows it.
	this->m_enemies.push_back(enemy.get());
	this->m_physicsObjects.emplace_back(std::move(enemy));
}

void ScenePhysics::loadNextBird()
{
	// Check if there are any Bird Projectiles left to load.
	if (this->m_birdStack.empty() == true)
	{
		// DEBUG
		std::cout << "No birds remaining." << std::endl;

		// TODO: Show the lose screen and allow a restart.

		this->m_activeBird = nullptr;
		return;
	}

	// Pop the next bird type from the stack.
	const BirdType birdType = this->m_birdStack.back();
	this->m_birdStack.pop_back();

	// Create Bird Projectile
	auto bird = std::make_unique<BirdProjectile>(
		this->getBirdSprite(birdType),
		this->m_slingshot->getAnchorPosition(),
		this->m_physicsWorld.get()
	);

	// Set the Ability
	switch (birdType)
	{
		// Red Bird
		case (BirdType::Red): break; // The Red Bird has no ability.

		// Yellow Bird
		case (BirdType::Yellow):
		{
			// Speed Boost
			bird->setAbility([](BirdProjectile& self)
			{
				b2Body* body = self.getBody();
				body->SetLinearVelocity(2.75f * body->GetLinearVelocity());
			});
		} break;

		// Green Bird
		case (BirdType::Green):
		{
			// TODO: Split into multiple birds.
		} break;

		// Default
		default: break;
	}

	// Attach the bird using the distance joint.
	this->m_slingshot->loadBird(*bird);

	// Track the new bird, the scene owns it and the slingshot only borrows it.
	this->m_activeBird = bird.get();
	this->m_physicsObjects.emplace_back(std::move(bird));

	// Update the Bird Projectile Loading State
	this->m_birdFlightTime = 0.0f;
	this->m_birdStillTime = 0.0f;

	// DEBUG
	std::cout << "Bird loaded. Birds remaining after this one: " << this->m_birdStack.size() << std::endl;
}

std::shared_ptr<sf::Sprite> ScenePhysics::getBirdSprite(BirdType type) const
{
	switch (type)
	{
		case (BirdType::Red): return this->m_redBirdSprite;
		case (BirdType::Yellow): return this->m_yellowBirdSprite;
		case (BirdType::Green): return this->m_greenBirdSprite;
		default: return this->m_redBirdSprite;
	}
}

void ScenePhysics::drawBirdQueue(sf::RenderWindow& window) const
{
	// The queue is positioned relative to the slingshot.
	if (this->m_slingshot == nullptr) return;
	const float queueStartX = this->m_slingshot->getAnchorPosition().x - this->m_birdQueueOffsetX;

	// Draw from the next bird (back of the stack) outwards, away from the slingshot
	for (size_t i = 0; i < this->m_birdStack.size(); i++)
	{
		const BirdType birdType = this->m_birdStack[this->m_birdStack.size() - 1 - i];
		std::shared_ptr<sf::Sprite> sprite = this->getBirdSprite(birdType);

		// Scale of the Sprite (birds are 1 metre wide, matching the BirdProjectile)
		const sf::Vector2u textureSize = sprite->getTexture().getSize();
		const sf::Vector2f spriteScale(
			0.75f * pl::sizeScale / static_cast<float>(textureSize.x),
			0.75f * pl::sizeScale / static_cast<float>(textureSize.y)
		);

		// Set the Sprite's Scale, Origin, Position, and Rotation
		// Sprites are shared with the flying bird, so every property is set before drawing
		sprite->setScale(spriteScale);
		sprite->setOrigin(sprite->getLocalBounds().getCenter());
		sprite->setRotation(sf::degrees(0.0f));
		sprite->setPosition({
			(queueStartX - static_cast<float>(i) * this->m_birdQueueSpacing) * pl::sizeScale,
			this->m_birdQueueY * pl::sizeScale
		});

		// Draw the Sprite
		window.draw(*sprite);
	}
}

void ScenePhysics::updateBirdLoading(float timeStep)
{
	// Only track a Bird Projectile that has actually been launched
	if (this->m_activeBird == nullptr || this->m_slingshot == nullptr) return;
	if (this->m_activeBird->getState() != BirdProjectile::State::Flying) return;

	this->m_birdFlightTime += timeStep;

	b2Body* birdBody = this->m_activeBird->getBody();
	const b2Vec2 birdPosition = birdBody->GetPosition();
	const float birdSpeed = birdBody->GetLinearVelocity().Length();

	// Track how long the Bird Projectile has been nearly still
	if (birdSpeed < this->m_stillSpeed)
	{
		this->m_birdStillTime += timeStep;
	}
	else
	{
		this->m_birdStillTime = 0.0f;
	}

	// Check if the Bird Projectile is still and has therefore settled
	const bool hasSettled = this->m_birdFlightTime >= this->m_minFlightTime &&
		(this->m_birdStillTime >= this->m_requiredStillTime || birdBody->IsAwake() == false);

	// Check if the Bird Projectile is within the screen
	const bool isOutOfBounds = birdPosition.x < -5.0f || birdPosition.x > 30.0f || birdPosition.y > 25.0f;

	// Safety Timer to prevent infinite waiting
	const bool hasTimedOut = this->m_birdFlightTime >= this->m_maxFlightTime;

	// Check all if any of the conditions are met
	if (hasSettled == true || isOutOfBounds == true || hasTimedOut == true)
	{
		// Stop tracking the recently launched Bird Projectile
		this->m_activeBird = nullptr;

		// Check how many Enemies remain before loading the next Bird Projectile
		this->checkEnemiesRemaining();
		if (this->m_enemies.empty() == true) return;

		// Load the next bird
		this->loadNextBird();
	}
}

void ScenePhysics::checkEnemiesRemaining()
{
	// DEBUG
	std::cout << "Enemies remaining: " << this->m_enemies.size() << std::endl;

	if (this->m_enemies.empty() == true)
	{
		// DEBUG
		std::cout << "All enemies defeated! Level complete." << std::endl;

		// TODO: Go to the next level or show the win screen.

		this->m_activeBird = nullptr;
		return;
	}
}

void ScenePhysics::resetLevel()
{
	// Simply rebuild the level
	this->createPhysicsWorld();
}