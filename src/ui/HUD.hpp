// ================================================================================================
// File: HUD.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// Description: Defines the HUD class, which is responsible for rendering the heads-up display (HUD) of the game.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>

class HUD
{
public:
	HUD(const sf::Font& font, sf::Vector2u windowSize);

	void update(float fixedTimeStep, int lives, int gold, int wave);
	void render(float interpolationFactor, sf::RenderWindow& window);

private:

	sf::Text livesText;
	sf::Text goldText;
	sf::Text waveText;

	sf::Vector2u windowSize;
};