// ================================================================================================
// File: UIManager.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// Description: Defines the UIManager class, which is responsible for managing the user interface of the game.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include "HUD.hpp"
#include "TowerBuildMenu.hpp"
#include "TowerInfoPanel.hpp"
#include "../entities/Tower.hpp"

class UIManager
{
public:
	UIManager(sf::Vector2u windowSize);

	void processInput(sf::Vector2f mousePosition, bool isMouseReleased);
	void update(float fixedTimeStep, sf::Vector2f mousePosition, int lives, int gold, int wave);
	void render(float interpolationFactor, sf::RenderWindow& window);

	void setSelectedTile(sf::Vector2i tilePosition);
	void setSelectedTower(std::shared_ptr<Tower> tower);
	void clearSelection();

	bool isAnElementHoveredOver(sf::Vector2f mousePosition) const;

	static const sf::Color TEXT_COLOR;
	static const sf::Color TEXT_HOVER_COLOR;
	static const sf::Color TEXT_OUTLINE_COLOR;
	static const float TEXT_OUTLINE_THICKNESS;
	static const sf::Color BUTTON_COLOR;
	static const sf::Color BUTTON_HOVER_COLOR;
	static const sf::Color BUTTON_OUTLINE_COLOR;
	static const sf::Color BUTTON_OUTLINE_HOVER_COLOR;
	static const float BUTTON_OUTLINE_THICKNESS;
	static const sf::Color BACKGROUND_COLOR;
	static const sf::Color BACKGROUND_OUTLINE_COLOR;
	static const float BACKGROUND_OUTLINE_THICKNESS;

private:
	sf::Font font;

	HUD hud;
	TowerBuildMenu towerBuildMenu;
	TowerInfoPanel towerInfoPanel;

	sf::Vector2u windowSize;
};