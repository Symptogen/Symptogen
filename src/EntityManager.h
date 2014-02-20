#ifndef _H_SYMPTOGEN_ENTITY_MANAGER_H_
#define _H_SYMPTOGEN_ENTITY_MANAGER_H_

#include <vector>

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>

#include "render/Render.h"
#include "render/RenderEntity.h"
#include "physic/PhysicalEntity.h"
#include "physic/PhysicalWorld.h"
#include "sound/SoundEntity.h"
#include "Singleton.h"

namespace Symp {

/* *************************************************************************************** */
/* CLASS DEFINITION */
/* *************************************************************************************** */
/**
*	Manages all the entites of the game(of any type).
*	It is a singleton.
*	It has the knowledge of all entities which exist at a specific moment.
*	It provides tools to : 
*		- Add entities
*		- Update entities
*		- Delete entitites
*/

class EntityManager : public Singleton<EntityManager> {

	// Friend to use private constructor/destructor
	friend class Singleton<EntityManager>;

public:
	
	/**
	*
	*/
	static void initRender(Render* pRender);

	/**
	*	Adds a new entity. 
	*	Pushes back one object of each entity type in the corresponding array. 
	*	If the entity does not specify a component, the value of the object is set to a NULL. 
	*	The layer parameter corresponds to the layer in which the render entity has to be displayed (cf : Render).
	*	@see addEntity()
	*	@param pRenderEntity : render entity
	*	@param layer : layer of the render entity
	*	@param pPhysicalEntity: physical entity
	*	@sound pSoundEntity : sound entity
	*	@return boolean that indicates if the entity has been added correctly
	*	
	*/
	bool addEntity(RenderEntity* pRenderEntity, unsigned int layer, PhysicalEntity* pPhysicalEntity, SoundEntity* pSoundEntity);


	/**
	*	Adds a new render entity
	*	The physicalEntity and soundEntity components are set to NULL
	*	@see addEntity()
	*	@see addPhysicalEntity()
	*	@see addSoundEntity()
	*	@param pRenderEntity : render entity
	*	@param layer : layder of the render entity
	*	@return boolean that indicates if the entity has been added correctly
	*/
	bool addRenderEntity(RenderEntity* pRenderEntity, unsigned int layer);
	
	/**
	*	Adds a new physical entity
	*	The renderEntity and soundEntity components are set to NULL
	*	@see addEntity()
	*	@see addRenderEntity()
	*	@see addSoundEntity()
	*	@param pPhysicalEntity : physical entity
	*	@return boolean that indicates if the entity has been added correctly
	*/
	bool addPhysicalEntity(PhysicalEntity* pPhysicalEntity);
	
	/**
	*	Adds a new sound entity
	*	The renderEntity and physicalEntity components are set to NULL
	*	@see addEntity()
	*	@see addRenderEntity()
	*	@see addPhysicalEntity()
	*	@param pSoundEntity : sound entity
	*	@return boolean that indicates if the entity has been added correctly
	*/
	bool addSoundEntity(SoundEntity* pSoundEntity);
		
	/**
	*	Render all the entities
	*/
	void renderEntities();
	
	/**
	*	Update all the entities
	*/
	void updateEntities();
	
	/**
	*	Delete all the entities
	*/
	void deleteAllEntities();

	/**
	*	Delete the entity containes at index
	*	@param index : index of the entity we want to remove
	*	@return boolean is true if the deletion went correctly
	*/
	bool deleteEntity(size_t index);

	/**
	*
	*/
	void addDino();

	//Temporary !
	void loadTestWorld();

	/**
	*	Getters
	*/
	inline std::vector<RenderEntity*> 		getRenderEntityArray() const { return m_renderEntityArray;}
	inline std::vector<PhysicalEntity*> 	getPhysicalEntityArray() const { return m_physicalEntityArray;}
	inline std::vector<SoundEntity*>		getSoundEntityArray() const { return m_soundEntityArray;}
	inline IND_Entity2dManager* 			getIND_Entity2dManager() const {return m_pEntity2dManager;}
	//inline PhysicalWorld*					getPhysicalWorld() const { return m_pPhysicalWorld;}
	inline PhysicalWorld*					getPhysicalWorld() const {return m_pPhysicalWorld;} // Has to be remplaced by getPhysicalWorld()
	inline unsigned int 					getNbEntities() const { return m_renderEntityArray.size();}
	RenderEntity*							getRenderDino() const;
	PhysicalEntity*							getPhysicalDino() const;
	SoundEntity*							getSoundDino() const;

private:
	//all ***EntityArray have always the same size
	//this enable to always have a correspondance between the vectors.
	std::vector<RenderEntity*>		m_renderEntityArray;
	std::vector<PhysicalEntity*>	m_physicalEntityArray;
	std::vector<SoundEntity*>		m_soundEntityArray;
	
	/**
	*	Power’s instances. 
	*	Thanks to it, the EntityManager can reach the different information relative to each power such as the temperature value, the last time the character sneezed and so on.
	*/
	//std::vector<Power*>				m_powerArray;

	static IND_Entity2dManager*			m_pEntity2dManager;

	/**
	*	Instance of the PhysicalWorld class which manages the physics in the game.
	*/
	//PhysicalWorld*					m_pPhysicalWorld;
	PhysicalWorld*						m_pPhysicalWorld;		// Has to be remplaced by PhysicalWorld
	EntityManager*						m_pEntityManager;

	/** 
	*	Private constructor (because it is a singleton)
	*	@see EntityManager()
	*	@see ~EntityManager()
	*	@see getInstance()
	*	@see removeInstance()
	*/
	EntityManager();

	/**
	*	Private destructor (because it is a singleton)
	*	@see EntityManager()
	*	@see ~EntityManager()
	*	@see getInstance()
	*	@see removeInstance()
	*/
	~EntityManager();
};

}

#endif //_H_SYMPTOGEN_ENTITY_MANAGER_H_