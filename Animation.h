#pragma once
#include <vector>

using namespace std;

class Animation
{
public:
	float speed;
	vector<SDL_Rect> frames;

private:
	float current_frame;
	bool isLastFrame;

public:
	Animation() : frames(), speed(1.0f), current_frame(0.0f)
	{}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;

		if(current_frame >= frames.size())
			current_frame = 0.0f;

		isLastFrame = current_frame + speed >= frames.size() ? true : false;
		return frames[(int)current_frame];
	}

	float IsLastFrame() const
	{
		return isLastFrame;
	}
};