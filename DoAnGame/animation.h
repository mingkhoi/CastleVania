#pragma once
#include "./sprite.h"

using namespace std;

class frame
{
public:
	DWORD time;
	LPSPRITE img;

	frame(LPSPRITE img, DWORD t);
	~frame();

	DWORD getTime();

	LPSPRITE getImg();

	void draw(float x, float y, int directionX, D3DCOLOR color);
	void drawFromBottom(float x, float y, int directionX, D3DCOLOR color);
	void drawCenter(float x, float y, int directionX, D3DCOLOR color); 
};

class _animation
{
protected:
	int currentFrame;

	DWORD lastTime;
	DWORD lastPause;
	DWORD pauseTime;

	std::vector<frame*> data;

public:
	bool loop = true;
	bool pause = false;
	bool hasPause = false;

	_animation();

	_animation(const _animation& value);

	~_animation();

	int getSize() { return data.size(); }

	void reset() { currentFrame = -1; }

	LPSPRITE getFrame(int id);

	void add(LPSPRITE sprite, DWORD times);
	LPSPRITE getCurrentFrame();

	// shoud be use only for un-looped animation
	bool isDone();

	int remainFrame();

	void renderNoPause(float x, float y, int directionX, D3DCOLOR color);
	void renderNoPause(float x, float y, int directionX, int alpha);

	void render(float x, float y, int directionX, D3DCOLOR color);
	void render(float x, float y, int directionX, int alpha);

	void renderFromBottom(float x, float y, int directionX, D3DCOLOR color);
	void renderFromBottom(float x, float y, int directionX, int alpha);

	void renderCenter(float x, float y, int directionX, D3DCOLOR color);
	void renderCenter(float x, float y, int directionX, int alpha);

	void stopAtCurrentFrame(float x, float y, int directionX, int alpha);
	void stopAtCurrentFrame(float x, float y, int dSrectionX, D3DCOLOR color);
	void stopAtCurrentFrameFromBottom(float x, float y, int directionX, int alpha);
	void stopAtCurrentFrameFromBottom(float x, float y, int directionX, D3DCOLOR color);
	void stopAtCurrentFrameCenter(float x, float y, int directionX, int alpha); 
	void stopAtCurrentFrameCenter(float x, float y, int directionX, D3DCOLOR color); 

	void increaseFrame();

	// increase the number of animation loop
	void clone(int value);

	void addEndAnimation();
};
typedef _animation* LPANIMATION;
