#include "PhysicalWorld.h"

namespace Symp{

PhysicalWorld::PhysicalWorld(float x, float y) : m_fTimeStep(1.0f/60.0f), m_uiVelocityIterations(6), m_uiPositionIterations(2){
	m_gravity = b2Vec2(0.0f, -10.0f);
	m_pWorld = new b2World(m_gravity);
}

PhysicalWorld::~PhysicalWorld(){
	delete m_pWorld;
}

void PhysicalWorld::updatePhysics(){
	// Instruct the world to perform a single step of simulation.
	m_pWorld->Step(m_fTimeStep, m_uiVelocityIterations, m_uiPositionIterations);
}

}