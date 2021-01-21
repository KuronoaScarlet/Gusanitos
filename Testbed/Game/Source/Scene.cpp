#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "Collisions.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 0, 12, 11 });
}

// Destructor
Scene::~Scene()
{
}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	
	integrator.player->texture = app->tex->Load("Assets/Textures/player.png");
	

	app->collisions->active = true;

	app->collisions->AddCollider({ 0, 300, 1280, 30 }, Collider::Type::FLOOR, this);

	firstEntry = false;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (integrator.player->position.y <= 186)
	{
		integrator.player->currentAnim = &idleAnimation;


		integrator.ForceGrav();

		integrator.player->position.y += integrator.player->gravity.y;


	}
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	ret = false;
	
	integrator.player->Draw();
	

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	if (!active)return true;

	app->player->CleanUp();
	app->collisions->CleanUp();
	
	app->player->scene1 = false;

	app->scene->active = false;

	LOG("Freeing scene");
	return true;
}