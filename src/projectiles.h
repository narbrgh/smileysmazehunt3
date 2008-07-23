#ifndef _PROJECTILES_H_
#define _PROJECTILES_H_

#include <list>
#include "hge.h" //For HTEXTURE

class CollisionCircle;
class hgeParticleSystem;
class hgeRect;
class hgeSprite;

//Projectile Types
#define NUM_PROJECTILES 9
#define PROJECTILE_1 0
#define PROJECTILE_FRISBEE 1
#define PROJECTILE_LIGHTNING_ORB 2
#define PROJECTILE_CACTUS_SPIKE 3
#define PROJECTILE_PENGUIN_FISH 4
#define PROJECTILE_FIREBALL 5
#define PROJECTILE_TURRET_CANNONBALL 6
#define PROJECTILE_MINI_MUSHROOM 7
#define PROJECTILE_LASER 8

#define LIGHTNING_ORB_SPEED 650.0

struct Projectile {

	//Generic shit
	float x, y, dx, dy, speed, angle;
	float damage;
	int gridX, gridY;
	bool changedGridSquare;
	int id;					//ID used to choose graphic and shit
	bool hostile;			//If this is an enemy bullet
	hgeRect *collisionBox;
	hgeRect *terrainCollisionBox;
	hgeParticleSystem *particle;
	bool makesSmileyFlash;
	float timeReflected;

	//Frisbee shit
	float frisbeeAngle;

	//Orb shit
	bool waitingToReflect;
	float reflectX, reflectY;	//(x,y) that the orb will reflect at
	int reflectDirection;		//what direction to go once it reflects
	int facing;

};

struct ProjectileType {
	hgeSprite *sprite;
	int radius;
};

class ProjectileManager {

public:
	ProjectileManager();
	~ProjectileManager();

	//methods
	void draw(float dt);
	void update(float dt);
	void addProjectile(float x, float y, float speed, float angle, float damage, bool hostile, int id, bool makesSmileyFlash);
	void reset();
	void initProjectiles();
	void reflectProjectile(std::list<Projectile>::iterator projectile);
	bool reflectProjectilesInBox(hgeRect *collisionBox, int type);
	bool reflectProjectilesInCircle(float x, float y, float radius, int type);
	bool frisbeeActive();
	int killProjectilesInBox(hgeRect *collisionBox, int type);
	int killProjectilesInBox(hgeRect *collisionBox, int type, bool killHostile, bool killNonhostile);
	int killProjectilesInCircle(float x, float y, float radius, int type);

	ProjectileType projectileTypes[NUM_PROJECTILES];
	std::list<Projectile> theProjectiles;
	bool canPass[256];
	HTEXTURE projectileTexture;

};

#endif