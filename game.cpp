#include "game.h"
#include "surface.h"
#include "template.h"
#include "windows.h"
#include <cstdio> //printf
#include "LDtkLoader/Project.hpp"
#include "collision.h"

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		ldtk::Project ldtk_project;
		ldtk_project.loadFromFile("map/ldtk/testmap.ldtk");
		const auto& world = ldtk_project.getWorld("");
		const auto& level1 = world.getLevel("Level_0");
		const auto& entities = level1.getLayer("Entities");
		const auto& collisions = entities.getEntitiesByName("Collision");
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	int camerax = 0;
	int cameray = 0;

	Sprite background(new Surface("map/ldtk/testmap/simplified/Level_0/IntGrid.png"), 1);
	int backgroundWidth = background.GetWidth();
	int backgroundHeight = background.GetHeight();
	float scale = 4;
	int scaleWidth = backgroundWidth * scale;
	int scaleHeight = backgroundHeight * scale;
	int offset = scale * backgroundHeight - 512; 
	Sprite player(new Surface("assets/slime.png"), 1);
	float playerx = ((2 * 12 * scale) - camerax);
	float playery = ((512 - (5 * 12 * scale)) + cameray);
	int playerr = scale * 8;

	float speedX = 0;
	float speedY = -3 * -1 *scale;

    // -----------------------------------------------------------
    // Main application tick function
    // -----------------------------------------------------------

	void Game::Tick(float deltaTime)
	{
		//camerax = mouseX;
		//cameray = mouseY;
		screen->Clear(255);
		background.DrawScaled(0 - camerax, (512 - scaleHeight + cameray), scaleWidth, scaleHeight, screen);

		playerx += speedX;
		playery += speedY;
		//printf("num: %i", num);
		//printf("x: %i", playerx);
		//printf(" y: %i\n", playerx);
		
		//Bounding box player
		int cx = playerx + (player.GetWidth() * scale / 4) + scale / 4;
		int cy = playery + (player.GetHeight() * scale / 4) + scale / 4;
		int playercolor = (234<<16) + (255);
		screen->DrawCircle(cx, cy, playerr/2, playercolor);
		//Bounding box level
		
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
			int min_x = ((pos.x * scale) - camerax);
			int min_y = ((pos.y * scale - offset) + cameray);
			int max_x = (((pos.x + size.x) * scale) - camerax);
			int max_y = (((pos.y + size.y) * scale - offset) + cameray);
			bool hasCollision = CollisionCircleAABB(playerx, playery, playerr, min_x, min_y, max_x, max_y);
			if (hasCollision)
			{
				color = (34 << 16) + (255 << 8);
			}
			screen->Box(min_x, min_y, max_x, max_y, color);
		}
		player.DrawScaled(playerx, playery, (8 * scale), (8 * scale), screen);
		
		printf("");
	}
};