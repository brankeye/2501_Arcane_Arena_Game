/////////////////////////////////
// Filename: Effect.h
// Jeffrey Nehring
/////////////////////////////////
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <ctime>

const int BASE_MAX_POISON_STACKS = 10;
const int BASE_POISON_DELAY = 500;
const int BASE_BURN_DELAY = 500;

const float BASE_BURN_AMOUNT = 10.0f;
const float BASE_SLOW_AMOUNT = 0.5f;
const float BASE_POISON_AMOUNT = 2.0f;
const float BASE_STUN_AMOUNT = 0.0f;
const float BASE_JUICED_AMOUNT = 1.20f;


class Effect{

	//Variables
private:
	int burnEnd;
	int slowEnd;
	int stunEnd;
	int poisonEnd;
	int juicedEnd;

	int maxPoisonStacks;
	int poisonDelay;
	int burnDelay;
	int poisonStacks;

	int lastBurnTick;
	int lastPoisonTick;

	float burnDamage;
	float slowAmount;
	float poisonDamage; //Per stack
	float stunAmount;
	float juicedAmount;

	bool burned;
	bool slowed;
	bool poisoned;
	bool stunned;
	bool juiced;



	//Methods
public:
	Effect();
	~Effect(void);

	bool isBurned();
	bool isSlowed();
	bool isPoisoned();
	bool isStunned();
	bool isJuiced();

	bool stillBurning(int);
	bool stillSlowed(int);
	bool stillPoisoned(int);
	bool stillStunned(int);
	bool stillJuiced(int);

	int getLastBurnTick();
	int getLastPoisonTick();
	int getBurnDelay();
	int getPoisonDelay();
	int getPoisonStacks();

	void updateNewBurnTick();
	void updateNewPoisonTick();

	float getBurnDamage();
	float getPoisonDamage();
	float getSlowAmount();
	float getStunAmount();
	float getJuicedAmount();

	void applyBurn(int,int);
	void applyPoison(int,int);
	void applyStun(int,int);
	void applySlow(int,int);
	void applyJuiced(int,int);
	


	

};

#endif