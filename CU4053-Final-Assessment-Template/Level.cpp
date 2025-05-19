#include "Level.h"
Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, sf::View* v, World* w, TileManager* tm)
{
	window = hwnd;
	input = in;
	gameState = gs;
	view = v;
	world = w;
	tileManager = tm;
	audioManager = new AudioManager();

	if (!font.loadFromFile("font/arial.ttf")) {
		std::cout << "error loading font" << std::endl;
	};

	cloud.loadFromFile("gfx/cloud.png");

	cloudSprite.setTexture(cloud);


	audioManager->addMusic("sfx/Cantina.ogg", "bgm");
	audioManager->addSound("sfx/smb_jump-super.wav", "jump");

	mario.setInput(input);
	mario.setAudio(audioManager);

	audioManager->playMusicbyName("bgm");

	//world->AddGameObject(zomb);
	world->AddGameObject(mario);
	for (size_t i = 0; i < numberOfGoombas; i++)
	{
		world->AddGameObject(goombaArray[i]);
		goombaArray[i].setPosition(500 + i * 300, 100);
	}

	


	CollectableCollected.setFont(font);
	CollectableCollected.setCharacterSize(24);
	CollectableCollected.setFillColor(sf::Color::Yellow);
	CollectableCollected.setPosition(window->getSize().x, 0);
	CollectableCollected.setString("X");

	if (!CollectablesUITex.loadFromFile("gfx/Collectable.png"));
	CollectablesUI.setTexture(CollectablesUITex);
	CollectablesUI.setPosition(0, 0);
	CollectablesUI.setScale(0.1, 0.1);
}

Level::~Level()
{
	//Making pointers null
	window = nullptr;
	input = nullptr;
	gameState = nullptr;
	view = nullptr;
	world = nullptr;
	tileManager = nullptr;
	if (audioManager != nullptr) {
		delete audioManager;
		audioManager = nullptr;
	}
}

void Level::handleInput(float dt)
{
	if (input->isKeyDown(sf::Keyboard::Escape))
	{
		exit(0);
	}
	if (input->isKeyDown(sf::Keyboard::Tab))
	{
		input->setKeyUp(sf::Keyboard::Tab);
		gameState->setCurrentState(State::TILEEDITOR);
	}
	mario.handleInput(dt);
}

// Update game objects
void Level::update(float dt)
{
	sf::Vector2f viewSize = sf::Vector2f(window->getSize().x, window->getSize().y);
	CollectableCollected.setPosition(view->getCenter().x - viewSize.x / 2.1, view->getCenter().y - viewSize.y / 2.04);
	CollectablesUI.setPosition(view->getCenter().x - viewSize.x / 2, view->getCenter().y - viewSize.y / 2);
	if (mario.CollisionWithTag("Collectable"))
	{
		// Player is Colliding with Ring
		mario.addCollected(1); // Increment ring count
		tileManager->RemoveCollectable(); // Remove the collectable tile

		// Update the RingsCollectedText to display the new number of rings collected
		int ringCount = mario.getCollectableCount(); // Assume p1 is the player object and has the getRingCount method
		CollectableCollected.setString("X" + std::to_string(ringCount));
	}

	//Move the view to follow the player
	view->setCenter(view->getCenter().x, 500);
	sf::Vector2f playerPosition = mario.getPosition();
	float newX = std::max(playerPosition.x, view->getSize().x / 2.0f);
	view->setCenter(newX, view->getCenter().y);
	window->setView(*view);


	if (mario.CollisionWithTag("Checkpoint"))
	{
		std::cout << "Mario Colliding with Checkpoint\n";
		mario.setPosition(100, 100);
	}

	for (size_t i = 0; i < numberOfGoombas; i++)
	{
		goombaArray[i].update(dt);
	}

	for (int i = 0; i < numberOfGoombas; i++)
	{
		if (goombaArray[i].CollisionWithTag("Player"))
		{
			if (mario.CollisionWithTag("Enemy"))
			{
				//std::cout << "Mario Collision Direction: " << mario.getCollisionDirection() << std::endl;

				if (mario.getCollisionDirection() == "Down")
				{
					goombaArray[i].setAlive(false);
					world->RemoveGameObject(goombaArray[i]);
				}
				else
				{
					std::cout << "Player hit enemy from the side\n";
					mario.setPosition(56, 0);
				}
			}
		}
	}
}

// Render level
void Level::render()
{
	if (gameState->getCurrentState() == State::LEVEL)
	{
		tileManager->render(false);
	}
	
	window->draw(CollectablesUI);
	window->draw(CollectableCollected);
	// Render level
	for (size_t i = 0; i < numberOfGoombas; i++)
	{
		if (goombaArray[i].isAlive())
		{
			window->draw(goombaArray[i]);
		}
	}
	window->draw(mario);
}


void Level::adjustViewToWindowSize(unsigned int width, unsigned int height)
{
	sf::FloatRect visibleArea(0, 0, static_cast<float>(width), static_cast<float>(height));
	view->setSize(static_cast<float>(width), static_cast<float>(height));
	view->setCenter(static_cast<float>(width) / 2, static_cast<float>(height) / 2);
}