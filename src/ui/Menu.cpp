// ================================================================================================
// File: Menu.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 3, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Menu.hpp"
#include "UIManager.hpp"

Menu::Menu(const sf::Font& font, const std::string& title, sf::Vector2f size) :
	m_isActive(false),
	m_isHovered(false),
	padding(20.f, 20.f),
	titleText(font, title, 32U)
{
	background.setSize(size);
	background.setFillColor(UIManager::BACKGROUND_COLOR);
	background.setOutlineColor(UIManager::BACKGROUND_OUTLINE_COLOR);
	background.setOutlineThickness(UIManager::BACKGROUND_OUTLINE_THICKNESS);
	this->titleText.setFillColor(UIManager::TEXT_COLOR);
	this->titleText.setOutlineThickness(UIManager::TEXT_OUTLINE_THICKNESS);
	this->titleText.setOutlineColor(UIManager::TEXT_OUTLINE_COLOR);
}