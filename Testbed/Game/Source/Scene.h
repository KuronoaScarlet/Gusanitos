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
		center = { w / 2, h / 2 };

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
	fPoint gravity;

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
	Body* player = new Body(20.0f,20.0f,60,30,30);
	Body* world = new Body(0, 200, 5000000,30000,30000);
	

public:
	void ForceGrav()
	{
		player->gravity.x = G * ((player->mass * world->mass) / (pow(norm(world->center, player->center), 2))) * (world->center.x - player->center.x);
		player->gravity.y = G * ((player->mass * world->mass) / (pow(norm(world->center, player->center), 2))) * (world->center.y - player->center.y);
		
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