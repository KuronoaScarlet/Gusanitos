#include "ModulePhysics.h"

#include "App.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

fPoint PhysicsEngine::IntegratePhysics(fPoint position, float mass, fPoint massCentre)
{
	ForceGrav(position, mass, massCentre);
	return Acceleration(mass);
}

void PhysicsEngine::ForceGrav(fPoint position, float mass, fPoint massCentre)
{
	norm = PIXEL_TO_METERS(Normalize(world->position, massCentre));
	normR = (pow(norm, 2));
	vector = PIXEL_TO_METERS((world->position.x - position.x));
	gravityForce.x = G * ((mass * world->mass) / normR) * vector;
	gravityForce.y = G * ((mass * world->mass) / normR) * PIXEL_TO_METERS((world->centre.y - massCentre.y));
}

fPoint PhysicsEngine::Acceleration(float mass)
{
	df.x = gravityForce.x + aerodinamicForce.x + hydrodinamicForce.x + normalForce.x;
	df.y = gravityForce.y + aerodinamicForce.y + hydrodinamicForce.y + normalForce.y;
	return { df.x / mass, df.y / mass };
}

fPoint PhysicsEngine::Integrator(float dt, fPoint* position, fPoint* velocity, fPoint* acceleration)
{
	position->x += velocity->x * dt + 0.5f * (acceleration->x * dt * dt);
	position->y += velocity->y * dt + 0.5f * (acceleration->y * dt * dt);

	velocity->x += acceleration->x * dt;
	velocity->y += acceleration->y * dt;

	return (*position);
}

void PhysicsEngine::AddMomentum(fPoint momentum)
{
	/*if (momentum.x != 0) player->velocity.x = momentum.x / player->mass;
	if (momentum.y != 0) player->velocity.y = momentum.y / player->mass;*/
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