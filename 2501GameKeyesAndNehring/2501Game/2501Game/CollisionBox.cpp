#include "stdafx.h"
#include "CollisionBox.h"

using namespace DirectX;

// Default Constructor
CollisionBox::CollisionBox()
{
	position = XMFLOAT2(0, 0);
	width    = 0.0f;
	height   = 0.0f;
	active   = false;
}

CollisionBox::CollisionBox(XMFLOAT2 p, float w, float h) 
{
	position = p;
	width    = w;
	height   = h;
	active   = false;
}

CollisionBox::CollisionBox(XMFLOAT2 p, float w, float h, bool a) 
{
	position = p;
	width    = w;
	height   = h;
	active   = a;
}

// Getter Functions
XMFLOAT2 CollisionBox::getPos()       { return position;   }
float    CollisionBox::getXPosition() { return position.x; }
float    CollisionBox::getYPosition() { return position.y; }
float    CollisionBox::getWidth()     { return width;      }
float    CollisionBox::getHeight()    { return height;     }
bool     CollisionBox::getActive()    { return active;     }

// Setter Functions
void CollisionBox::setPos(float xPos, float yPos) { position.x = xPos, 
	                                                position.y = yPos;   }
void CollisionBox::setPos(DirectX::XMFLOAT2 pos)  { position = pos; }
void CollisionBox::setXPosition(float aFloat)     { position.x = aFloat; }
void CollisionBox::setYPosition(float aFloat)     { position.y = aFloat; }
void CollisionBox::setWidth(float aFloat)         { width = aFloat;      }
void CollisionBox::setHeight(float aFloat)        { height = aFloat;     }
void CollisionBox::setActive(bool aBool)          { active = aBool;      }