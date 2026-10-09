/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : PhysicsContactListener.h
Description : Declares the PhysicsContactListener Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#pragma once
#include <box2d/box2d.h>

// Forward Declarations
class ScenePhysics;

class PhysicsContactListener : public b2ContactListener
{
private:
	// -- Physics Contact Listener Properties -- //
	b2World* m_physicsWorld = nullptr;
	ScenePhysics* m_physicsScene = nullptr;
	// -- //

public:
	/// <summary>
	///		Constructor.
	/// </summary>
	/// 
	/// <param name="physicsWorld">Pointer to the Physics World</param>
	/// <param name="physicsScene">Pointer to the Physics Scene</param>
	PhysicsContactListener(b2World* physicsWorld, ScenePhysics* physicsScene);

	/// <summary>
	///		Destructor.
	/// </summary>
	~PhysicsContactListener();

	/// <summary>
	///		Called after contact is updated. 
	///		This allows for inspection of a contact before it goes to the solver.
	/// </summary>
	/// 
	/// <param name="contact"></param>
	/// <param name="oldManifold"></param>
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
};
