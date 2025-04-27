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
	while (isRunning)
	{
		processInput();
		update();
		render();
	}
	return 0;
}

void Game::processInput()
{

}

void Game::update()
{

}

void Game::render()
{
	window.clear();
	window.display();
}