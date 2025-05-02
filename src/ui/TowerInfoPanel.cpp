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
	upgradeButton(sf::Vector2f(170.f, 100.f), font, "UPGRADE\n(N/Ag)"),
	sellButton(sf::Vector2f(130.f, 100.f), font, "SELL\n(N/Ag)"),
	hasTowerReachedMaxLevel(false),
	info(font, "N/A", 30U)
{
	background.setSize({ 400.f, 300.f });
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

	updateInfoText();

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
	selectedTower->setRangeCircleVisible(true);
	updateInfoText();

	sf::Vector2f position = Utility::tileToPixelPosition(tower->getTilePosition().x, tower->getTilePosition().y);
	sf::Vector2f backgroundOffset = { 35.f, 0.f };
	position += backgroundOffset;
	if (position.x + background.getSize().x > windowSize.x)
		position.x -= backgroundOffset.x * 2 + background.getSize().x;

	background.setPosition(position);
	info.setPosition(background.getPosition() + sf::Vector2f(20.f, 15.f));
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

	std::string levelAppend;
	std::string damageAppend;
	std::string rangeAppend;
	std::string fireRateAppend;

	sellButton.setText("SELL\n" + std::to_string(attributes.sellCost) + "g");

	if (selectedTower->getLevel() < selectedTower->getMaxLevel())
	{
		std::ostringstream upgradeButtonTextS;
		upgradeButtonTextS << "UPGRADE\n" << selectedTower->attributes[selectedTower->getLevel() + 1].buyCost << "g";
		upgradeButton.setText(upgradeButtonTextS.str());

		if (upgradeButton.isHovered())
		{
			const auto& nextLevelAttributes = selectedTower->attributes[selectedTower->getLevel() + 1];

			levelAppend = " > " + std::to_string(selectedTower->getLevel() + 2);
			damageAppend = " > " + std::to_string(nextLevelAttributes.damage);
			rangeAppend = " > " + std::to_string(static_cast<int>(nextLevelAttributes.range));
			fireRateAppend = " > " + Utility::removeTrailingZeros(nextLevelAttributes.fireRate);
		}
	}
	else if (selectedTower->getLevel() >= selectedTower->getMaxLevel())
	{
		upgradeButton.setText("UPGRADE\n     N/A");
	}

	ss << "Level: " << selectedTower->getLevel() + 1 << levelAppend << "\n"
		<< "Damage: " << attributes.damage << damageAppend << "\n"
		<< "Range: " << static_cast<int>(attributes.range) << rangeAppend << "\n"
		<< "Fire Rate: " << attributes.fireRate << fireRateAppend;

	info.setString(ss.str());
}
