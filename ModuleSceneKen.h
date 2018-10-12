#ifndef __MODULESCENEKEN_H__
#define __MODULESCENEKEN_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"

struct SDL_Texture;

class ModuleSceneKen : public Module
{
public:
	ModuleSceneKen( bool start_enabled = true);
	~ModuleSceneKen();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	
	SDL_Texture* graphics = nullptr;
	SDL_Rect ground;
	SDL_Rect background;
	SDL_Rect foreground;

	Animation flag;
	Animation girl;

private:
	int ComputeShipMovement();
	void UpdateTime();

private:
	int direction = 1;
	int shiptimer = 0;
	int last_time;
	int delta_time = 0;
};

#endif // __MODULESCENEKEN_H__