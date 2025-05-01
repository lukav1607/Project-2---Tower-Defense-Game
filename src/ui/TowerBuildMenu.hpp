// ================================================================================================
// File: TowerBuildMenu.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// Description: Defines the TowerBuildMenu class, which is responsible for rendering the tower build menu.
//              This class handles the display of available towers, their attributes, and buy cost.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "../entities/Tower.hpp"

class TowerBuildMenu
{
public:
	TowerBuildMenu(const sf::Font& font, sf::Vector2u windowSize);

	void update(float fixedTimeStep, sf::Vector2f mousePosition);
	void render(float interpolationFactor, sf::RenderWindow& window);

	void show(sf::Vector2f position);
	void hide();

	bool isHovered(sf::Vector2f mousePosition) const;
	bool isTileSelected() const { return isOpen; }

private:
	std::vector<Button> buttons;
	bool isOpen;

	sf::RectangleShape background;

	sf::Vector2f position;

	sf::Vector2u windowSize;
};