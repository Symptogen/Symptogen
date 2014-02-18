#include "Camera.h"

namespace Symp{

Camera::Camera(){
	m_pCamera = new IND_Camera2d(0, 0);
}

Camera::~Camera(){
	delete m_pCamera;
}

void Camera::reset(float dinoPosX, float dinoPosY){
	m_pCamera->setPosition(dinoPosX, dinoPosY); 	
}

void Camera::setPosition(float posX, float posY){
	m_pCamera->setPosition (posX, posY); 	
}

void Camera::setZoom(float zoom){
	m_pCamera->setZoom(zoom); 	
}

}