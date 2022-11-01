#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.2)
{
	_frameCount = 0;
	_paused = false;
	_started = true;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _playerTexture;
	delete _playerSourceRect;
	delete _coin1Texture;
	delete _coin2Texture;
	delete _coinRect;
	delete _menuBackground;
	delete _menuRectangle;
	delete _menuStringPosition;
	delete _startBackground;
	delete _startRectangle;
	delete _startStringPosition;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_playerTexture = new Texture2D();
	_playerTexture->Load("Textures/Player.png", false);
	_playerPosition = new Vector2(350.0f, 350.0f);
	_playerSourceRect = new Rect(0.0f, 0.0f, 64, 64);

	// Load Munchie
	_coin1Texture = new Texture2D();
	_coin1Texture->Load("Textures/Coin.png", true);
	_coin2Texture = new Texture2D();
	_coin2Texture->Load("Textures/Coin.png", true);
	_coinRect = new Rect(100.0f, 450.0f, 32, 32);

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

void Pacman::Update(int elapsedTime)
{

	if (!_started)
	{
		if (!_paused)
		{
			// Gets the current state of the keyboard
			Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

			// Checks if D key is pressed
			if (keyboardState->IsKeyDown(Input::Keys::D))
				_playerPosition->X += _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis

			// Checks if A key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::A))
				_playerPosition->X -= _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis

		// Checks if W key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::W))
				_playerPosition->Y -= _cPlayerSpeed * elapsedTime; //Moves Pacman across Y axis

		// Checks if S key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::S))
				_playerPosition->Y += _cPlayerSpeed * elapsedTime; //Moves Pacman across Y axis

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

			_frameCount++;
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

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _playerPosition->X << " Y: " << _playerPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_playerTexture, _playerPosition, _playerSourceRect); // Draws Pacman

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_coin2Texture, _coinRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_coin1Texture, _coinRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}
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

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}