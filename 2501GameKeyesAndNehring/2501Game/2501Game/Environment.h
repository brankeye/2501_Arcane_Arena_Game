/////////////
//
//
//
//////////////

#pragma once

#include "Sprite.h"

class Environment{

public:
	Environment(XMFLOAT2, Sprite*, Sprite*, Sprite*);
	~Environment(void);
	bool IsDrawing();
	void Draw(SpriteBatch*, long int);


private:
	Sprite* map;
	Sprite* water;
	Sprite* clouds;
	float cloudwidth;
	float waterheight;
	XMFLOAT2 screenRes;
	bool drawing;

};