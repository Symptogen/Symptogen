#include "GuiComponent.h"

namespace Symp{

GuiComponent::GuiComponent(){
	m_pEntity2d = IND_Entity2d::newEntity2d();
}

GuiComponent::~GuiComponent(){
	m_pEntity2d->destroy();
}

void GuiComponent::init(Render* pRender){

}

void GuiComponent::end(){

}

}