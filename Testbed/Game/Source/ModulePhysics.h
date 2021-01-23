#ifndef _MODULEPHYSICS_H_
#define _MODULEPHYSICS_H_

#include "Module.h"
#include "Point.h"
#include "Defs.h"


class PhysicsEngine
{
public:
	PhysicsEngine() {}
	~PhysicsEngine() {}

	fPoint IntegratePhysics(fPoint position, float mass, fPoint massCentre, fPoint dirVelo, float surface, float cd, float velRelative, float volume, bool inWater);

	fPoint AddMomentum(fPoint momentum, float mass, fPoint velocity);

	void AddForce(fPoint force);

	fPoint Integrator(float dt, fPoint* position, fPoint* velocity, fPoint* acceleration);

	//Forces
	fPoint gravityForce;
	fPoint aerodinamicForce;
	fPoint hydrodinamicForce;
	fPoint normalForce;
	fPoint df;

private:
	void ForceGrav(fPoint position, float mass, fPoint massCentre);

	void Aerodeynamics(fPoint dirVelo, float surface, float cd, float velRelative);

	void Bouyancy(fPoint position, float mass, float volume);

	void Springs();

	fPoint Acceleration(float mass);

	float Normalize(fPoint p0, fPoint p1);

	float norm;
	float normR;
	float vector;

public:
	class World 
	{
	public:
		World() {}
		World(fPoint pos, float m, float w, float h)
		{
			position.x = pos.x;
			position.y = pos.y;
			mass = m;
			weight = w;
			height = h;
			size = { weight, height };
			centre = { size.x / 2, size.y / 2 };
		}
		~World() {}

		fPoint position;

		float weight;
		float height;
		float mass;
		fPoint size;
		fPoint centre;
	};

	World* world = new World({ 250.0f, 63000000.0f }, (5.94f * pow(10,24)), 6300.0f, 6300.0f);
};

#endif