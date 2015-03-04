#include "stdafx.h"
#include "Projectile.h"

using namespace DirectX;

Projectile::Projectile()
{
}

Projectile::Projectile(Sprite* body, float x, float y, int idTemp)
{
	sprite = body;
	collisionBox = CollisionBox(XMFLOAT2(x, y), body->getWidth() + 10, body->getHeight() + 10);
	speed = 1.5f;
	id = idTemp;
}

void Projectile::draw(SpriteBatch* spriteBatch, float gameTime) 
{
	sprite->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(collisionBox.getXPosition() - 5, collisionBox.getYPosition() - 5), 1.0f);
}

// Getter Functions
XMFLOAT2 Projectile::getPos()         { return collisionBox.getPos();  }
float    Projectile::getXPosition()   { return collisionBox.getXPosition(); }
float    Projectile::getYPosition()   { return collisionBox.getYPosition(); }
float    Projectile::getWidth()       { return sprite->getWidth();     }
float    Projectile::getHeight()      { return sprite->getHeight();    }
float    Projectile::getSpeed()       { return speed;                  }
bool     Projectile::getActive()      { return active;                 }
float    Projectile::getAngle()       { return angle;                  }
Sprite*  Projectile::getSprite()      { return sprite;                 }
CollisionBox* Projectile::getCollisionBox() { return &collisionBox;    }
int Projectile::getID() { return id; }
float Projectile::getDamage() {
	if(id < 0) {
		return 15;
	} else {
		return 25;
	}
}

// Setter Functions
void Projectile::setPos(float x, float y) { collisionBox.setXPosition(x); collisionBox.setYPosition(y); }
void Projectile::setActive(bool value) { active = value; }
void Projectile::setSpeed(float value) { speed = value; }
void Projectile::setAngle(float value) { angle = value; }
