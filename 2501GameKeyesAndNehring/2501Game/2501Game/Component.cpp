#include "Component.h"

Component::Component(XMFLOAT2 pos, Sprite* img){
	position = pos;
	image = img;
}

Component::~Component(void){
}