#include "LevelSystem.h"


LevelSystem::LevelSystem()
{
	level = 1;
	modifier = 20000;
	rate = 20000;
	spawn = 20000;
	spawnTime = spawn;
	spawnReady = false;
}

LevelSystem::~LevelSystem(void)
{
}

void LevelSystem::handleLevels(long int timePlaying)
{
	if(spawnReady) {
		spawnReady = false;
	}
	if(rate < timePlaying) {
		level++;
		rate += modifier;
		spawnTime -= 500 * level;
	}

	if(spawn < timePlaying + spawnTime) {
		spawnReady = true;
		spawn += spawnTime;
	}
	if(level <= 0) {
		level = 1;
	}
}

int LevelSystem::getLevel() { return level; }

void LevelSystem::resetLevel() { level = 1; }
