/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Window.cpp
Description : Defines the Window Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <iostream>
#include "myproject/core/Window.h"
#include "myproject/core/Settings.h"
#include "myproject/core/Command.h"
#include "myproject/states/state.h"

Window::Window()
{
	// Default Window Properties
	Settings& settings = Settings::getInstance();
	sf::VideoMode defaultMode({ settings.windowWidth, settings.windowHeight });
	sf::String defaultName = "SFML v3.1.0 Template!";

	// Create an SFML RenderWindow
	this->create(defaultMode, defaultName, sf::Style::Close);
	this->m_resolution = defaultMode.size;
	this->setKeyRepeatEnabled(false);
	this->setFramerateLimit(settings.maxFrameramte);

	// Context
	this->m_commandContext = new CommandContext();
	this->m_stateContext = new StateContext();
}

Window::Window(sf::VideoMode mode, const std::string& name)
{
	// Create an SFML RenderWindow
	this->create(mode, name, sf::Style::Close);
	this->m_resolution = mode.size;
	this->setKeyRepeatEnabled(false);
	this->setFramerateLimit(Settings::getInstance().maxFrameramte);

	// Context
	this->m_commandContext = new CommandContext();
	this->m_stateContext = new StateContext();
}

Window::~Window()
{
	if (this->m_commandContext != nullptr)
	{
		delete(this->m_commandContext);
		this->m_commandContext = nullptr;
	}

	if (this->m_stateContext != nullptr)
	{
		delete(this->m_stateContext);
		this->m_stateContext = nullptr;
	}

}

void Window::process()
{
	// Relevant Context
	this->m_commandContext->window = this;
	this->m_commandContext->stateManager = &this->m_stateManager;
	this->m_commandContext->sceneManager = nullptr; // Set within the State
	this->m_stateContext->window = this;

	// Clock
	sf::Clock clock;

	// -- Main Process Loop -- //
	while (this->isOpen() == true)
	{
		// Delta Time
		float dt = clock.restart().asSeconds();
		this->m_stateContext->dt = dt;

		// Process all raw SFML Events
		while (const auto event = this->pollEvent())
		{
			// -- 1. Global Raw SFML Event Handling -- //
			// Window Close Event in Global Context
			if (event->is<sf::Event::Closed>())
			{
				// DEBUG
				std::cout << "Window Close Event triggered in Global Context!" << std::endl;

				// Close the Window
				this->close();
				break;
			}

			// Key Pressed Event in Global Context
			if (const auto* key = event->getIf<sf::Event::KeyPressed>())
			{
				// Escape Key Pressed in Global Context
				if (key->scancode == sf::Keyboard::Scancode::Escape)
				{
					// DEBUG
					std::cout << "Escape-Key Pressed in Global Context!" << std::endl;

					// Close the Window
					this->close();
					break;
				}
			}
			// -- //

			// -- 2. Otherwise, Send to State Manager for State-Specific Event Handling -- // 
			this->m_stateManager.handleEvent(*event, *this->m_commandContext);
			// -- //
		}

		// -- Update Loops -- //
		this->m_stateManager.update(*this->m_stateContext);
		// -- //

		// -- Rendering -- //
		this->clear(); // Step 1: Clear the back buffer
		this->draw(); // Step 2: Draw and render objects to the back buffer
		this->display(); // Step 3: Swap buffers and display the back buffer
		// -- //
	}
	// -- //
}

void Window::clear()
{
	sf::RenderWindow::clear(this->m_stateManager.getActiveState().lock()->getBackgroundColor());
}

void Window::draw()
{
	this->m_stateManager.render(*this->m_stateContext);
}

void Window::display()
{
	sf::RenderWindow::display();
}
