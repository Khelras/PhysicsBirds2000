/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : PhysicsObject.h
Description : Holds the Physics Library namespace with physics-relevant functions and constants.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <box2d/box2d.h>

namespace pl
{
	/// <summary>
	///		The size in pixels of a Box2D metre unit.
	/// </summary>
	const inline float sizeScale = 50.0f;

	/// <summary>
	///		Gravity vector for the Box2D physics world.
	///		Pointing downwards with a magnitude of 9.81 m/s².
	/// </summary>
	const inline b2Vec2 gravity(0.0f, 9.81f);
}