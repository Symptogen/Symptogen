#include "Button.h"

namespace Symp {

Button::Button(std::string name, const char* filePath)
	: GuiComponent(name) {
	setSurface(filePath);
}

}