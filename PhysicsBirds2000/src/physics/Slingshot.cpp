/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Slingshot.cpp
Description : Defines the Slingshot Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <cmath>
#include "myproject/physics/Slingshot.h"
#include "myproject/physics/PhysicsLibrary.h"

namespace
{
    // Draws a thick line between two points using a rotated Rectangle
    void drawThickLine(sf::RenderWindow& window, const sf::Vector2f& a, const sf::Vector2f& b, float thickness, const sf::Color& color)
    {
        sf::Vector2f delta = b - a;
        float length = std::hypot(delta.x, delta.y);

        sf::RectangleShape line({ length, thickness });
        line.setOrigin({ 0.0f, thickness / 2.0f });
        line.setPosition(a);
        line.setRotation(sf::radians(std::atan2(delta.y, delta.x)));
        line.setFillColor(color);
        window.draw(line);
    }
}

Slingshot::Slingshot(b2World& world, const sf::Vector2f& anchorPx, float maxPullPx, float springFrequencyHz, float dampingRatio)
{
    this->m_anchorPx = anchorPx;
    this->m_maxPullPx = maxPullPx;
	this->m_springFrequencyHz = springFrequencyHz;
	this->m_dampingRatio = dampingRatio;
    this->m_postHeightPx = 120.0f;

    // Static Body at the anchor point for the Spring Joint to attach to
    b2BodyDef anchorDef;
    anchorDef.type = b2_staticBody;
    anchorDef.position.Set(PhysicsLibrary::toMetres(anchorPx.x), PhysicsLibrary::toMetres(anchorPx.y));
    this->m_anchorBody = world.CreateBody(&anchorDef);
}

Slingshot::~Slingshot()
{

}

sf::Vector2f Slingshot::clampPull(const sf::Vector2f& mousePx) const
{
    sf::Vector2f offset = mousePx - this->m_anchorPx;
    float length = std::hypot(offset.x, offset.y);

    if (length > this->m_maxPullPx)
    {
        offset *= (this->m_maxPullPx / length);
    }

    return this->m_anchorPx + offset;
}

void Slingshot::fire(Bird& bird) const
{
    bird.launchWithSpring(*this->m_anchorBody, this->m_springFrequencyHz, this->m_dampingRatio);
}

void Slingshot::render(sf::RenderWindow& window, const sf::Vector2f& bandEndPx) const
{
    // -- Post -- //
    sf::RectangleShape post({ 16.0f, this->m_postHeightPx });
    post.setOrigin({ 8.0f, 0.0f });
    post.setPosition(this->m_anchorPx + sf::Vector2f(0.0f, 10.0f));
    post.setFillColor(sf::Color(110, 70, 30));
    window.draw(post);
    // -- //

    // -- Rubber Bands (Left and Right Prongs) -- //
    sf::Vector2f leftPronged = this->m_anchorPx + sf::Vector2f(-14.0f, 0.0f);
    sf::Vector2f rightPronged = this->m_anchorPx + sf::Vector2f(14.0f, 0.0f);
    drawThickLine(window, leftPronged, bandEndPx, 5.0f, sf::Color(60, 30, 10));
    drawThickLine(window, rightPronged, bandEndPx, 5.0f, sf::Color(60, 30, 10));
    // -- //
}