#pragma once
#include "Sprite.h"


class Component{
public:
	Component(XMFLOAT2, Sprite*);
	~Component(void);

	XMFLOAT2 position;
	Sprite*  image;
};