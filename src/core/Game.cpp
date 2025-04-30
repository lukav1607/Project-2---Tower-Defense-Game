// ================================================================================================
// File: Game.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include "Game.hpp"
#include "Utility.hpp"

const bool Game::IS_DEBUG_MODE_ON = false;

Game::Game() :
	isRunning(true),
	antiAliasingLevel(8),
	isVSyncEnabled(true),
	gameState(GameState::Gameplay),
	lives(5),
	gold(20),
	grid(10, 8),
	timeBetweenWaves(5.f),
	timeSinceLastWaveEnded(5.f),
	timeBetweenEnemies(0.75f),
	timeSinceLastEnemySpawned(0.f),
	wave(0),
	enemiesPerWave(5),
	enemiesSpawnedThisWave(0)
{
	auto settings = sf::ContextSettings();
	settings.antiAliasingLevel = antiAliasingLevel;
	window.create(sf::VideoMode(WINDOW_SIZE), WINDOW_TITLE, sf::Style::Close, sf::State::Windowed, settings);
	window.setVerticalSyncEnabled(isVSyncEnabled);
}

int Game::run()
{
	const float FIXED_TIME_STEP = 1.f / 60.f; // Fixed time step per update
	sf::Clock clock;						  // Clock to measure time
	float timeSinceLastUpdate = 0.f;		  // Time accumulator for fixed timestep
	float interpolationFactor = 0.f;		  // Interpolation factor for rendering

	while (isRunning)
	{
		timeSinceLastUpdate += clock.restart().asSeconds();

		processInput();

		while (timeSinceLastUpdate >= FIXED_TIME_STEP)
		{
			update(FIXED_TIME_STEP);
			timeSinceLastUpdate -= FIXED_TIME_STEP;
		}

		interpolationFactor = timeSinceLastUpdate / FIXED_TIME_STEP;
		render(interpolationFactor);
	}
	return 0;
}

void Game::processInput()
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			isRunning = false;
		}
	}

	switch (gameState)
	{
	case GameState::MainMenu:
		break;

	case GameState::Gameplay:
		if (Utility::isMouseButtonReleased(sf::Mouse::Button::Right))
		{
			sf::Vector2i clickedTile = Utility::pixelToTilePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

			if (grid.isTowerPlaceableAtTile(clickedTile))
			{
				grid.placeTower(clickedTile.x, clickedTile.y, std::make_shared<Tower>(Tower::Type::Bullet, clickedTile));
			}
		}
		break;

	case GameState::GameOver:
		break;
	}
}

void Game::update(float fixedTimeStep)
{
	switch (gameState)
	{
	case GameState::MainMenu:
		break;

	case GameState::Gameplay:

		updateWave(fixedTimeStep);

		for (auto& enemy : enemies)
		{
			enemy.update(fixedTimeStep, grid);
			if (enemy.hasReachedEnd())
				lives--;
		}

		enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& enemy) { return enemy.hasReachedEnd(); }), enemies.end());

		break;

	case GameState::GameOver:
		break;
	}
}

void Game::render(float interpolationFactor)
{
	window.clear();

	switch (gameState)
	{
	case GameState::MainMenu:
		break;

	case GameState::Gameplay:
		grid.render(interpolationFactor, window);
		for (auto& enemy : enemies)
			enemy.render(interpolationFactor, window);
		break;

	case GameState::GameOver:
		break;
	}

	window.display();
}

void Game::updateWave(float fixedTimeStep)
{
	timeSinceLastEnemySpawned += fixedTimeStep;

	if (timeSinceLastWaveEnded >= timeBetweenWaves)
	{
		if (enemiesSpawnedThisWave == 0)
			wave++;

		if (timeSinceLastEnemySpawned >= timeBetweenEnemies && enemiesSpawnedThisWave < enemiesPerWave)
		{
			timeSinceLastEnemySpawned = 0.f;
			enemiesSpawnedThisWave++;

			enemies.emplace_back(grid.getStartTile(), Enemy::BASE_SPEED, Enemy::BASE_HEALTH);
		}

		if (enemiesSpawnedThisWave >= enemiesPerWave)
		{
			timeSinceLastWaveEnded = 0.f;
			enemiesSpawnedThisWave = 0;
		}
	}
	else
	{
		timeSinceLastWaveEnded += fixedTimeStep;
	}
}

void Game::switchGameState(GameState newGameState)
{
	// TODO: Implement state transition logic

	switch (gameState)
	{
	case GameState::MainMenu:
		gameState = newGameState;
		break;

	case GameState::Gameplay:
		gameState = newGameState;
		break;

	case GameState::GameOver:
		gameState = newGameState;
		break;
	}
}