#include "Image.h"

namespace Symp {

Image::Image( const char* filePath)
	: GuiComponent() {
	setSurface(filePath);
}
}