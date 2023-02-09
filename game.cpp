#include "game.h"
#include "surface.h"
#include <cstdio> //printf


namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	Sprite background(new Surface("map/ldtk/testmap/simplified/Level_0/IntGrid.png"), 1);
	int backgroundWidth = background.GetWidth();
	int backgroundHeight = background.GetHeight();
	int scale = 4;
	int scaleWidth = backgroundWidth*scale;
	int scaleHeight = backgroundHeight*scale;
	Sprite player(new Surface("assets/slime.png"), 1);
	Sprite player2(new Surface("assets/slime.png"), 1);

    // -----------------------------------------------------------
    // Main application tick function
    // -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		screen->Clear(0);
		background.DrawScaled(0, (512 - scaleHeight), scaleWidth, scaleHeight, screen);
		player.DrawScaled(100, (512-(2*12*scale)), (8 * scale), (8 * scale), screen);
		//printf("%i\n", scale);
		//scale++;
	}
};