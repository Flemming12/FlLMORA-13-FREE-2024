#include "game.h"
#include "template.h"
#include "windows.h"
#include <cstdio> //printf
#include "collision.h"
#include <typeinfo>
#include "player.h"
#include <SDL_keycode.h>
#include <string>

#include <Audio/Sound.hpp>
#include <Audio/Device.hpp>

namespace Tmpl8
{

	Game::Game()
		:startButton(new Surface("assets/Menus/Buttons/StartButtons.png"), 2)
		,quitButton(new Surface("assets/Menus/Buttons/QuitButtons.png"), 2)
		,continueButton(new Surface("assets/Menus/Buttons/ContinueButtons.png"), 2)
		,menuButton(new Surface("assets/Menus/Buttons/MenuButtons.png"), 2)
		,controlsButton(new Surface("assets/Menus/Buttons/ControlsButtons.png"), 2)
		,backButton(new Surface("assets/Menus/Buttons/BackButtons.png"), 2)
		,controlMenu(new Surface("assets/Menus/ControlMenu.png"), 1)
		,finishScreen(new Surface("assets/Menus/FinishScreen.png"),1)
		,timerBackground(new Surface("assets/Menus/timerBackground.png"), 1)
		,background(new Surface("assets/map/ldtk/testmap/simplified/Level_0/_composite.png"), 1)
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

		startMenu = true;
		hitboxes = false;
	}

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		ldtk_project.loadFromFile("assets/map/ldtk/testmap.ldtk");
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
		gameTimer = 0;
		gameTimerMinutes = 0;
		gameTimerSeconds = 0;
		gameTimerMilliseconds = 0;
		isGrounded = false;
		click = false;
		pauseMenu = false;
		controlsMenu = false;
		finished = false;
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

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

			screen->Print("HIGH", 572, 216, 14795096, 4);
			screen->Print("SCORE", 572, 246, 14795096, 4);
			
			myFile.open("highscore.txt", std::ios::in);
			if (myFile.is_open()) {
				std::string line;
				while (std::getline(myFile, line)) {
					highscore = std::stoi(line);
				}
				myFile.close();
			}
			
			if (highscore == 100000) {
				screen->Print("__:__:___", 572, 276, 14795096, 4);
			}
			else {
				highscoreMinutes = highscore / (60 * 1000);
				highscoreSeconds = (highscore / 1000) % 60;
				highscoreMilliseconds = highscore % 1000;
				char bufferMainMenu[20];
				sprintf(bufferMainMenu, "%02d:%02d:%03d", highscoreMinutes, highscoreSeconds, highscoreMilliseconds);
				std::string highscoreString(bufferMainMenu);
				//screen->Print(highscoreString.c_str(), 163, 240, (71 << 16) + (119 << 8) + 87, 4);
				screen->Print(highscoreString.c_str(), 572, 276, 14795096, 4);
			}
			
			//screen->Print("00:00:000", 572, 276, (255 << 16) + (255 << 8) + 255, 4);

			//middle 680

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
		else if (finished) {
			screen->Clear((14 << 16) + (7 << 8) + 27);
			finishScreen.Draw(screen, 0, 0);
			
			highscoreMinutes = highscore / (60 * 1000);
			highscoreSeconds = (highscore / 1000) % 60;
			highscoreMilliseconds = highscore % 1000;

			char bufferHighscore[16];
			sprintf(bufferHighscore, "%02d:%02d:%03d", highscoreMinutes, highscoreSeconds, highscoreMilliseconds);
			std::string highscoreTimeString(bufferHighscore);
			std::string highscoreString = "Highscore: " + highscoreTimeString;
			screen->Print(highscoreString.c_str(), 163, 240, (71 << 16) + (119 << 8) + 87, 4);

			char bufferTimer[12];
			sprintf(bufferTimer, "%02d:%02d:%03d", gameTimerMinutes, gameTimerSeconds, gameTimerMilliseconds);
			std::string timerTimeString(bufferTimer);
			std::string timerString = "Time: " + timerTimeString;
			screen->Print(timerString.c_str(), 223, 280, (71 << 16) + (119 << 8) + 87, 4);

			if (mouseX > 250 && mouseX < 550 && mouseY > 392 && mouseY < 492) {
				menuButton.SetFrame(1);
				menuButton.Draw(screen, 235, 387);
				if (click) {
					finished = false;
					click = false;
					mouseDown = false;
					startMenu = true;
				}
			}
			else {
				menuButton.SetFrame(0);
				menuButton.Draw(screen, 235, 387);
			}
		}
		else {
			screen->Clear((14 << 16) + (7 << 8) + 27);
			background.DrawScaled(0 - camera.x, (512 - scaleHeight + camera.y), scaleWidth, scaleHeight, screen);
			timerBackground.Draw(screen, 0, 0);

			gameTimer += deltaTime;
			gameTimerMinutes = gameTimer / (60 * 1000);
			gameTimerSeconds = (gameTimer / 1000) % 60;
			gameTimerMilliseconds = gameTimer % 1000;
			char buffer[12];
			sprintf(buffer, "%02d:%02d:%03d", gameTimerMinutes, gameTimerSeconds, gameTimerMilliseconds);
			std::string timerString(buffer);
			screen->Print(timerString.c_str(), 10, 10, (71 << 16) + (119 << 8) + 87, 4);		

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

			if (pauseMenu == false) {
				if (isGrounded == false) {
					player.x += player.speedx * deltaTime / 16.5;
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
							if (isFinish) {
								myFile.open("highscore.txt", std::ios::in);
								if (myFile.is_open()) {
									std::string line;
									while (std::getline(myFile, line)) {
										highscore = std::stoi(line);
									}
									myFile.close();
								}
								if (gameTimer < highscore) {
									highscore = gameTimer;
									myFile.open("highscore.txt", std::ios::out);
									if (myFile.is_open()) {
										myFile << highscore;
										myFile.close();
									}
								}
								finished = true;
							}
							if (floor == 1 || floor == 2)
							{
								if (floor == 1) {
									player.x = max_x + camera.x + playerSprite.GetWidth();
								}
								else if (floor == 2) {
									player.x = min_x + camera.x - playerSprite.GetWidth() - 1;
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
							}
							else if (floor == 3 || floor == 4)
							{
								if (floor == 4) {
									player.y = min_y - camera.y - playerSprite.GetHeight();
								}
								else if (floor == 3) {
									player.y = max_y - camera.y + playerSprite.GetHeight();
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
								player.speedx = player.speedx;
								player.speedy = -player.speedy * 0.8;
							}
						}
						if (hitboxes) {
							screen->Box(min_x, min_y, max_x, max_y, (255 << 16) + (153 << 8));
						}
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
							player.speedx = launchX;
							player.speedy = launchY;
							isGrounded = false;

							click = false;
							mouseDown = false;
						}
					}
				}
			}
			playerSprite.DrawScaled((player.x - (playerSprite.GetWidth() * scale / 4)) - camera.x, (player.y - (playerSprite.GetHeight() * scale / 4)) + camera.y, (8 * scale), (8 * scale), screen);
			if (hitboxes) {
				screen->DrawCircle(player.x - camera.x , player.y + camera.y , player.r, (234 << 16) + (255));
			}
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

			if (keyDown == SDL_SCANCODE_F5 && hitboxes) {
				keyDown = 0;
				hitboxes = false;
			}
			else if (keyDown == SDL_SCANCODE_F5 && hitboxes == false) {
				keyDown = 0;
				hitboxes = true;
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
		if (click) {
			click = false;
			mouseDown = false;
		}
	}
};