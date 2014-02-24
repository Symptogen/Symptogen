#ifndef _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_
#define _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_

#include <Box2D/Box2D.h>

namespace Symp {

/* *************************************************************************************** */
/* CLASS DEFINITION */
/* *************************************************************************************** */
/**
*	Facade of Box2D entity.
*	This class corresponds to all our physical elements. 
*	They are store in the array m_physicalEntityArray, contains in the EntityManager.
*	@see EntityManager
*/
class PhysicalEntity {

public:

	/**
	* @brief Create a physical entity, which use Box2D to manage the physics.
	* Create a physical entity means creating his body (b2Body) and his shape (b2Shape). Also create a fixture (b2FixtureDef) related to the body, and let the physical world (b2World) knows the body.
	*/
	PhysicalEntity(b2World* world, b2Vec2 origin, b2Vec2 hitBoxDimensions);

	/**
	* @brief Delete the physical entity.
	*/
	~PhysicalEntity();

	/**
	* Getters
	*/
	b2Body* 		getb2Body() const {return m_pBody;}
	b2Vec2 			getPosition() const {return m_pBody->GetPosition();} //Get the world body origin position.
	float			getWidth() const {return m_hitboxWidth;}
	float			getHeight() const {return m_hitboxHeight;}
	float 			getAngle() const {return m_pBody->GetAngle();}
	float 			getMass() const {return m_pBody->GetMass();}
	bool			isAwake() const {return m_pBody->IsAwake();}
	const b2Vec2&	getLinearVelocity() const {return m_pBody->GetLinearVelocity();}
	bool 			hasToBeDestroyed(){return m_hasToBeDestroyed;}

	/**
	* Setters
	*/
	void 		setActive(bool flag){m_pBody->SetActive(flag);}
	void 		setPosition(float pX, float pY){m_pBody->SetTransform(b2Vec2(pX, pY), m_pBody->GetAngle());}
	void 		setRotation(float angle){m_pBody->SetTransform(m_pBody->GetPosition(), angle);} //the angle is in randian
	void 		setMass(float mass, float inertia);
	void 		setLinearVelocity(const b2Vec2& v) {m_pBody->SetLinearVelocity(v);}
	void		setAngularVelocity(float omega) {m_pBody->SetAngularVelocity(omega);}
	void 		hasToBeDestroyed(bool flag){m_hasToBeDestroyed = flag;}
	
	/**
	* Tools for physics.
	*/
	void 		resetVelocities();
	bool 		isMovingOnX() const {return ((getLinearVelocity().x*getLinearVelocity().x) > 10) ? true : false;}
	bool 		isMovingOnY() const {return ((getLinearVelocity().y*getLinearVelocity().y) > 10) ? true : false;}

private:
	b2Body*			m_pBody;
	bool			m_hasToBeDestroyed;
	float			m_hitboxWidth;
	float			m_hitboxHeight;

	//TODO : make it more general (all hitBox are not like a polygon)
	//b2Shape*		m_pShape;
	b2PolygonShape*	m_pShape;
};

}

#endif //_H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_