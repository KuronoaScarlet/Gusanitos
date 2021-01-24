#include "Gun.h"

#include "App.h"
#include"Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Collider.h"
#include "Audio.h"
#include "EntityManager.h"
#include "ModulePhysics.h"

#define SPEED 1

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

	collider = collider = app->collisions->AddCollider(SDL_Rect{ (int)position.x,(int)position.y, 22, 25 }, Collider::Type::GUN, listener);

	dirVelo = { 0,0 };
	surface = 0;
	cd = 0;
	velRelative = 0;
	volume = 0;
	inWater = false;



	fPoint vOrigin = { (float)app->input->GetMouseX() - position.x, 0 };
	fPoint vDestination = { (float)app->input->GetMouseX() - position.x, (float)app->input->GetMouseY() - position.y };

	float pointProduct = (vOrigin.x * vDestination.x) + (vOrigin.y * vDestination.y);

	float originMagnitude = sqrt(pow(vOrigin.x, 2) + pow(vOrigin.y, 2));
	float destinationMagnitude = sqrt(pow(vDestination.x, 2) + pow(vDestination.y, 2));

	angle = acos(pointProduct / (originMagnitude * destinationMagnitude));

	if ((float)app->input->GetMouseX() > position.x)
	{
		speedX = SPEED * cos(angle);
	}
	else speedX = -SPEED * cos(angle);

	if ((float)app->input->GetMouseY() > position.y)
	{
		speedY = SPEED * sin(angle);
	}
	else speedY = SPEED * sin(-angle);

}

bool Gun::Start()
{
	return true;
}

bool Gun::Update(float dt)
{
	/*if (app->input->GetKey(SDL_SCANCODE_A))
	{
		velocity.x = app->entityManager->integrator->AddMomentum(fPoint{ -5,0 }, mass, velocity).x;
		velocity.y = app->entityManager->integrator->AddMomentum(fPoint{ -5,0 }, mass, velocity).y;
	}
	if (app->input->GetKey(SDL_SCANCODE_D))
	{
		velocity.x = app->entityManager->integrator->AddMomentum(fPoint{ 5,0 }, mass, velocity).x;
		velocity.y = app->entityManager->integrator->AddMomentum(fPoint{ 5,0 }, mass, velocity).y;
	}*/



	/////////////////////////////////////////PHYSICS LOGIC/////////////////////////////////////////
	//acceleration.x = app->entityManager->integrator->IntegratePhysics(position, mass, center, dirVelo, surface, cd, velRelative, volume, inWater).x;
	//acceleration.y = app->entityManager->integrator->IntegratePhysics(position, mass, center, dirVelo, surface, cd, velRelative, volume, inWater).y;

	//position.x = app->entityManager->integrator->Integrator(dt, &position, &velocity, &acceleration).x;
	//position.y = app->entityManager->integrator->Integrator(dt, &position, &velocity, &acceleration).y;
	position.x += speedX;
	position.y += speedY;

	/*if ((float)app->input->GetMouseX() > position.x)
	{
		speedX = SPEED * cos(angle);
	}
	else speedX = -SPEED * cos(angle);

	if ((float)app->input->GetMouseY() > position.y)
	{
		speedY = SPEED * sin(angle);
	}
	else speedY = SPEED * sin(-angle);*/

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

	}
	if (coll->type == Collider::Type::WATER)
	{

	}
}

void Gun::CleanUp()
{

}