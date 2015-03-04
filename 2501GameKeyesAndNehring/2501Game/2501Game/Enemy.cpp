#include "Enemy.h"
#include <cmath>

Enemy::Enemy(void)
{
}

Enemy::Enemy(Sprite* head, Sprite* body, Sprite* foot, Sprite* hand, Sprite* staff, vector<Sprite*>* projectiles)
{
	g_head = new Component(XMFLOAT2(-5,8),head);
	g_body = new Component(XMFLOAT2(-5,20),body);
	g_leftfoot = new Component(XMFLOAT2(3,55),foot);
	g_rightfoot = new Component(XMFLOAT2(3,55),foot);
	g_lefthand = new Component(XMFLOAT2(-10,30),hand);
	g_righthand = new Component(XMFLOAT2(25,30),hand);
	g_staff = new Component(XMFLOAT2(-24,0),staff);
	waypoint.x = rand() % 550 + 750;
	waypoint.y = rand() % 700 + 10;

	casting = new Casting(projectiles);

	chargeTime = 0;
	fired = false;

	specialManaCost = 25;
	
	vitals.setMaxHealth(50);
	vitals.setHealth(vitals.getMaxHealth());
	
	vitals.setMaxMana(100.0f);
	vitals.setMana(vitals.getMaxMana() / 2);
	vitals.setManaCount(0);
}

void Enemy::initLocation(float x, float y) {
	collisionBox = CollisionBox(XMFLOAT2(x,y), 25, 60);
} 

void Enemy::draw(SpriteBatch* sprites, long int t){
	double time = t;
	float xMod = -5.0f;
	float yMod = 12.0f;
	if(isMoving){
		g_body->image->Draw(sprites,0,1.0f,     XMFLOAT2(collisionBox.getXPosition()+g_body->position.x + xMod,collisionBox.getYPosition()+g_body->position.y+sin(time/300) + yMod), 0.5f);
		g_head->image->Draw(sprites,0,1.0f,     XMFLOAT2(collisionBox.getXPosition()+g_head->position.x + xMod,collisionBox.getYPosition()+g_head->position.y+sin(time/300) + yMod), 0.5f);
		if(isMovingLeft){
			g_rightfoot->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_rightfoot->position.x+7*cos(-time/100) + xMod,collisionBox.getYPosition()+g_rightfoot->position.y+3*sin(-time/100) + yMod), 0.5f);
			g_leftfoot->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_leftfoot->position.x-7*cos(-time/100) + xMod,collisionBox.getYPosition()+g_leftfoot->position.y-3*sin(-time/100) + yMod), 0.5f);
		}else{
			g_leftfoot->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_leftfoot->position.x-7*cos(time/100) + xMod,collisionBox.getYPosition()+g_leftfoot->position.y-3*sin(time/100) + yMod), 0.5f);
			g_rightfoot->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_rightfoot->position.x+7*cos(time/100) + xMod,collisionBox.getYPosition()+g_rightfoot->position.y+3*sin(time/100) + yMod), 0.5f);
		}
		g_staff->image->Draw(sprites,0,1.0f,    XMFLOAT2(collisionBox.getXPosition()+g_staff->position.x+sin(time/400) + xMod,collisionBox.getYPosition()+g_staff->position.y + yMod), 0.5f);
		g_lefthand->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_lefthand->position.x+sin(time/400) + xMod,collisionBox.getYPosition()+g_lefthand->position.y + yMod), 0.5f);
		g_righthand->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_righthand->position.x+cos(time/400) + xMod,collisionBox.getYPosition()+g_righthand->position.y + yMod), 0.5f);
	}else{

		g_body->image->Draw(sprites,0,1.0f,     XMFLOAT2(collisionBox.getXPosition()+g_body->position.x + xMod,collisionBox.getYPosition()+g_body->position.y+sin(time/300) + yMod), 0.5f);
		g_head->image->Draw(sprites,0,1.0f,     XMFLOAT2(collisionBox.getXPosition()+g_head->position.x + xMod,collisionBox.getYPosition()+g_head->position.y+sin(time/300) + yMod), 0.5f);
		g_rightfoot->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_rightfoot->position.x-5 + xMod,collisionBox.getYPosition()+g_rightfoot->position.y + yMod), 0.5f);
		g_leftfoot->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_leftfoot->position.x+5 + xMod,collisionBox.getYPosition()+g_leftfoot->position.y + yMod), 0.5f);
		g_staff->image->Draw(sprites,0,1.0f,    XMFLOAT2(collisionBox.getXPosition()+g_staff->position.x+sin(time/400) + xMod,collisionBox.getYPosition()+g_staff->position.y + yMod), 0.5f);
		g_lefthand->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_lefthand->position.x+sin(time/400) + xMod,collisionBox.getYPosition()+g_lefthand->position.y + yMod), 0.5f);
		g_righthand->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_righthand->position.x+cos(time/400) + xMod,collisionBox.getYPosition()+g_righthand->position.y + yMod), 0.5f);
	}
}

void Enemy::move(KeyInput* keyInput){
	/*
	float moveSpeed = 0.1;
	const int ascii_W = 87; bool moveUp = false;
	const int ascii_A = 65; bool moveLeft = false;
	const int ascii_S = 83; bool moveDown = false;
	const int ascii_D = 68; bool moveRight = false;
	if(keyInput->IsKeyDown(ascii_W)){
		moveUp = true;
	}
	if(keyInput->IsKeyDown(ascii_A)){
		moveLeft = true;
	}
	if(keyInput->IsKeyDown(ascii_S)){
		moveDown = true;
	}
	if(keyInput->IsKeyDown(ascii_D)){
		moveRight = true;
	}

	if(moveUp)    collisionBox.setYPosition(collisionBox.getYPosition() - moveSpeed);
	if(moveLeft)  collisionBox.setXPosition(collisionBox.getXPosition() - moveSpeed);
	if(moveDown)  collisionBox.setYPosition(collisionBox.getYPosition() + moveSpeed);
	if(moveRight) collisionBox.setXPosition(collisionBox.getXPosition() + moveSpeed);
	*/
}

void Enemy::update(long int time, vector<Unit*>* unitList){
	while(sqrt((waypoint.x - collisionBox.getXPosition())*(waypoint.x - collisionBox.getXPosition()) + (waypoint.y - collisionBox.getYPosition())*(waypoint.y - collisionBox.getYPosition())) < 50){
		waypoint.x = rand() % 550 + 750;
		waypoint.y = rand() % 700 + 10;
	}


	for (int i = 0; i < unitList->size(); i++){
      Unit* u = (*unitList)[i];
        if (u != this){
          if (sqrt(pow(u->getPos().x-this->getPos().x,2)+pow(u->getPos().y-this->getPos().y,2)) < 50){
            waypoint.x = 2*this->getPos().x - u->getPos().x + rand() % 20 - 10;
            waypoint.y = 2*this->getPos().y - u->getPos().y + rand() % 20 - 10;

              
            if(waypoint.x < 750){waypoint.x = 750;}
			if(waypoint.x > 1200){waypoint.x = 1200;}
			if(waypoint.y < 10){waypoint.x = 10;}
			if(waypoint.y > 710){waypoint.x = 710;}
          }
        }
      }



	isMoving = false;
	isMovingLeft = false;
	if(waypoint.x < collisionBox.getXPosition()){
		isMoving = true;
		isMovingLeft = true;
	}else{
		isMoving = true;
		isMovingLeft = false;
	}
	moveEnemy(atan2(waypoint.y - collisionBox.getYPosition(),(waypoint.x - collisionBox.getXPosition())));
}

void Enemy::moveEnemy(float angle){
	collisionBox.setXPosition((collisionBox.getXPosition())+(vitals.getSpeed()*cos(angle)));
	collisionBox.setYPosition((collisionBox.getYPosition())+(vitals.getSpeed()*sin(angle)));
}

void Enemy::handleCasting(XMFLOAT2 playerPosition)
{
	if(fired) {
		fired = false;
	}
	if(chargeTime == 0) {
		// generate a random number to determine which ability to use (basic or special?)
		int floor = 0;
		int ceiling = 2;
		int range = ceiling - floor;
		int rnd = floor + int((range * rand()) / (RAND_MAX + 1.0f));

		// generate a random number to add to the enemy's x target
		float xFloor = 0;
		float xCeiling = 100;
		float xRange = xCeiling - xFloor;
		int xRND = xFloor + ((xRange * rand()) / (RAND_MAX + 1.0f));

		// generate a random number to add to the enemy's y target
		float yFloor = 0;
		float yCeiling = 100;
		float yRange = yCeiling - yFloor;
		int yRND = yFloor + ((yRange * rand()) / (RAND_MAX + 1.0f));

		xRND *= rnd; yRND *= rnd;
		
		// where the enemy will be shooting
		XMFLOAT2 target = XMFLOAT2(playerPosition.x + xRND, playerPosition.y + yRND);

		

		if(rnd == 1 && vitals.getMana() >= specialManaCost) {
			// generate a random number to determine which special ability to use
			int eFloor = 0;
			int eCeiling = 5; // 0, 1, 2, 3, 4
			int eRange = eCeiling - eFloor;
			int eRND = eFloor + ((eRange * rand()) / (RAND_MAX + 1.0f));

			fired = true;
			vitals.setMana(vitals.getMana() - specialManaCost);
			XMFLOAT2 newPos = getPos();
			newPos.x -= 40;
			casting->specialCast(newPos, target, eRND);

		} else {
			XMFLOAT2 newPos = getPos();
			newPos.x -= 40;
			casting->basicCast(newPos, target);
			fired = true;
		}
		chargeTime++;

		// adjust the random modifier so that all enemies shoot at semi-random times
		int modFloor = 1000;
		int modCeiling = 3000;
		int modRange = modCeiling - modFloor;
		randomModifier = modFloor + int((modRange * rand()) / (RAND_MAX + 1.0f));
	}
	if(chargeTime > 0) {
		chargeTime++;
		if(chargeTime > randomModifier) {
			chargeTime = 0;
		}
	}

	vitals.setMana(vitals.getMana() + 0.003);

	if(vitals.getMana() < 0) {
		vitals.setMana(0);
	} 
	else if(vitals.getMana() > vitals.getMaxMana()) {
		vitals.setMana(vitals.getMaxMana());
	}
}

void Enemy::updateCasting(XMFLOAT2 screenSize)
{
	casting->updateProjectiles(screenSize);
}

CollisionBox* Enemy::getCollisionBox() { return &collisionBox; }

std::vector<CollisionBox*>* Enemy::getCollisionList()
{
	return casting->getCollisionList();
}

std::vector<Projectile*>* Enemy::getProjectileList()
{
	return casting->getProjectileList();
}

Casting* Enemy::getCasting()
{
	return casting;
}

void Enemy::drawCasting(SpriteBatch* spriteBatch, float gameTime)
{
	casting->drawProjectiles(spriteBatch, gameTime);
}

void Enemy::manaBoost() {
	vitals.setMana(vitals.getMana() + 5);
}