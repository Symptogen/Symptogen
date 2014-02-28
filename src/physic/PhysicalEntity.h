#ifndef _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_
#define _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_

#include <Box2D/Box2D.h>

namespace Symp {

/**
* This enum is used to know what we need to do with the physical entity when we detect a collision.
* @see ContactListener
*/
enum PhysicalType{
	Dino, 
	Ground,
	Flower,
	MovableObject,
	Spikes
};

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
	* Create a physical entity means setting his physical type, creating his body (b2Body) and his shape (b2Shape). 
	* Also create a fixture (b2FixtureDef) related to the body, and let the physical world (b2World) knows the body.
	* @see enum PhysicalType
	*/
	PhysicalEntity(b2World* world, const b2Vec2 origin, const b2Vec2 hitBoxDimensions, const PhysicalType physicalType);

	/**
	* @brief Delete the physical entity.
	*/
	~PhysicalEntity();

	/**
	* Getters
	*/
	PhysicalType	getType() const {return m_type;}
	b2Body* 		getb2Body() const {return m_pBody;}
	b2Vec2 			getPosition() const {return m_pBody->GetWorldCenter();} //Get the world body origin position.
	float			getWidth() const {return m_fHitboxWidth;}
	float			getHeight() const {return m_fHitboxHeight;}
	float 			getAngle() const {return m_pBody->GetAngle();}
	float 			getMass() const {return m_pBody->GetMass();}
	bool			isAwake() const {return m_pBody->IsAwake();}
	const b2Vec2&	getLinearVelocity() const {return m_pBody->GetLinearVelocity();}
	bool 			hasToBeDestroyed() const {return m_bHasToBeDestroyed;}
	size_t			getNumContacts() const {return m_iNumContacts;}
	bool			isContactingLeft() const {return m_bHasContactLeft;}
	bool			isContactingRight() const {return m_bHasContactRight;}
	bool			isContactingBelow() const {return m_bHasContactBelow;}
	bool			isContactingAbove() const {return m_bHasContactAbove;}

	/**
	* Setters
	*/
	void 		setActive(bool flag){m_pBody->SetActive(flag);}
	void 		setPosition(float pX, float pY){m_pBody->SetTransform(b2Vec2(pX, pY), m_pBody->GetAngle());}
	void 		setRotation(float angle){m_pBody->SetTransform(m_pBody->GetPosition(), angle);} //the angle is in randian
	void 		setMass(float mass, float inertia);
	void 		setLinearVelocity(const b2Vec2& v) {m_pBody->SetLinearVelocity(v);}
	void		setAngularVelocity(float omega) {m_pBody->SetAngularVelocity(omega);}
	void 		hasToBeDestroyed(bool flag){m_bHasToBeDestroyed = flag;}

	void		hasContactLeft(bool flag){m_bHasContactLeft = flag;}
	void		hasContactRight(bool flag){m_bHasContactRight = flag;}
	void		hasContactBelow(bool flag){m_bHasContactBelow = flag;}
	void		hasContactAbove(bool flag){m_bHasContactAbove = flag;}

	/**
	* Set a polygon hitbox for the physical entity.
	*/
	void		setDefaultHitbox(const b2Vec2 hitBoxDimensions);
	
	/**
	* Set a custom hitbox for the physical entity (from json file, created by the software Box2D-editor)
	*/
	void		setCustomHitbox(const b2Vec2* vertexArray, size_t vertexCount);
	
	/**
	* Tools for physics.
	*/
	void 		resetVelocities();

	void 		startContact() {m_iNumContacts++; /*std::cout<<"num contact start "<<m_iNumContacts<<std::endl; */}
  	void 		endContact() {m_iNumContacts--;   /*std::cout<<"num contact end "<<m_iNumContacts<<std::endl;*/ }
	bool 		isMovingOnX() const {return ((getLinearVelocity().x*getLinearVelocity().x) > 10) ? true : false;}
	bool 		isMovingOnY() const {return ((getLinearVelocity().y*getLinearVelocity().y) > 10) ? true : false;}

private:
	PhysicalType	m_type;
	b2Body*			m_pBody;
	bool			m_bHasToBeDestroyed;
	float			m_fHitboxWidth;
	float			m_fHitboxHeight;
	size_t 			m_iNumContacts;

	bool 			m_bHasContactBelow;
	bool 			m_bHasContactAbove;
	bool 			m_bHasContactRight;
	bool 			m_bHasContactLeft;

	b2Shape*		m_pShape;
};

}

#endif //_H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_