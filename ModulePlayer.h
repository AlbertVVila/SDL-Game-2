#ifndef __MODULEPLAYER_H__
#define __MODULEPLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "Point.h"

struct SDL_Texture;

enum PlayerState
{
	IDLE = 0,
	MOVING_FORWARD,
	MOVING_BACKWARD,
	PUNCH,
	KICK,
	JUMP_PUNCH,
	HADOUKEN,
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool start_enabled = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void ResetPlayer();

public:

	SDL_Texture* graphics = nullptr;

	//Player animations
	Animation idle;
	Animation backward;
	Animation forward;
	Animation kick;
	Animation punch;
	Animation hadouken;
	Animation jump_punch;

	//Player effect animations
	Animation hadouken_effect;
	int hadouken_ball_position = 0;
	bool hadouken_ball = false;

	//Player position
	int ryu_height = 0;

	iPoint position;
	PlayerState currentState = IDLE;

private:
	void UpdateFSM();
	void ExecuteState();
	void ActivateHadouken();
};

#endif // __MODULEPLAYER_H__