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
	int offset = scale * backgroundHeight - 512; 
	Sprite player(new Surface("assets/slime.png"), 1);
	int camerax = 0;
	int cameray = 0;
    // -----------------------------------------------------------
    // Main application tick function
    // -----------------------------------------------------------

	void Game::Tick(float deltaTime)
	{
		screen->Clear(255);
		background.DrawScaled(0-camerax, (512 - scaleHeight+cameray), scaleWidth, scaleHeight, screen);
		player.DrawScaled(((2 * 12 * scale)-camerax), ((512 - (2 * 12 * scale))+cameray), (8 * scale), (8 * scale), screen);
		ldtk::Project ldtk_project;
		ldtk_project.loadFromFile("map/ldtk/testmap.ldtk");
		const auto& world = ldtk_project.getWorld("");
		const auto& level1 = world.getLevel("Level_0");
		const auto& entities = level1.getLayer("Entities");
		const auto& collisions = entities.getEntitiesByName("Collision");
		for (auto& collision : collisions)
		{
			auto& e = collision.get();
			auto& pos = e.getPosition();
			auto& size = e.getSize();
			auto& sticky = e.getField<ldtk::FieldType::Bool>("Sticky");
			auto& isSticky = sticky.value();
			int color = 0;
			if (isSticky == 1)
			{
				color = (255 << 16) + (153 << 8);
			}
			else
			{
				color = (255 << 16);
			}
			printf("color: %i\n", backgroundHeight);
			screen->Box(((pos.x*scale)-camerax), ((pos.y*scale- offset)+cameray), (((pos.x + size.x)*scale)-camerax), (((pos.y+size.y)*scale- offset)+cameray), color);
		}
		printf("");
	}
};