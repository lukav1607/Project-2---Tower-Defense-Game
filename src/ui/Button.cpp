// ================================================================================================
// File: Button.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 1, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include "Button.hpp"
#include "UIManager.hpp"

Button::Button(const sf::Font& font, const std::string& text, sf::Vector2f size) :
	m_isActive(true),
	wasHoveredLastFrame(false),
	m_isHovered(false),
	needsColorUpdate(false),
	m_isClicked(false),
	text(font, text, 24U)
{
	background.setOutlineThickness(UIManager::BUTTON_OUTLINE_THICKNESS);
	background.setSize(size);
	updateColors();
}

void Button::processInput(sf::Vector2f mousePosition, bool isMouseReleased)
{
	if (!m_isActive) return;

	m_isClicked = false;

	m_isHovered = background.getGlobalBounds().contains(mousePosition);

	if (wasHoveredLastFrame != m_isHovered)
	{
		needsColorUpdate = true;
		wasHoveredLastFrame = m_isHovered;
	}

	if (m_isHovered && isMouseReleased)
		m_isClicked = true;

}

void Button::update(float fixedTimeStep)
{
	if (needsColorUpdate)
	{
		updateColors();
		needsColorUpdate = false;
	}
}

void Button::render(float interpolationFactor, sf::RenderWindow& window)
{
	window.draw(background);
	window.draw(text);
}

void Button::setIsActive(bool isActive)
{
	m_isActive = isActive;
	needsColorUpdate = true;
}

void Button::setPosition(sf::Vector2f position)
{
	this->position = position;
	background.setPosition(position);
	centerText();
}

void Button::setText(const std::string& text)
{
	this->text.setString(text);
	centerText();
}

void Button::updateColors()
{
	if (m_isActive)
	{
		if (m_isHovered)
		{
			background.setFillColor(UIManager::BUTTON_HOVER_COLOR);
			background.setOutlineColor(UIManager::BUTTON_OUTLINE_HOVER_COLOR);
			text.setFillColor(UIManager::TEXT_HOVER_COLOR);
		}
		else
		{
			background.setFillColor(UIManager::BUTTON_COLOR);
			background.setOutlineColor(UIManager::BUTTON_OUTLINE_COLOR);
			text.setFillColor(UIManager::TEXT_COLOR);
		}
	}
	else
	{
		background.setFillColor(UIManager::BACKGROUND_COLOR);
		background.setOutlineColor(UIManager::BACKGROUND_OUTLINE_COLOR);
		text.setFillColor(UIManager::BACKGROUND_OUTLINE_COLOR);
	}
}

void Button::centerText()
{
	sf::FloatRect textBounds = text.getGlobalBounds();
	text.setOrigin({ textBounds.size.x / 2.f, textBounds.size.y / 1.5f });
	text.setPosition({ position.x + background.getSize().x / 2.f, position.y + background.getSize().y / 2.f });
}