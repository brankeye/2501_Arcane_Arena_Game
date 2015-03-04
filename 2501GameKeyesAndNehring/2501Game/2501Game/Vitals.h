////////////////////////////////////////////////////////////////////////////////
// Filename: Vitals.h
// Author: Jeffrey Nehring
////////////////////////////////////////////////////////////////////////////////
#ifndef _VITALS_H_
#define _VITALS_H_

class Vitals{
	//Variables
	//Base
private:
	float speed;
	float health;
	float maxhealth;
	float mana;
	float maxmana;
	float manaCount; // count to determine when to regen mana

	//Stat mods
	float coolDownModifier;
	float chargeTimeModifier;
	float speedModifier;		
	float shieldModifier;		//Modifies damaging effects


	//Methods
public:
	Vitals();
	Vitals(float, float);
	~Vitals(void);

	float getSpeed();
	float getHealth();
	float getMaxHealth();
	float getMana();
	float getMaxMana();
	void damage(float);

	void setMaxHealth(float value);
	void setMana(float mana);
	void setMaxMana(float max);
	void setManaCount(float count);

	void setHealth(float health);

};

#endif