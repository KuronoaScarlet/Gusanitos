#include "Player.h"

#include "App.h"
#include"Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Collider.h"
#include "Audio.h"
#include "EntityManager.h"
#include "ModulePhysics.h"
#include "Scene.h"

Player::Player(Module* listener, fPoint position, float mass, float weight, float height, SDL_Texture* texture, Type type) : Body(listener, position, mass, weight, height, texture, type)
{
	leftAnimation.loop = true;
	leftAnimation.PushBack({ 21, 17, 21, 26 });
	leftAnimation.PushBack({ 20, 75, 22, 26 });
	leftAnimation.PushBack({ 20, 135, 22, 26 });
	leftAnimation.PushBack({ 20, 195, 22, 26 });
	leftAnimation.PushBack({ 20, 256, 22, 25 });
	leftAnimation.PushBack({ 20, 316, 22, 25 });
	leftAnimation.PushBack({ 20, 376, 22, 25 });
	leftAnimation.PushBack({ 20, 436, 22, 25 });
	leftAnimation.PushBack({ 20, 496, 22, 25 });
	leftAnimation.PushBack({ 20, 556, 22, 25 });
	leftAnimation.PushBack({ 20, 616, 22, 25 });
	leftAnimation.PushBack({ 20, 676, 22, 25 });
	leftAnimation.PushBack({ 20, 736, 22, 25 });
	leftAnimation.PushBack({ 20, 796, 22, 25 });
	leftAnimation.PushBack({ 20, 856, 22, 26 });
	leftAnimation.PushBack({ 20, 916, 22, 26 });
	leftAnimation.PushBack({ 20, 976, 22, 25 });
	leftAnimation.PushBack({ 19, 1037, 23, 25 });
	leftAnimation.PushBack({ 20, 976, 22, 25 });
	leftAnimation.PushBack({ 20, 915, 22, 26 });
	leftAnimation.PushBack({ 20, 855, 22, 26 });
	leftAnimation.PushBack({ 20, 796, 22, 25 });
	leftAnimation.PushBack({ 20, 736, 22, 25 });
	leftAnimation.PushBack({ 20, 676, 22, 25 });
	leftAnimation.PushBack({ 20, 616, 22, 25 });
	leftAnimation.PushBack({ 20, 556, 22, 25 });
	leftAnimation.PushBack({ 20, 496, 22, 25 });
	leftAnimation.PushBack({ 20, 376, 22, 25 });
	leftAnimation.PushBack({ 20, 316, 22, 25 });
	leftAnimation.PushBack({ 20, 256, 22, 25 });
	leftAnimation.PushBack({ 20, 195, 22, 26 });
	leftAnimation.PushBack({ 20, 135, 22, 26 });
	leftAnimation.PushBack({ 20, 75, 22, 26 });
	leftAnimation.PushBack({ 21, 16, 21, 25 });
	leftAnimation.speed = 0.1f;

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

	collider = collider = app->collisions->AddCollider(SDL_Rect{ (int)position.x,(int)position.y, 22, 25 }, Collider::Type::PLAYER, listener);

	dirVelo = { 1.0f,0 };
	surface = 1;
	cd = 1;
	velRelative = 20;
	volume = 0;
	inWater = false;

	/**/
	
	dirVelo = { 0.0f,0 };
	surface = 0;
	cd = 0;
	velRelative = 0;
	volume = 0;
	inWater = false;
	isJumping = false;
}

bool Player::Start()
{
	return true;
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		currentAnimation = &leftAnimation;
		velocity.x = app->entityManager->integrator->AddMomentum(fPoint{ -5000,0 }, mass, velocity).x;
		//velocity.y = app->entityManager->integrator->AddMomentum(fPoint{ -5,0 }, mass, velocity).y;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		currentAnimation = &rightAnimation;
		velocity.x = app->entityManager->integrator->AddMomentum(fPoint{ 5000,0 }, mass, velocity).x;
		//velocity.y = app->entityManager->integrator->AddMomentum(fPoint{ 5,0 }, mass, velocity).y;
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && isJumping == false)
	{
		currentAnimation = &rightAnimation;
		velocity.y = app->entityManager->integrator->AddMomentum(fPoint{ 0,-4000 }, mass, velocity).y;
		isJumping = true;
	}

	/////////////////////////////////////////PHYSICS LOGIC/////////////////////////////////////////
	acceleration.x = app->entityManager->integrator->IntegratePhysics(position, mass, center, dirVelo, surface, cd, velRelative, volume, inWater).x;
	acceleration.y = app->entityManager->integrator->IntegratePhysics(position, mass, center, dirVelo, surface, cd, velRelative, volume, inWater).y;

	position.x = app->entityManager->integrator->Integrator(dt, &position, &velocity, &acceleration).x;
	position.y = app->entityManager->integrator->Integrator(dt, &position, &velocity, &acceleration).y;
	
	volume = PIXELS_TO_METERS((position.y + collider->rect.h) - 200);

	if (volume <= 0)
	{
		inWater = false;
	}
	velocity.x = 0;
	collider->SetPos(position.x, position.y);
	currentAnimation->Update();

	return true;
}

bool Player::Draw()
{
	SDL_Rect playerRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &playerRect);

	app->render->DrawTexture(app->scene->waterBckGrnd, 183, 200, NULL);

	return true;
}

void Player::Collision(Collider* coll)
{
	if (coll->type == Collider::Type::FLOOR)
	{
		velocity = { velocity.x , 0 };
		app->entityManager->integrator->normalForce.y = -app->entityManager->integrator->gravityForce.y;
		isJumping = false;

		//position.y = coll->rect.y - coll->rect.h + 25;
	}
	if (coll->type == Collider::Type::WATER)
	{
		inWater = true;
		//volume = (position.y + collider->rect.h) - coll->rect.y;
	}
	if (coll->type == Collider::Type::AIR)
	{
		inWater = false;
		app->entityManager->integrator->normalForce.x = 0;
		app->entityManager->integrator->normalForce.y = 0;
	}
	if (coll->type == Collider::Type::LEVEL2)
	{
		dirVelo = { -1.0f,0 };
		surface = 20;
		cd = 20;
		velRelative = 20;
		volume = 0;
		inWater = false;
	}
	if (coll->type == Collider::Type::LEVEL3)
	{
		dirVelo = { 0, 1.0 };
		surface = 19;
		cd = 4;
		velRelative = 10;
		volume = 0;
		inWater = false;
	}

}

void Player::CleanUp()
{

}