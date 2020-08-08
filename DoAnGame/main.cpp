#include "./CastleVania.h"
#include "./animationSet.h"
#include "./database.h"
#include "./framework.h"
#include "./gamePlay.h"
#include "./map.h"
#include "./SoundManager.h"
#include "./texture.h"

#include "./listGameobject.h"


using namespace std;

long long Counter::index = 0;

void update(float dt);

class _keyboard : public keyboard_handler
{
public:
	virtual void getKeyHandle(const BYTE* keyState,
		const BYTE* keyEvent);
};

_keyboard* keyboardHandler = new _keyboard();

#pragma region load resources

void loadResource() {
	// load xml database
	database::getInstance()->getData("data.xml");

	// load texture
	texture::getInstance()->loadSingletonTexture();

	// load animationSet
	animationSet::getInstance()->loadSingletonResource();

	// load sound effect
	SoundManager::GetInstance()->loadSingletonResource();

	// load map list
	gameMap::getInstance()->loadData();

	// reflection for C++
	listGameobject::getInstance()->init();

	// start game
	gamePlay::getInstance()->startGame();
}

#pragma endregion

#pragma region window event

LRESULT CALLBACK wndProc(HWND handle, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		game::gameEnd = TRUE;
		PostQuitMessage(0);

		break;

	case WM_MOVING:
	case WM_MOVE:
		game::gameFreezeWindow = true;
		break;
	}
	return DefWindowProc(handle, msg, wParam, lParam);
}

#pragma endregion

#pragma region keyboard handler

void _keyboard::getKeyHandle(const BYTE* keyState,
	const BYTE* keyEvent) {
	gamePlay::getInstance()->calculateState(keyState, keyEvent);
}

#pragma endregion

#pragma region game loop

void update(float dt) {
	gamePlay::getInstance()->update(dt);
}

void render() {
	game::getInstance()->getDevice()->BeginScene();

	game::getInstance()->getDevice()->ColorFill(
		game::getInstance()->getBackBuffer(), NULL, D3DCOLOR_ARGB(255, 0, 0, 0));

	game::getInstance()->getSpriteHandler()->Begin(D3DXSPRITE_ALPHABLEND);

	gamePlay::getInstance()->render();

	game::getInstance()->getSpriteHandler()->End();

	game::getInstance()->getDevice()->EndScene();

	game::getInstance()->getDevice()->Present(NULL, NULL, NULL, NULL);
}

#pragma endregion

#pragma region create window

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	LPSTR cmdLine, int cmdShow) {
	srand(time(NULL));

	if (!game::getInstance()->initWindow(hInstance, wndProc)) {
		MessageBox(NULL, L"no window\n", L"error", MB_OKCANCEL);
		return false;
	}

	if (!game::getInstance()->initDevice()) {
		MessageBox(NULL, L"no device\n", L"error", MB_OKCANCEL);
		return false;
	}

	if (!game::getInstance()->initKeyboard(keyboardHandler)) {
		MessageBox(NULL, L"no keyboard\n", L"error", MB_OKCANCEL);
		return false;
	}

	loadResource();

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	DWORD tickFrame = 1000.0f / wndFps, delta = 0.0f;

	DWORD lastTime = GetTickCount64();

	while (game::gameEnd != TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (game::gameFreezeWindow == true) {
			game::gameFreezeWindow = false;

			delta = tickFrame;
		}
		else
			delta = GetTickCount64() - lastTime;

		if (delta >= tickFrame) {
			lastTime = GetTickCount64();

			/*game::getInstance()->getAudioDevice()->Resume();*/
			game::getInstance()->getKeyStroke();

			update(delta / 1000.0f);
			render();
		}
		else
			Sleep(tickFrame - delta);
	}

	if (game::getInstance()->getDevice() != NULL)
		game::getInstance()->getDevice()->Release();

	if (game::getInstance()->getObject() != NULL)
		game::getInstance()->getObject()->Release();

	return true;
}

#pragma endregion