// ================================================================================================
// File: TowerInfoMenu.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// Description: Defines the TowerInfoMenu class, which is responsible for rendering the tower information panel.
//              This class displays the attributes of the selected tower, including its level, damage, and sell cost,
//              as well as the option to upgrade the tower.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include "Menu.hpp"
#include "../entities/Tower.hpp"

class TowerInfoMenu : public Menu
{
public:
	TowerInfoMenu(const sf::Font& font, const std::string& title, sf::Vector2f size, const std::shared_ptr<int>& gold);

	void processInput(sf::Vector2f mousePosition, bool isMouseReleased) override;
	void update(float fixedTimeStep) override;
	void render(float interpolationFactor, sf::RenderWindow& window) override;

	void setSelectedTower(std::shared_ptr<Tower> tower, sf::Vector2u windowSize);
	void clearTowerSelection();

	inline bool isTowerSelected() const { return selectedTower != nullptr; }
	inline bool isActive() const { return m_isActive; }
	inline bool isHovered() const { return m_isHovered; }

private:
	void updateInfoText();
	void updateLayout(sf::Vector2u windowSize);

	const std::shared_ptr<int>& gold;

	bool wasUpgradeButtonHoveredLastFrame;
	bool needsTextUpdate;

	std::shared_ptr<Tower> selectedTower;
	sf::Text infoText;
	Button upgradeButton;
	Button sellButton;
};