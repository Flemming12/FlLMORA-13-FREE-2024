#include "game.h"
#include "surface.h"
#include "template.h"
#include "windows.h"
#include <cstdio> //printf
#include "LDtkLoader/Project.hpp"



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
	int scaleWidth = backgroundWidth * scale;
	int scaleHeight = backgroundHeight * scale;
	Sprite player(new Surface("assets/slime.png"), 1);

    // -----------------------------------------------------------
    // Main application tick function
    // -----------------------------------------------------------

	void Game::Tick(float deltaTime)
	{
		//ldtk::Project ldtk_project;
		//ldtk_project.loadFromFile("map/ldtk/testmap.ldtk");
		//const auto& world = ldtk_project.getWorld("testmap");
		//const auto& level = world.getLevel("Level_0");
		//const auto& entities = level.getLayer("Entities");
		//const auto& collisions = entities.getEntitiesByName("Collision");
		//for (auto& collision : collisions)
		//{
		//	auto& e = collision.get();
		//}
		screen->Clear(0);
		background.DrawScaled(0, (512 - scaleHeight), scaleWidth, scaleHeight, screen);
		player.DrawScaled((2*12*scale), (512-(2*12*scale)), (8 * scale), (8 * scale), screen);
	}

};