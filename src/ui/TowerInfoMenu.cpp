// ================================================================================================
// File: TowerInfoMenu.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include <sstream>
#include "TowerInfoMenu.hpp"

TowerInfoMenu::TowerInfoMenu(const sf::Font& font, const std::string& title, sf::Vector2f size, const std::shared_ptr<int>& gold) :
	Menu(font, title, size),
	gold(gold),
	wasUpgradeButtonHoveredLastFrame(false),
	needsTextUpdate(false),
	infoText(font, "N/A", 24U),
	upgradeButton(font, "UPGRADE\n(N/Ag)", { 130.f, 60.f }),
	sellButton(font, "SELL\n(N/Ag)", { 90.f, 60.f })
{

}

void TowerInfoMenu::processInput(sf::Vector2f mousePosition, bool isMouseReleased)
{
	if (!m_isActive) return;

	// Deactivate the upgrade button if the tower is at max level
	if (upgradeButton.isActive() && selectedTower && selectedTower->getLevel() >= selectedTower->getMaxLevel())
		upgradeButton.setIsActive(false);

	m_isHovered = background.getGlobalBounds().contains(mousePosition);

	// Check if the mouse has JUST started or stopped hovering over the upgrade button
	if (wasUpgradeButtonHoveredLastFrame != upgradeButton.isHovered())
	{
		needsTextUpdate = true;
		wasUpgradeButtonHoveredLastFrame = upgradeButton.isHovered();
	}

	upgradeButton.processInput(mousePosition, isMouseReleased);
	sellButton.processInput(mousePosition, isMouseReleased);

	// Check if the upgrade button is clicked and the tower is not at max level
	if (upgradeButton.isClicked() && selectedTower && selectedTower->getLevel() < selectedTower->getMaxLevel())
	{
		selectedTower->markForUpgrade();
		needsTextUpdate = true;
	}
	// Check if the sell button is clicked
	else if (sellButton.isClicked() && selectedTower)
	{
		selectedTower->markForSale();
		clearTowerSelection();
	}
}

void TowerInfoMenu::update(float fixedTimeStep)
{
	if (!m_isActive) return;

	if (needsTextUpdate)
	{
		updateInfoText();
		needsTextUpdate = false;
	}

	if (*gold >= selectedTower->attributes[selectedTower->getLevel() + 1].buyCost)
		upgradeButton.setIsActive(true);
	else
		upgradeButton.setIsActive(false);
	
	upgradeButton.update(fixedTimeStep);
	sellButton.update(fixedTimeStep);	
}

void TowerInfoMenu::render(float interpolationFactor, sf::RenderWindow& window)
{
	if (!m_isActive) return;

	window.draw(background);
	window.draw(titleText);
	window.draw(infoText);
	upgradeButton.render(interpolationFactor, window);
	sellButton.render(interpolationFactor, window);
}

void TowerInfoMenu::setSelectedTower(std::shared_ptr<Tower> tower, sf::Vector2u windowSize)
{
	m_isActive = true;
	selectedTower = tower;
	selectedTower->setRangeCircleVisible(true);

	// Deactivate upgrade button if the tower is at max level
	if (selectedTower->getLevel() >= selectedTower->getMaxLevel())
		upgradeButton.setIsActive(false);
	else
		upgradeButton.setIsActive(true);

	titleText.setString(selectedTower->getName());
	updateInfoText();	
	updateLayout(windowSize);
}

void TowerInfoMenu::clearTowerSelection()
{
	if (selectedTower)
	{
		selectedTower->setRangeCircleVisible(false);
		selectedTower.reset();
	}
	m_isActive = false;
}

void TowerInfoMenu::updateInfoText()
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
		std::ostringstream upgradeButtonTextSS;
		upgradeButtonTextSS << "UPGRADE\n" << selectedTower->attributes[selectedTower->getLevel() + 1].buyCost << "g";
		upgradeButton.setText(upgradeButtonTextSS.str());

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
		upgradeButton.setText("UPGRADE\nN/A");
	}

	ss << "Level: " << selectedTower->getLevel() + 1 << levelAppend << "\n"
		<< "Damage: " << attributes.damage << damageAppend << "\n"
		<< "Range: " << static_cast<int>(attributes.range) << rangeAppend << "\n"
		<< "Fire Rate: " << attributes.fireRate << fireRateAppend;

	infoText.setString(ss.str());
}

void TowerInfoMenu::updateLayout(sf::Vector2u windowSize)
{
	if (!selectedTower) return;

	position = Utility::tileToPixelPosition(selectedTower->getTilePosition().x, selectedTower->getTilePosition().y);

	// Offset the position to center the menu below the tower
	sf::Vector2f offset = { -background.getSize().x / 2.f, 60.f };
	position += offset;

	// Check if the menu goes out of window bounds and adjust the position accordingly
	if (position.x + background.getSize().x > windowSize.x)
		position.x = windowSize.x - background.getSize().x;
	else if (position.x < 0)
		position.x = 0;
	if (position.y + background.getSize().y > windowSize.y)
		position.y = windowSize.y - background.getSize().y;
	else if (position.y < 0)
		position.y = 0;

	// Set the position of the background, info text, and buttons
	background.setPosition(position);

	titleText.setOrigin(
		{ 
			titleText.getGlobalBounds().size.x / 2.f,
			titleText.getGlobalBounds().size.y / 2.f
		});
	titleText.setPosition(
		{ 
			background.getPosition().x + background.getSize().x / 2.f,
			background.getPosition().y + titleText.getGlobalBounds().size.y / 2.f + padding.y
		});
	infoText.setPosition(
		{
			background.getPosition().x + padding.x,
			background.getPosition().y + titleText.getGlobalBounds().size.y + padding.y * 2.f
		});

	upgradeButton.setPosition(
		{ 
			background.getPosition().x + padding.x,
			background.getPosition().y + background.getSize().y - upgradeButton.getSize().y - padding.y
		});
	sellButton.setPosition(
		{
			background.getPosition().x + background.getSize().x - sellButton.getSize().x - padding.x,
			background.getPosition().y + background.getSize().y - sellButton.getSize().y - padding.y
		});
}