#include "Globals.h"
#include "Application.h"
#include "ModuleSceneKen.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"
#include "math.h"

#define SHIP_MOVEMENT_TIME 5000
#define SHIP_MOVEMENT_AMOUNT 6

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneKen::ModuleSceneKen(bool start_enabled) : Module(start_enabled)
{
	// ground
	ground.x = 8;
	ground.y = 391;
	ground.w = 896;
	ground.h = 72;

	// TODO 2 : setup the foreground (red ship) with
	// coordinates x,y,w,h from ken_stage.png
	foreground.x = 8;
	foreground.y = 24;
	foreground.w = 522;
	foreground.h = 181;
	// Background / sky
	background.x = 72;
	background.y = 208;
	background.w = 768;
	background.h = 176;

	// flag animation
	flag.frames.push_back({848, 208, 40, 40});
	flag.frames.push_back({848, 256, 40, 40});
	flag.frames.push_back({848, 304, 40, 40});
	flag.speed = 0.1f;

	// TODO 4: Setup Girl Animation from coordinates from ken_stage.png
	girl.frames.push_back({624,16,32,54});
	girl.frames.push_back({624,80,32,54});
	girl.frames.push_back({624,144,32,54});
	girl.speed = .1f;
	

}

ModuleSceneKen::~ModuleSceneKen()
{}

// Load assets
bool ModuleSceneKen::Start()
{
	LOG("Loading ken scene");
	
	graphics = App->textures->Load("ken_stage.png");

	// TODO 7: Enable the player module
	App->player->Enable();
	// TODO 0: trigger background music
	App->audio->PlayMusic("ken.ogg",9.0f);

	last_time = SDL_GetTicks();

	return true;
}

// UnLoad assets
bool ModuleSceneKen::CleanUp()
{
	LOG("Unloading ken scene");

	App->textures->Unload(graphics);
	App->player->Disable();
	
	return true;
}

// Update: draw background
update_status ModuleSceneKen::Update()
{
	UpdateTime();
	// TODO 5: make sure the ship goes up and down
	int shipOffsetY = ComputeShipMovement();
	// Draw everything --------------------------------------
	// TODO 1: Tweak the movement speed of the sea&sky + flag to your taste
	App->renderer->Blit(graphics, 0, 0, &background, PARALAX_SPEED); // sea and sky
	App->renderer->Blit(graphics, 560, 8, &(flag.GetCurrentFrame()), PARALAX_SPEED); // flag animation

	// TODO 3: Draw the ship. Be sure to tweak the speed.
	App->renderer->Blit(graphics, 0, -5+shipOffsetY, &foreground, PARALAX_SPEED); // red ship
	// TODO 6: Draw the girl. Make sure it follows the ship movement!
	App->renderer->Blit(graphics, 191, 99+shipOffsetY, &(girl.GetCurrentFrame()), PARALAX_SPEED); //girl in red ship

	App->renderer->Blit(graphics, 0, 170, &ground);

	// TODO 10: Build an entire new scene "honda", you can find its
	// and music in the Game/ folder

	// TODO 11: Make that pressing space triggers a switch to honda logic module
	// using FadeToBlack module
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) ChangeScene();

	return UPDATE_CONTINUE;
}

int ModuleSceneKen::ComputeShipMovement()
{
	if (shiptimer <= 0)
	{
		direction = -direction;
		shiptimer = SHIP_MOVEMENT_TIME;
	}

	shiptimer -= delta_time;
	float percentage = direction == 1 ? shiptimer : SHIP_MOVEMENT_TIME-shiptimer;
	return round(percentage /SHIP_MOVEMENT_TIME*SHIP_MOVEMENT_AMOUNT);

}

void ModuleSceneKen::UpdateTime()
{
	unsigned int current_time = SDL_GetTicks();
	delta_time = current_time - last_time;
	last_time = current_time;
}

void ModuleSceneKen::ChangeScene()
{
	App->fade->FadeToBlack((Module *)App->scene_honda,this, 3.0f);
}
