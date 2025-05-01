// ================================================================================================
// File: UIManager.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "UIManager.hpp"

const sf::Color UIManager::TEXT_COLOR = sf::Color(220, 223, 225);
const sf::Color UIManager::TEXT_HOVER_COLOR = sf::Color(240, 244, 249, 215);
const sf::Color UIManager::TEXT_OUTLINE_COLOR = sf::Color(50, 53, 55);
const float UIManager::TEXT_OUTLINE_THICKNESS = 2.f;
const sf::Color UIManager::BUTTON_COLOR = sf::Color(130, 134, 136, 215);
const sf::Color UIManager::BUTTON_HOVER_COLOR = sf::Color(150, 155, 158, 215);
const sf::Color UIManager::BUTTON_OUTLINE_COLOR = sf::Color(110, 114, 116, 215);
const sf::Color UIManager::BUTTON_OUTLINE_HOVER_COLOR = sf::Color(130, 134, 136, 215);
const float UIManager::BUTTON_OUTLINE_THICKNESS = 3.f;
const sf::Color UIManager::BACKGROUND_COLOR = sf::Color(100, 103, 105, 215);
const sf::Color UIManager::BACKGROUND_OUTLINE_COLOR = sf::Color(80, 83, 85, 215);
const float UIManager::BACKGROUND_OUTLINE_THICKNESS = 3.f;

UIManager::UIManager(sf::Vector2u windowSize) :
	windowSize(windowSize),
	font("assets/fonts/BRLNSR.TTF"),
	hud(font, windowSize),
	towerBuildMenu(font, windowSize),
	towerInfoPanel(font, windowSize)
{
	
}

void UIManager::processInput(sf::Vector2f mousePosition, bool isMouseReleased)
{
	if (towerInfoPanel.isTowerSelected())
		towerInfoPanel.processInput(mousePosition, isMouseReleased);
}

void UIManager::update(float fixedTimeStep, sf::Vector2f mousePosition, int lives, int gold, int wave)
{
	hud.update(fixedTimeStep, lives, gold, wave);
	if (towerInfoPanel.isTowerSelected())
		towerInfoPanel.update(fixedTimeStep);
}

void UIManager::render(float interpolationFactor, sf::RenderWindow& window)
{
	hud.render(interpolationFactor, window);
	if (towerInfoPanel.isTowerSelected())
		towerInfoPanel.render(interpolationFactor, window);
}

void UIManager::setSelectedTile(sf::Vector2i tilePosition)
{
	towerBuildMenu.show(Utility::tileToPixelPosition(tilePosition.x, tilePosition.y));
}

void UIManager::setSelectedTower(std::shared_ptr<Tower> tower)
{
	towerInfoPanel.setSelectedTower(tower);
}

void UIManager::clearSelection()
{
	towerInfoPanel.clearTowerSelection();
	towerBuildMenu.hide();
}

bool UIManager::isAnElementHoveredOver(sf::Vector2f mousePosition) const
{
	return towerInfoPanel.isHovered(mousePosition) ||
		   towerBuildMenu.isHovered(mousePosition);
}
