#include "Player.h"

#include <sstream>

Player::Player(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.2f), _cPlayerFrameTime(250), _cEnemyFrameTime(250), _cCivillianFrameTime(250)
{
	
	//Initialise enemy attributes
	_enemy = new MovingEnemy();
	_enemy->direction = 0;
	_enemy->speed = 0.1f;
	_enemy->currentFrameTime = 0;
	_enemy->frame = 0;

	// Initialise player attributes
	_player = new Scientist();
	_paused = false;
	_started = true;
	_player->currentFrameTime = 0;
	_player->frame = 0;
	_player->spriteTraversal = 0;
	_player->direction = 0;
	_player->isShooting = false;
	_player->dead = false;

	//Initialise Score attributes
	initialScore = 0;
	currentScore = initialScore;
	scoreIncrease = 100;

	//Initialise Civillian attributes
	_civillian = new Civillian();
	_civillian->direction = 0;
	_civillian->speed = 0.1f;

	//Initialise Bullet attributes
	for (int i = 0; i < BULLETCOUNT; i++)
	{
		_bullet[i] = new Bullet();
		_bullet[i]->speed = 1.0f;
		_bullet[i]->direction = 0;
		_bullet[i]->position = _player->position;
	}

	//Initialise Audio
	_oof = new SoundEffect();
	_powerUp = new SoundEffect();
	_ohNo = new SoundEffect();
	_background = new SoundEffect();

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Robotron", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Player::~Player()
{
	//deleted player items
	delete _player->texture;
	delete _player->sourceRect;
	delete _player;
	//deleted bullet items
	delete _bullet[0]->texture;
	int nCount = 0;
	for (nCount = 0; nCount < BULLETCOUNT; nCount++) 
	{
		delete _bullet[nCount]->position;
		delete _bullet[nCount]->rect;
		delete _bullet[nCount];
	}
	delete [] &_bullet;
	//deleted menu items
	delete _menuBackground;
	delete _menuRectangle;
	delete _menuStringPosition;
	delete _startBackground;
	delete _startRectangle;
	delete _startStringPosition;
	delete _gameOverBackground;
	delete _gameOverRect;
	//deleted enemy items
	delete _enemy->texture;
	delete _enemy->rect;
	delete _enemy->position;
	//deleted civillian items
	delete _civillian;
	delete _civillian->texture;
	delete _civillian->rect;
	delete _civillian->position;
	delete _targetPosition;
	delete _targetRect;
	//deleted audio items
	delete _oof;
	delete _powerUp;
	delete _ohNo;
	delete _background;
}

void Player::LoadContent()
{
	// Load Pacman
	_player->texture = new Texture2D();
	_player->texture->Load("Textures/Player.png", false);
	_player->position = new Vector2(350.0f, 350.0f);
	_player->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Bullet
	Texture2D* bulletTexture = new Texture2D();
	bulletTexture->Load("Textures/PlayerBullet.png", false);
	for (int i = 0; i < BULLETCOUNT; i++) 
	{
		_bullet[i]->texture = bulletTexture;
		_bullet[i]->rect = new Rect(0.0f, 0.0f, 32, 32);
	}
	

	//Load Civillian
	_civillian->texture = new Texture2D();
	_civillian->texture->Load("Textures/Civillian.png", false);
	_civillian->position = new Vector2(200.0f, 200.0f);
	_civillian->rect = new Rect(0.0f, 0.0f, 32, 32);

	//Load Civillian Target
	_targetPosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_targetRect = new Rect(0.0f, 0.0f, 32, 32);

	//Load Enemy
	_enemy->texture = new Texture2D();
	_enemy->texture->Load("Textures/Enemy.png", false);
	_enemy->position = new Vector2((rand()% Graphics::GetViewportWidth()), (rand()%Graphics::GetViewportHeight()));
	_enemy->rect = new Rect(0.0f, 0.0f, 64, 64);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	//Set Start parameters
	_startBackground = new Texture2D();
	_startBackground->Load("Textures/Transparency.png", false);
	_startRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_startStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	
	//Set Game Over parameters
	_gameOverBackground = new Texture2D();
	_gameOverBackground->Load("Textures/GameOver.png", false);
	_gameOverRect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	
	//Load sounds
	_oof->Load("Sounds/SadTrombone.wav");
	_powerUp->Load("Sounds/PowerUp.wav");
	_ohNo->Load("Sounds/OhNo.wav");
	_background->Load("Sounds/BackgroundMusic.wav");
}

void Player::Update(int elapsedTime)
{

	if (!_started)
	{
		if (!_paused)
		{
			PlayerMovement(elapsedTime);
			EnemyMovement(_enemy, elapsedTime);
			CivillianMovement(elapsedTime);
			//UpdatePlayer(elapsedTime);



			for (int i = 0; i < BULLETCOUNT; i++)
			{
				AttackInput(_bullet[i], elapsedTime);
			}
			if (CheckObjectPosition(_player->position->X, _player->position->Y) == true) 
			{
				Audio::Play(_background);
			}
			
			
			
			
			if (CheckCollision(_player->position->X, _player->position->Y, _player->sourceRect->Width, _player->sourceRect->Height,
					_enemy->position->X, _enemy->position->Y, _enemy->rect->Width, _enemy->rect->Height)) 
			{
				_player->position->X = 10000.0f;
				_player->position->Y = 10000.0f;
				Audio::Play(_oof);
				_player->dead = true;
				//Audio::Stop(_background);				
			}
			if (CheckCollision(_player->position->X, _player->position->Y, _player->sourceRect->Width, _player->sourceRect->Height,
				_civillian->position->X, _civillian->position->Y, _civillian->rect->Width, _civillian->rect->Height)) 
			{
				_civillian->position->X = 11000.0f;
				_civillian->position->Y = 11000.0f;
				Audio::Play(_powerUp);
				currentScore += scoreIncrease;
			}
			if (CheckCollision(_civillian->position->X, _civillian->position->Y, _civillian->rect->Width, _civillian->rect->Height,
				_enemy->position->X, _enemy->position->Y, _enemy->rect->Width, _enemy->rect->Height)) 
			{
				_civillian->position->X = 11000.0f;
				_civillian->position->Y = 11000.0f;
				Audio::Play(_ohNo);
			}
			
		}
	}
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	//Check for button press to start game
	if (keyboardState->IsKeyDown(Input::Keys::SPACE)) 
	{
		_started = false;
	}
	
	// Check for button press to pause game
	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::ESCAPE)) 
	{
		_pKeyDown = false;
	}
	
}

void Player::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Score: " << currentScore;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_player->texture, _player->position, _player->sourceRect); // Draws Pacman

		//Draw Pause Menu
	if (_paused) 
	{
		std::stringstream menuStream;
		menuStream << "Paused!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

		//Draw Start Menu
	if (_started) 
	{
		std::stringstream startStream;
		startStream << "Press Space to Start!";

		SpriteBatch::Draw(_startBackground, _startRectangle, nullptr);
		SpriteBatch::DrawString(startStream.str().c_str(), _startStringPosition, Color::Green);
	}

	//Draw Game Over Screen
	if (_player->dead == true) 
	{
		SpriteBatch::Draw(_gameOverBackground, _gameOverRect, nullptr);
	}
	
	//Draw Enemy
	
	SpriteBatch::Draw(_enemy->texture, _enemy->position, _enemy->rect);
	
	

	//Draw Bullet
	if (_player->isShooting == true)
	{
		for (int i = 0; i < BULLETCOUNT; i++) 
		{
			SpriteBatch::Draw(_bullet[i]->texture, _bullet[i]->position, _bullet[i]->rect);
		}
		
	}

	//Draw Civillian
	SpriteBatch::Draw(_civillian->texture, _civillian->position, _civillian->rect);

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}
void Player::AttackInput(Bullet*, int elapsedTime)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	//check for input
	if (keyboardState->IsKeyDown(Input::Keys::LEFT)) 
	{
		_player->isShooting = true;
		for (int i = 0; i < BULLETCOUNT; i++) 
		{
			_bullet[i]->direction = 1;
			BulletMovement(_bullet[i], _bullet[i]->direction, elapsedTime);
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::RIGHT)) 
	{
		_player->isShooting = true;
		for (int i = 0; i < BULLETCOUNT; i++)
		{
			_bullet[i]->direction = 0;
			BulletMovement(_bullet[i], _bullet[i]->direction, elapsedTime);
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::UP))
	{
		_player->isShooting = true;
		for (int i = 0; i < BULLETCOUNT; i++)
		{
			_bullet[i]->direction = 3;
			BulletMovement(_bullet[i], _bullet[i]->direction, elapsedTime);
		}
	}
	else if (keyboardState->IsKeyDown(Input::Keys::DOWN))
	{
		_player->isShooting = true;
		for (int i = 0; i < BULLETCOUNT; i++)
		{
			_bullet[i]->direction = 2;
			BulletMovement(_bullet[i], _bullet[i]->direction, elapsedTime);
		}
	}
}
void Player::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey) 
{

}
void Player::CheckViewportCollision() 
{
	// collide with viewport walls
	if (_player->position->X + _player->sourceRect->Width >= Graphics::GetViewportWidth())
	{
		_player->position->X = Graphics::GetViewportWidth() - _player->sourceRect->Width;
	}
	if (_player->position->X + _player->sourceRect->Width <= 0)
	{
		_player->position->X = 33.0f - _player->sourceRect->Width;
	}
	if (_player->position->Y + _player->sourceRect->Height >= Graphics::GetViewportHeight())
	{
		_player->position->Y = Graphics::GetViewportHeight() - _player->sourceRect->Height;
	}
	if (_player->position->Y + _player->sourceRect->Height <= 0)
	{
		_player->position->Y = 32.0f - _player->sourceRect->Height;
	}
	
}
void Player::UpdatePlayer(int elapsedTime) 
{
	//Frame Check
	if (_player->currentFrameTime > _cPlayerFrameTime)
	{
		_player->frame++;
		if (_player->frame >= 4)
		{
			_player->frame = 0;
		}
		_player->currentFrameTime = 0;
	}

	//Traverses the sprite sheet when direcitons are input
	_player->sourceRect->Y = _player->sourceRect->Height * _player->spriteTraversal;
	_player->sourceRect->X = _player->sourceRect->Width *  _player->frame * elapsedTime;
	_player->frame++;
}
void Player::PlayerMovement(int elapsedTime) 
{
	
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D))
	{
		_player->position->X += _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
		_player->spriteTraversal = 2;
	}

	// Checks if A key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		_player->position->X -= _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
		_player->spriteTraversal = 3;
	}

	// Checks if W key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::W))
	{
		_player->position->Y -= _cPlayerSpeed * elapsedTime; //Moves Pacman across Y axis
		_player->spriteTraversal = 1;
	}

	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_player->position->Y += _cPlayerSpeed * elapsedTime; //Moves Pacman across Y axis
		_player->spriteTraversal = 0;
	}
	UpdatePlayer(elapsedTime);
	CheckViewportCollision();
}
void Player::BulletMovement(Bullet*, int direction, int elapsedTime) 
{
	//Changes tragectory of the bullet depending on it's direction which is defined by which directional key is pressed
	if (direction == 0) 
	{
		for (int i = 0; i < BULLETCOUNT; i++) 
		{
			_bullet[i]->position->X += _bullet[i]->speed * elapsedTime;
		}
	}
	else if (direction == 1)
	{
		for (int i = 0; i < BULLETCOUNT; i++)
		{
			_bullet[i]->position->X -= _bullet[i]->speed * elapsedTime;
		}
	}
	else if (direction == 2)
	{
		for (int i = 0; i < BULLETCOUNT; i++)
		{
			_bullet[i]->position->Y += _bullet[i]->speed * elapsedTime;
		}
	}
	if (direction == 3)
	{
		for (int i = 0; i < BULLETCOUNT; i++)
		{
			_bullet[i]->position->Y -= _bullet[i]->speed * elapsedTime;
		}
	}
}
void Player::EnemyMovement(MovingEnemy* _enemy, int elapsedTime) 
{
	_enemy->speed = 0.01f;
	//If the enemy is closer to the player then it will track the player
	if ((_player->position->X - _enemy->position->X) && (_player->position->Y - _enemy->position->Y) < (_civillian->position->X - _enemy->position->X) && (_civillian->position->Y - _enemy->position->Y))
	{
		float targetX = _player->position->X - _enemy->position->X;
		float targetY = _player->position->Y - _enemy->position->Y;
		_enemy->position->X += targetX * _enemy->speed;
		_enemy->position->Y += targetY * _enemy->speed;
	}
	//If the enemy is closer to the civillian then it will track the civillian
	else if ((_player->position->X - _enemy->position->X) && (_player->position->Y - _enemy->position->Y) > (_civillian->position->X - _enemy->position->X) && (_civillian->position->Y - _enemy->position->Y))
	{
		float targetX = _civillian->position->X - _enemy->position->X;
		float targetY = _civillian->position->Y - _enemy->position->Y;
		_enemy->position->X += targetX * _enemy->speed;
		_enemy->position->Y += targetY * _enemy->speed;
	}
	//track player by deafault
	else 
	{
		float targetX = _player->position->X - _enemy->position->X;
		float targetY = _player->position->Y - _enemy->position->Y;
		_enemy->position->X += targetX * _enemy->speed;
		_enemy->position->Y += targetY * _enemy->speed;
	}
	UpdateEnemy(elapsedTime);
	
}
void Player::UpdateEnemy(int elapsedTime) 
{
	//Frame Check
	if (_enemy->currentFrameTime > _cEnemyFrameTime)
	{
		_enemy->frame++;
		if (_enemy->frame >= 5)
		{
			_enemy->frame = 0;
		}
		_enemy->currentFrameTime = 0;
	}

	_enemy->rect->X = _enemy->rect->Width * _enemy->frame * elapsedTime;
	_enemy->frame++;
}
void Player::CivillianMovement(int elapsedTime) 
{
	UpdateCivillian(elapsedTime);
	if(_civillian->direction == 0) //Moves Down
	{
		_civillian->position->Y += _civillian->speed * elapsedTime;
		if (_civillian->position->Y + _civillian->rect->Height >= 300) //hits bottom wall
		{
			_civillian->direction = (rand() % 4);
		}
	}
	else if (_civillian->direction == 1)//Moves Up
	{
		_civillian->position->Y -= _civillian->speed * elapsedTime;
		if (_civillian->position->X <= 32); //hits left wall
		{
			_civillian->direction = (rand() % 4);
		}
	}
	else if (_civillian->direction == 2)//Moves Right
	{
		_civillian->position->X += _civillian->speed * elapsedTime;
		if (_civillian->position->X >= 400)//hits right wall
		{
			_civillian->direction = (rand() % 4);
		}
	}
	else if (_civillian->direction == 3) //Moves Left
	{
		_civillian->position->X -= _civillian->speed * elapsedTime;
		if (_civillian->position->Y <= 32)//hits left edge
		{
			_civillian->direction = (rand() % 4);
		}
	}
}
void Player::UpdateCivillian(int elapsedTime) 
{
	//Frame Check
	if (_civillian->currentFrameTime > _cCivillianFrameTime)
	{
		_civillian->frame++;
		if (_civillian->frame >= 4)
		{
			_civillian->frame = 0;
		}
		_civillian->currentFrameTime = 0;
	}

	//Traverses the sprite sheet when direcitons are input
	_civillian->rect->Y = _civillian->rect->Height * _civillian->direction;
	_civillian->rect->X = _civillian->rect->Width * _civillian->frame * elapsedTime;
	_civillian->frame++;
}

bool Player::CheckCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) 
{
	//declaration of function variables
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	//collision check
	if (bottom1 < top2)
		return false;
	if (top1 > bottom2)
		return false;
	if (right1 < left2)
		return false;
	if (left1 > right2)
		return false;
	else
		return true;

}
bool Player::CheckObjectPosition(float objectPositionX, float objectPositionY) 
{
	
	if (objectPositionX >= 0 && objectPositionY <= Graphics::GetViewportWidth())
	{
		return true;
	}
	else if (objectPositionY >= 0 && objectPositionY <= Graphics::GetViewportHeight())
	{
		return true;
	}
	else
		return false;
}
