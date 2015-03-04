#pragma once

#include <iostream>
#include <string>
#include <DirectXMath.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: CollisionBox
////////////////////////////////////////////////////////////////////////////////

class CollisionBox
{
public:
	// Default Constructor
	CollisionBox();
	CollisionBox(DirectX::XMFLOAT2 pos, float width, float height);
	CollisionBox(DirectX::XMFLOAT2 pos, float width, float height, bool active);

	// Getter Functions
	DirectX::XMFLOAT2 getPos();
	float    getXPosition();
	float    getYPosition();
	float    getWidth();
	float    getHeight();
	bool     getActive();

	// Setter 
	void setPos(float xPos, float yPos);
	void setPos(DirectX::XMFLOAT2);
	void setXPosition(float aFloat);
	void setYPosition(float aFloat);
	void setWidth(float aFloat);
	void setHeight(float aFloat);
	void setActive(bool aBool);

	int leftBound, upperBound, bottomBound;

private:
	DirectX::XMFLOAT2 position;
	float    width;
	float    height;
	bool     active;		// The Collision Box Exists
};