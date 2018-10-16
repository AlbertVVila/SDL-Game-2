#include "Globals.h"
#include "Application.h"
#include "ModuleSceneHonda.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "SDL/include/SDL.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleSceneHonda::ModuleSceneHonda(bool start_enabled) : Module(start_enabled)
{
	// ground
	ground.x = 7;
	ground.y = 375;
	ground.w = 849;
	ground.h = 65;

	// bath
	bath.x = 163;
	bath.y = 65;
	bath.w = 337;
	bath.h = 52;
	//foreground.x = 8;
	//foreground.y = 24;
	//foreground.w = 522;
	//foreground.h = 181;

	background.x = 120;
	background.y = 128;
	background.w = 671;
	background.h = 199;



}

ModuleSceneHonda::~ModuleSceneHonda()
{}

// Load assets
bool ModuleSceneHonda::Start()
{
	LOG("Loading honda scene");

	graphics = App->textures->Load("honda_stage2.png");

	// TODO 7: Enable the player module
	App->player->Enable();
	// TODO 0: trigger background music
	App->audio->PlayMusic("honda.ogg", 5.0f);

	return true;
}

// UnLoad assets
bool ModuleSceneHonda::CleanUp()
{
	LOG("Unloading honda scene");

	App->textures->Unload(graphics);
	App->player->Disable();

	return true;
}

// Update: draw background
update_status ModuleSceneHonda::Update()
{


	//App->renderer->Blit(graphics, 0, 0, &background, PARALAX_SPEED); 
	//App->renderer->Blit(graphics, 560, 8, &(flag.GetCurrentFrame()), PARALAX_SPEED); 


	App->renderer->Blit(graphics, 100, -15, &background, PARALAX_SPEED);

	App->renderer->Blit(graphics, 0, 160, &ground);
	App->renderer->Blit(graphics, 250, 130, &bath, PARALAX_SPEED);


	return UPDATE_CONTINUE;
}
