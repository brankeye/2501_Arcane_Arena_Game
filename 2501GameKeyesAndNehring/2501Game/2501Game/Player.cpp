// Author: Jeffrey Nehring
// Date:   March 9th, 2014
//
#include "Player.h"
using namespace DirectX;

Player::Player(){
	isMoving = false;
	isMovingLeft = false;
}

Player::Player(Sprite* hat, Sprite* head, Sprite* body, Sprite* foot, Sprite* hand, Sprite* staff, vector<Sprite*>* projectiles){
	g_hat = new Component(XMFLOAT2(-6,-5),hat);
	g_head = new Component(XMFLOAT2(-5,8),head);
	g_body = new Component(XMFLOAT2(-5,20),body);
	g_leftfoot = new Component(XMFLOAT2(3,55),foot);
	g_rightfoot = new Component(XMFLOAT2(3,55),foot);
	g_lefthand = new Component(XMFLOAT2(-10,30),hand);
	g_righthand = new Component(XMFLOAT2(25,30),hand);
	g_staff = new Component(XMFLOAT2(15,0),staff);

	isMoving = false;
	isMovingLeft = false;

	casting = new Casting(projectiles);

	collisionBox = CollisionBox(XMFLOAT2(10,0), 25, 65);
	chargeTime = 0;
	fired = false;
	
	vitals.setMaxMana(100.0f);
	vitals.setMana(vitals.getMaxMana());
	vitals.setManaCount(0);
}

void Player::draw(SpriteBatch* sprites, long int t){
	double time = t;
	float yMod = -10.0f;
	if(isMoving){
		g_body->image->Draw(sprites,0,1.0f,     XMFLOAT2(collisionBox.getXPosition()+g_body->position.x, collisionBox.getYPosition()+g_body->position.y+sin(time/300) + yMod), 0.5f);
		g_head->image->Draw(sprites,0,1.0f,     XMFLOAT2(collisionBox.getXPosition()+g_head->position.x, collisionBox.getYPosition()+g_head->position.y+sin(time/300) + yMod), 0.5f);
		g_hat->image->Draw(sprites,0,1.0f,      XMFLOAT2(collisionBox.getXPosition()+g_hat->position.x, collisionBox.getYPosition()+g_hat->position.y+sin(time/300) + yMod), 0.5f);
		if(isMovingLeft){
			g_leftfoot->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_leftfoot->position.x-7*cos(-time/100), collisionBox.getYPosition()+g_leftfoot->position.y-3*sin(-time/100) + yMod), 0.5f);
			g_rightfoot->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_rightfoot->position.x+7*cos(-time/100), collisionBox.getYPosition()+g_rightfoot->position.y+3*sin(-time/100) + yMod), 0.5f);
		}else{
			g_leftfoot->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_leftfoot->position.x-7*cos(time/100), collisionBox.getYPosition()+g_leftfoot->position.y-3*sin(time/100) + yMod), 0.5f);
			g_rightfoot->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_rightfoot->position.x+7*cos(time/100), collisionBox.getYPosition()+g_rightfoot->position.y+3*sin(time/100) + yMod), 0.5f);
		}
		g_staff->image->Draw(sprites,0,1.0f,    XMFLOAT2(collisionBox.getXPosition()+g_staff->position.x+sin(time/400), collisionBox.getYPosition()+g_staff->position.y + yMod), 0.5f);
		g_lefthand->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_lefthand->position.x+cos(time/400), collisionBox.getYPosition()+g_lefthand->position.y + yMod), 0.5f);
		g_righthand->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_righthand->position.x+sin(time/400), collisionBox.getYPosition()+g_righthand->position.y + yMod), 0.5f);
	}else{

		g_body->image->Draw(sprites,0,1.0f,     XMFLOAT2(collisionBox.getXPosition()+g_body->position.x, collisionBox.getYPosition()+g_body->position.y+sin(time/300) + yMod), 0.5f);
		g_head->image->Draw(sprites,0,1.0f,     XMFLOAT2(collisionBox.getXPosition()+g_head->position.x, collisionBox.getYPosition()+g_head->position.y+sin(time/300) + yMod), 0.5f);
		g_hat->image->Draw(sprites,0,1.0f,      XMFLOAT2(collisionBox.getXPosition()+g_hat->position.x, collisionBox.getYPosition()+g_hat->position.y+sin(time/300) + yMod), 0.5f);
		g_leftfoot->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_leftfoot->position.x+5, collisionBox.getYPosition()+g_leftfoot->position.y + yMod), 0.5f);
		g_rightfoot->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_rightfoot->position.x-5, collisionBox.getYPosition()+g_rightfoot->position.y + yMod), 0.5f);
		g_staff->image->Draw(sprites,0,1.0f,    XMFLOAT2(collisionBox.getXPosition()+g_staff->position.x+sin(time/400), collisionBox.getYPosition()+g_staff->position.y + yMod), 0.5f);
		g_lefthand->image->Draw(sprites,0,1.0f, XMFLOAT2(collisionBox.getXPosition()+g_lefthand->position.x+cos(time/400), collisionBox.getYPosition()+g_lefthand->position.y + yMod), 0.5f);
		g_righthand->image->Draw(sprites,0,1.0f,XMFLOAT2(collisionBox.getXPosition()+g_righthand->position.x+sin(time/400), collisionBox.getYPosition()+g_righthand->position.y + yMod), 0.5f);
	}
}

void Player::move(KeyInput* keyInput, int rightBoundary, int screenHeight){
	const int ascii_W = 87; bool moveUp = false;
	const int ascii_A = 65; bool moveLeft = false;
	const int ascii_S = 83; bool moveDown = false;
	const int ascii_D = 68; bool moveRight = false;
	isMoving = false;
	isMovingLeft = false;

	if(keyInput->IsKeyDown(ascii_W)){
		moveUp = true;
		isMoving = true;
	}
	if(keyInput->IsKeyDown(ascii_A)){
		moveLeft = true;
		isMoving = true;
		isMovingLeft = true;
	}
	if(keyInput->IsKeyDown(ascii_S)){
		moveDown = true;
		isMoving = true;
	}
	if(keyInput->IsKeyDown(ascii_D)){
		moveRight = true;
		isMoving = true;
	}
	// .707 * speed
	if(moveUp) {
		if(moveLeft) {
			moveLeft = false;
			collisionBox.setXPosition(collisionBox.getXPosition() - vitals.getSpeed() * sin(XM_PIDIV4));
			collisionBox.setYPosition(collisionBox.getYPosition() - vitals.getSpeed() * sin(XM_PIDIV4));
		} else if(moveRight) {
			moveRight = false;
			collisionBox.setXPosition(collisionBox.getXPosition() + vitals.getSpeed() * sin(XM_PIDIV4));
			collisionBox.setYPosition(collisionBox.getYPosition() - vitals.getSpeed() * sin(XM_PIDIV4));
		} else {
			collisionBox.setYPosition(collisionBox.getYPosition() - vitals.getSpeed());
		}
		moveUp = false;
	}
	if(moveLeft) {
		if(moveUp) {
			moveUp = false;
			collisionBox.setXPosition(collisionBox.getXPosition() - vitals.getSpeed() * sin(XM_PIDIV4));
			collisionBox.setYPosition(collisionBox.getYPosition() - vitals.getSpeed() * sin(XM_PIDIV4));
		} else if(moveDown) {
			moveDown = false;
			collisionBox.setXPosition(collisionBox.getXPosition() - vitals.getSpeed() * sin(XM_PIDIV4));
			collisionBox.setYPosition(collisionBox.getYPosition() + vitals.getSpeed() * sin(XM_PIDIV4));
		} else {
			collisionBox.setXPosition(collisionBox.getXPosition() - vitals.getSpeed());
		}
		moveLeft = false;
	}
	if(moveDown) {
		if(moveRight) {
			moveRight = false;
			collisionBox.setXPosition(collisionBox.getXPosition() + vitals.getSpeed() * sin(XM_PIDIV4));
			collisionBox.setYPosition(collisionBox.getYPosition() + vitals.getSpeed() * sin(XM_PIDIV4));
		} else if(moveLeft) {
			moveLeft = false;
			collisionBox.setXPosition(collisionBox.getXPosition() - vitals.getSpeed() * sin(XM_PIDIV4));
			collisionBox.setYPosition(collisionBox.getYPosition() + vitals.getSpeed() * sin(XM_PIDIV4));
		} else {
			collisionBox.setYPosition(collisionBox.getYPosition() + vitals.getSpeed());
		}
		moveDown = false;
	}
	if(moveRight) {
		if(moveUp) {
			moveUp = false;
			collisionBox.setXPosition(collisionBox.getXPosition() + vitals.getSpeed());
			collisionBox.setYPosition(collisionBox.getYPosition() - vitals.getSpeed() * sin(XM_PIDIV4));
		} else if(moveDown) {
			moveDown = false;
			collisionBox.setXPosition(collisionBox.getXPosition() + vitals.getSpeed());
			collisionBox.setYPosition(collisionBox.getYPosition() + vitals.getSpeed() * sin(XM_PIDIV4));
		} else {
			collisionBox.setXPosition(collisionBox.getXPosition() + vitals.getSpeed());
		}
		moveRight = false;
	}

	// check if player passes borders
	if(collisionBox.getXPosition() < 10) {
		collisionBox.setXPosition(10);
	}
	if(collisionBox.getXPosition() > rightBoundary) {
		collisionBox.setXPosition(rightBoundary);
	}
	if(collisionBox.getYPosition() + collisionBox.getHeight() > screenHeight + 15) {
		collisionBox.setYPosition(screenHeight - collisionBox.getHeight() + 15);
	}
	if(collisionBox.getYPosition() < 10) {
		collisionBox.setYPosition(10);
	}


	if(keyInput->IsKeyDown(0x31)){//1 key
		currentElement = 0;
	}
	if(keyInput->IsKeyDown(0x32)){//2 key
		currentElement = 1;
	}
	if(keyInput->IsKeyDown(0x33)){//3 key
		currentElement = 2;
	}
	if(keyInput->IsKeyDown(0x34)){//4 key
		currentElement = 3;
	}
	if(keyInput->IsKeyDown(0x35)){//5 key
		currentElement = 4;
	}
}

void Player::visualUpdate(){

}

void Player::handleCasting(bool leftMouse, bool rightMouse, XMFLOAT2 mouse)
{
	if(fired) {
		fired = false;
	}
	if(leftMouse == true || rightMouse == true) {
		if(chargeTime == 0) {
			if(leftMouse == true) {
				XMFLOAT2 newPos = getPos();
				newPos.x += 25;
				newPos.y -= 18;
				casting->basicCast(newPos, mouse);
				fired = true;
			}
			if(rightMouse == true && vitals.getMana() >= 25) {
				fired = true;
				vitals.setMana(vitals.getMana() - 25);
				XMFLOAT2 newPos = getPos();
				newPos.x += 25;
				newPos.y -= 18;
				casting->specialCast(newPos, mouse, currentElement);
			}
			chargeTime++;
		}
	}
	if(chargeTime > 0) {
		chargeTime++;
		if(chargeTime > 500) {
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

void Player::updateCasting(XMFLOAT2 screenSize)
{
	casting->updateProjectiles(screenSize);
}

CollisionBox* Player::getCollisionBox() { return &collisionBox; }

std::vector<CollisionBox*>* Player::getCollisionList()
{
	return casting->getCollisionList();
}

std::vector<Projectile*>* Player::getProjectileList()
{
	return casting->getProjectileList();
}

Casting* Player::getCasting()
{
	return casting;
}

void Player::drawCasting(SpriteBatch* spriteBatch, float gameTime)
{
	casting->drawProjectiles(spriteBatch, gameTime);
}

void Player::manaBoost() {
	vitals.setMana(vitals.getMana() + 5);
}

XMFLOAT2 Player::getStaffPos(long int time) { 
	return XMFLOAT2(collisionBox.getXPosition()+g_staff->position.x+sin(time/400),collisionBox.getYPosition()+g_staff->position.y);
}

void Player::update(long int time, vector<Unit*>* unitList){

}