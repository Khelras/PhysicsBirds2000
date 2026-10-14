/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : LevelTwo.h
Description : Declares the LevelTwo Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include "myproject/scenes/ScenePhysics.h"

/// <summary>
///		Level two scene that derives from the base physics scene class.
/// </summary>
class LevelTwo : public ScenePhysics
{
private:
	// -- Level Two Properties -- //

	// -- //

public:
	/// <summary>
	///		Constructor.
	/// </summary>
	LevelTwo();

	/// <summary>
	///		Destructor.
	/// </summary>
	~LevelTwo();

	/// <summary>
	///     Create the Box2D Physics World.
	/// </summary>
	void createPhysicsWorld() override;

// PRIVATE HELPER FUNCTIONS
private:
	/// <summary>
	///     Go to the next level.
	/// </summary>
	void goToNextLevel(StateContext ctx) override;

	/// <summary>
	///		Create a T-shaped structure: a post and a top plank held together with a Weld Joint.
	/// </summary>
	/// 
	/// <param name="position">Position of the post's centre (the post stands on the ground).</param>
	void createWeldedTee(b2Vec2 position);
};