#include "Render.h"

namespace Symp{

Render::Render(){
	m_pRender = new IND_Render();
	m_pCamera = new Camera();
	m_bIsFullScreen = false;
}

IND_Window* Render::init(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer) {
	IND_WindowProperties props(title, width, height, bpp, vsync, fs, dBuffer);
	return m_pRender->initRenderAndWindow(props); 
}

Render::~Render(){
	delete m_pCamera;
	fprintf(stderr, "seg fault here\n");
	m_pRender->end();
	fprintf(stderr, "ok\n");
	DISPOSE(m_pRender);
}

void Render::beginScene(){
	m_pRender->beginScene();
}

void Render::endScene(){
	m_pRender->endScene();
}

void Render::clearViewPort(unsigned char pR, unsigned char pG, unsigned char pB) {
	m_pRender->clearViewPort(pR, pG, pB);
}

void Render::setCameraPosition(float posX, float posY) {
	m_pCamera->setPosition(posX, posY);
}

void Render::setZoom(float zoom){
	m_pCamera->setZoom(zoom);
}

void Render::setCamera(){
	m_pRender->setCamera2d(m_pCamera->getIND_Camera2d());
}

void Render::setCameraAngle(float angle){
	m_pCamera->setAngle(angle);
}

void Render::resetCamera(float dinoPosX, float dinoPosY){
	m_pCamera->reset(dinoPosX, dinoPosY);
}

void Render::toggleFullScreen() {
	m_pRender->toggleFullScreen();
	m_bIsFullScreen = !m_bIsFullScreen;
}

}