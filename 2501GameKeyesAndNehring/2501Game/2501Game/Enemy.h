#pragma once

#include "Unit.h"
#include "Sprite.h"
#include "Component.h"
#include "KeyInput.h"
#include "Casting.h"
#include <vector>

class Enemy : public Unit
{
public:
	friend Unit;
	Enemy();
	Enemy(Sprite*, Sprite*, Sprite*, Sprite*, Sprite*, vector<Sprite*>*);

	void draw(SpriteBatch*, long int);
	void move(KeyInput*);
	void moveEnemy(float);
	void initLocation(float x, float y);

	int health;
	int specialManaCost;

	CollisionBox* getCollisionBox();
	void update(long int, vector<Unit*>*);

	void handleCasting(DirectX::XMFLOAT2);
	void updateCasting(DirectX::XMFLOAT2);
	void drawCasting(SpriteBatch* spriteBatch, float gameTime);

	Casting* getCasting();
	std::vector<Projectile*>* getProjectileList();
	std::vector<CollisionBox*>* getCollisionList();

	int chargeTime, randomModifier;
	bool fired;
	void manaBoost();

protected:
	Component* g_head;
	Component* g_body;
	Component* g_leftfoot;
	Component* g_rightfoot;
	Component* g_lefthand;
	Component* g_righthand;
	Component* g_staff;

	XMFLOAT2 waypoint;
	float angle;

private:
	Casting* casting;
};

