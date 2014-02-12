#ifndef _H_SYMPTOGEN_MENU_SLIDER_H_
#define _H_SYMPTOGEN_MENU_SLIDER_H_

#include "GuiComponent.h"
#include "Image.h"

/** @namespace Symp */
namespace Symp {

/**
* @class Slider class inherits the #GuiComponent class
* The #Slider class is part of the menu graphical components and can be used only in the menu context. A #Slider
* can used in two situation : for displaying a progression, or as an event receiver for a collaboration with a 
* #ScrollArea. The #Slider is composed of a primitive associated with a texture. Only a texture, or a Indielib 
* IND_Surface can have a specific region shown. The image part of the #Slider must be added separatly to the 
* #MenuManager for being displayed. 
* @see MenuManager
* @see GuiComponent
* @see Slider()
* @see ~Slider()
*/
class Slider : public GuiComponent {
public:
	Slider( float percentage, float iPosX=0, float iPosY=0, int iWidth = 0, int iHeight =0);
	~Slider();

	virtual void update();

	void fill(Color color);

	//Setter
	void setPercentage(float fPercentage) { m_fPercentage = fPercentage;}

	//Getter
	Image* getImage() const {return m_pImage;}
	float getPercentage() const {return m_fPercentage;}

private:
	float m_fPercentage; /**< the relative size of the foeground part of the #Slider */
	Image* m_pImage; /** < the reference to the #Image that compose the foreground of the #Slider */

};

}

#endif //_H_SYMPTOGEN_MENU_SLIDER_H_