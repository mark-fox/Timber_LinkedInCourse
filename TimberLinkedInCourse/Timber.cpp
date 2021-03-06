// Timber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace sf;


// Function declaration.
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	//// Create a video mode object.
	//VideoMode vm(1920, 1080);

	//// Create and open a window for the game.
	//RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	//View view(sf::FloatRect(0, 0, 1280, 720));
	//window.setView(view);



	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Timber", Style::Fullscreen);
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));



	// Create a texture to hold graphic.
	Texture textureBackground;

	// Load graphic into the texture.
	textureBackground.loadFromFile("graphics/background.png");

	//Create a sprite object.
	Sprite spriteBackground;

	// Attach the texture to the sprite.
	spriteBackground.setTexture(textureBackground);

	// Position sprite in corner.
	spriteBackground.setPosition(0, 0);

	// Make tree sprite.
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	//spriteTree.setPosition(810, 0);
	spriteTree.setPosition(resolution.x * .42, 0);

	// Add background trees.
	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree2);
	Sprite spriteTree6;
	spriteTree6.setTexture(textureTree2);

	spriteTree2.setScale(sf::Vector2f(0.5, 1.5));
	spriteTree3.setScale(sf::Vector2f(0.5, 1));
	spriteTree4.setScale(sf::Vector2f(0.5, 1));
	spriteTree5.setScale(sf::Vector2f(0.5, 1));
	spriteTree6.setScale(sf::Vector2f(0.5, 1.5));

	float qrt = resolution.y * .25;

	// Position background trees.
	//spriteTree2.setPosition(20, 0);
	spriteTree2.setPosition(resolution.x * .01, 0);
	/*spriteTree3.setPosition(300, -400);
	spriteTree4.setPosition(1300, -400);
	spriteTree5.setPosition(1500, -500);
	spriteTree6.setPosition(1900, 0);*/
	
	spriteTree3.setPosition(resolution.x * .15, resolution.y * -.37 + qrt);
	spriteTree4.setPosition(resolution.x * .67, resolution.y * -.37 + qrt);
	spriteTree5.setPosition(resolution.x * .78, resolution.y * -.46 + qrt);
	spriteTree6.setPosition(resolution.x * .98, 0);

	// Make the bee.
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	//spriteBee.setPosition(0, 800);
	spriteBee.setPosition(0, resolution.y * .74);
	// Flag for whether bee is moving.
	bool beeActive = false;
	// Bee's speed.
	float beeSpeed = 0.0f;

	// Make cloud sprites.
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");

	const int NUM_CLOUDS = 6;
	Sprite clouds[NUM_CLOUDS];
	int cloudSpeeds[NUM_CLOUDS];
	bool cloudsActive[NUM_CLOUDS];

	for (int i = 0; i < NUM_CLOUDS; i++)
	{
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(-300, i * 150);
		cloudsActive[i] = false;
		cloudSpeeds[i] = 0;
	}
	
	// Clock object.
	Clock clock;

	// Time bar.
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track whether the game is running.
	bool paused = true;

	// Draw some text.
	int score = 0;

	Text messageText;
	Text scoreText;
	Text fpsText;

	// Setting font.
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Assign font to text.
	messageText.setFont(font);
	scoreText.setFont(font);
	fpsText.setFont(font);

	// Set fps text.
	fpsText.setFillColor(Color::White);
	fpsText.setCharacterSize(100);
	//fpsText.setPosition(1200, 20);
	fpsText.setPosition(resolution.x * .625, resolution.y * .018);

	// Set text message.
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	// Set text size.
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// Set text color.
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	// Position the text.
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// Set backgrounds for text.
	RectangleShape rect1;
	rect1.setFillColor(sf::Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(600, 105));
	rect1.setPosition(0, 30);

	RectangleShape rect2;
	rect2.setFillColor(sf::Color(0, 0, 0, 150));
	rect2.setSize(Vector2f(1000, 105));
	rect2.setPosition(1150, 30);

	// Prepare branches.
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		// Set the branch's origin to the center for easier
		// position changes.
		//branches[i].setOrigin(220, 20);
		branches[i].setOrigin(resolution.x * .114, resolution.y * .018);
	}

	// Prepare the player.
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	//spritePlayer.setPosition(580, 720);
	spritePlayer.setPosition(resolution.x * .3, resolution.y * .667);

	// Assigns player to the left for start.
	side playerSide = side::LEFT;

	// Prepare the gravestone.
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	//spriteRIP.setPosition(600, 860);
	spriteRIP.setPosition(resolution.x * .312, resolution.y * .796);

	// Prepare the axe.
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	//spriteAxe.setPosition(700, 830);
	spriteAxe.setPosition(resolution.x * .364, resolution.y * .768);

	// Line the axe up with the tree.
	const float AXE_POSITION_LEFT = resolution.x * .364; // 700;
	const float AXE_POSITION_RIGHT = resolution.x * .559; // 1075;

	// Prepare the flying log.
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	//spriteLog.setPosition(810, 720);
	spriteLog.setPosition(resolution.x * .421, resolution.y * .667);

	// Log variables.
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input.
	bool acceptInput = false;

	// Prepare the sound.
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	// Control how often the score is drawn.
	int lastDrawn = 0;




	while (window.isOpen()) {
		// ******************************************************
		// Handle the players input.
		// ******************************************************
		
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				// Listen for keypress again.
				acceptInput = true;

				// Hide the axe.
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		// Exit the game.
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		// Start the game.
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			// Reset the time and the score.
			score = 0;
			timeRemaining = 6;

			// Make all the branches disappear.
			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}

			// Hide the gravestone off screen.
			spriteRIP.setPosition(675, 2000);

			// Move player into position.
			//spritePlayer.setPosition(580, 720);
			spritePlayer.setPosition(resolution.x * .3, resolution.y * .667);

			acceptInput = true;
		}

		// Player controls.	
		if (acceptInput) {
			// Right key.
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				// Check the player is on the right.
				playerSide = side::RIGHT;

				score++;

				// Add to the amount of time remaining.
				timeRemaining += (2 / score) + 0.15f;

				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				//spritePlayer.setPosition(1200, 720);
				spritePlayer.setPosition(resolution.x * .625, resolution.y * .667);

				// Update the branches.
				updateBranches(score);

				// Set the log flying to the left.
				//spriteLog.setPosition(810, 720);
				spriteLog.setPosition(resolution.x * .421, resolution.y * .667);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				// Play chop sound.
				chop.play();
			}

			// Left key.
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				// Check the player is on the left.
				playerSide = side::LEFT;

				score++;

				// Add to the amount of time remaining.
				timeRemaining += (2 / score) + 0.15f;

				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				//spritePlayer.setPosition(580, 720);
				spritePlayer.setPosition(resolution.x * .302, resolution.y * .667);

				// Update the branches.
				updateBranches(score);

				// Set the log flying.
				//spriteLog.setPosition(810, 720);
				spriteLog.setPosition(resolution.x * .421, resolution.y * .667);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				// Play chop sound.
				chop.play();
			}
		}



		// ******************************************************
		// Update the scene.
		// ******************************************************
		if (!paused) {
			// Measure time.
			Time dt = clock.restart();	// delta time.

			// Subtract from the amount of time remaining.
			timeRemaining -= dt.asSeconds();
			// Size up the time bar.
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining,
				timeBarHeight));

			if (timeRemaining <= 0.0f) {
				// Pause the game.
				paused = true;

				// Change the message shown to the player.
				messageText.setString("Out of time!");

				// Reposition the text based on its new size.
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play out of time sound.
				outOfTime.play();
			}

			// Setup the bee.
			if (!beeActive) {
				// Bee's speed.
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				// Bee's height.
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else {
				// Move the bee.
				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				// Check whether bee has reached the right edge of screen.
				if (spriteBee.getPosition().x < -100) {
					// Reset.
					beeActive = false;
				}
			}


			// Manage the clouds.
			for (int i = 0; i < NUM_CLOUDS; i++) {
				if (!cloudsActive[i]) {
					// Cloud speed.
					srand((int)time(0) * i);
					cloudSpeeds[i] = (rand() % 200);

					// Cloud height.
					srand((int)time(0) * i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudsActive[i] = true;
				}
				else {
					// Set the new position.
					clouds[i].setPosition(
						clouds[i].getPosition().x +
						(cloudSpeeds[i] * dt.asSeconds()),
						clouds[i].getPosition().y);

					// Check if reached right side of window.
					if (clouds[i].getPosition().x > 1920) {
						// Reset.
						cloudsActive[i] = false;
					}
				}
			}

			// Draw the score and fps every 100 frames.
			lastDrawn++;
			if (lastDrawn == 100) {
				// Update the score text.
				std::stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());

				// Update the fps text.
				std::stringstream ss2;
				ss2 << "FPS = " << 1 / dt.asSeconds();
				fpsText.setString(ss2.str());
				lastDrawn = 0;
			}

			// Update the branches.
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					// Move the branch to the left side.
					branches[i].setPosition(610, height);
					// Flip the sprite.
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {
					// Move the branch to the right side.
					branches[i].setPosition(1330, height);
					// Set the branch rotation to normal.
					branches[i].setOrigin(220, 40);
					branches[i].setRotation(0);
				}
				else {
					// Hide the branch.
					branches[i].setPosition(3000, height);
				}
			}
			
			// Handle the flying log.
			if (logActive) {
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				// Check if log has reached right edge of window.
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000) {
					// Reset.
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// Check if player has been squished.
			if (branchPositions[5] == playerSide) {
				// Pancaked.
				paused = true;
				acceptInput = false;

				// Position the gravestone.
				spriteRIP.setPosition(525, 760);

				// Hide the player.
				spritePlayer.setPosition(2000, 660);

				// Change the message text.
				messageText.setString("SQUISHED!!");

				// Center message on screen.
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play death sound.
				death.play();
			}
		}


		// ******************************************************
		// Draw the scene.
		// ******************************************************
		
		// Clear everything from the last frame.
		window.clear();

				// Draw the game scene.
		window.draw(spriteBackground);

		// Draw the clouds.
		for (int i = 0; i < NUM_CLOUDS; i++) {
			window.draw(clouds[i]);
		}

		// Draw background trees.
		window.draw(spriteTree2);
		window.draw(spriteTree3);
		window.draw(spriteTree4);
		window.draw(spriteTree5);
		window.draw(spriteTree6);

		// Draw the branches.
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		// Draw the tree.
		window.draw(spriteTree);

		// Draw the player.
		window.draw(spritePlayer);

		// Draw the axe.
		window.draw(spriteAxe);

		// Draw the flying log.
		window.draw(spriteLog);

		// Draw the gravestone.
		window.draw(spriteRIP);

		// Draw text backgrounds.
		window.draw(rect1);
		window.draw(rect2);

		// Draw the bee.
		window.draw(spriteBee);

		// Draw the score.
		window.draw(scoreText);

		// Draw the fps.
		window.draw(fpsText);

		// Draw the time bar.
		window.draw(timeBar);

		if (paused) {
			// Draw the message text.
			window.draw(messageText);
		}

		// Show everything just drawn.
		window.display();
	}
    return 0;
}



// Function definition.
void updateBranches(int seed) {
	// Move all the branches down one place.
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0.
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}
