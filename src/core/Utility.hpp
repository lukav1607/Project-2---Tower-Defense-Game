// ================================================================================================
// File: Utility.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// Description: Defines an assortment of utility functions, which are often used throughout the
//              codebase. These functions include mathematical operations, string manipulations, and
//              other general-purpose functions.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Utility
{
	float randomNumber(float min, float max);
	int randomNumber(int min, int max);

	sf::Vector2f interpolate(sf::Vector2f previous, sf::Vector2f current, float factor);
	sf::Vector2f normalize(sf::Vector2f vector);
	inline float distance(sf::Vector2f a, sf::Vector2f b) { return std::hypotf(b.x - a.x, b.y - a.y); }

	sf::Vector2f tileToPixelPosition(int col, int row, bool getCenterOfTile = true);
	sf::Vector2f tileToPixelPosition(sf::Vector2i, bool getCenterOfTile = true);
	sf::Vector2i pixelToTilePosition(sf::Vector2f pixelPosition);

	bool isMouseButtonReleased(sf::Mouse::Button button);
	bool isKeyReleased(sf::Keyboard::Key key);

	std::string removeTrailingZeros(float number);
}