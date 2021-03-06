#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"
#include <assert.h>

#define RYU_SPEED 2
#define HADOUKEN_SPEED 5
#define JUMP_HEIGHT 50
// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA
ModulePlayer::ModulePlayer(bool start_enabled) : Module(start_enabled)
{
	position.x = 100;
	position.y = 216;

	// idle animation (arcade sprite sheet)
	//idle.frames.push_back({0, 14, 67, 90});
	idle.frames.push_back({85, 13, 70, 92});
	idle.frames.push_back({174, 12, 70, 93});
	idle.frames.push_back({268, 11, 70, 93});
	idle.frames.push_back({356, 10, 70, 94});
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

	kick.frames.push_back({ 588, 267, 90, 95 });
	kick.frames.push_back({ 678, 266, 98, 94 });
	kick.frames.push_back({ 777, 265, 113, 94 });
	kick.frames.push_back({ 678, 266, 98, 94 });
	kick.speed = 0.15f;

	punch.frames.push_back({ 244, 268, 80, 94 });
	punch.frames.push_back({ 332, 268, 88, 94 });
	punch.frames.push_back({ 431, 268, 108, 94 });
	punch.frames.push_back({ 332, 268, 88, 94 });
	punch.speed = 0.15f;

	jump_punch.frames.push_back({ 17, 1778, 80, 150});
	jump_punch.frames.push_back({ 106, 1778, 80, 150 });
	jump_punch.frames.push_back({ 200, 1783, 80, 150 });
	jump_punch.frames.push_back({ 292, 1783, 80, 150 });
	jump_punch.frames.push_back({ 373, 1783, 80, 150 });
	jump_punch.frames.push_back({ 452, 1778, 80, 150,});
	jump_punch.speed = 0.15f;

	hadouken.frames.push_back({ 27, 1545, 88, 89 });
	hadouken.frames.push_back({ 124, 1545, 95, 89 });
	hadouken.frames.push_back({ 239, 1545, 94, 89 });
	hadouken.frames.push_back({ 357, 1545, 106, 89 });
	hadouken.frames.push_back({ 124, 1545, 95, 89 });
	hadouken.speed = 0.15f;

	hadouken_effect.frames.push_back({ 480, 1563, 55, 31 });
	hadouken_effect.frames.push_back({ 510, 1600, 55, 31 });
	hadouken_effect.frames.push_back({ 550, 1563, 55, 31 });
	hadouken_effect.speed = 0.2f;

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

void ModulePlayer::ResetPlayer()
{
	hadouken_ball = false;
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
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			currentState = KICK;
		}
		else if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			currentState = PUNCH;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			currentState = JUMP_PUNCH;
		}
		else if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			currentState = HADOUKEN;
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			currentState = MOVING_BACKWARD;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			currentState = MOVING_FORWARD;
		}
		break;
	case MOVING_FORWARD:
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
				currentState = MOVING_BACKWARD;
			else currentState = IDLE;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			currentState = JUMP_PUNCH;
		}
		else if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			currentState = KICK;
		}
		else if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			currentState = PUNCH;
		}
		else if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			currentState = HADOUKEN;
		}
		break;
	case MOVING_BACKWARD:
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
				currentState = MOVING_FORWARD;
			else currentState = IDLE;
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			currentState = JUMP_PUNCH;
		}
		else if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			currentState = KICK;
		}
		else if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			currentState = PUNCH;
		}
		else if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			currentState = HADOUKEN;
		}
		break;
	case KICK:
		if (kick.IsLastFrame())
		{
			currentState = IDLE;
		}
		break;
	case PUNCH:
		if (punch.IsLastFrame())
		{
			currentState = IDLE;
		}
		break;
	case JUMP_PUNCH:
		if (jump_punch.IsLastFrame())
		{
			ryu_height = 0;
			currentState = IDLE;
		}
		break;
	case HADOUKEN:
		if (hadouken.IsLastFrame())
		{
			currentState = IDLE;
			ActivateHadouken();
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
		App->renderer->camera.x = MAX(App->renderer->camera.x - RYU_SPEED, -STAGE_LIMIT);
		break;
	case MOVING_BACKWARD:
		current_animation = &backward;
		App->renderer->camera.x = MIN(App->renderer->camera.x + RYU_SPEED, 0);
		break;
	case KICK:
		current_animation = &kick;
		break;
	case PUNCH:
		current_animation = &punch;
		break;
	case JUMP_PUNCH:
		current_animation = &jump_punch;
		ryu_height=JUMP_HEIGHT;
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			App->renderer->camera.x = MAX(App->renderer->camera.x - RYU_SPEED, -STAGE_LIMIT);
		}
		else if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			App->renderer->camera.x = MIN(App->renderer->camera.x + RYU_SPEED, 0);
		}
		break;
	case HADOUKEN:
		current_animation = &hadouken;
		break;
	}

	if (hadouken_ball)
	{
		App->renderer->Blit(graphics, 150+hadouken_ball_position++, 130, &hadouken_effect.GetCurrentFrame(), 1);
		if (hadouken_ball_position > 1000)
			hadouken_ball = false;
	}

	assert(current_animation!=nullptr);
	App->renderer->Blit(graphics, 60, 110-ryu_height, &current_animation->GetCurrentFrame(), 0);
}

void ModulePlayer::ActivateHadouken()
{
	hadouken_ball_position = -App->renderer->camera.x/SCREEN_SIZE;
	hadouken_ball = true;
}
