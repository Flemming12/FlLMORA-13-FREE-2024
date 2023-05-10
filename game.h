#pragma once
#include <cstdio> //printf
#include "LDtkLoader/Project.hpp"
#include "player.h"
#include "surface.h"
#include <fstream>
#include <strstream>

#include <Audio/Sound.hpp>
#include <Audio/Device.hpp>

namespace Tmpl8 {
class CollisionClass {
	public:
		std::vector<std::reference_wrapper<ldtk::Entity>> collision;
	};
class Surface;
class Game
{
public:
	Game();
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp(int button) { mouseUp = button;}
	void MouseDown(int button) { mouseDown = button;}
	void MouseMove(int x, int y) 
	{ 
		mouseX = x;
		mouseY = y;
	}
	void KeyUp( int key ) { }
	void KeyDown( int key ) 
	{ 
		keyDown = key;
	}
private:
	Surface* screen;
	int mouseX, mouseY, mouseUp, mouseDown, keyDown, gameTimer, gameTimerMinutes, gameTimerSeconds, gameTimerMilliseconds, highscoreMinutes, highscoreSeconds, highscoreMilliseconds, scaleWidth, scaleHeight, offset, playerWidth, playerHeigth, highscore, backgroundWidth, backgroundHeight;
	bool isGrounded, click, startMenu, pauseMenu, controlsMenu, finished, hitboxes;
	float scale;
	Sprite startButton, quitButton, background, continueButton, menuButton, controlsButton, backButton, controlMenu, finishScreen, timerBackground, playerSpriteR, playerSpriteL;

	CollisionClass collisions;
	ldtk::Project ldtk_project;
	Player player;
	Camera camera;

	std::string timerString;
	std::fstream myFile;

	Audio::Sound slime{ "assets/Audio/slime.wav", Audio::Sound::Type::Sound };
	Audio::Sound bounce{ "assets/Audio/bounce.wav", Audio::Sound::Type::Sound };
};

}; // namespace Tmpl8