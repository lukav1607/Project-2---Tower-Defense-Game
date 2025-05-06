// ================================================================================================
// File: Game.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// Description: Contains the implementation of the Game class, which manages the game loop,
//              updates entities, handles events, and renders graphics.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "Grid.hpp"
#include "../entities/Enemy.hpp"
#include "../entities/Tower.hpp"
#include "../ui/UIManager.hpp"

class Game
{
public:
	Game();
	int run();

	static const bool IS_DEBUG_MODE_ON;

private:
	void processInput();
	void update(float fixedTimeStep);
	void render(float interpolationFactor);

	enum class GameState
	{
		MainMenu,
		Gameplay,
		GameOver
	};
	void switchGameState(GameState newGameState);

	void updateWave(float fixedTimeStep);
	void deselectAllTowers();
	void resetGame();

	// Loop and state management
	GameState gameState;
	bool isRunning;

	// Window
	sf::RenderWindow window;
	const sf::Vector2u WINDOW_SIZE = { 1200, 1200 };
	const sf::String WINDOW_TITLE = "Project 2 - Tower Defense Game";
	unsigned antiAliasingLevel;
	bool isVSyncEnabled;

	// Gameplay data
	const int STARTING_LIVES = 5;
	const int STARTING_GOLD = 100;
	int lives;
	std::shared_ptr<int> gold;

	Grid grid;

	std::vector<std::shared_ptr<Tower>> towers;

	// Enemies
	std::vector<Enemy> enemies;
	float timeBetweenWaves;
	float timeSinceLastWaveEnded;
	float timeBetweenEnemies;
	float timeSinceLastEnemySpawned;
	int wave;
	int enemiesPerWave;
	int enemiesSpawnedThisWave;
	bool waitingForFirstEnemyInWave;

	sf::Font font;
	UIManager ui;
	sf::Text titleText;
	sf::Text startText;
	sf::Text authorText;
	sf::Text gameOverText;
	sf::Text gameOverWaveText;
	sf::Text restartText;
};