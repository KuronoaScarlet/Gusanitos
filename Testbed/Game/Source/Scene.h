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
		center = { posx + (w / 2), posy + (h / 2) };

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

	//Forces
	fPoint gravityForce;
	fPoint aerodinamicForce;
	fPoint hydrodinamicForce;
	fPoint normalForce;
	fPoint df;

public:
	void Draw() 
	{
		SDL_Rect rectPlayer;
		rectPlayer = currentAnim->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rectPlayer);
	}
	void AddForce(fPoint df)
	{
		force += df;
	}
	void AddMomentum(fPoint dv)
	{
		velocity += dv;
	}

};

class PhysicsEngine
{
public:
	PhysicsEngine() {}
	Body* player = new Body(200.0f, 0.0f,60,16,16);
	Body* world = new Body(490.0f, 8000.0f, 100000000.0f,300.0f, 300.0f);
	

public:
	void ForceGrav()
	{
		player->gravityForce.x = G * ((player->mass * world->mass) / PIXEL_TO_METERS((pow(norm(world->center, player->center), 2)))) * PIXEL_TO_METERS((world->center.x - player->center.x));
		player->gravityForce.y = G * ((player->mass * world->mass) / PIXEL_TO_METERS((pow(norm(world->center, player->center), 2)))) * PIXEL_TO_METERS((world->center.y - player->center.y));
		
	}
	void Acceleration()
	{
		player->df.x = player->gravityForce.x + player->aerodinamicForce.x  + player->hydrodinamicForce.x + player->normalForce.x;
		player->df.y = player->gravityForce.y + player->aerodinamicForce.y  + player->hydrodinamicForce.y + player->normalForce.y;
		player->acceleration = { player->df.x / player->mass, player->df.y / player->mass };

	}

	float norm(fPoint p0, fPoint p1)
	{
		fPoint v{ (p0.x - p1.x),(p0.y - p1.y) };
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

		bool firstEntry = true;


private:
	
	PhysicsEngine integrator;
	Animation idleAnimation;
};


#endif // __SCENE_H__