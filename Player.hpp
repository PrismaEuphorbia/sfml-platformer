#include "Entity.hpp"
#include <SFML/System/Time.hpp>
#include <string>

class Player : public Entity {

public:
	enum Character {
		Bubble
	};

public:
	Player();
	explicit Player (Character chr);
	void calcPlayerMovement(bool up, bool down, bool left, bool right, bool jump, int height, int width, sf::Time deltaTime);
	float getSpeed() const;
	float getWidth() const;
	float getHeight() const;
	float getCooldownTimer() const;
	float getxPos() const;
	float getyPos() const;
	bool getCanDash() const;
	bool getmIsMovingUp() const;
	bool getmIsMovingDown() const;
	bool getmIsMovingLeft() const;
	bool getmIsMovingRight() const;
	bool getFacingRight() const;

private:
	Character mChr;
	
	float PlayerSpeed;
	float JumpSpeed;
	float DashSpeed;
	float Gravity;
	float CooldownTimer;
	float DashCooldown;
	float GraceTimer;
	float DashGrace;
	sf::Vector2f dimensions;
	sf::Vector2f position;
	sf::Vector2f origin;

	std::string Sprite;

	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	sf::Vector2f DashVelo;
	sf::Vector2f TrueVelo;

	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingLeft;
	bool mIsMovingRight;
	bool FacingRight;

	bool canDash;
	bool Grounded;


};

