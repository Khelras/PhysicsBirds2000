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
};
