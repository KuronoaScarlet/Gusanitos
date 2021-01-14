#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"


class body
{
public:
	fPoint position;
	fPoint velocity;
	fPoint acceleration;
	fPoint force;
	float mass;
	SDL_Texture* texture;
	Animation* currentAnim = nullptr;

public:
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
	

public:
	float ForceGrav(float mass, float acc)
	{
		return mass * acc;;;;;;;;;;;;;;
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
	body player;
	PhysicsEngine integrator;
	Animation idleAnimation;
};


#endif // __SCENE_H__