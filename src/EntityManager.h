#ifndef _H_SYMPTOGEN_ENTITY_MANAGER_H_
#define _H_SYMPTOGEN_ENTITY_MANAGER_H_

#include <vector>

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>

#include "Singleton.h"
#include "render/Render.h"
#include "render/RenderEntity.h"
#include "physic/PhysicalEntity.h"
#include "physic/PhysicalWorld.h"
#include "sound/SoundEntity.h"
#include "power/Power.h"

namespace Symp {

/**
* This enum is used to define the index of render entities corresponding to the Dino.
* It will probably be used for the same things with the sound entities.
*/
enum DinoAction {
	Stop,
	WalkRight,
	WalkLeft,
	Jump,
	Sneezing,
	DieByFall,
	DieBySpikes,
	DieByFreeze,
	DieByHot
};

/**
* This enum is used to define the index of the power in m_powerArray.
*/
enum PowerType {
	SneezeType,
	FeverType,
	HeadacheType
};

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
	*	@param renderEntityArray : array of all rendering corresponding to the entity
	*	@param layer : layer of the render entity
	*	@param pPhysicalEntity: physical entity
	*	@sound pSoundEntityArray : array of all sounds corresponding to the entity
	*	@return boolean that indicates if the entity has been added correctly
	*	
	*/
	bool addEntity(std::vector<RenderEntity*> renderEntityArray, unsigned int layer, PhysicalEntity* pPhysicalEntity, std::vector<SoundEntity*> pSoundEntityArray);


	/**
	*	Adds a new render entity
	*	The physicalEntity and soundEntity components are set to NULL
	*	@see addEntity()
	*	@see addPhysicalEntity()
	*	@see addSoundEntity()
	*	@param renderEntityArray : array of all rendering corresponding to the entity
	*	@param layer : layder of the render entity
	*	@return boolean that indicates if the entity has been added correctly
	*/
	bool addRenderEntity(std::vector<RenderEntity*> renderEntityArray, unsigned int layer);
	
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
	*	@param pSoundEntityArray : array of all sounds corresponding to the entity
	*	@return boolean that indicates if the entity has been added correctly
	*/
	bool addSoundEntity(std::vector<SoundEntity*> pSoundEntityArray);
	
	/**
	* Add a render entity to an existing entity.
	*/
	bool addRenderEntityToExistingEntity(RenderEntity* renderEntity, size_t indexExistingEntity);

	/**
	* Add a sound entity to an existing entity.
	*/
	bool addSoundEntityToExistingEntity(SoundEntity* soundEntity, size_t indexExistingEntity);

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
	* 	Add all needed entities for the dino (render, physical, and sound).
	*	@param posX : the X position of the center of the dino we want to create
	*	@param posY : the Y position of the center of the dino we want to create
	*	@param height : the height of the dino we want to create. The width is setted automaticaly.
	*/
	void addDino(int posX, int posY, int height);

	/**
	* Set the correction renderEntity of the dino, depending on the dinoAction.
	*/
	void updateDinoRender(DinoAction dinoAction) const;

	/*
	*	Kill Dino
	*	Animate and play sound
	*/
	void killDino(DinoAction action);

	/**
	* Launch the execute function of all power stored in the array m_powerArray
	*/
	void executePowers();

	/**
	*	Getters
	*/
	inline std::vector<std::vector<RenderEntity*>> 		getRenderEntityArray() const { return m_renderEntityArray;}
	inline std::vector<PhysicalEntity*> 				getPhysicalEntityArray() const { return m_physicalEntityArray;}
	inline std::vector<std::vector<SoundEntity*>>		getSoundEntityArray() const { return m_soundEntityArray;}
	
	inline std::vector<RenderEntity*>	getRenderEntity(size_t index) const {return m_renderEntityArray[index];}
	inline PhysicalEntity*				getPhysicalEntity(size_t index) const {return m_physicalEntityArray[index];}
	inline std::vector<SoundEntity*>	getSoundEntity(size_t index) const {return m_soundEntityArray[index];}
	
	std::vector<RenderEntity*>		getRenderDino() const {return m_renderEntityArray[m_uiDinoIndex];}
	PhysicalEntity*					getPhysicalDino() const {return m_physicalEntityArray[m_uiDinoIndex];}
	std::vector<SoundEntity*>		getSoundDino() const {return m_soundEntityArray[m_uiDinoIndex];}
	DinoAction						getCurrentDinoAction() const;
	bool 							isDinoReady() const {return (getRenderDino().size() > 0 && getPhysicalDino() != NULL) ? true : false;}

	std::vector<Power*>	getPowers() const {return m_powerArray;}
	Power*				getPower(PowerType powerType) const {return (powerType > m_powerArray.size()) ? NULL : m_powerArray[powerType];}

	inline IND_Entity2dManager* 	getIND_Entity2dManager() const {return m_pEntity2dManager;}
	inline PhysicalWorld*			getPhysicalWorld() const {return m_pPhysicalWorld;}
	inline unsigned int 			getNbEntities() const { return m_renderEntityArray.size();}

private:
	//all ***EntityArray have always the same size
	//this enable to always have a correspondance between the vectors.
	//Warning : m_renderEntityArray is an array of arrays, because an entity can have several rendering (for example : the dino walk, jump...).
	//Warning : m_soundEntityArray is an array of arrays, because an entity can have several sounds (for example : the dino walk, jump...).
	std::vector<std::vector<RenderEntity*>>		m_renderEntityArray;	
	std::vector<PhysicalEntity*>				m_physicalEntityArray;
	std::vector<std::vector<SoundEntity*>>		m_soundEntityArray;

	/**
	*	The index of the entities corresponding to the Dino
	*/
	unsigned int m_uiDinoIndex;
	
	/**
	*	Power’s instances. 
	*	Thanks to it, the EntityManager can reach the different information relative to each power such as the temperature value, the last time the character sneezed and so on.
	*/
	std::vector<Power*> m_powerArray;

	/**
	*
	*/
	static IND_Entity2dManager*			m_pEntity2dManager;

	/**
	*	Instance of the PhysicalWorld class which manages the physics in the game.
	*/
	PhysicalWorld*						m_pPhysicalWorld;

	/**
	*
	*/
	EntityManager*						m_pEntityManager;

	/** 
	*	Private constructor (because it is a singleton)
	*	@see EntityManager()
	*	@see ~EntityManager()
	*/
	EntityManager();

	/**
	*	Private destructor (because it is a singleton)
	*	@see EntityManager()
	*	@see ~EntityManager()
	*/
	~EntityManager();
};

}

#endif //_H_SYMPTOGEN_ENTITY_MANAGER_H_