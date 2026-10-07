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

SceneMain::SceneMain()
{
	// Register the Commands
	this->registerCommands();
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
		}
	});
	// -- //
}

void SceneMain::update(StateContext ctx)
{}

void SceneMain::render(StateContext ctx)
{
	// -- TEMPORARY: Draw a simple Green Circle Shape to the Window -- //
	float centerX = static_cast<float>(Settings::getInstance().windowWidth) / 2.0f;
	float centerY = static_cast<float>(Settings::getInstance().windowHeight) / 2.0f;
	sf::Vector2f centerPosition(centerX, centerY);

	sf::CircleShape circle(50.0f);
	circle.setOrigin(circle.getGeometricCenter());
	circle.setFillColor(sf::Color::Green);
	circle.setPosition(centerPosition);
	ctx.window->draw(circle);
	// -- //
}