// ================================================================================================
// File: Button.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// Description: Defines the Button class, which represents a clickable button in the user interface.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>

class Button
{
public:
	Button(sf::Vector2f size, const sf::Font& font, const std::string& text);

	void processInput(sf::Vector2f mousePosition, bool isMouseReleased);
	void update(float fixedTimeStep, bool isActive);
	void render(float interpolationFactor, sf::RenderWindow& window);

	void setPosition(sf::Vector2f position);
	void setText(const std::string& text);

	inline bool isHovered() const { return m_isHovered; }
	inline bool isClicked() const { return m_isClicked; }
	sf::Vector2f getSize() const { return shape.getSize(); }

private:
	sf::RectangleShape shape;
	sf::Text text;

	bool m_isHovered;
	bool m_isClicked;
};