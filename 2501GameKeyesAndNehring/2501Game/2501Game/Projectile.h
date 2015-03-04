#pragma once

#include "CollisionBox.h"
#include "Component.h"
#include "Sprite.h"
#include "KeyInput.h"
#include <DirectXMath.h>

////////////////////////////////////////////////////////////////////////////////
// Class name:  Collison Box Linked List
// Description: Handles all of the elements needed for a single projectile
// Author:      Brandon Keyes
////////////////////////////////////////////////////////////////////////////////

class Projectile
{
public:
	Projectile();
	Projectile(Sprite* body, float x, float y, int id);

	void draw(SpriteBatch*, float);

	// Getter Functions
	XMFLOAT2 getPos();
	float getXPosition();
	float getYPosition();
	float getWidth();
	float getHeight();
	float getSpeed();
	bool  getActive();
	float getAngle();
	float getDamage();
	int getID();
	Sprite* getSprite();
	CollisionBox* Projectile::getCollisionBox();

	// Setter Functions
	void setPos(float x, float y);
	void setActive(bool value);
	void setSpeed(float value);
	void setAngle(float value);

private:
	Sprite* sprite;
	float speed;
	float angle; // angle the projectile was fired at, this should never be changed after initialization
	bool active;
	float damage;
	int id;
	CollisionBox collisionBox; // contains the position of the projectile as well as it's width and height
};

