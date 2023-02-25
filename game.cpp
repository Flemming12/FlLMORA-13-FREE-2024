#include "game.h"
#include "surface.h"
#include "template.h"
#include "windows.h"
#include <cstdio> //printf
#include "collision.h"
#include <typeinfo>

namespace Tmpl8
{
	

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		ldtk_project.loadFromFile("map/ldtk/testmap.ldtk");
		const auto& world = ldtk_project.getWorld("");
		const auto& level1 = world.getLevel("Level_1");
		const auto& entities = level1.getLayer("Entities");
		collisions.collision = entities.getEntitiesByName("Collision");
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	float camerax = 0;
	float cameray = 0;

	Sprite background(new Surface("map/ldtk/testmap/simplified/Level_1/IntGrid.png"), 1);
	int backgroundWidth = background.GetWidth();
	int backgroundHeight = background.GetHeight();
	float scale = 4;
	int scaleWidth = backgroundWidth * scale;
	int scaleHeight = backgroundHeight * scale;
	int offset = scale * backgroundHeight - 512; 
	Sprite player(new Surface("assets/slime.png"), 1);
	int playerWidth = player.GetWidth();
	int playerHeigth = player.GetHeight();
	float playerx = (4 * 8 * scale - 8 * scale - camerax) + (player.GetWidth() * scale / 4);
	float playery = ((512 - (8 * 8 * scale)) + cameray) + (player.GetHeight() * scale / 4);
	int playerr = scale * 4;

	float speedX = 0.2 * scale;
	float speedY = -0.5 * scale;
	float gravity = 0.3;
	float elasticity = 0.8; // coefficient of restitution
	int bounceCount = 0;
	int maxBounces = 8; // maximum number of bounces
	float restThreshold = 3; // threshold velocity for resting


    // -----------------------------------------------------------
    // Main application tick function
    // -----------------------------------------------------------

	void Game::Tick(float deltaTime)
	{
		//printf("%f\n", speedY);
		//camerax = playerx;
		screen->Clear(255);
		background.DrawScaled(0 - camerax, (512 - scaleHeight + cameray), scaleWidth, scaleHeight, screen);


		//printf("x: %i", playerx);
		//printf("%f\n", speedY);
		playerx += speedX;
		playery += speedY;
		speedY += gravity;
		//Bounding box level
		for (auto& collision : collisions.collision)
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
			int cx = playerx - camerax;
			int cy = playery + cameray;
			int cr = playerr;
			bool hasCollision;
			bool floor;
			CollisionCircleAABB(cx, cy, playerr, min_x, min_y, max_x, max_y, &hasCollision, &floor);
			if (hasCollision)
			{
				if (floor)
				{
					speedX = -speedX;
					speedY = speedY;

					color = (255 << 16) + (238 << 8);
				}
				else
				{
					
					speedX = speedX;
					speedY = -speedY * elasticity;
					playery = playery + speedY;
					color = (255 << 8) + 13;
					printf("%f\n", speedY);
					if (abs(speedY) < restThreshold) {
						// Ball has come to rest
						speedY = 0;
						speedX = 0;
					}
					//bounceCount++;
					//if (bounceCount >= maxBounces) {
					//	// Ball has reached maximum number of bounces
					//	speedY = 0;
					//}
				}
							
			}
			screen->Box(min_x, min_y, max_x, max_y, color);
		}


		player.DrawScaled((playerx - (player.GetWidth() * scale / 4)) - camerax, (playery - (player.GetHeight() * scale / 4)) + cameray, (8 * scale), (8 * scale), screen);
		int cx = playerx - camerax;
		int cy = playery + cameray;
		int playercolor = (234 << 16) + (255);
		screen->DrawCircle(cx, cy, playerr, playercolor);
		//printf("");
	}
};