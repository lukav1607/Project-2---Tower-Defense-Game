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
	gold(1140),
	grid(10, 8),
	timeBetweenWaves(10.f),
	timeSinceLastWaveEnded(10.f),
	timeBetweenEnemies(0.75f),
	timeSinceLastEnemySpawned(0.f),
	wave(0),
	enemiesPerWave(5),
	enemiesSpawnedThisWave(0),
	waitingForFirstEnemyInWave(false),
	ui(WINDOW_SIZE)
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
	{
		break;
	}
	case GameState::Gameplay:
	{
		mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		bool isLeftMouseReleased = Utility::isMouseButtonReleased(sf::Mouse::Button::Left);
		bool isRightMouseReleased = Utility::isMouseButtonReleased(sf::Mouse::Button::Right);

		ui.processInput(mousePosition, isLeftMouseReleased);

		if (isRightMouseReleased)
		{
			ui.clearSelection();

			sf::Vector2i clickedTile = Utility::pixelToTilePosition(mousePosition);

			// Try to find the tower at the clicked tile
			auto it = std::find_if(towers.begin(), towers.end(), [&](std::shared_ptr<Tower> tower)
				{ return tower->getTilePosition() == clickedTile; });

			// If there is NOT a tower at the clicked tile and the tile is buildable
			if (it == towers.end() && grid.getTileType(clickedTile) == Tile::Type::Buildable)
			{
				ui.setSelectedTile(clickedTile);
				
				towers.emplace_back(std::make_shared<Tower>(Tower::Type::Bullet, clickedTile));
				int previousGold = gold;
				gold -= towers.back()->attributes[towers.back()->getLevel()].buyCost;
				if (gold < 0)
				{
					towers.pop_back();
					gold = previousGold;
				}
			}
			else if (it != towers.end() && grid.getTileType(clickedTile) == Tile::Type::Buildable)
			{
				ui.setSelectedTower(*it);
			}
		}
		else if (isLeftMouseReleased && !ui.isAnElementHoveredOver(mousePosition))
		{
			ui.clearSelection();
		}
		break;
	}
	case GameState::GameOver:
	{
		break;
	}
	}
}

void Game::update(float fixedTimeStep)
{
	switch (gameState)
	{
	case GameState::MainMenu:
	{
		break;
	}
	case GameState::Gameplay:
	{
		updateWave(fixedTimeStep);

		for (auto& enemy : enemies)
		{
			enemy.update(fixedTimeStep, grid);
			if (enemy.hasReachedEnd())
				lives--;
		}

		for (auto& tower : towers)
		{
			tower->update(fixedTimeStep, enemies);

			for (auto& enemy : enemies)
			{
				if (enemy.isDead())
				{
					gold += enemy.getWorth();
					break;
				}

				if (Utility::distance(tower->getPixelPosition(), enemy.getPixelPosition()) <= tower->attributes[tower->getLevel()].range)
				{
					if (tower->canFire())
					{
						tower->fireAt(enemy.getPixelPosition());
						break; // Make sure we only shoot one enemy at a time
					}
				}
			}
			if (tower->isMarkedForUpgrade())
			{
				if (tower->tryUpgrade(gold))
					gold -= tower->attributes[tower->getLevel()].buyCost;
			}
			if (tower->isMarkedForSale())
			{
				gold += tower->attributes[tower->getLevel()].sellCost;
				break;
			}
		}

		towers.erase(std::remove_if(towers.begin(), towers.end(),
			[](const std::shared_ptr<Tower>& tower)
			{
				return tower->isMarkedForSale();
			}),
			towers.end()
		);

		enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
			[](const Enemy& enemy)
			{
				return enemy.hasReachedEnd() || enemy.isDead();
			}),
			enemies.end()
		);

		ui.update(fixedTimeStep, mousePosition, lives, gold, wave);

		break;
	}
	case GameState::GameOver:
	{
		break;
	}
	}
}

void Game::render(float interpolationFactor)
{
	window.clear(sf::Color(110, 115, 120));

	switch (gameState)
	{
	case GameState::MainMenu:
		break;

	case GameState::Gameplay:
		grid.render(interpolationFactor, window);

		for (auto& tower : towers)
			tower->render(interpolationFactor, window);

		for (auto& enemy : enemies)
			enemy.render(interpolationFactor, window);

		ui.render(interpolationFactor, window);

		break;

	case GameState::GameOver:
		break;
	}

	window.display();
}

void Game::updateWave(float fixedTimeStep)
{
	/*
	float timeBetweenWaves;
	float timeSinceLastWaveEnded;
	float timeBetweenEnemies;
	float timeSinceLastEnemySpawned;
	int wave;
	int enemiesPerWave;
	int enemiesSpawnedThisWave;
	bool waitingForFirstEnemyInWave;
	*/
	static int healthModifier = 0;

	timeSinceLastEnemySpawned += fixedTimeStep;

	if (timeSinceLastWaveEnded >= timeBetweenWaves)
	{
		if (enemiesSpawnedThisWave == 0 && !waitingForFirstEnemyInWave)
		{
			wave++;
			timeBetweenEnemies -= 0.01f;
			if (wave % 2 == 0)
			{
				enemiesPerWave++;
			}
			if (wave % 5 == 0)
			{
				healthModifier++;
			}
			waitingForFirstEnemyInWave = true;
		}

		if (timeSinceLastEnemySpawned >= timeBetweenEnemies && enemiesSpawnedThisWave < enemiesPerWave)
		{
			waitingForFirstEnemyInWave = false;
			timeSinceLastEnemySpawned = 0.f;
			enemiesSpawnedThisWave++;

			enemies.emplace_back(grid.getStartTile(), Enemy::BASE_SPEED + wave, Enemy::BASE_HEALTH + healthModifier);
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