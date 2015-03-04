#include "Casting.h"

using namespace DirectX;

Casting::Casting(std::vector<Sprite*>* projectiles)
{
	p_Sprites = projectiles;
}

Casting::~Casting()
{
}

void Casting::specialCast(XMFLOAT2 player, XMFLOAT2 mouse, int elementSelection)
{
	Projectile* p_NewProjectile = new Projectile((*p_Sprites)[elementSelection + 1], player.x, player.y, elementSelection);
	p_NewProjectile->setAngle(calcAngle(player, mouse));
	projectileList.push_back(p_NewProjectile);
}

void Casting::basicCast(XMFLOAT2 player, XMFLOAT2 mouse)
{
	Projectile* p_NewProjectile = new Projectile((*p_Sprites)[0], player.x, player.y, -1);
	p_NewProjectile->setAngle(calcAngle(player, mouse));
	projectileList.push_back(p_NewProjectile);
}

void Casting::updateProjectiles(XMFLOAT2 screenSize) // have a look at the HUD class about drawing sprites...
{
	for(int i = 0; i < projectileList.size(); i++) {
		float xPos = projectileList[i]->getXPosition();
		xPos += cos(projectileList[i]->getAngle()) * projectileList[i]->getSpeed();
		float yPos = projectileList[i]->getYPosition();
		yPos += sin(projectileList[i]->getAngle()) * projectileList[i]->getSpeed();
		projectileList[i]->setPos(xPos, yPos);
		if( projectileList[i]->getXPosition() + projectileList[i]->getWidth() < 0 ||
		    projectileList[i]->getXPosition() > screenSize.x ||
		    projectileList[i]->getYPosition() + projectileList[i]->getHeight() < 0 ||
			projectileList[i]->getYPosition() > screenSize.y ) {
			deleteProjectile(i);
			i--;
		}
	}
}

void Casting::drawProjectiles(SpriteBatch* spriteBatch, float gameTime)
{
	for(int i = 0; i < projectileList.size(); i++) {
		projectileList[i]->draw(spriteBatch, gameTime);
	}
	
}

float Casting::calcAngle(XMFLOAT2 p, XMFLOAT2 m)
{
	return atan2(m.y - p.y, m.x - p.x);
}

std::vector<Projectile*>* Casting::getProjectileList()
{
	return &projectileList;
}

std::vector<CollisionBox*>* Casting::getCollisionList()
{
	return &collisionList;
}

void Casting::deleteProjectile(int i)
{
	projectileList.erase(projectileList.begin() + i);
}