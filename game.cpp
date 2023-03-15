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
		const auto& level1 = world.getLevel("Level_0");
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

	Sprite background(new Surface("map/ldtk/testmap/simplified/Level_0/IntGrid.png"), 1);
	int backgroundWidth = background.GetWidth();
	int backgroundHeight = background.GetHeight();
	float scale = 4;
	int scaleWidth = backgroundWidth * scale;
	int scaleHeight = backgroundHeight * scale;
	int offset = scale * backgroundHeight - 512; 
	Sprite player(new Surface("assets/slime.png"), 1);
	int playerWidth = player.GetWidth();
	int playerHeigth = player.GetHeight();
	float playerx = (3 * 8 * scale - 8 * scale - camerax) + (player.GetWidth() * scale / 4);
	float playery = ((512 - (3 * 8 * scale)) + cameray) + (player.GetHeight() * scale / 4);
	int playerr = scale * 4;

	float speedX = 0 * scale;
	float speedY = 0 * scale;
	float gravity = 0.3;
	float elasticity = 0.8; // coefficient of restitution
	int bounceCount = 0;
	int maxBounces = 8; // maximum number of bounces
	float restThreshold = 3.2; // threshold velocity for resting

	bool isGrounded = false;
	bool click = false;

	int biggest = 0;

    // -----------------------------------------------------------
    // Main application tick function
    // -----------------------------------------------------------

	void Game::Tick(float deltaTime)
	{
		screen->Clear(255);
		background.DrawScaled(0 - camerax, (512 - scaleHeight + cameray), scaleWidth, scaleHeight, screen);
		//screen->Line(test, 0, test, 512, (234 << 16) + (255));
				
		camerax = playerx - 400;
		cameray = -playery + 256;
		if (mouseDown) {
			click = true;
			mouseUp = false;
		}
		if (mouseUp) {
			mouseDown = false;
		}
		//printf("%i ", mouseDown);
		//printf("%i\n", mouseUp);
		
		if (isGrounded == false) {
			playerx += speedX;
			playery += speedY;
			speedY += gravity;
		}
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
			int floor;
			CollisionCircleAABB(cx, cy, playerr, min_x, min_y, max_x, max_y, &hasCollision, &floor, speedX, speedY);
			if (hasCollision)
			{
				// 1 = |O max_x -- 2 = O| min_x -- 3 = ō max_y -- 4 = ⍜ min_y
				if (floor != -858993460) {
					printf("%i\n", floor);
				}
				if (floor == 1 || floor == 2)
				{
					if (floor == 1) {
						playerx = max_x + camerax + player.GetWidth(); // + player.GetHeight()
					}
					else if (floor == 2) {
						playerx = min_x + camerax - player.GetWidth() - 1; // + player.GetHeight()
					}
					

					speedX = -speedX;
					speedY = speedY;
					if (isSticky == 1) {
						isGrounded = true;
						speedX = 0;
						speedY = 0;
					}
					color = (255 << 16) + (238 << 8);
				}
				else if (floor == 3 || floor == 4)
				{
					if (floor == 4) {
						playery = min_y - cameray - player.GetHeight(); // + player.GetHeight()
					}
					else if (floor == 3) {
						playery = max_y - cameray + player.GetHeight(); // + player.GetHeight()
					}
					if (isSticky == 1) {
						isGrounded = true;
						speedX = 0;
						speedY = 0;
					}
					bounceCount++;
					if (abs(speedY) < restThreshold && bounceCount > 8) {
						speedY = 0;
						speedX = 0;
					}
					speedX = speedX;
					speedY = -speedY * elasticity;
					color = (255 << 8) + 13;
				}
			}
			if (max_x > biggest) {
				biggest = max_x;
				printf("%i \n", biggest);
			}
			
			//screen->Line(min_x, min_y, min_x, max_y, color); //min x
			//screen->Line(max_x, min_y, max_x, max_y, color); //max x
			//screen->Line(min_x, min_y, max_x, min_y, color); //min y
			//screen->Line(min_x, max_y, max_x, max_y, color); //max y
			
			//screen->Box(min_x, min_y, max_x, max_y, color);
		}
		int cx = playerx - camerax;
		int cy = playery + cameray;
		if (isGrounded) {
			int distance = sqrt((mouseX - cx) * (mouseX - cx)  + (mouseY - cy) * (mouseY - cy));
			if (distance > (4 * scale)) {
				int arrowX = cx + ((15 * scale) / distance) * (mouseX - cx);
				int arrowY = cy + ((15 * scale) / distance) * (mouseY - cy);
				screen->ThickLine(cx, cy, arrowX, arrowY, (0.75 * scale), 255 << 16);
				if (click) {
					float launchX = arrowX - cx;
					float launchY = arrowY - cy;
					launchX = launchX/4.5;
					launchY = launchY/4.5;
					printf("%f ", launchX);
					printf("%f\n", launchY);
					speedX = launchX;
					speedY = launchY;
					isGrounded = false;
					
					click = false;
					mouseDown = false;
					bounceCount = 0;
				}
			}
		}
		player.DrawScaled((playerx - (player.GetWidth() * scale / 4)) - camerax, (playery - (player.GetHeight() * scale / 4)) + cameray, (8 * scale), (8 * scale), screen);
		int playercolor = (234 << 16) + (255);
		//screen->DrawCircle(cx, cy, playerr, playercolor)
	}
};