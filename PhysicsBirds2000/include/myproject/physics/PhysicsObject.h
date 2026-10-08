/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : PhysicsObject.h
Description : Declares the PhysicsObject Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

/// <summary>
///     The base physics object class which serves as a blueprint for all physics objects in the game.
/// </summary>
class PhysicsObject
{
private:
    // -- Physics Object Properties -- //
	sf::Sprite* m_sprite = nullptr;
	b2Body* m_body = nullptr;
	b2World* m_physicsWorld = nullptr;
    b2Vec2 m_size;
    // -- //

public:
    /// <summary>
    ///     Constructor.
    /// </summary>
    /// 
    /// <param name="shapeType">Shape of the physics object.</param>
    /// <param name="sprite">Pointer to sprite representing this physics object.</param>
    /// <param name="size">Size of the physics object.</param>
    /// <param name="position">Position of this physics object.</param>
    /// <param name="rotation">Rotation of this physics object.</param>
    /// <param name="bodyType">Body type of this physics object (rigid, kinematic, or dynamic).</param>
    /// <param name="physicsWorld">Pointer to the physics world.</param>
    PhysicsObject(b2Shape::Type shapeType, sf::Sprite* sprite, b2Vec2 size,
        b2Vec2 position, sf::Angle rotation, b2BodyType bodyType, b2World* physicsWorld);

    /// <summary>
    ///     Destructor.
    /// </summary>
    ~PhysicsObject();

	/// <returns>
	///	    Pointer to the Box2D Body of this physics object.
    /// </returns>
	b2Body* getBody() const;
    
	/// <summary>
	///     Draw the sprite of this physics object to the given window.
	/// </summary>
    /// 
	/// <param name="window">Reference to the render window target.</param>
	void draw(sf::RenderWindow& window) const;
};