// ================================================================================================
// File: Utility.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include <random>
#include "Utility.hpp"
#include "Grid.hpp"

float Utility::randomNumber(float min, float max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

int Utility::randomNumber(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

sf::Vector2f Utility::interpolate(sf::Vector2f previous, sf::Vector2f current, float factor)
{
	return sf::Vector2f
	(
		previous.x + (current.x - previous.x) * factor,
		previous.y + (current.y - previous.y) * factor
	);
}

sf::Vector2f Utility::tileToPixelPosition(int col, int row, bool getCenterOfTile)
{
	if (getCenterOfTile)
		return { col * Grid::TILE_SIZE + Grid::TILE_SIZE / 2.f, row * Grid::TILE_SIZE + Grid::TILE_SIZE / 2.f };
	else
		return { col * Grid::TILE_SIZE, row * Grid::TILE_SIZE };
}

sf::Vector2f Utility::tileToPixelPosition(sf::Vector2i tilePosition, bool getCenterOfTile)
{
	return tileToPixelPosition(tilePosition.x, tilePosition.y, getCenterOfTile);
}

sf::Vector2i Utility::pixelToTilePosition(sf::Vector2f pixelPosition)
{
	return { static_cast<int>(pixelPosition.x / Grid::TILE_SIZE), static_cast<int>(pixelPosition.y / Grid::TILE_SIZE) };
}

bool Utility::isMouseButtonReleased(sf::Mouse::Button button)
{
	static bool wasPressedLastUpdate = false;

	if (sf::Mouse::isButtonPressed(button))
	{
		wasPressedLastUpdate = true;
	}
	else if (wasPressedLastUpdate)
	{
		wasPressedLastUpdate = false;
		return true;
	}
	return false;
}

bool Utility::isKeyReleased(sf::Keyboard::Key key)
{
	static bool wasPressedLastUpdate = false;

	if (sf::Keyboard::isKeyPressed(key))
	{
		wasPressedLastUpdate = true;
	}
	else if (wasPressedLastUpdate)
	{
		wasPressedLastUpdate = false;
		return true;
	}
	return false;
}
