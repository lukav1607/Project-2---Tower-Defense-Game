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

Button::Button(sf::Vector2f size, const sf::Font& font, const std::string& text) :
	m_isHovered(false),
	m_isClicked(false),
	text(font, text, 30U)
{
	shape.setFillColor(UIManager::BUTTON_COLOR);
	shape.setOutlineColor(UIManager::BUTTON_OUTLINE_COLOR);
	shape.setOutlineThickness(UIManager::BUTTON_OUTLINE_THICKNESS);
	shape.setSize(size);

	this->text.setFillColor(UIManager::TEXT_COLOR);
}

void Button::processInput(sf::Vector2f mousePosition, bool isMouseReleased)
{
	m_isClicked = false;
	m_isHovered = shape.getGlobalBounds().contains(mousePosition);

	if (m_isHovered && isMouseReleased)
		m_isClicked = true;
}

void Button::update(float fixedTimeStep, bool isActive)
{
	if (!isActive)
	{
		shape.setFillColor(UIManager::BACKGROUND_COLOR - sf::Color(0, 0, 0, 150));
		shape.setOutlineColor(UIManager::BACKGROUND_OUTLINE_COLOR - sf::Color(0, 0, 0, 150));
		text.setFillColor(UIManager::BACKGROUND_OUTLINE_COLOR - sf::Color(0, 0, 0, 150));
	}
	else
	{
		shape.setFillColor(m_isHovered ? UIManager::BUTTON_HOVER_COLOR : UIManager::BUTTON_COLOR);
		shape.setOutlineColor(m_isHovered ? UIManager::BUTTON_OUTLINE_HOVER_COLOR : UIManager::BUTTON_OUTLINE_COLOR);
		text.setFillColor(m_isHovered ? UIManager::TEXT_HOVER_COLOR : UIManager::TEXT_COLOR);
	}
}

void Button::render(float interpolationFactor, sf::RenderWindow& window)
{
	window.draw(shape);
	window.draw(text);
}

void Button::setPosition(sf::Vector2f position)
{
	shape.setPosition(position);
	sf::FloatRect textBounds = text.getGlobalBounds();
	text.setOrigin({ textBounds.size.x / 2.f, textBounds.size.y / 1.5f });
	text.setPosition(position + shape.getSize() / 2.f);
}

void Button::setText(const std::string& text)
{
	this->text.setString(text);
}