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
#include "../entities/BulletTower.hpp"
#include "../entities/SplashTower.hpp"
#include "../entities/SlowTower.hpp"

const bool Game::IS_DEBUG_MODE_ON = false;

Game::Game() :
	isRunning(true),
	antiAliasingLevel(8),
	isVSyncEnabled(true),
	gameState(GameState::MainMenu),
	lives(STARTING_LIVES),
	gold(std::make_shared<int>(STARTING_GOLD)),
	grid(10, 8),
	timeBetweenWaves(10.f),
	timeSinceLastWaveEnded(10.f),
	timeBetweenEnemies(0.7f),
	timeSinceLastEnemySpawned(0.f),
	wave(0),
	enemiesPerWave(5),
	enemiesSpawnedThisWave(0),
	waitingForFirstEnemyInWave(false),
	font("assets/fonts/BRLNSR.TTF"),
	ui(font, WINDOW_SIZE, gold),
	titleText(font, "Tower Defense", 128U),
	startText(font, "Press ENTER to start", 64U),
	authorText(font, "Luka Vukorepa 2025", 32U),
	gameOverText(font, "Game Over!", 104U),
	gameOverWaveText(font, "Reached wave: ", 64U),
	restartText(font, "Press ENTER to return to Main Menu", 64U)
{
	auto settings = sf::ContextSettings();
	settings.antiAliasingLevel = antiAliasingLevel;
	window.create(sf::VideoMode(WINDOW_SIZE), WINDOW_TITLE, sf::Style::Close, sf::State::Windowed, settings);
	window.setVerticalSyncEnabled(isVSyncEnabled);

	titleText.setFillColor(sf::Color(255, 255, 255));
	titleText.setOutlineThickness(2.f);
	titleText.setOutlineColor(sf::Color(50, 53, 55));
	titleText.setOrigin({ titleText.getGlobalBounds().size.x / 2.f, titleText.getGlobalBounds().size.y / 2.f });
	titleText.setPosition({ WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y / 2.f - 100.f });

	startText.setFillColor(sf::Color(255, 255, 255));
	startText.setOutlineThickness(2.f);
	startText.setOutlineColor(sf::Color(50, 53, 55));
	startText.setOrigin({ startText.getGlobalBounds().size.x / 2.f, startText.getGlobalBounds().size.y / 2.f });
	startText.setPosition({ WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y / 2.f + 100.f });

	authorText.setFillColor(sf::Color(255, 255, 255));
	authorText.setOutlineThickness(2.f);
	authorText.setOutlineColor(sf::Color(50, 53, 55));
	authorText.setOrigin({ authorText.getGlobalBounds().size.x / 2.f, authorText.getGlobalBounds().size.y / 2.f });
	authorText.setPosition({ WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y - authorText.getGlobalBounds().size.y * 2.f });

	gameOverText.setFillColor(sf::Color(255, 255, 255));
	gameOverText.setOutlineThickness(2.f);
	gameOverText.setOutlineColor(sf::Color(50, 53, 55));
	gameOverText.setOrigin({ gameOverText.getGlobalBounds().size.x / 2.f, gameOverText.getGlobalBounds().size.y / 2.f });
	gameOverText.setPosition(titleText.getPosition() - sf::Vector2f(0.f, 50.f));

	gameOverWaveText.setFillColor(sf::Color(255, 255, 255));
	gameOverWaveText.setOutlineThickness(2.f);
	gameOverWaveText.setOutlineColor(sf::Color(50, 53, 55));
	gameOverWaveText.setOrigin({ gameOverWaveText.getGlobalBounds().size.x / 2.f, gameOverWaveText.getGlobalBounds().size.y / 2.f });
	gameOverWaveText.setPosition(titleText.getPosition() + sf::Vector2f(0.f, 50.f));

	restartText.setFillColor(sf::Color(255, 255, 255));
	restartText.setOutlineThickness(2.f);
	restartText.setOutlineColor(sf::Color(50, 53, 55));
	restartText.setOrigin({ restartText.getGlobalBounds().size.x / 2.f, restartText.getGlobalBounds().size.y / 2.f });
	restartText.setPosition(startText.getPosition());

	soundManager.loadSounds();
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
	if (Utility::isKeyReleased(sf::Keyboard::Key::Escape))
	{
		isRunning = false;
	}

	switch (gameState)
	{
	case GameState::MainMenu:
	{
		if (Utility::isKeyReleased(sf::Keyboard::Key::Enter))
		{
			switchGameState(GameState::Gameplay);
		}
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
				[hoveredTile](std::shared_ptr<Tower> tower)
				{
					return tower->getTilePosition() == hoveredTile;
				});

			if (!ui.isAnyMenuHoveredOver())
			{
				// If a tower is found at the hovered tile
				if (towerAtTile != towers.end())
				{
					ui.dismissAllMenus();
					grid.deselectAllTiles();
					deselectAllTowers();
					ui.showTowerInfoMenu(*towerAtTile, WINDOW_SIZE);
					(*towerAtTile)->isSelected = true;
				}
				// If no tower is found at the hovered tile
				else
				{
					ui.dismissAllMenus();
					grid.deselectAllTiles();
					deselectAllTowers();
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
				deselectAllTowers();
			}
		}

		break;
	}
	case GameState::GameOver:
	{
		if (Utility::isKeyReleased(sf::Keyboard::Key::Enter))
		{
			switchGameState(GameState::MainMenu);
		}
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
		static int updateCount = 0;

		updateWave(fixedTimeStep);		

		for (auto& enemy : enemies)
		{
			enemy.update(fixedTimeStep, grid);

			if (enemy.hasReachedEnd())
			{
				lives--;
				soundManager.playSound(SoundManager::SoundID::LIFE_LOST);
			}
			if (enemy.isDead())
			{
				*gold += enemy.getWorth();
			}
		}

		for (auto& tower : towers)
		{
			tower->update(fixedTimeStep, enemies, soundManager);

			if (tower->isMarkedForUpgrade())
			{
				if (tower->tryUpgrade(*gold))
				{
					*gold -= tower->getAttributes().at(tower->getLevel()).buyCost;
					soundManager.playSound(SoundManager::SoundID::TOWER_UPGRADE);
				}
			}
			if (tower->isMarkedForSale())
			{
				*gold += tower->getAttributes().at(tower->getLevel()).sellCost;
				soundManager.playSound(SoundManager::SoundID::BUTTON_CLICK);
			}
		}

		// Remove towers that are sold
		towers.erase(std::remove_if(towers.begin(), towers.end(),
			[](const std::shared_ptr<Tower>& tower)
			{
				return tower->isMarkedForSale();
			}),
			towers.end()
		);

		// Remove enemies that have reached the end or are dead
		enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
			[](const Enemy& enemy)
			{
				return enemy.hasReachedEnd() || enemy.isDead();
			}),
			enemies.end()
		);

		// Tower placement logic
		if (ui.getRequestedTowerType() != TowerRegistry::Type::Count && ui.getSelectedTile() != sf::Vector2i(-1, -1))
		{
			switch (ui.getRequestedTowerType())
			{
			case TowerRegistry::Type::Bullet:
			{
				towers.push_back(std::make_shared<BulletTower>(ui.getSelectedTile()));
				break;
			}
			case TowerRegistry::Type::Splash:
			{
				towers.push_back(std::make_shared<SplashTower>(ui.getSelectedTile()));
				break;
			}
			case TowerRegistry::Type::Slow:
			{
				towers.push_back(std::make_shared<SlowTower>(ui.getSelectedTile()));
				break;
			}
			}
			*gold -= towers.back()->getAttributes().at(0).buyCost;
			soundManager.playSound(SoundManager::SoundID::BUTTON_CLICK);
			ui.dismissAllMenus();
			grid.deselectAllTiles();
		}

		ui.update(fixedTimeStep, lives, *gold, wave);

		if (updateCount % 60 == 0) {
			soundManager.cleanupSounds();
		}
		updateCount++;

		if (lives <= 0)
			switchGameState(GameState::GameOver);

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
		window.draw(titleText);
		window.draw(startText);
		window.draw(authorText);
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
		window.draw(gameOverText);
		window.draw(gameOverWaveText);
		window.draw(restartText);
		break;
	}

	window.display();
}

void Game::updateWave(float fixedTimeStep)
{
	//static int healthModifier = 0;

	timeSinceLastEnemySpawned += fixedTimeStep;

	if (timeSinceLastWaveEnded >= timeBetweenWaves)
	{
		if (enemiesSpawnedThisWave == 0 && !waitingForFirstEnemyInWave)
		{
			soundManager.playSound(SoundManager::SoundID::NEW_WAVE);

			wave++;
			timeBetweenEnemies -= 0.01f;
			enemiesPerWave++;

			/*if (wave % 2 == 0)
			{
				healthModifier++;
			}*/
			waitingForFirstEnemyInWave = true;
		}

		if (timeSinceLastEnemySpawned >= timeBetweenEnemies && enemiesSpawnedThisWave < enemiesPerWave)
		{
			waitingForFirstEnemyInWave = false;
			timeSinceLastEnemySpawned = 0.f;
			enemiesSpawnedThisWave++;

			enemies.emplace_back(grid.getStartTile(), Enemy::BASE_SPEED + wave * 0.15f, Enemy::BASE_HEALTH + wave / 3.f);
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

void Game::deselectAllTowers()
{
	for (auto& tower : towers)
		tower->isSelected = false;
}

void Game::resetGame()
{
	ui.dismissAllMenus();
	grid.deselectAllTiles();
	deselectAllTowers();

	towers.clear();
	enemies.clear();

	timeBetweenWaves = 10.f;
	timeSinceLastWaveEnded = 10.f;
	timeBetweenEnemies = 0.7f;
	timeSinceLastEnemySpawned = 0.f;
	wave = 0;
	enemiesPerWave = 5;
	enemiesSpawnedThisWave = 0;
	waitingForFirstEnemyInWave = false;

	lives = STARTING_LIVES;
	*gold = STARTING_GOLD;

	grid.generateNewRandomLevel(10, 8);
}

void Game::switchGameState(GameState newGameState)
{
	soundManager.playSound(SoundManager::SoundID::BUTTON_CLICK);

	switch (gameState)
	{
	case GameState::MainMenu:
		gameState = newGameState;
		break;

	case GameState::Gameplay:
		gameOverWaveText.setString("Reached wave: " + std::to_string(wave));
		resetGame();
		gameState = newGameState;
		break;

	case GameState::GameOver:
		gameState = newGameState;
		break;
	}
}