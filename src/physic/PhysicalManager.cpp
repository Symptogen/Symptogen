#include "PhysicalManager.h"

namespace Symp{

PhysicalManager::PhysicalManager(){
	m_gravity = b2Vec2(0.0f, 9.8f);
	m_pWorld = new b2World(m_gravity);

	m_fTimeStep = 1.0f / 60.0f; //60Hz
	m_uiVelocityIterations = 6;
	m_uiPositionIterations = 2;
}

PhysicalManager::~PhysicalManager(){
	delete m_pWorld;
}

void PhysicalManager::updatePhysics(){
	// Instruct the world to perform a single step of simulation.
	m_pWorld->Step(m_fTimeStep, m_uiVelocityIterations, m_uiPositionIterations);
}

}