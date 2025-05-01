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
#include <map>
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
	return previous * (1.f - factor) + current * factor;
}

sf::Vector2f Utility::normalize(sf::Vector2f vector)
{
	float length = std::hypotf(vector.x, vector.y);

	if (length != 0.f)
		return vector / length;

	return { 0.f, 0.f };
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
	static std::map<sf::Mouse::Button, bool> buttonStates;
	if (buttonStates.find(button) == buttonStates.end())
		buttonStates[button] = false;
	bool isPressedNow = sf::Mouse::isButtonPressed(button);
	bool wasPressedLastFrame = buttonStates[button];
	buttonStates[button] = isPressedNow;
	return !isPressedNow && wasPressedLastFrame;
}

bool Utility::isKeyReleased(sf::Keyboard::Key key)
{
	static std::map<sf::Keyboard::Key, bool> keyStates;
	if (keyStates.find(key) == keyStates.end())
		keyStates[key] = false;
	bool isPressedNow = sf::Keyboard::isKeyPressed(key);
	bool wasPressedLastFrame = keyStates[key];
	keyStates[key] = isPressedNow;
	return !isPressedNow && wasPressedLastFrame;
}
