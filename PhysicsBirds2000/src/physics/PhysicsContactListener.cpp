/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2026 Media Design School
File Name   : PhysicsContactListener.cpp
Description : Defines the PhysicsContactListener Class Functions and Properties.
Author      : Angelo Joseph Arawiran Bohol
Mail        : angelo.bohol@mds.ac.nz
**************************************************************************/

#include <iostream>
#include "myproject/physics/PhysicsContactListener.h"
#include "myproject/scenes/ScenePhysics.h"

PhysicsContactListener::PhysicsContactListener(b2World* physicsWorld, ScenePhysics* physicsScene)
{
	this->m_physicsWorld = physicsWorld;
	this->m_physicsScene = physicsScene;
}

PhysicsContactListener::~PhysicsContactListener()
{
	
}

void PhysicsContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	// Convert local contact points to world space contacts
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	// The state of the contacts (not contacting, new contact this frame, persistent contact, and contact removed this frame)
	b2PointState previousState[2], currentState[2];

	// Get the contact state from the old manifold and the current contact manifold
	b2GetPointStates(previousState, currentState, oldManifold, contact->GetManifold());

	if (currentState[0] == b2PointState::b2_addState || currentState[1] == b2PointState::b2_addState)
	{
		// Get the two bodies involved in the contact
		b2Body* bodyA = contact->GetFixtureA()->GetBody();
		b2Body* bodyB = contact->GetFixtureB()->GetBody();

		// calculate the relative velocity of the two bodies at the contact point
		b2Vec2 contactPoint = worldManifold.points[0]; // World Space Contact Point
		b2Vec2 velocityA = bodyA->GetLinearVelocityFromWorldPoint(contactPoint);
		b2Vec2 velocityB = bodyB->GetLinearVelocityFromWorldPoint(contactPoint);

		// Then calculate the approach velocity along the contact normal
		float approachVelocity = b2Dot(velocityB - velocityA, -worldManifold.normal);

		// Get the Physics Objects from the Scene
		for (auto physicsObject : this->m_physicsScene->getPhysicsObjects())
		{
			// Check if the Physics Object's body is either body A or B
			if (physicsObject->getBody() == bodyA || physicsObject->getBody() == bodyB)
			{
				std::cout << approachVelocity << std::endl;
				physicsObject->receiveImpact(approachVelocity);
			}
		}
	}
}
