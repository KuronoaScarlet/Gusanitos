#include "Gun.h"

#include "App.h"
#include"Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Collider.h"
#include "Audio.h"
#include "EntityManager.h"
#include "ModulePhysics.h"
#include "Log.h"

Gun::Gun(Module* listener, fPoint position, float mass, float weight, float height, SDL_Texture* texture, Type type) : Body(listener, position, mass, weight, height, texture, type)
{

	rightAnimation.loop = true;
	rightAnimation.PushBack({ 0, 0, 16, 23 });
	rightAnimation.speed = 0.1f;

	currentAnimation = &rightAnimation;

	collider = collider = app->collisions->AddCollider(SDL_Rect{ (int)position.x,(int)position.y, 16, 16 }, Collider::Type::GUN, listener);

	dirVelo = { 1.0f,0 };
	surface = 1;
	cd = 1;
	velRelative = 20;
	volume = 0;
	inWater = false;

	app->input->GetMousePosition(x, y);	
	app->render->ScreenToWorld(x,y);

	vDestination = { x - (float)50, y - (float)190};
	modDestination = sqrt(pow(vDestination.x, 2) + pow(vDestination.y, 2));
	normDestination = { vDestination.x / modDestination, vDestination.y / modDestination }; 

	velocity.x = app->entityManager->bulletIntegrator->AddMomentum(fPoint{ normDestination.x * 4000, normDestination.y * 4000 }, mass, velocity).x;
	velocity.y = app->entityManager->bulletIntegrator->AddMomentum(fPoint{ normDestination.x * 4000, normDestination.y * 4000 }, mass, velocity).y;

}

bool Gun::Start()
{
	return true;
}

bool Gun::Update(float dt)
{
	/////////////////////////////////////////PHYSICS LOGIC/////////////////////////////////////////
	acceleration.x = app->entityManager->bulletIntegrator->IntegratePhysics(position, mass, center, dirVelo, surface, cd, velRelative, volume, inWater).x;
	acceleration.y = app->entityManager->bulletIntegrator->IntegratePhysics(position, mass, center, dirVelo, surface, cd, velRelative, volume, inWater).y;

	position.x = app->entityManager->bulletIntegrator->Integrator(dt, &position, &velocity, &acceleration).x;
	position.y = app->entityManager->bulletIntegrator->Integrator(dt, &position, &velocity, &acceleration).y;

	if (volume <= 0) inWater = false;

	if (position.x > 0 && position.x <= 509)
	{
		dirVelo = { -1.0f,0 };
		surface = 2;
		cd = 0;
		velRelative = 0;
	}
	if (position.x > 509 && position.x < 974)
	{
		dirVelo = { -1.0f,0 };
		surface = 2;
		cd = 20;
		velRelative = 12;
	}
	if (position.x > 974 && position.x < 1600)
	{
		dirVelo = { 0, 1.0 };
		surface = 4;
		cd = 20;
		velRelative = 12;
	}

	collider->SetPos(position.x, position.y);
	currentAnimation->Update();

	return true;
}

bool Gun::Draw()
{
	SDL_Rect GunRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &GunRect);

	return true;
}

void Gun::Collision(Collider* coll)
{
	if (coll->type == Collider::Type::FLOOR)
	{
		pendingToDelete = true;
		collider->pendingToDelete = true;
	}
	if (coll->type == Collider::Type::WATER)
	{
		inWater = true;
	}
	if (coll->type == Collider::Type::TARGET)
	{
		pendingToDelete = true;
		collider->pendingToDelete = true;
		
	}
}

void Gun::CleanUp()
{

}