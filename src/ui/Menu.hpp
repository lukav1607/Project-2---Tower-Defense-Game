// ================================================================================================
// File: Menu.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 3, 2025
// Description: Defines the Menu class, which is the base class for creating menus in the game.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include "Button.hpp"

class Menu
{
public:
	Menu(const sf::Font& font, const std::string& title, sf::Vector2f size);
	Menu(const Menu&) = default;
	Menu& operator=(const Menu&) = default;
	virtual ~Menu() = default;

	virtual void processInput(sf::Vector2f mousePosition, bool isMouseReleased) = 0;
	virtual void update(float fixedTimeStep) = 0;
	virtual void render(float interpolationFactor, sf::RenderWindow& window) = 0;

	inline bool isHovered() const { return m_isHovered; }

protected:
	bool m_isActive;
	bool m_isHovered;

	sf::Vector2f padding;
	sf::Vector2f position;

	sf::RectangleShape background;
	sf::Text titleText;
};