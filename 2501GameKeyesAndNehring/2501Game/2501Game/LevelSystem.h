#pragma once
class LevelSystem
{
public:
	LevelSystem();
	~LevelSystem(void);

	int getLevel();
	void resetLevel();
	void handleLevels(long int timePlaying);

	long int rate;
	long int spawn;
	long int spawnTime;

	bool spawnReady;

private:
	long int level;
	long int modifier;
	
};

