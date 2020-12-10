#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Collisions.h"
#include "EntityManager.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");
	idleAnim.loop = true;
	idleAnim.PushBack({ 0, 0, 12, 11 });

    walkAnimRight.PushBack({ 13,0, 12, 11 });
	walkAnimRight.PushBack({ 26,0, 12, 11 });
	walkAnimRight.PushBack({ 39,0, 12, 11 });
	walkAnimRight.PushBack({ 53,0, 14, 11 });
	walkAnimRight.loop = true;
	walkAnimRight.speed = 0.1f;

	walkAnimLeft.PushBack({ 13,12, 12, 11 });
	walkAnimLeft.PushBack({ 26,12, 12, 11 });
	walkAnimLeft.PushBack({ 39,12, 12, 11 });
	walkAnimLeft.PushBack({ 53,12, 14, 11 });
	walkAnimLeft.loop = true;
	walkAnimLeft.speed = 0.1f;

	jumpAnim.PushBack({ 1, 23, 12, 12 });
	jumpAnim.loop = true;
}

// Destructor
Player::~Player()
{}

// Called before the first frame
bool Player::Start()
{
	playerData.texture = app->tex->Load("Assets/Textures/player.png");
	playerData.currentAnim = &idleAnim;

	SDL_Rect colP = { playerData.position.x, playerData.position.y, 12, 11 };
	collider = app->collisions->AddCollider(colP, Collider::Type::PLAYER, this);

	jumpFx = app->audio->LoadFx("Assets/Audio/FX/jump.wav");
	doubleJumpFx = app->audio->LoadFx("Assets/Audio/FX/double_jump.wav");
	checkPointFx = app->audio->LoadFx("Assets/Audio/FX/checkpoint.wav");
	killingEnemyFx = app->audio->LoadFx("Assets/Audio/FX/enemy_death.wav");

	InitialPos();
	return true;
}

// Called before render is available
bool Player::Awake()
{
	LOG("Loading Player");
	
	return true;
}

// Called each loop iteration
bool Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
		&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) {
		playerData.currentAnim = &idleAnim;
	}
	if(onGround == false)
	{
		playerData.vely += gravity;
		playerData.position.x += playerData.velx;
		playerData.position.y += playerData.vely;
	}

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debug = !debug;
	}

	if (debug == true)
	{
		app->collisions->DebugDraw();
	}

	//PlayerMovement
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		playerData.position.x -= 1;
		onGround = false;
		if (playerData.currentAnim != &walkAnimLeft) {
			walkAnimLeft.Reset();
			playerData.currentAnim = &walkAnimLeft;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		playerData.position.x += 1;
		onGround = false;
		if (playerData.currentAnim != &walkAnimRight) {
			walkAnimRight.Reset();
			playerData.currentAnim = &walkAnimRight;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		
		if (doubleJump == true && onGround == false) 
		{
			app->audio->PlayFx(doubleJumpFx);
			playerData.vely = -4.5f;
			doubleJump = false;
			if (playerData.currentAnim != &jumpAnim) 
			{
				jumpAnim.Reset();
				playerData.currentAnim = &jumpAnim;
			}
		}
		if (playerJumping == true ) 
		{
			app->audio->PlayFx(jumpFx);
			playerJumping = false;
			playerData.vely = -5.5f;
			playerData.position.y += playerData.vely;			
			doubleJump = true;
			if (playerData.currentAnim != &jumpAnim)
			{
				jumpAnim.Reset();
				playerData.currentAnim = &jumpAnim;
			}
			onGround = false;
		}
	}
	
	cameraControl = true;

	collider->SetPos(playerData.position.x, playerData.position.y);

	playerData.currentAnim->Update();

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;
	SDL_Rect rectPlayer;
	rectPlayer = playerData.currentAnim->GetCurrentFrame();
	app->render->DrawTexture(playerData.texture, playerData.position.x, playerData.position.y, &rectPlayer);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing Player");
	active = false;

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	pugi::xml_node play = data.child("position");
	playerData.position.x = play.attribute("x").as_int(0);
	playerData.position.y = play.attribute("y").as_int(0);

	pugi::xml_node level = data.child("level");
	playerData.currentLevel = level.attribute("l").as_int(0);

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node play = data.child("position");
	play.attribute("x").set_value(playerData.position.x);
	play.attribute("y").set_value(playerData.position.y);
	
	pugi::xml_node level = data.child("level");
	if(scene1 == true)
	{
		level.attribute("l").set_value("1");
	}

	return true;
}

bool Player::CheckCollision(int x, int y)
{
	iPoint posMapPlayer;

	return false;
}

void Player::OnCollision(Collider* a, Collider* b) 
{
	if (a == collider)
	{
		if (b->type == Collider::Type::FLOOR)
		{
			onGround = true;
			playerJumping = true;
			playerData.position.y -=1;
			playerData.vely = 0;
			playerData.position.y = playerData.position.y;
		}
	}
}

void Player::InitialPos() 
{
	if (scene1 == true && app->scene->firstEntry == true) 
	{
		playerData.position.x = 50.0f;
		playerData.position.y = 260.0f;
		playerData.vely = 0;

		app->render->camera.x = 0;
		app->render->camera.y = -playerData.position.y + 50;
	}
}