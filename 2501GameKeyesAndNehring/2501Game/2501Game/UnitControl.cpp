///////////////////////////////
// Filename: UnitControl.cpp
// Author: Jeffrey Nehring
///////////////////////////////

#include "UnitControl.h"
#include "./Inc/SpriteBatch.h"

//Default constructor
UnitControl::UnitControl(){
}

//Destructor
UnitControl::~UnitControl(void){
}

//Adds a already existing unit to the list
void UnitControl::addUnit(Unit* newUnit){
	unitList.push_back(newUnit);
	if(newUnit->isPlayerControlled()){
		thePlayer = newUnit;
	}
}

//Adds an already existing unit to the list, additional parameter for setting player value
void UnitControl::addUnit(Unit* newUnit,bool isPlayer){
	unitList.push_back(newUnit);
	newUnit->setPlayerControlled(isPlayer);
	if(isPlayer){
		thePlayer = newUnit;
	}
}

void UnitControl::removeUnit(Unit* checkUnit) {
	for(int i = 0; i < unitList.size(); i++) {
		if(unitList.at(i) == checkUnit) {
			unitList.erase(unitList.begin() + i);
			return;
		}
	}
}

void UnitControl::drawUnits(SpriteBatch* sprites, long int time){
	for(int i = 0; i < unitList.size(); i++){
		if(unitList[i]->getVitals()->getHealth() > 0) {
			unitList[i]->draw(sprites, time);
		}
	}
}

void UnitControl::updateUnits(long int time){
	for(int i = 0; i < unitList.size(); i++){
		unitList[i]->effectsUpdate(time);
		unitList[i]->update(time, &unitList);
	}
}

