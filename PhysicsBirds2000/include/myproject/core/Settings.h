/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : Settings.h
Description : Declares and Defines the Settings Class Interface and Template
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once

/// <summary>
///		Settings struct as a singleton pattern for global access.
/// </summary>
struct Settings
{
private:
	/// <summary>
	///		Private Default Constructor
	/// </summary>
	Settings() = default;

public:
	/// <summary>
	///		Deleted Copy Constructor for Singleton Pattern.
	/// </summary>
	Settings(const Settings&) = delete;

	/// <summary>
	///		Deleted Assignment Operator for Singleton Pattern.
	/// </summary>
	Settings& operator=(const Settings&) = delete;

	/// <summary>
	///		Global access point. Get the Instance of Settings.
	/// </summary>
	/// 
	/// <returns>
	///		The singleton instance of Settings.
	/// </returns>
	static Settings& getInstance()
	{
		static Settings instance;
		return instance;
	}

	//==================================================
	// SETTINGS PROPERTIES
	//==================================================

	// -- Window Size Settings -- //
	const unsigned int windowWidth = 1200;
	const unsigned int windowHeight = 800;
	const unsigned int maxFrameramte = 60;
	// -- //
};