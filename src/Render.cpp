#include "Render.h"

Render::Render(){
	m_pRender = new IND_Render();
}

IND_Window* Render::init(const char *title, int width, int height, int bpp, bool vsync, bool fs, bool dBuffer){
	IND_WindowProperties props(title, width, height, bpp, vsync, fs, dBuffer);
	return m_pRender->initRenderAndWindow(props); 
}

Render::~Render(){
	m_pRender->end();
	DISPOSE(m_pRender);
}

void Render::beginScene(){
	m_pRender->beginScene();
}

void Render::endScene(){
	m_pRender->endScene();
}

void Render::clearViewPort(unsigned char pR, unsigned char pG, unsigned char pB){
	m_pRender->clearViewPort(pR, pG, pB);
}