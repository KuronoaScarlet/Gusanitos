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
	integrator.player->idleAnimation.loop = true;
	integrator.player->idleAnimation.PushBack({ 21, 18, 21, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 77, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 20, 137, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 20, 197, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 20, 258, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 318, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 378, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 438, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 498, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 558, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 618, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 678, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 738, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 798, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 857, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 20, 917, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 20, 978, 22, 25 });

	integrator.player->idleAnimation.PushBack({ 19, 1039, 23, 25 });

	integrator.player->idleAnimation.PushBack({ 20, 978, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 917, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 20, 857, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 20, 798, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 738, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 678, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 618, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 558, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 498, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 378, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 318, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 258, 22, 25 });
	integrator.player->idleAnimation.PushBack({ 20, 197, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 20, 137, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 20, 77, 22, 26 });
	integrator.player->idleAnimation.PushBack({ 21, 18, 21, 25 });
	integrator.player->idleAnimation.speed = 0.1f;
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
	
	bckground = app->tex->Load("Assets/Textures/background.png");

	app->collisions->active = true;

	app->collisions->AddCollider({ 0,200,1000,50 }, Collider::Type::FLOOR, this);

	app->collisions->AddCollider({ 0,0,1000,200 }, Collider::Type::AIR, this);

	integrator.player->collider = app->collisions->AddCollider(SDL_Rect{ (int)integrator.player->position.x,(int)integrator.player->position.y, 22, 25 }, Collider::Type::PLAYER, this);


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
	integrator.player->collider->SetPos((int)integrator.player->position.x, (int)integrator.player->position.y);
	///////////////////////PLAYER//////////////////////////
	integrator.player->currentAnim = &integrator.player->idleAnimation;
	integrator.player->center = { integrator.player->position.x + (integrator.player->size.x/2), integrator.player->position.y + (integrator.player->size.y / 2) };

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		integrator.AddMomentum({ -3000,0 });
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		integrator.AddMomentum({ 3000,0 });
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && integrator.player->jumping == false)
	{
		integrator.AddMomentum({ 0,-5000 });
		integrator.player->jumping = true;
	}
	integrator.player->currentAnim->Update();
	/////////////////////////TOTAL FORCES////////////////////////////////////
	integrator.ForceGrav();
			
	integrator.Acceleration();

	integrator.Integrator(dt);

	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	ret = false;
	
	app->render->DrawTexture(bckground, 0, -40, NULL);
	
	SDL_Rect rectPlayer;
	rectPlayer = integrator.player->currentAnim->GetCurrentFrame();
	app->render->DrawTexture(integrator.player->texture, integrator.player->position.x, integrator.player->position.y, &rectPlayer);

	
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) app->collisions->debug = !app->collisions->debug;

	return ret;
}
void Scene::OnCollision(Collider* a, Collider* b)
{
	if (a->type == Collider::Type::FLOOR && b->type == Collider::Type::PLAYER)
	{
		integrator.player->velocity = { 0,0 };
		integrator.player->normalForce.x = -integrator.player->gravityForce.x;
		integrator.player->normalForce.y = -integrator.player->gravityForce.y;
		integrator.player->jumping = false;

		integrator.player->position.y = a->rect.y - a->rect.h + 25;
	}
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