// ================================================================================================
// File: TowerBuildMenu.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// Description: Defines the TowerBuildMenu class, which is responsible for rendering the tower build menu.
//              This class handles the display of available towers, their attributes, and buy cost.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include "Menu.hpp"
#include "../entities/Tower.hpp"

class TowerBuildMenu : public Menu
{
public:
	TowerBuildMenu(const sf::Font& font, const std::string& title, sf::Vector2f size, const std::shared_ptr<int>& gold);

	void processInput(sf::Vector2f mousePosition, bool isMouseReleased);
	void update(float fixedTimeStep);
	void render(float interpolationFactor, sf::RenderWindow& window);

	inline Tower::Type getRequestedTowerType() const { return requestedTowerType; }
	void setSelectedTile(sf::Vector2i tilePosition, sf::Vector2u windowSize);
	void clearTileSelection();

	sf::Vector2i getSelectedTile() const { return selectedTile; }
	inline bool isActive() const { return m_isActive; }
	inline bool isHovered() const { return m_isHovered; }

private:
	void updateLayout(sf::Vector2u windowSize);

	const std::shared_ptr<int>& gold;

	struct Option
	{
		Option(const sf::Font& font);
		Tower::Type type;
		sf::Text name;
		sf::Text description;
		int buyCost;
		Button button;
	};
	std::vector<Option> options;

	sf::Vector2i selectedTile;
	Tower::Type requestedTowerType;

//public:
//	TowerBuildMenu(const sf::Font& font, sf::Vector2u windowSize, std::shared_ptr<int> gold);
//
//	void processInput(sf::Vector2f mousePosition, bool isMouseReleased);
//	void update(float fixedTimeStep);
//	void render(float interpolationFactor, sf::RenderWindow& window);
//
//	void show(sf::Vector2i selectedTile);
//	void hide();
//	inline void clearRequestedTowerType() { requestedTowerType = Tower::Type::Count; }
//	inline void setRequestedTowerType(Tower::Type type) { requestedTowerType = type; }
//	inline void setSelectedTile(sf::Vector2i tilePosition) { selectedTile = tilePosition; }
//
//	bool isHovered(sf::Vector2f mousePosition) const;
//	bool isTileSelected() const { return isOpen; }
//	Tower::Type getRequestedTowerType() const { return requestedTowerType; }
//	sf::Vector2i getSelectedTile() const { return selectedTile; }
//	
//	std::function<void(Tower::Type, sf::Vector2i)> onTowerSelected;
//
//private:
//	Tower::Type requestedTowerType;
//
//	struct Option
//	{
//		Option();
//		Tower::Type type;
//		std::string name;
//		std::string description;
//		int buyCost;
//		Button button;
//	};
//	std::vector<Option> options;
//
//	bool isOpen;
//	std::shared_ptr<int> gold;
//
//	sf::RectangleShape background;
//	sf::Vector2i selectedTile;
//
//	sf::Vector2u windowSize;
};