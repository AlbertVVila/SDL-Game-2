#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include <assert.h>

#define RYU_SPEED 2
// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	position.x = 100;
	position.y = 216;

	// idle animation (arcade sprite sheet)
	idle.frames.push_back({7, 14, 60, 90});
	idle.frames.push_back({95, 15, 60, 89});
	idle.frames.push_back({184, 14, 60, 90});
	idle.frames.push_back({276, 11, 60, 93});
	idle.frames.push_back({366, 12, 60, 92});
	idle.speed = 0.1f;
	
	// walk backward animation (arcade sprite sheet)
	backward.frames.push_back({542, 131, 61, 87});
	backward.frames.push_back({628, 129, 59, 90});
	backward.frames.push_back({713, 128, 57, 90});
	backward.frames.push_back({797, 127, 57, 90});
	backward.frames.push_back({883, 128, 58, 91});
	backward.frames.push_back({974, 129, 57, 89});
	backward.speed = 0.1f;

	// TODO 8: setup the walk forward animation from ryu4.png
	forward.frames.push_back({ 0, 130, 72, 92 });
	forward.frames.push_back({ 78, 128, 72, 96 });
	forward.frames.push_back({ 162, 128, 72, 98 });
	forward.frames.push_back({ 250, 128, 72, 95 });
	forward.frames.push_back({ 341, 128, 72, 96 });
	forward.frames.push_back({ 420, 128, 72, 95 });
	forward.speed = 0.1f;

	kick.frames.push_back({ 572, 267, 100, 95 });
	kick.frames.push_back({ 669, 266, 99, 94 });
	kick.frames.push_back({ 777, 265, 113, 94 });
	kick.speed = 0.1f;


}

ModulePlayer::~ModulePlayer()
{
	// Homework : check for memory leaks
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("ryu4.png"); // arcade version

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}


// Update
update_status ModulePlayer::Update()
{
	// TODO 9: Draw the player with its animation
	// make sure to detect player movement and change its
	// position while cycling the animation(check Animation.h)
	UpdateFSM();
	return UPDATE_CONTINUE;
}

void ModulePlayer::UpdateFSM()
{
	switch (currentState)
	{
	case IDLE:
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			currentState = MOVING_BACKWARD;
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			currentState = MOVING_FORWARD;
		break;
	case MOVING_FORWARD:
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
				currentState = MOVING_BACKWARD;
			else currentState = IDLE;
		}
		break;
	case MOVING_BACKWARD:
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
				currentState = MOVING_FORWARD;
			else currentState = IDLE;
		}
		break;
	}
	ExecuteState();
}

void ModulePlayer::ExecuteState()
{
	Animation * current_animation = nullptr;
	switch (currentState)
	{
	case IDLE:
		current_animation = &idle;
		break;
	case MOVING_FORWARD:
		current_animation = &forward;
		App->renderer->camera.x = MAX(App->renderer->camera.x - RYU_SPEED, -KEN_STAGE_LIMIT);
		break;
	case MOVING_BACKWARD:
		current_animation = &backward;
		App->renderer->camera.x = MIN(App->renderer->camera.x + RYU_SPEED, 0);
		break;
	}
	assert(current_animation!=nullptr);
	App->renderer->Blit(graphics, 60, 110, &current_animation->GetCurrentFrame(), 0);
}