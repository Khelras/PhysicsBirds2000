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

	/// <summary>
	///		The size of a Bird Projectile in Box2D metres.
	/// </summary>
	const inline b2Vec2 birdSize(0.75f, 0.75f);

	/// <summary>
	///		Collision category bits.
	/// </summary>
	namespace category
	{
		constexpr uint16 defaultBits = 0x0001;
		constexpr uint16 bird = 0x0002;
	}

	/// <summary>
	///		Set the collision filter of every fixture on the given body so it is a Bird.
	///		Birds collide with everything except other Birds.
	/// </summary>
	/// 
	/// <param name="body">The body to filter.</param>
	inline void setBirdCollisionFilter(b2Body* body)
	{
		for (b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
		{
			b2Filter filter = fixture->GetFilterData();
			filter.categoryBits = category::bird;
			filter.maskBits = static_cast<uint16>(0xFFFF & ~category::bird);
			fixture->SetFilterData(filter);
		}
	}
}