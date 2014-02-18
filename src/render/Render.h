#ifndef _H_SYMPTOGEN_RENDER_RENDER_H_
#define _H_SYMPTOGEN_RENDER_RENDER_H_

#include <Indie.h>
#include <IND_Render.h>
#include <IND_Window.h>
#include "Camera.h"

namespace Symp{

/**
	Facade of IND_Render.
*/
class Render {
public:
	Render();
	IND_Window* init(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer);
	~Render();

	void beginScene();
	void endScene();
	void clearViewPort(unsigned char pR, unsigned char pG, unsigned char pB);

	IND_Render* getIND_Render(){return m_pRender;}

	void setCameraPosition(float posX, float posY);
	void setZoom(float zoom);
	void resetCamera(float dinoPosX, float dinoPosY);
	void setCamera();
	
private:
	IND_Render* 	m_pRender;
	Camera* 		m_pCamera;
};

}

#endif //_H_SYMPTOGEN_RENDER_RENDER_H_