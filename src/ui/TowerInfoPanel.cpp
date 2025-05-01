// ================================================================================================
// File: TowerInfoPanel.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include <sstream>
#include "TowerInfoPanel.hpp"
#include "UIManager.hpp"

TowerInfoPanel::TowerInfoPanel(const sf::Font& font, sf::Vector2u windowSize) :
	windowSize(windowSize),
	upgradeButton(sf::Vector2f(150.f, 50.f), font, "UPGRADE"),
	sellButton(sf::Vector2f(80.f, 50.f), font, "SELL"),
	wasTowerJustUpgraded(false),
	hasTowerReachedMaxLevel(false),
	info(font, "N/A", 24U)
{
	background.setSize({ 300.f, 250.f });
	background.setFillColor(UIManager::BACKGROUND_COLOR);
	background.setOutlineColor(UIManager::BACKGROUND_OUTLINE_COLOR);
	background.setOutlineThickness(UIManager::BACKGROUND_OUTLINE_THICKNESS);
}

void TowerInfoPanel::processInput(sf::Vector2f mousePosition, bool isMouseReleased)
{
	if (!isTowerSelected()) return;

	upgradeButton.processInput(mousePosition, isMouseReleased);
	sellButton.processInput(mousePosition, isMouseReleased);

	if (upgradeButton.isClicked() && (selectedTower->getLevel() < selectedTower->getMaxLevel()))
	{
		selectedTower->markForUpgrade();
		wasTowerJustUpgraded = true;
	}
	else if (sellButton.isClicked())
	{
		selectedTower->markForSale();
		clearTowerSelection();
	}
}

void TowerInfoPanel::update(float fixedTimeStep)
{
	if (!isTowerSelected()) return;

	if (wasTowerJustUpgraded)
	{
		updateInfoText();
		wasTowerJustUpgraded = false;
	}

	upgradeButton.update(fixedTimeStep, selectedTower->getLevel() < selectedTower->getMaxLevel());
	sellButton.update(fixedTimeStep, true);
}

void TowerInfoPanel::render(float interpolationFactor, sf::RenderWindow& window)
{
	if (!isTowerSelected()) return;

	window.draw(background);
	window.draw(info);
	upgradeButton.render(interpolationFactor, window);
	sellButton.render(interpolationFactor, window);
}

void TowerInfoPanel::setSelectedTower(std::shared_ptr<Tower> tower)
{
	selectedTower = tower;
	updateInfoText();
	selectedTower->setRangeCircleVisible(true);
	background.setPosition(Utility::tileToPixelPosition(tower->getTilePosition().x, tower->getTilePosition().y) + sf::Vector2f(30.f, 0.f));
	info.setPosition(background.getPosition() + sf::Vector2f(10.f, 10.f));
	upgradeButton.setPosition(
		{ 
			background.getGlobalBounds().position.x + 20.f,
			background.getGlobalBounds().position.y + background.getGlobalBounds().size.y - upgradeButton.getSize().y - 20.f
		});
	sellButton.setPosition(
		{
			background.getGlobalBounds().position.x + background.getGlobalBounds().size.x - sellButton.getSize().x - 20.f,
			background.getGlobalBounds().position.y + background.getGlobalBounds().size.y - sellButton.getSize().y - 20.f
		});
}

void TowerInfoPanel::clearTowerSelection()
{
	if (selectedTower)
	{
		selectedTower->setRangeCircleVisible(false);
		selectedTower.reset();
	}
}

bool TowerInfoPanel::isHovered(sf::Vector2f mousePosition) const
{
	return background.getGlobalBounds().contains(mousePosition) ||
		   sellButton.isHovered() ||
		   upgradeButton.isHovered();
}

void TowerInfoPanel::updateInfoText()
{
	if (!selectedTower) return;

	const auto& attributes = selectedTower->attributes[selectedTower->getLevel()];
	std::ostringstream ss;

	if (selectedTower->getLevel() >= selectedTower->getMaxLevel())
	{
		ss << "Level: " << selectedTower->getLevel() << "\n"
			<< "Damage: " << attributes.damage << "\n"
			<< "Range: " << static_cast<int>(attributes.range) << "\n"
			<< "Fire Rate: " << attributes.fireRate << "\n"
			<< "No more upgrades!" << "\n"
			<< "Sell Value: " << attributes.sellCost;
	}
	else
	{
		ss << "Level: " << selectedTower->getLevel() + 1 << "\n"
			<< "Damage: " << attributes.damage << "\n"
			<< "Range: " << static_cast<int>(attributes.range) << "\n"
			<< "Fire Rate: " << attributes.fireRate << "\n"
			<< "Upgrade Cost: " << selectedTower->attributes[selectedTower->getLevel() + 1].buyCost << "\n"
			<< "Sell Value: " << attributes.sellCost;
	}

	info.setString(ss.str());
}
