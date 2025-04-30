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

			// Try to find the tower at the clicked tile
			auto it = std::find_if(towers.begin(), towers.end(), [&](const Tower& tower) 
					{ return tower.getTilePosition() == clickedTile; });

			// If there is NOT a tower at the clicked tile and the tile is buildable
			if (it == towers.end() && grid.getTileType(clickedTile) == Tile::Type::Buildable)
			{
				//TODO: Show GUI - Select tower type, display tower info

				// Tower type selected:
					// Player has enough gold to place a tower:
						towers.emplace_back(Tower::Type::Bullet, clickedTile);

					// Not enough gold:
						// TODO: GUI - Not enough gold to place a tower
				// Else:
					// TODO: Hide GUI
			}
			// If there IS a tower at the clicked tile
			else 
			{
				//TODO: GUI - Show tower info, upgrade tower, sell tower
				
				// Upgrade tower selected:
					// Tower level++
				// Sell tower selected:
					// gold += tower->attributes[tower->level].sellCost;
					// Tower is removed from the tile
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

		for (auto& tower : towers)
		{
			tower.update(fixedTimeStep, enemies);

			for (auto& enemy : enemies)
			{
				if (Utility::distance(tower.getPixelPosition(), enemy.getPixelPosition()) <= tower.attributes[tower.getLevel() - 1].range)
				{
					if (tower.canFire())
					{
						tower.fireAt(enemy.getPixelPosition());
						break; // Make sure we only shoot one enemy at a time
					}
				}
			}
		}

		enemies.erase(std::remove_if
		(
			enemies.begin(),
			enemies.end(),
			[](const Enemy& enemy)
			{
				return enemy.hasReachedEnd() || enemy.isDead();
			}),
			enemies.end()
		);

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

		for (auto& tower : towers)
			tower.render(interpolationFactor, window);

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