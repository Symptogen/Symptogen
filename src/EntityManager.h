#ifndef _H_SYMPTOGEN_ENTITY_MANAGER_H_
#define _H_SYMPTOGEN_ENTITY_MANAGER_H_

#include <vector>
#include <array>

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>

#include "util/Singleton.h"
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
	NormalStop,
	Walk,
	Jump,
	Die,
	Sneezing,
	HotFever,
	FeverStop
	//ColdFever,
	//Headache,
};

/**
* This enum is used to define the index of the power in m_powerArray.
*/
enum PowerType {
	SneezeType,
	FeverType,
	HeadacheType,
	NormalType
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
	void initRender(Render* pRender);

	/************************************************************************************/
	/*							Manage entities 										*/
	/************************************************************************************/

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
	*	Delete all the entities : move physics and fit position of render entities on physical entities.
	*/
	void deleteAllEntities();
	/**
	*	Delete the entity containes at index
	*	@param index : index of the entity we want to remove
	*	@return boolean is true if the deletion went correctly
	*/
	bool deleteEntity(size_t index);

	/********************************************************************************/
	/*							Manage powers 										*/
	/********************************************************************************/

	/**
	* Add a power to the list of power
	*/
	void addPower(Power* newPower);
	/**
	* Launch the execute function of all power stored in the array m_powerArray
	*/
	void executePowers();
	/**
	*	Delete all the powers
	*/
	void deleteAllPowers();

	/************************************************************************************/
	/*							Manage specific entities 								*/
	/************************************************************************************/

	/**
	* Add all needed entities for the dino (render, physical, and sound).
	* @param posX : the X position of the center of the dino we want to create
	* @param posY : the Y position of the center of the dino we want to create
	* @param width : the width of the dino we want to create. The width is setted automaticaly.
	*/
	void addDino(int posX, int posY, int width);
	/*
	* Kill Dino
	* Set the animate and play a sound.
	*/
	void killDino();
	/**
	* Add all needed entities for the thermometer (render).
	*/
	void addThermometer();

	/************************************************************************************/
	/*									Getters			 								*/
	/************************************************************************************/
	inline std::vector<std::vector<RenderEntity*>> 		getRenderEntityArray() const { return m_renderEntityArray;}
	inline std::vector<PhysicalEntity*> 				getPhysicalEntityArray() const { return m_physicalEntityArray;}
	inline std::vector<std::vector<SoundEntity*>>		getSoundEntityArray() const { return m_soundEntityArray;}
	
	inline std::vector<RenderEntity*>					getRenderEntity(size_t index) const {return m_renderEntityArray[index];}
	inline PhysicalEntity*								getPhysicalEntity(size_t index) const {return m_physicalEntityArray[index];}
	inline std::vector<SoundEntity*>					getSoundEntity(size_t index) const {return m_soundEntityArray[index];}
	
	std::vector<RenderEntity*>							getRenderDino() const {return m_renderEntityArray[m_dinoIndex];}
	PhysicalEntity*										getPhysicalDino() const {return m_physicalEntityArray[m_dinoIndex];}
	std::vector<SoundEntity*>							getSoundDino() const {return m_soundEntityArray[m_dinoIndex];}
	DinoAction											getCurrentDinoAction() const;
	PowerType 											getCurrentPowerState() const;
	bool 												isDinoReady() const {return (getRenderDino().size() > 0 && getPhysicalDino() != NULL) ? true : false;}

	std::vector<Power*>									getPowers() const {return m_powerArray;}
	Power*												getPower(PowerType powerType) const {return (powerType > m_powerArray.size()) ? NULL : m_powerArray[powerType];}
	bool 												isPowerExisting(PowerType powerType) const;

	/**
	* The dino can't move when the sneeze power is activate.
	*/
	bool 												isDinoAllowToMove();

	inline IND_Entity2dManager* 						getIND_Entity2dManager() const {return m_pEntity2dManager;}
	inline PhysicalWorld*								getPhysicalWorld() const {return m_pPhysicalWorld;}
	inline size_t		 								getNbEntities() const { return m_renderEntityArray.size();}


	std::array<float, 2> 								getExitCoordinates() const { return m_exitCoordinates; }

	/************************************************************************************/
	/*									Setters			 								*/
	/************************************************************************************/
	void setExitCoordinates(float posX, float posY) { m_exitCoordinates[0] = posX; m_exitCoordinates[1] = posY; }
	/**
	* Set the correction renderEntity of the dino, depending on the dinoAction.
	*/
	void setDinoRender(DinoAction dinoAction);
	/**
	* Set the correction renderEntity of the thermometer, depending on the ferver power
	*/
	void updateThermomether();


private:
	
	//all ***EntityArray have always the same size
	//this enable to always have a correspondance between the vectors.
	//Warning : m_renderEntityArray is an array of arrays, because an entity can have several rendering (for example : the dino walk, jump...).
	//Warning : m_soundEntityArray is an array of arrays, because an entity can have several sounds (for example : the dino walk, jump...).
	std::vector<std::vector<RenderEntity*>>		m_renderEntityArray;	
	std::vector<PhysicalEntity*>				m_physicalEntityArray;
	std::vector<std::vector<SoundEntity*>>		m_soundEntityArray;

	/*
	*	The coordinates of the exit doors. May de deplaced to GameManager when it will be a singleton.
	*/
	std::array<float, 2>	m_exitCoordinates;

	/**
	*	The index of specific entities
	*/
	unsigned int m_uiDinoIndex;
	size_t m_dinoIndex;
	size_t m_thermometerSupportIndex;
	size_t m_thermometerTemperatureIndex;
	
	/**
	*	Power’s instances. 
	*	Thanks to it, the EntityManager can reach the different information relative to each power such as the temperature value, the last time the character sneezed and so on.
	*/
	std::vector<Power*> m_powerArray;

	/**
	*
	*/
	IND_Entity2dManager*				m_pEntity2dManager;

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