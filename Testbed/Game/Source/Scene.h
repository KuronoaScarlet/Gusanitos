#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Defs.h"


class Body
{
public:
	Body() {}
	Body(float posx,float posy,float m,float w, float h)
	{
		position = {posx, posy};
		mass = m;
		size = { w,h };
		center = { (w / 2), (h / 2) };

	}
	~Body(){}
	fPoint position;
	fPoint size;
	fPoint center;
	fPoint velocity;
	fPoint acceleration;
	float directionAngle;
	float rotationAngle;
	fPoint force;
	float mass;
	SDL_Texture* texture;
	Animation* currentAnim = nullptr;

	float Norm;
	float normR;
	float vector;

	//Forces
	fPoint gravityForce;
	fPoint aerodinamicForce;
	fPoint hydrodinamicForce;
	fPoint normalForce;
	fPoint df;

	Collider* collider = nullptr;

public:
	void Draw() 
	{
		/*SDL_Rect rectPlayer;
		rectPlayer = currentAnim->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rectPlayer);*/
	}
	

};
class Fluid
{
public:
	Fluid()
	{

	}
	~Fluid() {}	
	fPoint position;
	fPoint size;
	float volumeDisplaced;

	SDL_Texture* texture;

public:
	void Draw() 
	{

	}
};
class PhysicsEngine
{
public:
	PhysicsEngine() {}
	Body* player = new Body(100.0f, 185.0f,60,16,16);
	Body* world = new Body(250, 25000.0f, 900000000000.0f,1000.0f, 1000.0f); //490.0f, 8500.0f, 100000000000.0f,300.0f, 300.0f
	

public:
	void ForceGrav()
	{
		player->Norm = PIXEL_TO_METERS(norm(world->position, player->center));
		player->normR = (pow(player->Norm, 2));
		player->vector = PIXEL_TO_METERS((world->position.x - player->position.x));
		player->gravityForce.x = G * ((player->mass * world->mass) / player->normR) * player->vector;
		player->gravityForce.y = G * ((player->mass * world->mass) / player->normR) * PIXEL_TO_METERS((world->center.y - player->center.y));
		
	}
	void Aerodeynamics()
	{
		
	}
	void Bouyancy()
	{
		//player->hydrodinamicForce.x = 
	}
	void Springs()
	{
		
	}
	void Acceleration()
	{
		player->df.x = player->gravityForce.x + player->aerodinamicForce.x  + player->hydrodinamicForce.x + player->normalForce.x;
		player->df.y = player->gravityForce.y + player->aerodinamicForce.y  + player->hydrodinamicForce.y + player->normalForce.y;
		player->acceleration = { player->df.x / player->mass, player->df.y / player->mass };

	}
	void Integrator(float dt)
	{
		player->position.x += player->velocity.x * dt + 0.5f * (player->acceleration.x * dt * dt);
		player->position.y += player->velocity.y * dt + 0.5f * (player->acceleration.y * dt * dt);

		player->velocity.x += player->acceleration.x * dt;
		player->velocity.y += player->acceleration.y * dt;
	}
	void AddMomentum(fPoint momentum)
	{
		if (momentum.x != 0) player->velocity.x = momentum.x / player->mass;
		if (momentum.y != 0) player->velocity.y = momentum.y / player->mass;
	}
	float norm(fPoint p0, fPoint p1)
	{
		fPoint v;
		v.x = p0.x - p1.x;
		v.y	= p0.y - p1.y;
		return (float(sqrt((v.x * v.x) + (v.y * v.y))));
	}
};

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnCollision(Collider* a, Collider* b);

	bool firstEntry = true;


private:
	
	PhysicsEngine integrator;
	Animation idleAnimation;

	bool collision = false;
};


#endif // __SCENE_H__