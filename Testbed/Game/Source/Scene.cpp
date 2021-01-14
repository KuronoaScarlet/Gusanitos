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
	
	
	player.texture = app->tex->Load("Assets/Textures/player.png");
	player.position = { 10,10 };
	player.mass = 10;
	player.acceleration = { 0, 0 };
	player.force = { 0, 0};
	player.velocity = { 0, 0 };

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
	if (player.position.y < 180)
	{
		player.currentAnim = &idleAnimation;
		//player.force.y = integrator.ForceGrav(player.mass, -9.8f);

		player.position.x = player.velocity.x * dt;
		player.acceleration.y += -9.8;
		player.velocity.y += player.acceleration.y;
		player.position.y = -(player.velocity.y * dt - 0.5 * player.acceleration.y * dt * (dt-0.016f));
	}
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	ret = false;
	SDL_Rect rectPlayer;
	rectPlayer = player.currentAnim->GetCurrentFrame();
	app->render->DrawTexture(player.texture, player.position.x, player.position.y, &rectPlayer);

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