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
	gold(std::make_shared<int>(100)),
	grid(10, 8),
	timeBetweenWaves(10.f),
	timeSinceLastWaveEnded(10.f),
	timeBetweenEnemies(0.75f),
	timeSinceLastEnemySpawned(0.f),
	wave(0),
	enemiesPerWave(5),
	enemiesSpawnedThisWave(0),
	waitingForFirstEnemyInWave(false),
	font("assets/fonts/BRLNSR.TTF"),
	ui(font, WINDOW_SIZE, gold)
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
		sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		sf::Vector2i hoveredTile = Utility::pixelToTilePosition(mousePosition);

		bool isLeftReleased = Utility::isMouseButtonReleased(sf::Mouse::Button::Left);
		bool isRightReleased = Utility::isMouseButtonReleased(sf::Mouse::Button::Right);

		ui.processInput(mousePosition, isLeftReleased);

		if (isRightReleased)
		{
			auto towerAtTile = std::find_if(towers.begin(), towers.end(),
				[hoveredTile](const std::shared_ptr<Tower>& tower)
				{
					return tower->getTilePosition() == hoveredTile;
				});

			if (!ui.isAnyMenuHoveredOver())
			{
				// If a tower is found at the hovered tile
				if (towerAtTile != towers.end())
				{
					ui.dismissAllMenus();
					ui.showTowerInfoMenu(*towerAtTile, WINDOW_SIZE);
				}
				// If no tower is found at the hovered tile
				else
				{
					ui.dismissAllMenus();
					grid.deselectAllTiles();
					if (grid.getTileType(hoveredTile) == Tile::Type::Buildable)
					{
						ui.showTowerBuildMenu(hoveredTile, WINDOW_SIZE);
						grid.selectTile(hoveredTile);
					}
				}
			}
		}

		if (isLeftReleased)
		{
			if (!ui.isAnyMenuHoveredOver())
			{
				ui.dismissAllMenus();
				grid.deselectAllTiles();
			}
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
					*gold += enemy.getWorth();
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
				if (tower->tryUpgrade(*gold))
					*gold -= tower->attributes[tower->getLevel()].buyCost;
			}
			if (tower->isMarkedForSale())
			{
				*gold += tower->attributes[tower->getLevel()].sellCost;
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

		if (ui.getRequestedTowerType() != Tower::Type::Count && ui.getSelectedTile() != sf::Vector2i(-1, -1))
		{
			towers.push_back(std::make_shared<Tower>(ui.getRequestedTowerType(), ui.getSelectedTile()));
			*gold -= towers.back()->getBaseBuyCost();
			ui.dismissAllMenus();
			grid.deselectAllTiles();
		}

		ui.update(fixedTimeStep, lives, *gold, wave);

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