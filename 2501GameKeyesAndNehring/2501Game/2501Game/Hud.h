#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Sprite.h"
#include "SpriteBatch.h"
#include "Player.h"
#include "./Inc/CommonStates.h"



class Hud{

public:
	Hud(XMFLOAT2, Sprite*, Sprite**, Sprite*, Sprite*, Sprite*, Sprite*, Sprite*, Sprite*, Sprite*, Sprite*);
	~Hud(void);
	bool IsDrawing();
	void Draw(SpriteBatch*, long int, Player*);
	XMFLOAT2 calcParticleSphere(SpriteBatch* spriteBatch, long int gameTime, Player* player);
	void drawParticleSphere(SpriteBatch* spriteBatch, long int gameTime, Player* player);
	float particleTimer;
	bool paused;

	int getScore(); // get the score
	void setScore(int); // set the score
	void changeScore(int); // add the given number to the score
	void handleScore(int currentPlayerElement); // change the score based on the player's actions (did they hit an enemy? etc)
	void loadGame(); // upon starting the game, load the highest score and level.
	void saveGame(int currentLevel);

	int highestLevel;
	int highestScore;

	int pointsForKill;

	fstream saveFile;

private:
	Sprite* centerHud;
	XMFLOAT2 centerHudPos;

	Sprite** elementSpheres;
	XMFLOAT2 elementSpheresPos;
	Sprite* particleSphere;
	bool drawing;

	Sprite* hud_Top;
	Sprite* hud_Health;
	Sprite* hud_Bar;
	Sprite* hud_HP;
	Sprite* hud_Mana;
	Sprite* hud_MP;
	Sprite* hud_GreyBar;

	int score;
};