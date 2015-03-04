#pragma once

#include "Projectile.h"
#include "KeyInput.h"
#include "Sprite.h"
#include <vector>

class Casting
{
public:
	Casting(std::vector<Sprite*>*);
	~Casting();

	void  specialCast(DirectX::XMFLOAT2 player, DirectX::XMFLOAT2 mouse, int selectedElement); // cast the left mouse button ability
	void  basicCast(DirectX::XMFLOAT2 player, DirectX::XMFLOAT2 mouse); // cast the right mouse button ability
	void  updateProjectiles(DirectX::XMFLOAT2 screenSize); // update the positions of the projectiles and draw them
	float calcAngle(DirectX::XMFLOAT2 player, DirectX::XMFLOAT2 mouse); // calculate the angle between the player position and mouse position
	std::vector<Projectile*>* getProjectileList();
	std::vector<CollisionBox*>* getCollisionList();
	void deleteProjectile(int i);
	void drawProjectiles(SpriteBatch* spriteBatch, float gameTime);

private:
	std::vector<Projectile*> projectileList;
	std::vector<CollisionBox*> collisionList;
	std::vector<Sprite*>* p_Sprites;
};

