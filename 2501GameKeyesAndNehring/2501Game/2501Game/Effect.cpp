///////////////////////////////
// Filename: Effect.cpp
// Author: Jeffrey Nehring
///////////////////////////////

#include "Effect.h"

Effect::Effect(){
	int now = clock();

	burnEnd   = now;
	slowEnd   = now;
	stunEnd   = now;
	poisonEnd = now;
	juicedEnd  = now;

	lastBurnTick = now;
	lastPoisonTick = now;

	burned   = false;
	slowed   = false;
	stunned  = false;
	poisoned = false;
	juiced	 = false;

	maxPoisonStacks = BASE_MAX_POISON_STACKS;
	burnDelay       = BASE_BURN_DELAY;
	poisonDelay     = BASE_POISON_DELAY;

	burnDamage   = BASE_BURN_AMOUNT;
	slowAmount   = BASE_SLOW_AMOUNT;
	stunAmount   = BASE_STUN_AMOUNT;
	poisonDamage = BASE_POISON_AMOUNT;
	juicedAmount  = BASE_JUICED_AMOUNT;

	poisonStacks = 0;
}

Effect::~Effect(void){

}

//You should know what this does
bool Effect::isBurned(){
	return burned;
}

//And this
bool Effect::isPoisoned(){
	return poisoned;
}

//This too
bool Effect::isSlowed(){
	return slowed;
}

//This tells returns if stunned is true or false
bool Effect::isStunned(){
	return stunned;
}

//Likewise
bool Effect::isJuiced(){
	return juiced;
}

//Returns number of poison stacks in a poison effect, max determined by const MAX_POISON_STACKS
int Effect::getPoisonStacks(){
	return poisonStacks;
}

//Checks if burn effect expires
bool Effect::stillBurning(int now){
	if(now > burnEnd){
		burned = false;
		return false;
	}
	return true;
}

//Checks if slow effect expires
bool Effect::stillSlowed(int now){
	if(now > slowEnd){
		slowed = false;
		return false;
	}
	return true;
}

//Checks if stun effect expires
bool Effect::stillStunned(int now){
	if(now > stunEnd){
		stunned = false;
		return false;
	}
	return true;
}

//Checks if poison effect expires
bool Effect::stillPoisoned(int now){
	if(now > poisonEnd){
		poisoned = false;
		return false;
		poisonStacks = 0;
	}
	return true;
}

//Checks if juice effect expires
bool Effect::stillJuiced(int now){
	if(now > juicedEnd){
		juiced = false;
		return false;
	}
	return true;
}

//Returns last time burn damage occured
int Effect::getLastBurnTick(){
	return lastBurnTick;
}

//Returns last time poison damage occured
int Effect::getLastPoisonTick(){
	return lastPoisonTick;
}

//Returns delay between burn damage instances
int Effect::getBurnDelay(){
	return burnDelay;
}

//Returns delay between poison damage instances
int Effect::getPoisonDelay(){
	return poisonDelay;
}

//Updates when the last burn tick occured
void Effect::updateNewBurnTick(){
	lastBurnTick+= burnDelay;
}

//Updates when the last poison tick occured
void Effect::updateNewPoisonTick(){
	lastPoisonTick+= poisonDelay;
}

//Returns burn damage
float Effect::getBurnDamage(){
	return burnDamage;
}

//Returns poison damage
float Effect::getPoisonDamage(){
	return poisonDamage;
}

//Returns stun modifier
float Effect::getStunAmount(){
	if(stunned){
		return stunAmount;
	}else{
		return 1.0f;
	}
}

//Returns slow modifier
float Effect::getSlowAmount(){
	if(slowed){
		return slowAmount;
	}else{
		return 1.0f;
	}
}

//Returns juice modifier
float Effect::getJuicedAmount(){
	if(juiced){
		return juicedAmount;
	}else{
		return 1.0f;
	}
}

//Applies a burn effect while updating relevent time information
void Effect::applyBurn(int time, int duration){
	if(burned){
		if(time+duration > burnEnd){
			burnEnd = time + duration;
		}
	}else{
		burned = true;
		burnEnd = time + duration;
	}
}

//Applies a poison effect while updating relevent time information
void Effect::applyPoison(int time, int duration){
	poisonStacks++;
	if(poisonStacks > maxPoisonStacks){
		poisonStacks = maxPoisonStacks;
	}

	if(poisoned){
		if(time+duration > poisonEnd){
			poisonEnd = time + duration;
		}
	}else{
		poisoned = true;
		poisonEnd = time + duration;
	}
}

//Applies a stun effect while updating relevent time information
void Effect::applyStun(int time, int duration){
	if(stunned){
		if(time+duration > stunEnd){
			stunEnd = time + duration;
		}
	}else{
		stunned = true;
		stunEnd = time + duration;
	}
}

//Applies a slow effect while updating relevent time information
void Effect::applySlow(int time, int duration){
	if(slowed){
		if(time+duration > slowEnd){
			slowEnd = time + duration;
		}
	}else{
		slowed = true;
		slowEnd = time + duration;
	}
}

//Applies a juice effect while updating relevent time information
void Effect::applyJuiced(int time, int duration){
	if(juiced){
		if(time+duration > juicedEnd){
			juicedEnd = time + duration;
		}
	}else{
		juiced = true;
		juicedEnd = time + duration;
	}
}

