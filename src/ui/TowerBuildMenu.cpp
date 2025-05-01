// ================================================================================================
// File: TowerBuildMenu.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "TowerBuildMenu.hpp"
#include "UIManager.hpp"

TowerBuildMenu::TowerBuildMenu(const sf::Font& font, sf::Vector2u windowSize) :
	isOpen(false),
	windowSize(windowSize)
{
	background.setSize({ 250.f, 250.f });
	background.setFillColor(UIManager::BACKGROUND_COLOR);
	background.setOutlineColor(UIManager::BACKGROUND_OUTLINE_COLOR);
	background.setOutlineThickness(UIManager::BACKGROUND_OUTLINE_THICKNESS);
}

void TowerBuildMenu::update(float fixedTimeStep, sf::Vector2f mousePosition)
{
}

void TowerBuildMenu::render(float interpolationFactor, sf::RenderWindow& window)
{
}

void TowerBuildMenu::show(sf::Vector2f position)
{
	this->position = position;
	isOpen = true;
	background.setPosition(position);

	for (int i = 0; i < buttons.size(); i++)
	{
		buttons.at(i).setPosition(position);
	}
}

void TowerBuildMenu::hide()
{
	isOpen = false;
}

bool TowerBuildMenu::isHovered(sf::Vector2f mousePosition) const
{
	return background.getGlobalBounds().contains(mousePosition) ||
			std::any_of(buttons.begin(), buttons.end(), [&](const Button& button)
			{
				return button.isHovered();
			});
}
