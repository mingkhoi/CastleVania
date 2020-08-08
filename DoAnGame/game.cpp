#include "./game.h"
#include "./camera.h"
#include "./gamePlay.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <limits.h>

#pragma warning(disable : 4996)

bool game::gameEnd = false;
bool game::gameFreeze = false;
bool game::gameFreezeWindow = false;

// time-based seed random generator

std::mt19937 game::generator(std::chrono::duration_cast<std::chrono::seconds>(
	std::chrono::system_clock::now().time_since_epoch())
	.count());

//------------------------------------------

game* game::instance = NULL;

void game::debugOut(const wchar_t* fmt, ...) {
	va_list argList;
	va_start(argList, fmt);

	wchar_t buffer[2000];

	vswprintf_s(buffer, fmt, argList);

	va_end(argList);

	OutputDebugStringW(buffer);
}

void game::debugOutString(const char* fmt, ...) {
	va_list argList;
	va_start(argList, fmt);

	char buffer[2000];

	vsprintf_s(buffer, fmt, argList);

	va_end(argList);

	OutputDebugStringA(buffer);
}

bool game::isEventDown(const BYTE* buffer, int keycode) {
	return buffer[keycode];
}

bool game::isEventUp(const BYTE* buffer, int keycode) {
	return !buffer[keycode];
}

bool game::isKeyDown(const BYTE* buffer, int keyCode) {
	return (buffer[keyCode] & 0x80) > 0;
}

bool game::isKeyUp(const BYTE* buffer, int keyCode) {
	return !isKeyDown(buffer, keyCode);
}

std::unordered_map<std::string, bool> game::makeFilterCollision(std::vector<std::string> arr) {
	std::unordered_map<std::string, bool> res;

	for (auto x : arr)
		res[x] = true;

	return res;
}

bool game::isSamePoint(float val, float target, float speed) {
	if (speed < 0 && val < target) {
		return true;
	}
	if (speed > 0 && val > target) {
		return true;
	}
	if (val == target)
		return true;
	return false;
}

int game::getCell(float x, float y, int cellW, int cellH,
	int gridX) {

	x -= gamePlay::getInstance()->rootX;
	y -= gamePlay::getInstance()->rootY;

	int cellX = x / (float)cellW;
	int cellY = y / (float)cellH;

	return cellX + (cellY * gridX);
}

game* game::getInstance() {
	if (instance == NULL)
		instance = new game();
	return instance;
}

LPDIRECT3D9 game::getObject() {
	return directX_object;
}

LPDIRECT3DDEVICE9 game::getDevice() {
	return directX_device;
}

LPDIRECT3DSURFACE9 game::getBackBuffer() {
	return backBuffer;
}

HWND game::getHandle() {
	return windowHandle;
}

LPD3DXSPRITE game::getSpriteHandler() {
	return spriteHandler;
}

DirectX::AudioEngine* game::getAudioDevice() {
	return this->audio_device;
}

bool game::initWindow(HINSTANCE hInstance, WNDPROC procedure) {
	WNDCLASSEX wndClass;

	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = procedure;
	wndClass.cbSize = sizeof(wndClass);

	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIconSm = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WINLOGO));
	wndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WINLOGO));

	wndClass.lpszClassName = L"simple_sprite";
	wndClass.lpszMenuName = NULL;

	RegisterClassEx(&wndClass);

	windowHandle = CreateWindow(
		L"simple_sprite", L"hello world",
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wndWidth, wndHeight,
		NULL, NULL, hInstance, NULL);

	if (!windowHandle)
		return false;

	ShowWindow(windowHandle, SW_SHOW);
	UpdateWindow(windowHandle);

	SetWindowPos(windowHandle, HWND_TOP,
		100, 100, wndWidth * 3.0f, wndHeight * 3.0f,
		SWP_SHOWWINDOW);

	return true;
}

bool game::initDevice() {
	directX_object = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = wndWidth + 1;
	d3dpp.BackBufferHeight = wndHeight + 1;
	d3dpp.BackBufferCount = 1;

	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;

	directX_object->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		windowHandle, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &directX_device);

	if (!directX_device)
		return false;

	directX_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO,
		&backBuffer);

	D3DXCreateSprite(directX_device, &spriteHandler);

	this->audio_device = new DirectX::AudioEngine(DirectX::AudioEngine_Default | DirectX::AudioEngine_Debug);
	this->audio_device->Resume();

	return true;
}

bool game::initKeyboard(LPKEYBOARDHANDLER keyHandler) {
	DirectInput8Create(
		(HINSTANCE)GetWindowLong(windowHandle, GWL_HINSTANCE),
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(VOID**)&DI_object, NULL);

	if (!DI_object)
		return false;

	DI_object->CreateDevice(GUID_SysKeyboard, &DI_device, NULL);

	if (!DI_device)
		return false;

	DI_device->SetDataFormat(&c_dfDIKeyboard);

	DI_device->SetCooperativeLevel(windowHandle,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD inputProperty;

	inputProperty.diph.dwSize = sizeof(DIPROPDWORD);
	inputProperty.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	inputProperty.diph.dwObj = 0;
	inputProperty.diph.dwHow = DIPH_DEVICE;
	inputProperty.dwData = KEYBOARD_BUFFER_SIZE;

	DI_device->SetProperty(DIPROP_BUFFERSIZE, &inputProperty.diph);

	this->keyboardHandler = keyHandler;

	DI_device->Acquire();

	return true;
}

void game::getKeyStroke() {
	DI_device->Acquire();

	DI_device->GetDeviceState(sizeof(keyboardState), keyboardState);

	DWORD eventElements = KEYBOARD_BUFFER_SIZE;
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];

	DI_device->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents,
		&eventElements, 0);

	// clear keyboard event

	if (keyboardEvent != NULL)
		delete[] keyboardEvent;

	keyboardEvent = new BYTE[256]();

	for (DWORD i = 0; i < eventElements; i++) {
		int keyCode = keyEvents[i].dwOfs;
		int keyState = keyEvents[i].dwData;

		if (keyCode < 0 || keyCode >= 256)
			continue;

		if ((keyState & 0x80) > 0)
			keyboardEvent[keyCode] = true;
		else
			keyboardEvent[keyCode] = false;
	}

	this->keyboardHandler->getKeyHandle(keyboardState,
		keyboardEvent);
}

bool game::AABB(FRect objA, FRect objB) {
	if (objA.left < objB.right && objA.right > objB.left&&
		objA.top < objB.bottom && objA.bottom > objB.top) {
		return true;
	}
	return false;
}

float game::sweptAABB(FRect objA, FRect objB, float vx, float vy,
	float& nx, float& ny) {
	nx = ny = 0.0f;

	FRect broadPhase = objA;

	if (vx > 0)
		broadPhase.right += vx;
	else
		broadPhase.left += vx;

	if (vy > 0)
		broadPhase.bottom += vy;
	else
		broadPhase.top += vy;

	if (objB.left > broadPhase.right || objB.right < broadPhase.left ||
		objB.top > broadPhase.bottom || objB.bottom < broadPhase.top) {
		return 1.0f;
	}

	float xDisEntry, yDisEntry;
	float xDisExit, yDisExit;
	float xEntry, yEntry;
	float xExit, yExit;

	if (vx >= 0) {
		xDisEntry = objB.left - objA.right;
		xDisExit = objB.right - objA.left;
	}
	else {
		xDisEntry = objB.right - objA.left;
		xDisExit = objB.left - objA.right;
	}

	if (vy >= 0) {
		yDisEntry = objB.top - objA.bottom;
		yDisExit = objB.bottom - objA.top;
	}
	else {
		yDisEntry = objB.bottom - objA.top;
		yDisExit = objB.top - objA.bottom;
	}

	if (vx == 0.0f) {
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else {
		xEntry = xDisEntry / vx;
		xExit = xDisExit / vx;
	}

	if (vy == 0.0f) {
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else {
		yEntry = yDisEntry / vy;
		yExit = yDisExit / vy;
	}

	float entryTime = std::max(xEntry, yEntry);
	float exitTime = std::min(xExit, yExit);

	if (entryTime > exitTime || (xEntry < 0.0f && yEntry < 0.0f) || xEntry > 1.0f || yEntry > 1.0f) {
		return 1.0f;
	}
	else {
		if (xEntry > yEntry) {
			ny = 0.0f;

			if (vx > 0)
				nx = -1.0f;
			else
				nx = 1.0f;
		}
		else {
			nx = 0.0f;

			if (vy > 0)
				ny = -1.0f;
			else
				ny = 1.0f;
		}
	}

	return entryTime;
}

void game::draw(LPDIRECT3DTEXTURE9 texture, LPRECT sourceRect,
	float x, float y, int directionX, D3DCOLOR color) {
	camera* cam = camera::getInstance();

	int screen_x = x - cam->getX();
	int screen_y = y - cam->getY();

	int center_w = (sourceRect->right - sourceRect->left) / 2;
	int center_h = (sourceRect->bottom - sourceRect->top) / 2;

	D3DXMATRIX def, matrix;
	spriteHandler->GetTransform(&def);

	D3DXVECTOR3 position = D3DXVECTOR3(screen_x, screen_y, 0);
	D3DXVECTOR2 centerScale = D3DXVECTOR2(position.x + center_w, position.y + center_h);

	D3DXMatrixTransformation2D(&matrix, &centerScale,
		0, &D3DXVECTOR2(-directionX, 1), NULL, 0, NULL);

	D3DXMATRIX res;
	D3DXMatrixMultiply(&res, &def, &matrix);

	spriteHandler->SetTransform(&res);

	this->spriteHandler->Draw(texture, sourceRect, NULL,
		&position, color);

	spriteHandler->SetTransform(&def);
}

void game::drawBottom(LPDIRECT3DTEXTURE9 texture, LPRECT sourceRect,
	float x, float y, int directionX, D3DCOLOR color) {
	camera* cam = camera::getInstance();

	float screen_x = x - cam->getX();
	float screen_y = y - cam->getY();

	float w = (sourceRect->right - sourceRect->left) / 2.0f;
	float h = (sourceRect->bottom - sourceRect->top) / 2.0f;

	D3DXMATRIX def, matrix;
	spriteHandler->GetTransform(&def);

	D3DXMatrixTransformation2D(&matrix, &D3DXVECTOR2(screen_x + w, screen_y + h),
		0.0f, &D3DXVECTOR2(-directionX, 1), NULL, 0.0f, NULL);

	D3DXVECTOR3 position = D3DXVECTOR3(floor(screen_x),
		floor(screen_y), 0);

	D3DXVECTOR3 drawCenter = D3DXVECTOR3(0,
		sourceRect->bottom - sourceRect->top, 0);

	spriteHandler->SetTransform(&(def * matrix));

	this->spriteHandler->Draw(texture, sourceRect, &drawCenter,
		&position, color);

	spriteHandler->SetTransform(&def);
}

void game::drawCenter(LPDIRECT3DTEXTURE9 texture, LPRECT sourceRect,
	float x, float y, int directionX, D3DCOLOR color) {
	camera* cam = camera::getInstance();

	float screen_x = x - cam->getX();
	float screen_y = y - cam->getY();

	float w = (sourceRect->right - sourceRect->left) / 2.0f;
	float h = (sourceRect->bottom - sourceRect->top) / 2.0f;

	D3DXMATRIX def, matrix;
	spriteHandler->GetTransform(&def);

	D3DXMatrixTransformation2D(&matrix, &D3DXVECTOR2(screen_x + w, screen_y + h),
		0.0f, &D3DXVECTOR2(-directionX, 1), NULL, 0.0f, NULL);

	D3DXVECTOR3 position = D3DXVECTOR3(floor(screen_x),
		floor(screen_y), 0);

	int centerX = (sourceRect->right - sourceRect->left) / 2.0f;
	int centerY = (sourceRect->bottom - sourceRect->top) / 2.0f;

	D3DXVECTOR3 drawCenter = D3DXVECTOR3(centerX, centerY, 0);

	spriteHandler->SetTransform(&(def * matrix));

	this->spriteHandler->Draw(texture, sourceRect, &drawCenter,
		&position, color);

	spriteHandler->SetTransform(&def);
}

void game::drawNoCam(LPDIRECT3DTEXTURE9 texture, LPRECT sourceRect,
	float x, float y, int directionX, D3DCOLOR color) {
	D3DXVECTOR3 position = D3DXVECTOR3(floor(x), floor(y), 0);

	this->spriteHandler->Draw(texture, sourceRect, NULL,
		&position, color);
}