/*
* The Player class. Features a growing number of characters with individual physics.
*/

#include "Player.hpp"

Player::Player() {
	mChr = Bubble;

	PlayerSpeed = 0.4f;
	JumpSpeed = 200;
	DashSpeed = 400;
	Gravity = 4.5;
	CooldownTimer = 0;
	DashCooldown = 30;
	GraceTimer = 0;
	DashGrace = 30;
	dimensions = sf::Vector2f(80, 80);

	Sprite = "Media/Textures/bubble.png";

	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingLeft = false;
	mIsMovingRight = false;
	FacingRight = false;

	canDash = false;
	Grounded = false;

	origin = sf::Vector2f(dimensions.x / 2, dimensions.y / 2);
	position = sf::Vector2f(100, 100);

}

Player::Player(Character chr) : mChr(chr) {

	PlayerSpeed = 0.6f;
	JumpSpeed = 800;
	DashSpeed = 1100;
	Gravity = 12.9;
	CooldownTimer = 0;
	DashCooldown = 30;
	GraceTimer = 0;
	DashGrace = 40;
	dimensions = sf::Vector2f(80, 80);

	Sprite = "Media/Textures/bubble.png";

	mIsMovingUp = false;
	mIsMovingDown = false;
	mIsMovingLeft = false;
	mIsMovingRight = false;
	FacingRight = false;

	canDash = false;
	Grounded = false;

	switch (chr) {
	case(Bubble):

		PlayerSpeed = 0.6f;
		JumpSpeed = 650;
		DashSpeed = 600;
		Gravity = 20.9;
		CooldownTimer = 0;
		DashCooldown = 22;
		GraceTimer = 0;
		DashGrace = 22;
		dimensions = sf::Vector2f(80, 80);

	}

	origin = sf::Vector2f(dimensions.x / 2, dimensions.y / 2);
	position = sf::Vector2f(100, 100);

}

/* Determines the movement of Player using given key presses
*
*/
void Player::calcPlayerMovement(bool up, bool down, bool left, bool right, bool jump, int height, int width, sf::Time deltaTime) {
	// Up/Down and Left/Right inputs must be EXCLUSIVE!
	mIsMovingUp = down ? false : up;
	mIsMovingDown = up ? false : down;
	mIsMovingLeft = right ? false : left;
	mIsMovingRight = left ? false : right;

	// Determine player direction
	if (mIsMovingLeft || mIsMovingRight) FacingRight = mIsMovingRight ? true : false;

	Grounded = (position.y + dimensions.y / 2 > height - 5 && velocity.y > -5) ? true : false;

	// Landing
	if (Grounded) {;
		if (CooldownTimer <= 0)
			canDash = true;
	}

	// Jumping
	if (jump && Grounded) {
		velocity.y = -JumpSpeed;
		jump = false;
	}

	// Calculate true velocity by combinining dash and natural velocity
	TrueVelo = velocity + DashVelo;

	// Dashing (Must not be grounded!)
	if (jump && canDash && !Grounded) {

		// Default dash direction (Must not be dashing in any specified direction)
		if (!(mIsMovingUp || mIsMovingDown || mIsMovingLeft || mIsMovingRight)) (FacingRight ? mIsMovingRight : mIsMovingLeft) = true;

		if (mIsMovingUp || mIsMovingDown) DashVelo.y = (mIsMovingDown ? std::max(0.f, TrueVelo.y) + DashSpeed : std::min(0.f, TrueVelo.y) - DashSpeed);
		if (mIsMovingLeft || mIsMovingRight) DashVelo.x = (mIsMovingRight ? std::max(0.f, TrueVelo.x) + DashSpeed : std::min(0.f, TrueVelo.x) - DashSpeed);

		// Completely halts acceleration and converts velocity to dash velocity if not moving in the x or y direction
		velocity.x = 0;
		velocity.y = 0;
		acceleration.x = 0;
		acceleration.y = 0;

		CooldownTimer = DashCooldown;
		GraceTimer = DashGrace;
		canDash = false;
	}

	// Dashing causes you to maintain a speed!
	if (GraceTimer < 0) {

		// Adjust acceleration; allows for a quick brake!
		if (mIsMovingLeft)
			acceleration.x -= PlayerSpeed * (velocity.x > 0 ? 3 : 1);
		if (mIsMovingRight)
			acceleration.x += PlayerSpeed * (velocity.x < 0 ? 3 : 1);

		// Gravity
		velocity.y += Gravity;

		// Natural acceleration
		velocity.y += acceleration.y;
		velocity.x += acceleration.x;

		// Dash velocity decay
		DashVelo *= 0.90f;
		// Acceleration decay
		acceleration.x *= 0.95f;
		// Velocity decay
		velocity.x *= 0.97f;
	}

	// Get true velocity
	TrueVelo = velocity + DashVelo;

	// Colliding with walls
	if ((position.x - origin.x < 0 && (TrueVelo.x) * deltaTime.asSeconds() < 0)
		|| (position.x + origin.x > width && (TrueVelo.x) * deltaTime.asSeconds() > 0)) {
		position.x = (position.x < dimensions.x ? origin.x : width - origin.x);
		acceleration.x *= 0.3;
		velocity.x = 0;
		DashVelo.x = 0;
	}

	// Colliding with the ceiling
	if (position.y - origin.y - (TrueVelo.y) * deltaTime.asSeconds() < 0) {
		velocity.y *= -0.8;

		acceleration.x = -0.3;
		DashVelo.x = 0;
	}

	// Hitting the ground
	if (position.y + origin.y + (TrueVelo.y) * deltaTime.asSeconds() > height) {

		position.y = height - origin.y;
		acceleration.y = 0;
		velocity.y = 0;

		// Convert horizontal dash velocity to natural velocity
		velocity.x += DashVelo.x;
		DashVelo.x = 0;
		DashVelo.y = 0;
	}

	CooldownTimer -= 1;
	GraceTimer -= 1;

	position += (DashVelo + velocity) * deltaTime.asSeconds();


}

float Player::getSpeed() const {

	float velx = velocity.x + DashVelo.x;
	float vely = velocity.y + DashVelo.y;
	
	return sqrtf(velx * velx + vely * vely);

}

float Player::getWidth() const {
	return dimensions.x;
}

float Player::getHeight() const {
	return dimensions.x;
}

float Player::getCooldownTimer() const {
	return CooldownTimer;
}

float Player::getxPos() const {
	return position.x;
}

float Player::getyPos() const {
	return position.y;
}

bool Player::getCanDash() const {
	return canDash;
}

bool Player::getmIsMovingUp() const {
	return mIsMovingUp;
}

bool Player::getmIsMovingDown() const {
	return mIsMovingDown;
}

bool Player::getmIsMovingLeft() const {
	return mIsMovingLeft;
}

bool Player::getmIsMovingRight() const {
	return mIsMovingRight;
}

bool Player::getFacingRight() const {
	return FacingRight;
}