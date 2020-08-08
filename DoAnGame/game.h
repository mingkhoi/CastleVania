#pragma once
#include "./define.h"

#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#define KEYBOARD_BUFFER_SIZE 1024

using namespace std;

class keyboard_handler
{
public:
	virtual void getKeyHandle(const BYTE* keyState,
		const BYTE* keyEvent)
		= 0;
};
typedef keyboard_handler* LPKEYBOARDHANDLER;

class game
{
protected:
	static game* instance;

	LPD3DXSPRITE spriteHandler;

	LPDIRECT3D9 directX_object = NULL;
	LPDIRECT3DDEVICE9 directX_device = NULL;
	LPDIRECT3DSURFACE9 backBuffer = NULL;

	LPDIRECTINPUT8 DI_object = NULL;
	LPDIRECTINPUTDEVICE8 DI_device = NULL;
	LPKEYBOARDHANDLER keyboardHandler = NULL;

	DirectX::AudioEngine* audio_device;

	BYTE keyboardState[256];
	BYTE* keyboardEvent = NULL;

	HWND windowHandle;

public:

	static void debugOut(const wchar_t* fmt, ...);
	static void debugOutString(const char* fmt, ...);

	static bool isKeyDown(const BYTE* buffer, int keyCode);

	static bool isKeyUp(const BYTE* buffer, int keyCode);

	static bool isEventDown(const BYTE* buffer, int keycode);

	static bool isEventUp(const BYTE* buffer, int keycode);

	static int getCell(float x, float y, int cellW, int cellH,
		int gridX);

	static std::unordered_map<std::string, bool> makeFilterCollision(std::vector<std::string> arr);

	static game* getInstance();

	static bool isSamePoint(float val, float target, float speed);

	static bool gameEnd;
	static bool gameFreeze;
	static bool gameFreezeWindow;

	static std::mt19937 generator;

	LPDIRECT3D9 getObject();
	LPDIRECT3DDEVICE9 getDevice();
	LPDIRECT3DSURFACE9 getBackBuffer();
	DirectX::AudioEngine* getAudioDevice();

	LPD3DXSPRITE getSpriteHandler();

	HWND getHandle();

	bool initWindow(HINSTANCE hInstance, WNDPROC procedure);
	bool initDevice();

	bool initKeyboard(LPKEYBOARDHANDLER keyHandler);
	void getKeyStroke();

	// collision detection

	float sweptAABB(FRect objA, FRect objB, float vx, float vy,
		float& nx, float& ny);

	bool AABB(FRect objA, FRect objB);

	// draw function

	void draw(LPDIRECT3DTEXTURE9 texture, LPRECT sourceRect,
		float x, float y, int directionX, D3DCOLOR color);

	void drawBottom(LPDIRECT3DTEXTURE9 texture, LPRECT sourceRect,
		float x, float y, int directionX, D3DCOLOR color);

	void drawCenter(LPDIRECT3DTEXTURE9 texture, LPRECT sourceRect,
		float x, float y, int directionX, D3DCOLOR color);

	void drawNoCam(LPDIRECT3DTEXTURE9 texture, LPRECT sourceRect,
		float x, float y, int directionX, D3DCOLOR color);
};
