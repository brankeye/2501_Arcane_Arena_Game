//////////////////////////////
// Filename: UnitControl.h
// Author: Jeffrey Nehring
//////////////////////////////

#ifndef _UNITCONTROL_H_
#define _UNITCONTROL_H_

#include "Unit.h"
#include <vector>
#include "Sprite.h"
#include "Projectile.h"

using namespace std;


class UnitControl{

	//Variables
public:
	vector<Unit*> unitList;
	Unit* thePlayer;

	//Methods
public:
	UnitControl();
	~UnitControl(void);

	void addUnit(Unit*);
	void addUnit(Unit*,bool);//bool is true if added unit is the player
	void removeUnit(Unit*);
	void drawUnits(SpriteBatch*, long int);
	void updateUnits(long int);

public:
	

};

#endif