// ================================================================================================
// File: TowerInfoPanel.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// Description: Defines the TowerInfoPanel class, which is responsible for rendering the tower information panel.
//              This class displays the attributes of the selected tower, including its level, damage, and sell cost,
//              as well as the option to upgrade the tower.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include "../entities/Tower.hpp"

class TowerInfoPanel
{
public:
	TowerInfoPanel(const sf::Font& font, sf::Vector2u windowSize);

	void processInput(sf::Vector2f mousePosition, bool isMouseReleased);
	void update(float fixedTimeStep);
	void render(float interpolationFactor, sf::RenderWindow& window);

	void setSelectedTower(std::shared_ptr<Tower> tower);
	void clearTowerSelection();

	inline bool isTowerSelected() const { return selectedTower != nullptr; }
	bool isHovered(sf::Vector2f mousePosition) const;

private:
	void updateInfoText();

	Button upgradeButton;
	Button sellButton;
	bool wasTowerJustUpgraded;
	bool hasTowerReachedMaxLevel;

	std::shared_ptr<Tower> selectedTower;
	sf::RectangleShape background;
	sf::Text info;

	sf::Vector2u windowSize;
};