/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : PhysicsLibrary.h
Description : The overall Physics Library for the Project, containing helper functions and constants.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once

/// <summary>
///     Box2D works in meters whereas SFML works in Pixels. These helpers convert between the two.
/// </summary>
namespace PhysicsLibrary
{
    constexpr float PIXELS_PER_METRE = 30.0f;

    inline float toMetres(float pixels) { return pixels / PIXELS_PER_METRE; }
    inline float toPixels(float metres) { return metres * PIXELS_PER_METRE; }
}