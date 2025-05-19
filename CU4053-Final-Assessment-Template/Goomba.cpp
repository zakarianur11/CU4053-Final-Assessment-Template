#include "Goomba.h"
#include "Framework/Utilities.h"

Goomba::Goomba()
{
	speed = 100;

	GoombaSpriteSheet.loadFromFile("gfx/goomba-walk.png");
	setSize(sf::Vector2f(64, 78));
	setPosition(500, 100);
	setTexture(&GoombaSpriteSheet);
	setTag("Enemy");

	

	walk.addFrame(sf::IntRect(0, 0, 128, 157));
	walk.addFrame(sf::IntRect(125, 0, 132, 157));
	walk.addFrame(sf::IntRect(257, 0, 134, 157));
	walk.addFrame(sf::IntRect(390, 0, 125, 157));
	walk.addFrame(sf::IntRect(513, 0, 130, 157));
	walk.addFrame(sf::IntRect(637, 0, 128, 157));
	walk.setFrameSpeed(1.f / 8.f);

	currentAnimation = &walk;

	setCollisionBox(0, 0, 64, 78);

	velocity.x = -100;
}

Goomba::~Goomba()
{
}

void Goomba::update(float dt)
{
	setTextureRect(currentAnimation->getCurrentFrame());
	currentAnimation->animate(dt);

	if (CollisionWithTag("Wall"))
	{
		velocity.x = -velocity.x;
		currentAnimation->setFlipped(!currentAnimation->getFlipped()); // Toggle flip state
	}

	move(velocity * dt);

	 

}
