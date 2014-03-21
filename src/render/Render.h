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

	//tools to manage openGL scene
	void beginScene();
	void endScene();
	void clearViewPort(unsigned char pR, unsigned char pG, unsigned char pB);

	/*
	* Getters
	*/
	IND_Render* getIND_Render() const {return m_pRender;}
	float getZoom() const {return m_pCamera->getIND_Camera2d()->getZoom();}
	bool isFullScreen() const { return m_bIsFullScreen; }
	float getCameraAngle() const{return m_pCamera->getIND_Camera2d()->getAngle();}

	/*
	* Setters
	*/
	void setCameraPosition(float posX, float posY);
	void setZoom(float zoom);
	void setCameraAngle(float angle);	
	void resetCamera(float dinoPosX, float dinoPosY);
	void setCamera();
	void toggleFullScreen();

	
private:
	IND_Render* 	m_pRender;
	Camera* 		m_pCamera;
	bool			m_bIsFullScreen;
};

}

#endif //_H_SYMPTOGEN_RENDER_RENDER_H_