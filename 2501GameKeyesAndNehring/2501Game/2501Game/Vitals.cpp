////////////////////////////////////////////////////////////////////////////////
// Filename: Vitals.cpp
// Author: Jeffrey Nehring
////////////////////////////////////////////////////////////////////////////////

#include "Vitals.h"

using namespace std;

//Default Constructor
Vitals::Vitals(){
	speed  = 0.35f;
	health = 100.0f;
	maxhealth = 100.0f;
	coolDownModifier   = 1.0f;
	chargeTimeModifier = 1.0f;
	shieldModifier     = 1.0f;
	speedModifier      = 1.0f;
}


//Constructor
Vitals::Vitals(float newSpeed, float newHealth){
	speed  = newSpeed;
	health = newHealth;
	coolDownModifier   = 1.0f;
	chargeTimeModifier = 1.0f;
	shieldModifier     = 1.0f;
	speedModifier      = 1.0f;
}

//Destructor
Vitals::~Vitals(void){
}


float Vitals::getHealth(){
	return health;
}

float Vitals::getMaxHealth(){
	return maxhealth;
}

void Vitals::setMaxHealth(float value) {
	maxhealth = value;
}

float Vitals::getMana(){
	return mana;
}

float Vitals::getMaxMana(){
	return maxmana;
}

float Vitals::getSpeed(){
	return speed*speedModifier;
}


void Vitals::damage(float damageAmount){
	health -= (damageAmount*shieldModifier);
}

void Vitals::setMana(float newMana) { mana = newMana; }
void Vitals::setMaxMana(float max) { maxmana = max; }
void Vitals::setManaCount(float count) { manaCount = count; }
void Vitals::setHealth(float h) { health = h; }
