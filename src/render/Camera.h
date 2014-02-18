#ifndef _H_SYMPTOGEN_RENDER_CAMERA_H_
#define _H_SYMPTOGEN_RENDER_CAMERA_H_

#include <Indie.h>
#include <IND_Camera2d.h>

namespace Symp{

/**
	Facade of IND_Camera2d.
*/
class Camera {
public:
	Camera();
	~Camera();

	void 	reset(float dinoPosX, float dinoPosY);
	void	setPosition(float posX, float posY);
	void	setZoom(float zoom);

	//Getters
	IND_Camera2d*	getIND_Camera2d(){return m_pCamera;};


private:
	IND_Camera2d*	m_pCamera;
};

}

#endif //_H_SYMPTOGEN_RENDER_CAMERA_H_