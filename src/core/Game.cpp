// ================================================================================================
// File: Game.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// Description: Contains the implementation of the Game class, which manages the game loop,
//              updates entities, handles events, and renders graphics.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Game.hpp"

Game::Game() :
	isRunning(true),
	antiAliasingLevel(8),
	isVSyncEnabled(true)
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
}

void Game::update(float fixedTimeStep)
{
	
}

void Game::render(float interpolationFactor)
{
	window.clear();
	window.display();
}