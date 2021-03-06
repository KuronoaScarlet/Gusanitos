#include "Target.h"

#include "App.h"
#include"Input.h"
#include "Render.h"
#include "Collisions.h"
#include "Collider.h"
#include "Audio.h"
#include "EntityManager.h"
#include "ModulePhysics.h"
#include "Scene.h"

Target::Target(Module* listener, fPoint position, float mass, float weight, float height, SDL_Texture* texture, Type type) : Body(listener, position, mass, weight, height, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 0, 20, 20 });
	currentAnimation = &idleAnimation;

	collider = collider = app->collisions->AddCollider(SDL_Rect{ (int)position.x,(int)position.y, 20, 20 }, Collider::Type::TARGET, listener);

	dirVelo = { 0.0f,0 };
	surface = 0;
	cd = 0;
	velRelative = 0;
	volume = 0;
	inWater = false;
}

bool Target::Start()
{
	return true;
}

bool Target::Update(float dt)
{
	return true;
}

bool Target::Draw()
{
	SDL_Rect playerRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &playerRect);

	return true;
}

void Target::Collision(Collider* coll)
{
	if (coll->type == Collider::Type::GUN)
	{
		pendingToDelete = true;
		collider->pendingToDelete = true;
		app->entityManager->score += 50;
	}
}

void Target::CleanUp()
{

}