////////////////////////////////////////////////////////////////////////////////
// Filename: Unit.h
// Author: Jeffrey Nehring
////////////////////////////////////////////////////////////////////////////////
#ifndef _UNIT_H_
#define _UNIT_H_

#include "Vitals.h"
#include "Effect.h"
#include "CollisionBox.h"
#include <list>
#include <vector>
#include "Sprite.h"

using namespace std;


class Unit{
	////////////////
	//Variables
	////////////////
public:

protected:
	CollisionBox collisionBox;
	Vitals vitals;
	Effect effects;
	bool  playerControlled;
	int   faction;




	////////////////
	//Methods
	////////////////
public:
	Unit();
	Unit(bool,int);
	~Unit(void);

	DirectX::XMFLOAT2 getPos();
	Vitals* getVitals();
	Effect* getEffect();
	void setPlayerControlled(bool);
	bool isPlayerControlled();
	int getFaction();
	virtual void draw(SpriteBatch *, long int) {};
	virtual void visualUpdate() {};
	virtual void update(long int, vector<Unit*>*) {};
	bool lookingLeft;
	bool isAlive;

	void setPos(DirectX::XMFLOAT2);
	void setPos(float, float);
	void effectsUpdate(long int);
	void damageUnit(float);

	void burnUnit(int,int);
	void poisonUnit(int,int);
	void stunUnit(int,int);
	void slowUnit(int,int);
	void juiceUnit(int,int);
	float getSpeed();

	bool isMoving;
	bool isMovingLeft;
	int currentElement;

	float getHealthPercent();
	float getManaPercent();

	void handleDamage(float projectileDamage); // if unit is hit with a projectile, change the health based on the projectile damage


private:


};

#endif