#include "Player.h"

#include <sstream>

Player::Player(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.2f), _cPlayerFrameTime(250), _cBulletSpeed(100)
{
	_player = new player();
	_player->_frame = 0;
	_paused = false;
	_started = true;
	_player->currentFrameTime = 0;
	_player->_frame = 0;
	_player->_spriteTraversal = 0;
	_player->_direction = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Player::~Player()
{
	delete _player->texture;
	delete _player->walkTexture;
	delete _playerSourceRect;
	delete _bulletTexture;
	delete _bulletRect;
	delete _menuBackground;
	delete _menuRectangle;
	delete _menuStringPosition;
	delete _startBackground;
	delete _startRectangle;
	delete _startStringPosition;
	
}

void Player::LoadContent()
{
	// Load Pacman
	_playerTexture = new Texture2D();
	_playerTexture->Load("Textures/Player.png", false);
	_playerPosition = new Vector2(350.0f, 350.0f);
	_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_bulletTexture = new Texture2D();
	_bulletTexture->Load("Textures/PlayerBullet.png", false);
	_bulletRect = new Rect(0.0f, 0.0f, 32, 32);


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
}

void Player::Update(int elapsedTime)
{

	if (!_started)
	{
		if (!_paused)
		{
			
			// Gets the current state of the keyboard
			Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

			// Checks if D key is pressed
			if (keyboardState->IsKeyDown(Input::Keys::D))
			{
				_playerPosition->X += _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
				_playerSpriteTraversal = 2;
				_isMoving = true;
			}

			// Checks if A key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::A))
			{
				_playerPosition->X -= _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
				_playerSpriteTraversal = 3;
				_isMoving = true;
			}

			// Checks if W key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::W))
			{
				_playerPosition->Y -= _cPlayerSpeed * elapsedTime; //Moves Pacman across Y axis
				_playerSpriteTraversal = 1;
				_isMoving = true;
			}

			// Checks if S key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::S))
			{
				_playerPosition->Y += _cPlayerSpeed * elapsedTime; //Moves Pacman across Y axis
				_playerSpriteTraversal = 0;
				_isMoving = true;
			}
			CheckViewportCollision();
			UpdatePlayer(elapsedTime);	
			
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
	stream << "Player X: " << _playerPosition->X << " Y: " << _playerPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_playerTexture, _playerPosition, _playerSourceRect); // Draws Pacman

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
	

	//Draw Bullet
	if (_isShooting = true)
	{
		SpriteBatch::Draw(_bulletTexture, _playerPosition, _bulletRect);
	}

	

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}
void Player::AttackInput(int elapsedTime) 
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	//check for input
	if (keyboardState->IsKeyDown(Input::Keys::LEFT)) 
	{
		_isShooting = true;
		_bulletPosition->X -= _cBulletSpeed * elapsedTime;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::RIGHT)) 
	{
		_isShooting = true;
		_bulletPosition->X += _cBulletSpeed * elapsedTime;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::UP))
	{
		_isShooting = true;
		_bulletPosition->Y += _cBulletSpeed * elapsedTime;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::DOWN))
	{
		_isShooting = true;
		_bulletPosition->Y -= _cBulletSpeed * elapsedTime;
	}
}
void Player::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey) 
{

}
void Player::CheckViewportCollision() 
{
	// Wrap to other side of screen
	if (_playerPosition->X + _playerSourceRect->Width > Graphics::GetViewportWidth())
	{
		_playerPosition->X = -50 + _playerSourceRect->Width;
	}
	if (_playerPosition->X + _playerSourceRect->Width < -32)
	{
		_playerPosition->X = 1024 - _playerSourceRect->Width;
	}
	if (_playerPosition->Y + _playerSourceRect->Width > Graphics::GetViewportHeight())
	{
		_playerPosition->Y = -50 + _playerSourceRect->Width;
	}
	if (_playerPosition->Y + _playerSourceRect->Width < -32)
	{
		_playerPosition->Y = 768 - _playerSourceRect->Width;
	}
}
void Player::UpdatePlayer(int elapsedTime) 
{
	//Frame Check
	if (_playerCurrentFrameTime > _cPlayerFrameTime)
	{
		_playerFrame++;
		if (_playerFrame >= 4)
		{
			_playerFrame = 0;
		}
		_playerCurrentFrameTime = 0;
	}

	//Traverses the sprite sheet when direcitons are input
	_playerSourceRect->Y = _playerSourceRect->Height * _playerSpriteTraversal;
	_playerSourceRect->X = _playerSourceRect->Width * _playerFrame;
	_frameCount++;
}
void Player::UpdateCoin(int elapsedTime)
{

}
void Player::SpawnBullet() 
{
	
}
