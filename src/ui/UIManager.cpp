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
const sf::Color UIManager::TEXT_HOVER_COLOR = sf::Color(240, 244, 249);
const sf::Color UIManager::TEXT_OUTLINE_COLOR = sf::Color(50, 53, 55);
const float UIManager::TEXT_OUTLINE_THICKNESS = 2.f;
const sf::Color UIManager::BUTTON_COLOR = sf::Color(130, 134, 136);
const sf::Color UIManager::BUTTON_HOVER_COLOR = sf::Color(150, 155, 158);
const sf::Color UIManager::BUTTON_OUTLINE_COLOR = sf::Color(110, 114, 116);
const sf::Color UIManager::BUTTON_OUTLINE_HOVER_COLOR = sf::Color(130, 134, 136);
const float UIManager::BUTTON_OUTLINE_THICKNESS = 3.f;
const sf::Color UIManager::BACKGROUND_COLOR = sf::Color(100, 103, 105);
const sf::Color UIManager::BACKGROUND_OUTLINE_COLOR = sf::Color(80, 83, 85);
const float UIManager::BACKGROUND_OUTLINE_THICKNESS = 3.f;

UIManager::UIManager(const sf::Font& font, sf::Vector2u windowSize, const std::shared_ptr<int>& gold) :
	hud(font, { 1200, 1200 }),
	towerInfoMenu(font, "Tower Info", { 300.f, 300.f }, gold),
	towerBuildMenu(font, "Choose a Tower to Build", { 620.f, 300.f }, gold)
{}

void UIManager::processInput(sf::Vector2f mousePosition, bool isMouseReleased)
{
	if (towerInfoMenu.isActive())
		towerInfoMenu.processInput(mousePosition, isMouseReleased);
	else if (towerBuildMenu.isActive())
		towerBuildMenu.processInput(mousePosition, isMouseReleased);
}

void UIManager::update(float fixedTimeStep, int lives, int gold, int wave)
{
	hud.update(fixedTimeStep, lives, gold, wave);
	if (towerInfoMenu.isActive())
		towerInfoMenu.update(fixedTimeStep);
	else if (towerBuildMenu.isActive())
		towerBuildMenu.update(fixedTimeStep);
}

void UIManager::render(float interpolationFactor, sf::RenderWindow& window)
{
	hud.render(interpolationFactor, window);
	if (towerInfoMenu.isActive())
		towerInfoMenu.render(interpolationFactor, window);
	else if (towerBuildMenu.isActive())
		towerBuildMenu.render(interpolationFactor, window);
}

void UIManager::showTowerInfoMenu(std::shared_ptr<Tower> tower, sf::Vector2u windowSize)
{
	if (tower)
		towerInfoMenu.setSelectedTower(tower, windowSize);
	else
		towerInfoMenu.clearTowerSelection();
}

void UIManager::showTowerBuildMenu(sf::Vector2i selectedTile, sf::Vector2u windowSize)
{
	towerBuildMenu.clearTileSelection();
	towerBuildMenu.setSelectedTile(selectedTile, windowSize);
}

void UIManager::dismissAllMenus()
{
	towerInfoMenu.clearTowerSelection();
	towerBuildMenu.clearTileSelection();
}

bool UIManager::isAnyMenuHoveredOver() const
{
	return (towerInfoMenu.isHovered() && towerInfoMenu.isActive()) ||
		   (towerBuildMenu.isHovered() && towerBuildMenu.isActive());
}
