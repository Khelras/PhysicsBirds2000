/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Slingshot.h
Description : Declares the Slingshot Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "myproject/physics/Bird.h"

/// <summary>
///     The Slingshot. Holds the aiming rules and draws itself. It has no Physics Body of its own.
/// </summary>
class Slingshot
{
protected:
    // -- Slingshot Properties -- //
    sf::Vector2f m_anchorPx; // Where the Bird rests
    float m_maxPullPx; // Maximum pull-back distance
    float m_springFrequencyHz; // Spring stiffness expressed as a frequency
    float m_dampingRatio;
    float m_postHeightPx;
    b2Body* m_anchorBody; // Static Body at the anchor, owned by the World (never delete it)

    // -- //

public:
    /// <summary>
    ///     Constructor.
    /// </summary>
    Slingshot(b2World& world, const sf::Vector2f& anchorPx, float maxPullPx, float springFrequencyHz, float dampingRatio);

    /// <summary>
    ///     Destructor.
    /// </summary>
    ~Slingshot();

    /// <summary>
    ///     Clamps a Mouse position so it is never further than the maximum pull from the anchor.
    /// </summary>
    sf::Vector2f clampPull(const sf::Vector2f& mousePx) const;

    /// <summary>
    ///     Fires the Bird by attaching it to the anchor with a Spring.
    /// </summary>
    void fire(Bird& bird) const;

    /// <summary>
    ///     Draws the Slingshot post and the Rubber Bands.
    /// </summary>
    /// 
    /// <param name="bandEndPx">Where the Rubber Bands are stretched to.</param>
    void render(sf::RenderWindow& window, const sf::Vector2f& bandEndPx) const;

    /// <returns>
	///     Gets the anchor position in Pixels where the Bird rests.
    /// </returns>
    const sf::Vector2f& getAnchorPx() const { return this->m_anchorPx; }
};