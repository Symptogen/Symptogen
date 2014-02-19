#ifndef _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_
#define _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_

#include <Box2D/Box2D.h>
#include "../render/Render.h"

namespace Symp {

/**
	Facade of Box2D entity.
*/
class PhysicalEntity {
public:
	PhysicalEntity(b2World* world, b2Vec2 dimensions);
	~PhysicalEntity();

	//getters
	b2Body* 		getb2Body() const {return m_pBody;}
	b2Vec2 			getPosition() const {return m_pBody->GetPosition();}
	b2Vec2 			getDimensions() const {return m_dimensions;}
	float 			getAngle() const {return m_pBody->GetAngle();}
	float 			getMass() const {return m_pBody->GetMass();}
	bool			isAwake() const {return m_pBody->IsAwake();}
	const b2Vec2&	getLinearVelocity() const {return m_pBody->GetLinearVelocity();}
	bool 			hasToBeDestroyed(){return m_hasToBeDestroyed;}

	//setters
	void 		setActive(bool flag){m_pBody->SetActive(flag);}
	void 		setPosition(float pX, float pY){m_pBody->SetTransform(b2Vec2(pX, pY), m_pBody->GetAngle());}
	void 		setRotation(float angle){m_pBody->SetTransform(m_pBody->GetPosition(), angle);} //the angle is in randian
	void 		setMass(float mass, float inertia);
	void 		setLinearVelocity(const b2Vec2& v) {m_pBody->SetLinearVelocity(v);}
	void		setAngularVelocity(float omega) {m_pBody->SetAngularVelocity(omega);}
	void 		hasToBeDestroyed(bool flag){m_hasToBeDestroyed = flag;}
	
	//tools for physics
	void 		resetVelocities();
	bool 		isMovingOnX() const {return ((getLinearVelocity().x*getLinearVelocity().x) > 10) ? true : false;}
	bool 		isMovingOnY() const {return ((getLinearVelocity().y*getLinearVelocity().y) > 10) ? true : false;}

private:
	b2Body*			m_pBody;
	b2Vec2			m_dimensions;
	bool			m_hasToBeDestroyed;

	//TODO : make it more general (all hitBox are not like a polygon)
	//b2Shape*		m_pShape;
	b2PolygonShape*	m_pShape;
};

}

#endif //_H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_