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
    std::shared_ptr<sf::Sprite> m_sprite;
	b2Body* m_body = nullptr;
	b2World* m_physicsWorld = nullptr;
    b2Vec2 m_size;

	bool m_isIndustructible = false;
    bool m_isMakredForDestroy = false;
	float m_health = 10.0f; // Default Health to 10
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
    PhysicsObject(b2Shape::Type shapeType, std::shared_ptr<sf::Sprite> sprite, b2Vec2 size,
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
    ///     Physics Object receiving impact.
    /// </summary>
    /// 
    /// <param name="impactStrength">Strength of the impact.</param>
    void receiveImpact(float impactStrength);

	/// <summary>
	///     Draw the sprite of this physics object to the given window.
	/// </summary>
    /// 
	/// <param name="window">Reference to the render window target.</param>
	void draw(sf::RenderWindow& window) const;

    /// <param name="newHealth">New Health of this physics object .</param>
    /// <param name="isIndustructable">Is this physics object  industructable or not. Defaulted to false.</param>
    inline void setHealth(float newHealth, bool isIndustructable = false)
    {
		this->m_health = newHealth;
        this->m_isIndustructible = isIndustructable;
    };

    /// <summary>
	///     Check if this physics object is marked for destroy.
    /// </summary>
    inline bool isMarkedForDestroy() const { return this->m_isMakredForDestroy; };
};