#include "Menu.h"

Menu::Menu() {}

Menu::Menu(int screenW, int screenH)
{
	playPressed = false;
	optionsPressed = false;
	quitPressed = false;
	musicMuted = false;
	soundMuted = false;
	stopIntroMusic = false;

	pmResumePressed = false;
	pmQuitPressed = false;

	isGameOver = false;

	screenWidth = screenW;
	screenHeight = screenH;
	buttonWidth = 300;
	buttonHeight = 100;

	pmButtonWidth = 300;
	pmButtonHeight = 60;

	play = Text(screenW / 8, screenH * 0.2f, 1.0f, Colors::Yellow);
	options = Text(screenW / 8, screenH * 0.4f, 1.0f, Colors::Yellow);
	quit = Text(screenW / 8, screenH * 0.6f, 1.0f, Colors::Yellow);

	music = Text(screenW / 8, screenH * 0.2f, 1.0f, Colors::Yellow);
	sound = Text(screenW / 8, screenH * 0.4f, 1.0f, Colors::Yellow);
	back = Text(screenW / 8, screenH * 0.6f, 1.0f, Colors::Yellow);

	pmResume = Text(screenW / 2.7f, screenH * 0.325f, 0.4f, Colors::Yellow);
	pmMusic = Text(screenW / 2.7f, screenH * 0.425f, 0.4f, Colors::Yellow);
	pmSound = Text(screenW / 2.7f, screenH * 0.525f, 0.4f, Colors::Yellow);
	pmQuit = Text(screenW / 2.7f, screenH * 0.625f, 0.4f, Colors::Yellow);

	gameOver = Text(screenW / 2.7f, screenH * 0.325f, 0.4f, Colors::Yellow);
	quitMenu = Text(screenW / 2.7f, screenH * 0.425f, 0.4f, Colors::Yellow);
	quitDesktop = Text(screenW / 2.7f, screenH * 0.525f, 0.4f, Colors::Yellow);
}

void Menu::DrawMenu(XMFLOAT2 mouseLoc, bool leftMouseClicked, ID3D11DeviceContext* g_pImmediateContext, ID3D11Device* g_pd3dDevice )
{
	XMVECTOR inactiveColor = Colors::White;
	XMVECTOR activeColor   = Colors::DarkCyan;

	if(!optionsPressed) {
		Text::drawText(L"Arcane Arena", screenWidth / 8, screenHeight * 0.025f, 1.0f, activeColor, g_pImmediateContext, g_pd3dDevice );
		buttonWidth = 220;
		if(mouseLoc.x < play.x || mouseLoc.x > play.x + buttonWidth || mouseLoc.y < play.y || mouseLoc.y > play.y + buttonHeight ) {
			Text::drawText(L"Play", play.x, play.y, play.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
		} else {
			if(leftMouseClicked) {
				playPressed = true;
				stopIntroMusic = true;
			}
			Text::drawText(L"Play", play.x, play.y, play.size, activeColor, g_pImmediateContext, g_pd3dDevice );
		}
	
		buttonWidth = 360;
		if(mouseLoc.x < options.x || mouseLoc.x > options.x + buttonWidth || mouseLoc.y < options.y || mouseLoc.y > options.y + buttonHeight ) {
			Text::drawText(L"Options", options.x, options.y, options.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
		} else {
			if(leftMouseClicked) {
				optionsPressed = true;
			}
			Text::drawText(L"Options", options.x, options.y, options.size, activeColor, g_pImmediateContext, g_pd3dDevice );
		}

		buttonWidth = 220;
		if(mouseLoc.x < quit.x || mouseLoc.x > quit.x + buttonWidth || mouseLoc.y < quit.y || mouseLoc.y > quit.y + buttonHeight ) {
			Text::drawText(L"Quit", quit.x, quit.y, quit.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
		} else {
			if(leftMouseClicked) {
				quitPressed = true;
				exit(1);
			}
			Text::drawText(L"Quit", quit.x, quit.y, quit.size, activeColor, g_pImmediateContext, g_pd3dDevice );
		}
		Text::drawText(L"By Jeff and Brandon", screenWidth / 8, screenHeight * 0.8f, 1.0f, activeColor, g_pImmediateContext, g_pd3dDevice );
	} else {
		buttonWidth = 500;
		if(mouseLoc.x < music.x || mouseLoc.x > music.x + buttonWidth || mouseLoc.y < music.y || mouseLoc.y > music.y + buttonHeight ) {
			if(!musicMuted) {
				Text::drawText(L"Music [ON]", music.x, music.y, music.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
			} else {
				Text::drawText(L"Music [OFF]", music.x, music.y, music.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
			}
		} else {
			if(leftMouseClicked) {
				if(musicMuted) {
					musicMuted = false;
				} else {
					musicMuted = true;
				}
			}
			if(!musicMuted) {
				Text::drawText(L"Music [ON]", music.x, music.y, music.size, activeColor, g_pImmediateContext, g_pd3dDevice );
			} else {
				Text::drawText(L"Music [OFF]", music.x, music.y, music.size, activeColor, g_pImmediateContext, g_pd3dDevice );
			}
		}

		buttonWidth = 870;
		if(mouseLoc.x < sound.x || mouseLoc.x > sound.x + buttonWidth || mouseLoc.y < sound.y || mouseLoc.y > sound.y + buttonHeight ) {
			if(!soundMuted) {
				Text::drawText(L"Sound Effects [ON]", sound.x, sound.y, sound.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
			} else {
				Text::drawText(L"Sound Effects [OFF]", sound.x, sound.y, sound.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
			}
		} else {
			if(leftMouseClicked) {
				if(soundMuted) {
					soundMuted = false;
				} else {
					soundMuted = true;
				}
			}
			if(!soundMuted) {
				Text::drawText(L"Sound Effects [ON]", sound.x, sound.y, sound.size, activeColor, g_pImmediateContext, g_pd3dDevice );
			} else {
				Text::drawText(L"Sound Effects [OFF]", sound.x, sound.y, sound.size, activeColor, g_pImmediateContext, g_pd3dDevice );
			}
		}

		buttonWidth = 220;
		if(mouseLoc.x < back.x || mouseLoc.x > back.x + buttonWidth || mouseLoc.y < back.y || mouseLoc.y > back.y + buttonHeight ) {
			Text::drawText(L"Back", back.x, back.y, back.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
		} else {
			if(leftMouseClicked) {
				optionsPressed = false;
			}
			Text::drawText(L"Back", back.x, back.y, back.size, activeColor, g_pImmediateContext, g_pd3dDevice );
		}
	}
}

void Menu::DrawMiniMenu(XMFLOAT2 mouseLoc, bool leftMouseClicked, ID3D11DeviceContext* g_pImmediateContext, ID3D11Device* g_pd3dDevice) {
	XMVECTOR inactiveColor = Colors::White;
	XMVECTOR activeColor   = Colors::DarkCyan;

	pmButtonWidth = 350;
	if(mouseLoc.x < pmResume.x || mouseLoc.x > pmResume.x + pmButtonWidth || mouseLoc.y < pmResume.y || mouseLoc.y > pmResume.y + pmButtonHeight ) {
		Text::drawText(L"Resume", pmResume.x, pmResume.y, pmResume.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
	} else {
		if(leftMouseClicked) {
			pmResumePressed = true;
		}
		Text::drawText(L"Resume", pmResume.x, pmResume.y, pmResume.size, activeColor, g_pImmediateContext, g_pd3dDevice );
	}

	pmButtonWidth = 350;
	if(mouseLoc.x < pmMusic.x || mouseLoc.x > pmMusic.x + pmButtonWidth || mouseLoc.y < pmMusic.y || mouseLoc.y > pmMusic.y + pmButtonHeight ) {
		if(!musicMuted) {
			Text::drawText(L"Music [ON]", pmMusic.x, pmMusic.y, pmMusic.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
		} else {
			Text::drawText(L"Music [OFF]", pmMusic.x, pmMusic.y, pmMusic.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
		}
	} else {
		if(leftMouseClicked) {
			if(musicMuted) {
				musicMuted = false;
			} else {
				musicMuted = true;
			}
		}
		if(!musicMuted) {
			Text::drawText(L"Music [ON]", pmMusic.x, pmMusic.y, pmMusic.size, activeColor, g_pImmediateContext, g_pd3dDevice );
		} else {
			Text::drawText(L"Music [OFF]", pmMusic.x, pmMusic.y, pmMusic.size, activeColor, g_pImmediateContext, g_pd3dDevice );
		}
	}

	pmButtonWidth = 350;
	if(mouseLoc.x < pmSound.x || mouseLoc.x > pmSound.x + pmButtonWidth || mouseLoc.y < pmSound.y || mouseLoc.y > pmSound.y + pmButtonHeight ) {
		if(!soundMuted) {
			Text::drawText(L"Sound Effects [ON]", pmSound.x, pmSound.y, pmSound.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
		} else {
			Text::drawText(L"Sound Effects [OFF]", pmSound.x, pmSound.y, pmSound.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
		}
	} else {
		if(leftMouseClicked) {
			if(soundMuted) {
				soundMuted = false;
			} else {
				soundMuted = true;
			}
		}
		if(!soundMuted) {
			Text::drawText(L"Sound Effects [ON]", pmSound.x, pmSound.y, pmSound.size, activeColor, g_pImmediateContext, g_pd3dDevice );
		} else {
			Text::drawText(L"Sound Effects [OFF]", pmSound.x, pmSound.y, pmSound.size, activeColor, g_pImmediateContext, g_pd3dDevice );
		}
	}

	pmButtonWidth = 350;
	if(mouseLoc.x < pmQuit.x || mouseLoc.x > pmQuit.x + pmButtonWidth || mouseLoc.y < pmQuit.y || mouseLoc.y > pmQuit.y + pmButtonHeight ) {
		Text::drawText(L"Quit", pmQuit.x, pmQuit.y, pmQuit.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
	} else {
		if(leftMouseClicked) {
			pmQuitPressed = true;
		}
		Text::drawText(L"Quit", pmQuit.x, pmQuit.y, pmQuit.size, activeColor, g_pImmediateContext, g_pd3dDevice );
	}
}

void Menu::DrawGameOver(XMFLOAT2 mouseLoc, bool leftMouseClicked, ID3D11DeviceContext* g_pImmediateContext, ID3D11Device* g_pd3dDevice)
{
	XMVECTOR inactiveColor = Colors::White;
	XMVECTOR activeColor   = Colors::DarkCyan;

	Text::drawText(L"GAME OVER", gameOver.x, gameOver.y, gameOver.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );

	pmButtonWidth = 350;
	if(mouseLoc.x < quitMenu.x || mouseLoc.x > quitMenu.x + pmButtonWidth || mouseLoc.y < quitMenu.y || mouseLoc.y > quitMenu.y + pmButtonHeight ) {
		Text::drawText(L"Quit to Menu", quitMenu.x, quitMenu.y, quitMenu.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
	} else {
		if(leftMouseClicked) {
			playPressed = false;
			isGameOver = false;
			stopIntroMusic = true;
		}
		Text::drawText(L"Quit to Menu", quitMenu.x, quitMenu.y, quitMenu.size, activeColor, g_pImmediateContext, g_pd3dDevice );
	}

	pmButtonWidth = 350;
	if(mouseLoc.x < quitDesktop.x || mouseLoc.x > quitDesktop.x + pmButtonWidth || mouseLoc.y < quitDesktop.y || mouseLoc.y > quitDesktop.y + pmButtonHeight ) {
		Text::drawText(L"Quit to Desktop", quitDesktop.x, quitDesktop.y, quitDesktop.size, inactiveColor, g_pImmediateContext, g_pd3dDevice );
	} else {
		if(leftMouseClicked) {
			pmQuitPressed = true;
		}
		Text::drawText(L"Quit to Desktop", quitDesktop.x, quitDesktop.y, quitDesktop.size, activeColor, g_pImmediateContext, g_pd3dDevice );
	}
}