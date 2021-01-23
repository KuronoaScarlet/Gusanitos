#include "ModulePhysics.h"

#include "App.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

fPoint PhysicsEngine::IntegratePhysics(fPoint position, float mass, fPoint massCentre, fPoint dirVelo, float surface, float cd, float velRelative, float volume, bool inWater)
{
	ForceGrav(position, mass, massCentre);
	Aerodeynamics(dirVelo, surface, cd, velRelative);
	if (inWater) Bouyancy(position, mass, volume);
	else Bouyancy({ 0,0 }, 0, 0);
	return Acceleration(mass);
}

void PhysicsEngine::ForceGrav(fPoint position, float mass, fPoint massCentre)
{
	norm = Normalize(world->position, massCentre);
	normR = (pow(norm, 2));
	gravityForce.x = 0;
	gravityForce.y = G * (( world->mass) / normR) * (world->centre.y - massCentre.y);
	//4.05*10^13
}
void PhysicsEngine::Aerodeynamics(fPoint dirVelo, float surface, float cd, float velRelative)
{
	if (dirVelo != fPoint{ 0,0 })
	{
		dirVelo.x = dirVelo.x / (sqrt((dirVelo.x * dirVelo.x) + (dirVelo.y * dirVelo.y)));
		dirVelo.y = dirVelo.y / (sqrt((dirVelo.x * dirVelo.x) + (dirVelo.y * dirVelo.y)));
	}
	float moduleAF;
	moduleAF = 0.5 * AIR_DENSITY * pow(velRelative, 2) * surface * cd;
	aerodinamicForce.x = -moduleAF * dirVelo.x;
	aerodinamicForce.y = -moduleAF * dirVelo.y;
}
fPoint PhysicsEngine::Acceleration(float mass)
{
	df.x = gravityForce.x + aerodinamicForce.x + hydrodinamicForce.x + normalForce.x;
	df.y = gravityForce.y + aerodinamicForce.y + hydrodinamicForce.y + normalForce.y;
	return { df.x / mass, df.y / mass };
}
void PhysicsEngine::Bouyancy(fPoint position, float mass, float volume)
{
	hydrodinamicForce.x = 0;
	hydrodinamicForce.y = WATER_DENSITY * gravityForce.y * volume ;
}

fPoint PhysicsEngine::Integrator(float dt, fPoint* position, fPoint* velocity, fPoint* acceleration)
{

	position->x += velocity->x * dt + 0.5f * (acceleration->x * dt * dt);
	position->y += velocity->y * dt + 0.5f * (acceleration->y * dt * dt);

	velocity->x += acceleration->x * dt;
	velocity->y += acceleration->y * dt;

	return (*position);
}

fPoint PhysicsEngine::AddMomentum(fPoint momentum, float mass, fPoint velocity)
{
	fPoint vel;
	if (momentum.x != 0) vel.x = momentum.x / mass;
	if (momentum.y != 0) vel.y = momentum.y / mass;

	velocity.x = velocity.x + vel.x;
	velocity.y = velocity.y + vel.y;

	return velocity;
}

void PhysicsEngine::AddForce(fPoint force)
{
	//if (force.x != 0) player->acceleration.x = force.x / player->mass;
	//if (force.y != 0) player->acceleration.y = force.y / player->mass;
}

float PhysicsEngine::Normalize(fPoint p0, fPoint p1)
{
	fPoint v;
	v.x = p0.x - p1.x;
	v.y = p0.y - p1.y;
	return (float(sqrt((v.x * v.x) + (v.y * v.y))));
}