/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : LevelOne.h
Description : Declares the LevelOne Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include "myproject/scenes/ScenePhysics.h"

/// <summary>
///		Level one scene that derives from the base physics scene class.
/// </summary>
class LevelOne : public ScenePhysics
{
private:
	// -- Level One Properties -- //
	
	// -- //

public:
	/// <summary>
	///		Constructor.
	/// </summary>
	LevelOne();

	/// <summary>
	///		Destructor.
	/// </summary>
	~LevelOne();

	/// <summary>
	///     Create the Box2D Physics World.
	/// </summary>
	void createPhysicsWorld() override;

// PRIVATE HELPER FUNCTIONS
private:
	/// <summary>
	/// 	Create a lever.
	/// </summary>
	/// 
	/// <param name="position">2D position vector.</param>
	void createLever(b2Vec2 position);
};
