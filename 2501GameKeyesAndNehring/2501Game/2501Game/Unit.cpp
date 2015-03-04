//////////////////////////////
// Filename: Unit.cpp
// Author: Jeffrey Nehring
//////////////////////////////
#include "Unit.h"
using namespace DirectX;

//Default Constructor
Unit::Unit(){
	playerControlled = false;
	lookingLeft = true;
	currentElement = 2;
	isMoving = false;
	isMovingLeft = false;
}

//Constructor
Unit::Unit(bool pC,int fac){
	playerControlled = pC;
	faction = fac;
}

//Destructor
Unit::~Unit(void){
}

//Returns unit position
XMFLOAT2 Unit::getPos(){
	return collisionBox.getPos();
}

//Returns unit vitals
Vitals* Unit::getVitals(){
	return &vitals;
}

//Set if player is controlled
void Unit::setPlayerControlled(bool value){
	playerControlled = value;
}

//Returns if the unit is player controlled
bool Unit::isPlayerControlled(){
	return playerControlled;
}

//Returns the faction of the unit
int Unit::getFaction(){
	return faction;
}

//Returns a pointer to the unit effect list
Effect* Unit::getEffect(){
	return &effects;
}

//Sets position based off of a position object
void Unit::setPos(XMFLOAT2 newPos){
	collisionBox.setXPosition(newPos.x);
	collisionBox.setYPosition(newPos.y);
}

//Sets position based off of a pair of floats
void Unit::setPos(float x, float y){
	collisionBox.setXPosition(x);
	collisionBox.setYPosition(y);
}


float Unit::getHealthPercent(){
	return vitals.getHealth() / vitals.getMaxHealth();
}

float Unit::getManaPercent(){
	return vitals.getMana() / vitals.getMaxMana();
}


//Updates all current effects on the unit
void Unit::effectsUpdate(long int currentTime){
	//currentTime = clock();

	//Check for burns and apply burn damage
	if(effects.isBurned()){
		if(effects.stillBurning(currentTime)){
			if(currentTime - effects.getLastBurnTick() > effects.getBurnDelay()){
				effects.updateNewBurnTick();
				vitals.damage(effects.getBurnDamage());
			}
		}
	}
	//END

	//Check for poison and apply poison damage
	if(effects.isPoisoned()){
		if(effects.stillPoisoned(currentTime)){
			if(currentTime - effects.getLastPoisonTick() > effects.getPoisonDelay()){
				effects.updateNewPoisonTick();
				vitals.damage(effects.getPoisonDamage()*effects.getPoisonStacks());
			}
		}
	}
	//END

	//Check for slows
	if(effects.isSlowed()){
		if(effects.stillSlowed(currentTime)){

		}
	}

	//Check for stuns
	if(effects.isStunned()){
		if(effects.stillStunned(currentTime)){

		}
	}

	//Check for hyper
	if(effects.isJuiced()){
		if(effects.stillJuiced(currentTime)){

		}
	}

}

void Unit::handleDamage(float projectileDamage)
{
	getVitals()->setHealth(getVitals()->getHealth() - projectileDamage);
	if(getVitals()->getHealth() < 0) {
		getVitals()->setHealth(0);
	}
}

//Burns unit
void Unit::burnUnit(int time, int duration){
	effects.applyBurn(time,duration);
}

//Poisons unit
void Unit::poisonUnit(int time, int duration){
	effects.applyPoison(time,duration);
}

//Stuns unit
void Unit::stunUnit(int time, int duration){
	effects.applyStun(time,duration);
}

//Slows unit
void Unit::slowUnit(int time, int duration){
	effects.applySlow(time,duration);
}

//juices unit
void Unit::juiceUnit(int time, int duration){
	effects.applyJuiced(time,duration);
}

float Unit::getSpeed(){
	return vitals.getSpeed() * getEffect()->getSlowAmount() * getEffect()->getStunAmount() * getEffect()->getJuicedAmount();
}