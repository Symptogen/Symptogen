#include "Camera.h"

namespace Symp{

Camera::Camera(){
	m_pINDCamera = new IND_Camera2d(0, 0);
}

Camera::~Camera() {
	delete m_pINDCamera;
}

void Camera::reset(float dinoPosX, float dinoPosY){
	m_pINDCamera->setPosition(dinoPosX, dinoPosY); 	
}

void Camera::setPosition(float posX, float posY){
	m_pINDCamera->setPosition (posX, posY); 	
}

void Camera::setZoom(float zoom){
	m_pINDCamera->setZoom(zoom); 	
}

void Camera::setAngle(float angle){
	m_pINDCamera->setAngle(angle); 	
}

}