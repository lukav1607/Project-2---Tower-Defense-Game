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

#include "HUD.hpp"
#include "TowerBuildMenu.hpp"
#include "TowerInfoMenu.hpp"

class UIManager
{
public:
	UIManager(const sf::Font& font, sf::Vector2u windowSize, const std::shared_ptr<int>& gold);

	void processInput(sf::Vector2f mousePosition, bool isMouseReleased);
	void update(float fixedTimeStep, int lives, int gold, int wave);
	void render(float interpolationFactor, sf::RenderWindow& window);

	void showTowerInfoMenu(std::shared_ptr<Tower> tower, sf::Vector2u windowSize);
	void showTowerBuildMenu(sf::Vector2i selectedTile, sf::Vector2u windowSize);
	void dismissAllMenus();

	inline sf::Vector2i getSelectedTile() const { return towerBuildMenu.getSelectedTile(); }
	inline Tower::Type getRequestedTowerType() const { return towerBuildMenu.getRequestedTowerType(); }
	bool isAnyMenuHoveredOver() const;

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
	HUD hud;
	TowerBuildMenu towerBuildMenu;
	TowerInfoMenu towerInfoMenu;
};