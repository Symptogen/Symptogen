#ifndef _H_SYMPTOGEN_MENU_IMAGE_H_
#define _H_SYMPTOGEN_MENU_IMAGE_H_

#include "GuiComponent.h"

/** @namespace Symp */
namespace Symp {

/**
* @enum AspectRatio
* The AspectRation enum presents the three different types of updating / resizing that can be choosen for an #Image.
* @see GuiComponent
* @see Image
*/
enum AspectRatio {
	IGNORE_ASPECT_RATIO, /*!< will make the texture fit the rectangle with no regards to its initial ratio*/
	KEEP_ASPECT_RATIO, /*!< will keep the initial ratio of the image and make the texture smaller enought to fit the shape of the #Image*/
	EXPAND_ASPECT_RATIO /*!< will keep the initial ratio of the image but expand the texture, so the #Image shape is fully filled */
};

/**
* @class Image class
* This class inherits the #GuiComponent class. A #Image class display a texture. As an #Image got the inherited
* parameters of size, the #update is made following the type of ratio choosen for the #Image. The types of ratio
* are defined in the #AspectRatio enum.
* The simpliest way to use #Image is with the #Layout class, that handle the position and the size of the #Image. 
* of #Image that will be rendered. 
* @see MenuManager
* @see Image
* @see GuiComponent
* @see Layout
*/
class Image : public GuiComponent {
public:
	Image( const char* filePath, float iPosX=0, float iPosY=0, float iScale = 1.f);
	~Image();
	
	virtual void update();

	void show();
	void hide();

	//Getter
	AspectRatio getAspectRatio() const {return m_ratio;}

	//Setter
	void setAspectRatio(AspectRatio ratio) { m_ratio = ratio;}

private:
	AspectRatio m_ratio; /*!< the type of ratio that will be applyed for the updating of the #Image*/

};

}

#endif //_H_SYMPTOGEN_MENU_IMAGE_H_