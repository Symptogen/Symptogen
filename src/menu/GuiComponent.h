#ifndef _H_SYMPTOGEN_MENU_GUI_COMPONENT_H_
#define _H_SYMPTOGEN_MENU_GUI_COMPONENT_H_

#include <Indie.h>
#include <IND_Entity2d.h>
#include <IND_Entity2dManager.h>
#include <IND_Surface.h>
#include <IND_SurfaceManager.h>
#include <IND_Font.h>
#include <IND_FontManager.h>

#include "../render/Render.h"

/** @namespace Symp */
namespace Symp {

/**
* @class Color struct. 
* The Color struct presents convenient fields for the definition of a color, following the classic RGBA convention.
* Mostly used for defining background and border colors in Layout and Button class.
* @see GuiComponent
* @see Layout
* @see Button
*/
struct Color {
	/**
	* @brief Color struct constructor. 
	* Responsible for the initialization of private attributes of the Color class
	* @param rValue is the amount of red between 1 and 255
	* @param gValue is the amount of green between 1 and 255
	* @param bValue is the amount of blue between 1 and 255
	* @param aValue is the transparency velu between 1 and 255 ( default value set to 255, means no transparency )
	*/
	Color(unsigned int rValue, unsigned int gValue, unsigned int bValue, unsigned int aValue=255){
		r = rValue;
		g = gValue; 
		b = bValue;
		a = aValue;
	}

	static const Color BLUE;  /*!< Color blue (0, 0, 255)*/
	static const Color RED;   /*!< Color red (255, 0, 0)*/
	static const Color GREEN; /*!< Color green (0, 255, 0)*/
	static const Color BLACK; /*!< Color black (0, 0, 0)*/
	static const Color GREY;  /*!< Color grey (180, 180, 180)*/
	static const Color WHITE; /*!< Color white (255, 255, 255)*/

	unsigned int r; /**< the amount of red in the color between 1 and 255 */
	unsigned int g; /**< the amount of green in the color between 1 and 255 */
	unsigned int b; /**< the amount of blue in the color between 1 and 255 */
	unsigned int a; /**< the transparency of the color between 1 and 255 (255 means no transparency) */
};

/**
* @brief GuiComponent facade for the creation of Menu graphical entities
* The GuiComponent class implements a facade of the Indielib IND_ENtity2d elements for the unique use of the menus. 
* Each graphical element that compose a menu inherits from the GuiComponent class. The elements are stored in the 
* #MenuManager class and rendered/updated by the #GameManager class. In order to use the Indielib architecture, this
* class implements static method for the IND_ImageManager, IND_SurfaceManager and the IND_FontManager initialization 
* destruction.
* @see MenuManager
* @see GameManager::updateMenu()
* @see GuiComponent::init()
* @see GuiComponent::end()
*/
class GuiComponent {
public:
	GuiComponent();
	~GuiComponent();

	static void init(Render* pRender);
	static void end();
	virtual void update() = 0;

	bool isTargetedByMouse(int mouseX, int mouseY);
	bool loadFont();

	//Getters
	IND_Entity2d* getIND_Entity2d() const {return m_pEntity2d;}
	float getPosX() const {return m_pEntity2d->getPosX();}
	float getPosY() const {return m_pEntity2d->getPosY();}
	bool isHovered() const {return m_bIsHovered;}
	bool isEnabled() const {return m_bIsEnabled;}
	int getWidth() const {return m_iWidth;}
	int getHeight() const {return m_iHeight;}
	IND_Font* getFont() const {return m_pFont;}

	//Setters
	void enable();
	void disable();
	void setHovered(bool value) {m_bIsHovered = value; update();}
	void setSurface(const char* filePath);
	void setWidth(int width) {m_iWidth = width;}
	void setHeight(int height) {m_iHeight = height;}
	void setRectangle(int x, int y, int w, int h);
	void setPosition(float pX, float pY, int pZ){m_pEntity2d->setPosition(pX, pY, pZ);}
	void setScale(float pSx, float pSy){m_pEntity2d->setScale(pSx, pSy);}
	bool setHotSpot(float pX, float pY){return m_pEntity2d->setHotSpot(pX, pY);}

protected:
	IND_Font*					m_pFont;
	IND_Entity2d* 				m_pEntity2d; /**< the reference to the Indielib IND_Entity2d that stores the entities */
	static IND_ImageManager* 	s_pImageManager; /**< static variable that references the Indielib IND_ImageManager */
	static IND_SurfaceManager* 	s_pSurfaceManager; /**< static variable that references the Indielib IND_SurfaceManager */
	static IND_FontManager* 	s_pFontManager; /**< static variable that references the Indielib IND_FontManager */
	int 						m_iWidth; /**< the width of the component in pixels */
	int 						m_iHeight; /**< the height of the component in pixels */
	bool						m_bIsHovered; /**< boolean that is updated with events */
	bool 						m_bIsEnabled; /**< boolean that allow or not the events */
};

}

#endif //_H_SYMPTOGEN_MENU_GUI_COMPONENT_H_