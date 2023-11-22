/*
Code given by Packt
An application that renders a circle
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <format>
#include "ResourceHolder.hpp"
#include "Player.hpp"
#include "hsvrgb.hpp"

// The Game class, features several functions that power the game
class Game
{
public:
	Game();
	void run();
private:

	int width = 1366;
	int height = 768;
	int frame = 0;
	
	Player Bubble;

	float hue = 0.f, sat = 1.f, val = 1.f;
	sf::Font rodin;
	sf::Time TimePerFrame = sf::seconds(1.f / 120.f);

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool jump = false;

	void processEvents();
	void update(sf::Time deltaTime);
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
private:
	ResourceHolder <sf::Texture, Textures::ID> textures;
	ResourceHolder <sf::Font, Fonts::ID> fonts;
	sf::RenderWindow mWindow;
	sf::Sprite mPlayer;
	sf::Text debug;

};

// An instance of Game, complete with a title, a window size, and elements to apply to the game
Game::Game()
	:mWindow(sf::VideoMode(width, height), "SFML Application")
	, mPlayer()
{
	Bubble = Player(Player::Bubble);

	fonts.load(Fonts::Rodin, "Media/Fonts/FOT-Rodin Pro M.otf");
	textures.load(Textures::Bubble, "Media/Textures/bubble.png");

	// Debug text characteristics
	debug.setCharacterSize(20);
	debug.setString("Hello!");
	debug.setFillColor(sf::Color::White);
	debug.setFont(fonts.get(Fonts::Rodin));
	debug.setPosition(5.f, 5.f);

	std::cout << debug.getFont() << std::endl;
	std::cout << debug.getString().toAnsiString();

	mWindow.setVerticalSyncEnabled(true);
	mWindow.setKeyRepeatEnabled(false);

	mPlayer.setOrigin(40, 40);
	mPlayer.setPosition(100, 100.f);
	mPlayer.setColor(sf::Color(255, 0, 0));
	mPlayer.setTexture(textures.get(Textures::Bubble));
}

// Game class function that calls the other three functions to power the game
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			update(TimePerFrame);
		}
		render();
	}
}

// Game class function that polls the application window for any events
void Game::processEvents()
{
	sf::Event event;

	// Checks which event has occurred
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
			// If a key is pressed, enables a player input
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;
			// If a key is released, disables a player input
		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
			// If the X button is pressed, close the window
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}

}

void Game::handlePlayerInput(sf::Keyboard::Key key,
	bool isPressed)
{
	if (key == sf::Keyboard::W || key == sf::Keyboard::Up)
		up = isPressed;
	if (key == sf::Keyboard::S || key == sf::Keyboard::Down)
		down = isPressed;
	if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
		left = isPressed;
	if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
		right = isPressed;
	if (key == sf::Keyboard::Space || key == sf::Keyboard::L)
		jump = isPressed;
	if (key == sf::Keyboard::Escape)
		mWindow.close();
}

// Updates the logic of the game
void Game::update(sf::Time deltaTime)
{

	frame += 1;

	// Calculate movement
	Bubble.calcPlayerMovement(up, down, left, right, jump, height, width, deltaTime);
	
	jump = false;

	// Get the speed for color and debug purposes
	float speed = Bubble.getSpeed();

	// Paints a color depending on the bubble's speed
	hue = fmod(speed / 10, 360);
	float r = 0, g = 0, b = 0;
	HSVtoRGB(r, g, b, hue, sat, val);
	mPlayer.setColor(sf::Color(r * 255, g * 255, b * 255));
	mPlayer.setPosition(Bubble.getxPos(), Bubble.getyPos());

	// Debug text
	debug.setString(std::format("Frame: {}\nPosition: {:.2f}, {:.2f}\nHSV: {:.2f}, {:.2f}, {:.2f}"
		"\nRGB: {:.2f}, {:.2f}, {:.2f}\nSpeed: {:.2f}\nDash? {}({:.2f})"
		"\nUp? {}; Down? {}; Left? {}; Right? {}\nFacing right? {}",
		frame, Bubble.getxPos(), Bubble.getyPos(), hue, sat, val, r * 255, g * 255, b * 255, speed, Bubble.getCanDash(),
		Bubble.getCooldownTimer(), Bubble.getmIsMovingUp(), Bubble.getmIsMovingDown(), Bubble.getmIsMovingLeft(),
		Bubble.getmIsMovingRight(), (Bubble.getFacingRight() ? true : false)));

}

/* Updates the rendering of the game
* clear - Clears a window to black, usually
* draw  - Draws all the objects of the current frame
* display - Displays it on the screen
* All of this one "frame" of a game
*/
void Game::render()
{
	mWindow.clear();
	mWindow.draw(mPlayer);
	mWindow.draw(debug);
	mWindow.display();
}

int main()
{
	Game game;
	game.run();
}