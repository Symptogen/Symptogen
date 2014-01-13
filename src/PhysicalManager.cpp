#include "PhysicalManager.h"

namespace Symp{

PhysicalManager::PhysicalManager(float x, float y){
	m_gravity = b2Vec2(0.0f, -10.0f);
	m_pWorld = new b2World(m_gravity);

	m_timeStep = 1.0f / 60.0f; //60Hz
	m_velocityIterations = 6;
	m_positionIterations = 2;
}

PhysicalManager::~PhysicalManager(){
	delete m_pWorld;
}

void PhysicalManager::updatePhysics(){
	// Instruct the world to perform a single step of simulation.
	m_pWorld->Step(m_timeStep, m_velocityIterations, m_positionIterations);
}

}