/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Bird.h
Description : Declares the Bird Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

/// <summary>
///     The lifecycle states of a Bird.
/// </summary>
enum class BirdState
{
    Waiting,  // Sitting in the Slingshot
    Dragging, // Being pulled back by the Player
    Flying,   // Launched and under physics control
    Spent     // Came to rest, ready to be replaced by the next Bird
};

/// <summary>
///     The base class representing the overall Bird projectile.
/// </summary>
class Bird
{
protected:
    // -- Bird Properties -- //
    b2World* m_world; // Non-owning, used to destroy the Body
    b2Body* m_body; // Owned by the World
    BirdState m_state;

    float m_radiusPx;
    sf::Color m_color;

    float m_restTimer;
    float m_flightTimer;
    // -- //

public:
    /// <summary>
    ///     Constructor. Creates the Box2D Body held (kinematic) at the start position.
    /// </summary>
    /// 
    /// <param name="world">The Physics World that will own the Body.</param>
    /// <param name="startPx">The starting position in Pixels.</param>
    /// <param name="radiusPx">The radius in Pixels.</param>
    Bird(b2World& world, const sf::Vector2f& startPx, float radiusPx);

    /// <summary>
    ///     Destructor. Removes the Body from the World.
    /// </summary>
    virtual ~Bird();

    // The Bird owns a Body, so it must not be copied
    Bird(const Bird&) = delete;
    Bird& operator=(const Bird&) = delete;

    /// <summary>
    ///     Moves the held Bird to a position. Used while dragging.
    /// </summary>
    void setHeldPosition(const sf::Vector2f& positionPx);

    /// <summary>
    ///     Begins the drag. Only valid while Waiting.
    /// </summary>
    void beginDrag();

    /// <summary>
    ///     Cancels the drag and returns the Bird to the Waiting state.
    /// </summary>
    void cancelDrag();

    /// <summary>
    ///     Launches the Bird by applying an Impulse of mass * velocity.
    /// </summary>
    /// 
    /// <param name="velocity">The desired launch velocity in Metres per second.</param>
    void launch(const b2Vec2& velocity);

    /// <summary>
    ///     Tracks whether the Bird has come to rest. Derived Birds can extend this.
    /// </summary>
    virtual void update(float deltaTime);

    /// <summary>
    ///     Draws the Bird to the Window.
    /// </summary>
    virtual void render(sf::RenderWindow& window) const;

    /// <returns>
	/// Gets the current position of the Bird in Pixels.
    /// </returns>
    sf::Vector2f getPositionPx() const;

    /// <returns>
	///     Gets the radius of the Bird in Pixels.
    /// </returns>
    float getRadiusPx() const { return this->m_radiusPx; }

    /// <returns>
	///     Get the current state of the Bird.
    /// </returns>
    BirdState getState() const { return this->m_state; }
};