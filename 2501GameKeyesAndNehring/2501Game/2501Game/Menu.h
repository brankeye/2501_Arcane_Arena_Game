#pragma once

#include "Text.h"

class Menu
{
public:
	Menu();
	Menu(int screenW, int screenH);

	bool stopIntroMusic; // for when player presses play
	bool playPressed, optionsPressed, quitPressed, musicMuted, soundMuted;
	bool pmResumePressed, pmQuitPressed; // pm = pause menu
	bool isGameOver;
	int screenWidth, screenHeight;
	int buttonWidth, buttonHeight;

	int pmButtonWidth, pmButtonHeight;

	void DrawMenu(XMFLOAT2 mouseLoc, bool leftMouseClicked, ID3D11DeviceContext* g_pImmediateContext, ID3D11Device* g_pd3dDevice);
	void DrawMiniMenu(XMFLOAT2 mouseLoc, bool leftMouseClicked, ID3D11DeviceContext* g_pImmediateContext, ID3D11Device* g_pd3dDevice);
	void DrawGameOver(XMFLOAT2 mouseLoc, bool leftMouseClicked, ID3D11DeviceContext* g_pImmediateContext, ID3D11Device* g_pd3dDevice);

	Text play, options, quit, back, music, sound;
	Text pmResume, pmMusic, pmSound, pmQuit;
	Text gameOver, quitMenu, quitDesktop;
};

