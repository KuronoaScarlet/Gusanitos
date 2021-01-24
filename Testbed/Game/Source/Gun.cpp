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

#define SPEED 5

Gun::Gun(Module* listener, fPoint position, float mass, float weight, float height, SDL_Texture* texture, Type type) : Body(listener, position, mass, weight, height, texture, type)
{

	rightAnimation.loop = true;
	rightAnimation.PushBack({ 61, 17, 21, 26 });
	rightAnimation.PushBack({ 61, 75, 22, 26 });
	rightAnimation.PushBack({ 61, 135, 22, 26 });
	rightAnimation.PushBack({ 61, 195, 22, 26 });
	rightAnimation.PushBack({ 61, 256, 22, 25 });
	rightAnimation.PushBack({ 61, 316, 22, 25 });
	rightAnimation.PushBack({ 61, 376, 22, 25 });
	rightAnimation.PushBack({ 61, 436, 22, 25 });
	rightAnimation.PushBack({ 61, 496, 22, 25 });
	rightAnimation.PushBack({ 61, 556, 22, 25 });
	rightAnimation.PushBack({ 61, 616, 22, 25 });
	rightAnimation.PushBack({ 61, 676, 22, 25 });
	rightAnimation.PushBack({ 61, 736, 22, 25 });
	rightAnimation.PushBack({ 61, 796, 22, 25 });
	rightAnimation.PushBack({ 61, 856, 22, 26 });
	rightAnimation.PushBack({ 61, 916, 22, 26 });
	rightAnimation.PushBack({ 61, 976, 22, 25 });
	rightAnimation.PushBack({ 61, 1037, 23, 25 });
	rightAnimation.PushBack({ 61, 976, 22, 25 });
	rightAnimation.PushBack({ 61, 915, 22, 26 });
	rightAnimation.PushBack({ 61, 855, 22, 26 });
	rightAnimation.PushBack({ 61, 796, 22, 25 });
	rightAnimation.PushBack({ 61, 736, 22, 25 });
	rightAnimation.PushBack({ 61, 676, 22, 25 });
	rightAnimation.PushBack({ 61, 616, 22, 25 });
	rightAnimation.PushBack({ 61, 556, 22, 25 });
	rightAnimation.PushBack({ 61, 496, 22, 25 });
	rightAnimation.PushBack({ 61, 376, 22, 25 });
	rightAnimation.PushBack({ 61, 316, 22, 25 });
	rightAnimation.PushBack({ 61, 256, 22, 25 });
	rightAnimation.PushBack({ 61, 195, 22, 26 });
	rightAnimation.PushBack({ 61, 135, 22, 26 });
	rightAnimation.PushBack({ 61, 75, 22, 26 });
	rightAnimation.PushBack({ 61, 16, 21, 25 });
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

	vDestination = { x - (float)50, y - (float)170};
	modDestination = sqrt(pow(vDestination.x, 2) + pow(vDestination.y, 2));
	normDestination = { vDestination.x / modDestination, vDestination.y / modDestination }; 

	velocity.x = app->entityManager->integrator->AddMomentum(fPoint{ normDestination.x * 4000, normDestination.y * 4000 }, mass, velocity).x;
	velocity.y = app->entityManager->integrator->AddMomentum(fPoint{ normDestination.x * 4000, normDestination.y * 4000 }, mass, velocity).y;

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

	}
}

void Gun::CleanUp()
{

}