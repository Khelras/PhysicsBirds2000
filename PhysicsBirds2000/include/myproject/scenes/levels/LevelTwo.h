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
};