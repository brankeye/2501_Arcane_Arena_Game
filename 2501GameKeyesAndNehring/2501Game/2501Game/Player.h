#pragma once
//
// Author: Jeffrey Nehring
// Date: March 10th, 2014
//

#include "Unit.h"
#include "Sprite.h"
#include "Component.h"
#include "KeyInput.h"
#include "Casting.h"
#include "CollisionBox.h"

class Player : public Unit
{
public:
	friend Unit;
	Player();
	Player(Sprite*, Sprite*, Sprite*, Sprite*, Sprite*, Sprite*, std::vector<Sprite*>*);
	~Player(void);

	void moveSprites();
	void visualUpdate();
	void draw(SpriteBatch*, long int);
	void move(KeyInput*, int rightBoundary, int screenHeight);
	DirectX::XMFLOAT2 getStaffPos(long int time);

	void handleCasting(bool leftMouse, bool rightMouse, DirectX::XMFLOAT2 mouse);
	void updateCasting(DirectX::XMFLOAT2);
	void drawCasting(SpriteBatch* spriteBatch, float gameTime);

	Casting* getCasting();
	std::vector<Projectile*>* getProjectileList();
	std::vector<CollisionBox*>* getCollisionList();

	int chargeTime;
	bool fired;
	void manaBoost();

    CollisionBox* getCollisionBox();

	void update(long int, vector<Unit*>*);

protected:
	Component* g_hat;
	Component* g_head;
	Component* g_body;
	Component* g_leftfoot;
	Component* g_rightfoot;
	Component* g_lefthand;
	Component* g_righthand;
	Component* g_staff;

private:
	Casting* casting;
};