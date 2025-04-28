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

class Game
{
public:
	Game();
	int run();

private:
	void processInput();
	void update(float fixedTimeStep);
	void render(float interpolationFactor);

	bool isRunning;

	sf::RenderWindow window;
	const sf::Vector2u WINDOW_SIZE = { 1600, 1200 };
	const sf::String WINDOW_TITLE = "Project 2 - Tower Defense Game";
	unsigned antiAliasingLevel;
	bool isVSyncEnabled;
};