#include "Hud.h"

using namespace std;

Hud::Hud(XMFLOAT2 pos, Sprite* center, Sprite** spheres, Sprite* sphereParticle, Sprite* top, Sprite* health, Sprite* bar, Sprite* hp, Sprite* mana,
		 Sprite* mp, Sprite* grey){
	centerHud = center;
	centerHudPos.x = (pos.x/2 - 256);
	centerHudPos.y = (pos.y - 64);
	elementSpheres = spheres;
	elementSpheresPos = XMFLOAT2(pos.x, pos.y);
	particleSphere = sphereParticle;
	particleTimer = 1.0f;

	hud_Top = top;
	hud_Health = health;
	hud_Bar = bar;
	hud_HP = hp;
	hud_Mana = mana;
	hud_MP = mp;
	hud_GreyBar = grey;

	paused = false;
	score = 0;
	pointsForKill = 25;
}

Hud::~Hud(void){
}

bool Hud::IsDrawing(){
	return drawing;
}


void Hud::Draw(SpriteBatch* spriteBatch, long int gameTime, Player* player){
	float t = gameTime;
	centerHud->Draw(spriteBatch, gameTime, 1.0f,centerHudPos, 1.0f);
	//hud_Top->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(centerHudPos.x, -20), 1.0f);
	//hud_Bar->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(centerHudPos.x, 0),1.0f);

	hud_HP->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(elementSpheresPos.x/2-102, 6),1.0f,player->getHealthPercent(),1.0f);
	hud_Health->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(elementSpheresPos.x/2-130, 1),1.0f);
	hud_MP->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(elementSpheresPos.x/2-102, 33),1.0f,player->getManaPercent(),1.0f);
	hud_Mana->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(elementSpheresPos.x/2-130, 28),1.0f);

	for(int i = 0; i < 5; i++){
		if(player->currentElement == i){
			elementSpheres[i]->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2(((elementSpheresPos.x/2-222)+i*95)-32,elementSpheresPos.y-120), 1.0f);
		}else{
			elementSpheres[i]->Draw(spriteBatch, gameTime, 1.0f, XMFLOAT2((elementSpheresPos.x/2-222)+i*95,elementSpheresPos.y-120+32), 0.5f);
		}
	}

	XMFLOAT2 center(((elementSpheresPos.x/2-284+64)+player->currentElement*95)-32,elementSpheresPos.y-184+64);
	particleSphere->Draw(spriteBatch, gameTime, -cos(t/100), XMFLOAT2(center.x+64-(64*(-cos(t/100))),center.y+64-(64*(-cos(t/100)))) , abs(cos(t/100)));
}

XMFLOAT2 Hud::calcParticleSphere(SpriteBatch* spriteBatch, long int gameTime, Player* player) {
	float t = gameTime;
	return XMFLOAT2(player->getStaffPos(t).x + 10, player->getStaffPos(t).y - 10);
}

void Hud::drawParticleSphere(SpriteBatch* spriteBatch, long int gameTime, Player* player) {
	float t = gameTime;
	if(particleTimer > -25.0f) {
		XMFLOAT2 center = calcParticleSphere(spriteBatch, t, player);
		particleSphere->Draw(spriteBatch, -cos(t/100), particleTimer, XMFLOAT2(center.x,center.y) , 0.5f);
		particleTimer -= 0.5f;
	}
}//-cos(gameTime/100) replace with Draw gameTime

int Hud::getScore() { return score; }
void Hud::setScore(int newScore) { score = newScore; }
void Hud::changeScore(int num) { score += num; }

void Hud::handleScore(int currentPlayerElement) {
	if(currentPlayerElement < 0) {
		changeScore(2);
	} else if(currentPlayerElement >= 0) {
		changeScore(10);
	}
}

void Hud::loadGame() {
	saveFile.open("save.txt");
	if(saveFile.is_open()) {
		vector<string> temp;
		string line;
		while(getline(saveFile, line)) {
			temp.push_back(line);
		}
		if(temp.empty()) {
			temp.push_back("0"); // highest score / level if none before
			temp.push_back("1");
		}
		saveFile.close();
        highestScore = atoi(temp[0].c_str());
		highestLevel = atoi(temp[1].c_str());
	}
}

void Hud::saveGame(int currentLevel) {
	if(score > highestScore) {
		highestScore = score;
	}
	if(currentLevel > highestLevel) {
		highestLevel = currentLevel;
	}

	saveFile.open("save.txt");
	if(saveFile.is_open()) {
		saveFile << highestScore << endl;
		saveFile << highestLevel;
		saveFile.close();
	}
}