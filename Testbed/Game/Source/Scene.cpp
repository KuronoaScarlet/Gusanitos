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

	//app->collisions->AddCollider({ 0, 300, 1280, 30 }, Collider::Type::FLOOR, this);

	app->collisions->AddCollider({ 0,200,1000,100 }, Collider::Type::FLOOR, this);

	app->collisions->AddCollider({ 0,0,1000,200 }, Collider::Type::AIR, this);

	integrator.player->collider = app->collisions->AddCollider(SDL_Rect{ (int)integrator.player->position.x,(int)integrator.player->position.y,12,11 }, Collider::Type::PLAYER, this);


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
	if (integrator.player->position.y <= 1800.0f)
	{
		/////////////////////PLAYER//////////////////////////
		integrator.player->currentAnim = &idleAnimation;
		integrator.player->center = { integrator.player->position.x + (integrator.player->size.x/2), integrator.player->position.y + (integrator.player->size.y / 2) };

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			integrator.player->position += {-1, 0};

		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			integrator.player->position += {1, 0};
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
		{
			
		}
		
		///////////////////TOTAL FORCES////////////////////////////////////

		integrator.ForceGrav();
				
		integrator.Acceleration();

		integrator.Integrator(dt);

		
		
		
		


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
		
	}
	/*if (a->type == Collider::Type::AIR && b->type == Collider::Type::PLAYER && collision == true)
	{
		collision = false;
	}*/
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