#include "game.h"
#include "template.h"
#include "windows.h"
#include <cstdio> //printf
#include "collision.h"
#include <typeinfo>
#include "player.h"
#include <SDL_keycode.h>

#include <Audio/Sound.hpp>
#include <Audio/Device.hpp>

namespace Tmpl8
{

	Game::Game()
		:startButton(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/StartButtons.png"), 2)
		,quitButton(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/QuitButtons.png"), 2)
		,continueButton(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/ContinueButtons.png"), 2)
		,menuButton(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/MenuButtons.png"), 2)
		,controlsButton(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/ControlsButtons.png"), 2)
		,backButton(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/BackButtons.png"), 2)
		,controlMenu(new Surface("assets/Menu Buttons/ControlMenu.png"), 1)
		,background(new Surface("map/ldtk/testmap/simplified/Level_0/_composite.png"), 1)
		,playerSprite(new Surface("assets/slime.png"), 1)
	{
		backgroundWidth = background.GetWidth();
		backgroundHeight = background.GetHeight();
		scale = 4;
		scaleWidth = backgroundWidth * scale;
		scaleHeight = backgroundHeight * scale;
		offset = scale * backgroundHeight - 512;

		playerWidth = playerSprite.GetWidth();
		playerHeigth = playerSprite.GetHeight();

		startMenu = true; //ffff

		//screenWidth = 800;
		//screenHeight = 512;
	}

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
		player.x = 80.8;
		player.y = 432;
		player.r = 16;
		player.speedx = 0;
		player.speedy = 0;
		camera.x = 0;
		camera.y = 0;
		mouseUp = 0;
		mouseDown = 0;
		keyDown = 0;
		isGrounded = false;
		click = false;
		pauseMenu = false;
		controlsMenu = false;
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	//float camera.x = 0;
	//float camera.y = 0;



	//Sprite startButton(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/Start Button.png"), 1);
	//Sprite startButtonActive(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/Start Button2.png"), 1);
	//
	//Sprite quitButton(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/Quit Button.png"), 1);
	//Sprite quitButtonActive(new Surface("assets/Menu Buttons/Large Buttons/Large Buttons/Quit Button2.png"), 1);

	//float player.x = (3 * 8 * scale - 8 * scale - camera.x) + (playerSprite.GetWidth() * scale / 4);
	//float player.y = ((512 - (3 * 8 * scale)) + camera.y) + (playerSprite.GetHeight() * scale / 4);
	//int player.r = scale * 4;

	//float player.speedx = 0.1 * scale;
	//float player.speedy = 0.1 * scale;
	//float gravity = 0.3;
	//float elasticity = 0.8; // coefficient of restitution
	//int bounceCount = 0;
	//int maxBounces = 8; // maximum number of bounces
	//float restThreshold = 3.2; // threshold velocity for resting

	//bool isGrounded = false;
	//bool click = false;
	//bool startMenu = true;
	//bool pauseMenu = false;

	//float player.x = 0;
	//float player.y = 0;

	//int camx1, camx2, camy1, camy2;
	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------

	void Game::Tick(float deltaTime)
	{
		if (mouseDown) {
			click = true;
			mouseUp = false;
		}
		if (mouseUp) {
			mouseDown = false;
		}

		if (startMenu) {
			screen->Clear((14 << 16) + (7 << 8) + 27);
			if (mouseX > 250 && mouseX < 550 && mouseY > 86 && mouseY < 186) {
				startButton.SetFrame(1);
				startButton.Draw(screen, 235, 81);
				if (click) {

					Init();
					startMenu = false;
					click = false;
					mouseDown = false;
				}
			}
			else {
				startButton.SetFrame(0);
				startButton.Draw(screen, 235, 81);
			}
			if (mouseX > 250 && mouseX < 550 && mouseY > 206 && mouseY < 306) {
				controlsButton.SetFrame(1);
				controlsButton.Draw(screen, 235, 201);
				if (click) {
					startMenu = false;
					click = false;
					mouseDown = false;
					controlsMenu = true;
				}
			}
			else {
				controlsButton.SetFrame(0);
				controlsButton.Draw(screen, 235, 201);
			}

			if (mouseX > 250 && mouseX < 550 && mouseY > 326 && mouseY < 426) {
				quitButton.SetFrame(1);
				quitButton.Draw(screen, 235, 321);
				if (click) {
					exit(0);
				}
			}
			else {
				quitButton.SetFrame(0);
				quitButton.Draw(screen, 235, 321);
			}
		}
		else if (controlsMenu) {
			screen->Clear((14 << 16) + (7 << 8) + 27);
			controlMenu.Draw(screen, 0, 0);
			if (mouseX > 250 && mouseX < 550 && mouseY > 392 && mouseY < 492) {
				backButton.SetFrame(1);
				backButton.Draw(screen, 235, 387);
				if (click) {
					controlsMenu = false;
					click = false;
					mouseDown = false;
					startMenu = true;
				}
			}
			else {
				backButton.SetFrame(0);
				backButton.Draw(screen, 235, 387);
			}
		}
		else {

			//printf("%f", player.x);
			printf(" %f\n", player.speedy);
			//float player.x = player.x;
			//float player.y = player.y;
			screen->Clear((14 << 16) + (7 << 8) + 27);
			background.DrawScaled(0 - camera.x, (512 - scaleHeight + camera.y), scaleWidth, scaleHeight, screen);

			//screen->Line(test, 0, test, 512, (234 << 16) + (255));			

			camera.x = player.x - 400;
			camera.y = -player.y + 256;
			if (camera.x < 0) {
				camera.x = 0;
			}
			if (camera.y < 0) {
				camera.y = 0;
			}
			if (camera.x > 352) {
				camera.x = 352;
			}
			if (camera.y > 1088) {
				camera.y = 1088;
			}

			//printf("%f", camera.x - camera.x);
			//printf(" %f\n", camera.y - camera.y);


			//printf("%i ", mouseDown);
			//printf("%i\n", mouseUp);
			if (pauseMenu == false) {
				if (isGrounded == false) {
					player.x += player.speedx * deltaTime / 16.5;
					//printf("speed x%f\n", player.speedx);
					player.y += player.speedy * deltaTime / 16.5;
					player.speedy += 0.3 * deltaTime / 16.5;
					if (player.speedy > 30) {
						player.speedy = 30;
					}
					for (auto& collision : collisions.collision)
					{
						auto& e = collision.get();
						auto& pos = e.getPosition();
						auto& size = e.getSize();
						auto& finish = e.getField<ldtk::FieldType::Bool>("Finish");
						auto& sticky = e.getField<ldtk::FieldType::Bool>("Sticky");
						auto& isSticky = sticky.value();
						auto& isFinish = finish.value();
						int color = 0;
						if (isSticky == 1)
						{
							color = (255 << 16) + (153 << 8);
						}
						else
						{
							color = (255 << 16);
						}
						int min_x = ((pos.x * scale) - camera.x);
						int min_y = ((pos.y * scale - offset) + camera.y);
						int max_x = (((pos.x + size.x) * scale) - camera.x);
						int max_y = (((pos.y + size.y) * scale - offset) + camera.y);
						int cx = player.x - camera.x;
						int cy = player.y + camera.y;
						int cr = player.r;

						bool hasCollision;
						int floor;
						CollisionCircleAABB(cx, cy, player.r, min_x, min_y, max_x, max_y, &hasCollision, &floor, player.speedx, player.speedy);
						if (hasCollision)
						{
							// 1 = |O max_x -- 2 = O| min_x -- 3 = ō max_y -- 4 = ⍜ min_y
							//if (floor != -858993460) {
							//	printf("%i\n", floor);
							//}
							if (isFinish) {
								Init();
							}
							if (floor == 1 || floor == 2)
							{
								if (floor == 1) {
									player.x = max_x + camera.x + playerSprite.GetWidth(); // + player.GetHeight()
								}
								else if (floor == 2) {
									player.x = min_x + camera.x - playerSprite.GetWidth() - 1; // + player.GetHeight()
								}


								player.speedx = -player.speedx;
								player.speedy = player.speedy;
								if (isSticky == 1) {
									isGrounded = true;
									player.speedx = 0;
									player.speedy = 0;
									slime.replay();
								}
								else {
									bounce.replay();
								}
								color = (255 << 16) + (238 << 8);
							}
							else if (floor == 3 || floor == 4)
							{
								if (floor == 4) {
									player.y = min_y - camera.y - playerSprite.GetHeight(); // + player.GetHeight()
								}
								else if (floor == 3) {
									player.y = max_y - camera.y + playerSprite.GetHeight(); // + player.GetHeight()
								}
								if (isSticky == 1) {
									isGrounded = true;
									player.speedx = 0;
									player.speedy = 0;
									slime.replay();
								}
								else {
									bounce.replay();
								}
								//bounceCount++;
								//if (abs(player.speedy) < 3.2 && bounceCount > 8) {
								//	player.speedy = 0;
								//	player.speedx = 0;
								//}
								player.speedx = player.speedx;
								player.speedy = -player.speedy * 0.8;
								color = (255 << 8) + 13;
							}
						}

						//screen->Line(min_x, min_y, min_x, max_y, color); //min x
						//screen->Line(max_x, min_y, max_x, max_y, color); //max x
						//screen->Line(min_x, min_y, max_x, min_y, color); //min y
						//screen->Line(min_x, max_y, max_x, max_y, color); //max y

						//screen->Box(min_x, min_y, max_x, max_y, color);
					}
				}
				int cx = player.x - camera.x;
				int cy = player.y + camera.y;
				if (isGrounded) {
					int distance = sqrt((mouseX - cx) * (mouseX - cx) + (mouseY - cy) * (mouseY - cy));
					if (distance > (4 * scale)) {
						int arrowX = cx + ((15 * scale) / distance) * (mouseX - cx);
						int arrowY = cy + ((15 * scale) / distance) * (mouseY - cy);
						screen->ThickLine(cx, cy, arrowX, arrowY, (0.75 * scale), (77 << 16) + (70 << 8) + 92);
						if (click) {
							float launchX = arrowX - cx;
							float launchY = arrowY - cy;
							launchX = launchX / 4.5;
							launchY = launchY / 4.5;
							//printf("%f ", launchX);
							//printf("%f\n", launchY);
							player.speedx = launchX;
							player.speedy = launchY;
							isGrounded = false;

							click = false;
							mouseDown = false;
							//bounceCount = 0;
						}
					}
				}
			}
			playerSprite.DrawScaled((player.x - (playerSprite.GetWidth() * scale / 4)) - camera.x, (player.y - (playerSprite.GetHeight() * scale / 4)) + camera.y, (8 * scale), (8 * scale), screen);
			int playercolor = (234 << 16) + (255);
			//screen->DrawCircle(cx, cy, player.r, playercolor)

			if (keyDown == SDL_SCANCODE_ESCAPE) {
				if (pauseMenu == false) {
					keyDown = 0;
					pauseMenu = true;
				}
				else {
					keyDown = 0;
					pauseMenu = false;
				}
			}

			if (keyDown == SDL_SCANCODE_R) {
				Init();
			}

			if (pauseMenu) {
				if (mouseX > 250 && mouseX < 550 && mouseY > 146 && mouseY < 246) {
					continueButton.SetFrame(1);
					continueButton.Draw(screen, 235, 141);
					if (click) {
						pauseMenu = false;
						click = false;
						mouseDown = false;
					}
				}
				else {
					continueButton.SetFrame(0);
					continueButton.Draw(screen, 235, 141);
				}
				if (mouseX > 250 && mouseX < 550 && mouseY > 266 && mouseY < 366) {
					menuButton.SetFrame(1);
					menuButton.Draw(screen, 235, 261);
					if (click) {
						pauseMenu = false;
						startMenu = true;
						click = false;
						mouseDown = false;
					}
				}
				else {
					menuButton.SetFrame(0);
					menuButton.Draw(screen, 235, 261);
				}
			}

		}
	}
};