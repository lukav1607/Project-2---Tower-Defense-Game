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
	Button(const sf::Font& font, const std::string& text, sf::Vector2f size);

	void processInput(sf::Vector2f mousePosition, bool isMouseReleased);
	void update(float fixedTimeStep);
	void render(float interpolationFactor, sf::RenderWindow& window);

	void setIsActive(bool isActive);
	void setPosition(sf::Vector2f position);
	void setText(const std::string& text);

	inline bool isHovered() const { return m_isHovered; }
	inline bool isClicked() const { return m_isClicked; }
	inline bool isActive() const { return m_isActive; }
	sf::Vector2f getSize() const { return background.getSize(); }

private:
	void updateColors();
	void centerText();

	sf::Vector2f position;
	sf::RectangleShape background;
	sf::Text text;

	bool m_isActive;
	bool wasHoveredLastFrame;
	bool m_isHovered;
	bool needsColorUpdate; // When true, the button needs to update its color based on hover state
	bool m_isClicked;
};