#ifndef _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_
#define _H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_

#include <Box2D/Box2D.h>

#include <map>
#include <string>
#include <vector>

#include "../persistence/ParserCollision.h"

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
	DestructableObject,
	Spikes,
	Flames,
	HotZone,
	ColdZone
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
	inline PhysicalType		getType() const {return m_type;}
	inline b2Body* 			getb2Body() const {return m_pBody;}
	inline b2Shape*			getb2Shape() const {return m_pShape;}
	inline b2Vec2 			getPosition() const {return m_pBody->GetWorldCenter();} //Get the world body origin position.
	inline float			getWidth() const {return m_fHitboxWidth;}
	inline float			getHeight() const {return m_fHitboxHeight;}
	inline float 			getAngle() const {return m_pBody->GetAngle();}
	inline float 			getMass() const {return m_pBody->GetMass();}
	inline bool				isAwake() const {return m_pBody->IsAwake();}
	inline const b2Vec2&	getLinearVelocity() const {return m_pBody->GetLinearVelocity();}
	inline bool 			hasToBeDestroyed() const {return m_bHasToBeDestroyed;}

	inline size_t			getNumContacts() const {return m_iNumContacts;}
	inline bool				hasContactingLeft() const {return m_bHasContactLeft;}
	inline bool				hasContactingRight() const {return m_bHasContactRight;}
	inline bool				hasContactingBelow() const {return m_bHasContactBelow;}
	inline bool				hasContactingAbove() const {return m_bHasContactAbove;}
	
	/**
	* The skin helps prevent tunneling by keeping the polygons separated. This results in small gaps between the shapes.
	*/
	inline float			getSkin() const {return m_pShape->m_radius;}

	/**
	* Setters
	*/
	inline void 	setActive(bool flag){m_pBody->SetActive(flag);}
	inline void 	setPosition(float pX, float pY) {m_pBody->SetTransform(b2Vec2(pX, pY), m_pBody->GetAngle());} //This breaks any contacts and wakes the other bodies. Manipulating a body's transform may cause non-physical behavior.
	inline void 	setRotation(float angle){m_pBody->SetTransform(m_pBody->GetPosition(), angle);} //the angle is in randian
	void 			setMass(float mass, float inertia);
	inline void 	setLinearVelocity(const b2Vec2& v) {m_pBody->SetLinearVelocity(v);}
	inline void 	setLinearDamping(const float d) { m_pBody->SetLinearDamping(d);}
	inline void		setAngularVelocity(float omega) {m_pBody->SetAngularVelocity(omega);}
	inline void 	hasToBeDestroyed(bool flag){m_bHasToBeDestroyed = flag;}
 
	inline void		hasContactLeft(bool flag){m_bHasContactLeft = flag;}
	inline void		hasContactRight(bool flag){m_bHasContactRight = flag;}
	inline void		hasContactBelow(bool flag){m_bHasContactBelow = flag;}
	inline void		hasContactAbove(bool flag){m_bHasContactAbove = flag;}

	/**
	* Set a polygon hitbox (4 vertices) for the physical entity.
	*/
	void		setDefaultHitbox(const b2Vec2 hitBoxDimensions);
	/**
	* Set a custom hitbox of the physical entity (from json file, created by the software Box2D-editor).
	* The previous hitbox (a polygon shape with 4 vertices), created in the constructor, is replaced by the new one.
	* Warning : The collision between b2ChainShapes is not supported in Box2D. So only the dino has a b2ChainShape.
	*/
	void		setCustomChainHitbox(const char* collisionFileName);
	/**
	* Set a custom hitbox of the physical entity (from json file, created by the software Box2D-editor).
	* The previous hitbox (a polygon shape with 4 vertices), created in the constructor, is replaced by the new one.
	*/
	void		setCustomPolygonHitbox(const char* collisionFileName);
	
	/**
	* Tools for physics.
	*/
	void 		resetVelocities();
	inline void applyForce(float pX, float pY) {
		const b2Vec2 force = b2Vec2(pX, pY);
		m_pBody->ApplyLinearImpulse(force, m_pBody->GetPosition(), true);}


	inline void 		startContact() {m_iNumContacts++;}
   	inline void 		endContact() {m_iNumContacts--;}
	inline bool 		isMovingOnX() const {return ((getLinearVelocity().x*getLinearVelocity().x) > 10) ? true : false;}
	inline bool 		isMovingOnY() const {return ((getLinearVelocity().y*getLinearVelocity().y) > 10) ? true : false;}

	/**
	* Static Method necessary to the MovableObject
	*/
	static void 			checkMovableObject(bool);
	static inline void		clearMovableObjectArray() {m_movableObjectArray.clear();}

private:
	PhysicalType	m_type;
	b2Body*			m_pBody;
	b2Fixture*		m_pFixture;
	b2Shape*		m_pShape;

	bool			m_bHasToBeDestroyed;
	
	float			m_fHitboxWidth;
	float			m_fHitboxHeight;
	size_t 			m_iNumContacts;

	bool 			m_bHasContactBelow;
	bool 			m_bHasContactAbove;
	bool 			m_bHasContactRight;
	bool 			m_bHasContactLeft;

	/**
	* Functions used to replace an old fixture by a new one. We need this to change the shape of a PhysicalEntity.
	*/
	void attachedFixture();
	void detachedFixture();

	/**
	* Map of vertices (used for shapes) already loaded in the level.
	* Improve performences by get shape elements in this map.
	*/
	static std::map<std::string, std::vector<b2Vec2>> 	s_verticesMap;

	/**
	* Parser to have custom hitboxes.
	*/
	static ParserCollision*	m_pParserCollision;

	/**
	* Store all the movable objects of the current level.
	*/
	static std::vector<PhysicalEntity*> m_movableObjectArray;
};

}

#endif //_H_SYMPTOGEN_PHYSIC_PHYSICAL_ENTITY_H_