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

TowerBuildMenu::Option::Option(const sf::Font& font) :
	type(Tower::Type::Count),
	name(font, "N/A", 32U),
	description(font, "N/A", 24U),
	buyCost(0),
	button(sf::Font(), "N/A", sf::Vector2f(0.f, 0.f))
{}

TowerBuildMenu::TowerBuildMenu(const sf::Font& font, const std::string& title, sf::Vector2f size, const std::shared_ptr<int>& gold) :
	Menu(font, title, size),
	gold(gold),
	selectedTile({ -1, -1 }),
	requestedTowerType(Tower::Type::Count)
{
	for (int i = 0; i < static_cast<int>(Tower::Type::Count); ++i)
	{
		Tower dummy(static_cast<Tower::Type>(i), { 0, 0 });

		Option option(font);
		option.type = static_cast<Tower::Type>(i);
		option.buyCost = dummy.attributes[0].buyCost;
		option.name.setString(dummy.getName());
		option.name.setOutlineThickness(UIManager::TEXT_OUTLINE_THICKNESS);
		option.name.setOutlineColor(UIManager::TEXT_OUTLINE_COLOR);
		option.description.setString
		(
			"Damage: " + std::to_string(dummy.attributes[0].damage) +
			"\nRange: " + std::to_string(static_cast<int>(dummy.attributes[0].range)) +
			"\nFire Rate: " + Utility::removeTrailingZeros(dummy.attributes[0].fireRate)
		);
		option.button = Button(font, std::to_string(dummy.getBaseBuyCost()) + std::string("g"), sf::Vector2f(150.f, 50.f));

		options.push_back(option);
	}
}

void TowerBuildMenu::processInput(sf::Vector2f mousePosition, bool isMouseReleased)
{
	if (!m_isActive) return;

	m_isHovered = background.getGlobalBounds().contains(mousePosition);

	for (auto& option : options)
	{
		option.button.processInput(mousePosition, isMouseReleased);
		if (option.button.isClicked() && *gold >= option.buyCost)
			requestedTowerType = option.type;
	}
}

void TowerBuildMenu::update(float fixedTimeStep)
{
	if (!m_isActive) return;

	for (auto& option : options)
	{
		if (*gold >= option.buyCost)
			option.button.setIsActive(true);
		else
			option.button.setIsActive(false);

		option.button.update(fixedTimeStep);
	}
}

void TowerBuildMenu::render(float interpolationFactor, sf::RenderWindow& window)
{
	if (!m_isActive) return;

	window.draw(background);
	for (auto& option : options)
	{
		window.draw(option.name);
		window.draw(option.description);
		option.button.render(interpolationFactor, window);
	}
}

void TowerBuildMenu::setSelectedTile(sf::Vector2i tilePosition, sf::Vector2u windowSize)
{
	selectedTile = tilePosition;
	m_isActive = true;

	updateLayout(windowSize);

	for (auto& option : options)
	{
		if (*gold >= option.buyCost)
			option.button.setIsActive(true);
		else
			option.button.setIsActive(false);
	}
}

void TowerBuildMenu::clearTileSelection()
{
	selectedTile = { -1, -1 };
	requestedTowerType = Tower::Type::Count;
	m_isActive = false;
}

void TowerBuildMenu::updateLayout(sf::Vector2u windowSize)
{
	position = Utility::tileToPixelPosition(selectedTile.x, selectedTile.y);

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

	// Set the position of the background, name and descrpition text and buttons for each option
	background.setPosition(position);

	for (int i = 0; i < options.size(); ++i)
	{
		options[i].name.setPosition
		({
			position.x + padding.x + options[i].name.getGlobalBounds().size.x * i,
			position.y + padding.y
		});
		options[i].description.setPosition
		({
			position.x + padding.x * 1.5f + options[i].name.getGlobalBounds().size.x * i,
			position.y + padding.y * (i + 2) + options[i].name.getGlobalBounds().size.y
		});
		options[i].button.setPosition
		({
			position.x + padding.x * 1.5f + options[i].name.getGlobalBounds().size.x * i,
			position.y + padding.y * (i + 3) + options[i].name.getGlobalBounds().size.y + options[i].description.getGlobalBounds().size.y
		});
	}
}