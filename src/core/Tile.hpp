// ================================================================================================
// File: Tile.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// Description: Defines the Tile class, which represents a single tile on the game grid. Each tile
//              holds information about its position, size, and whether it is occupied. It is 
//              visually represented by an SFML RectangleShape for rendering.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../entities/Tower.hpp"

class Tile
{
public:
	enum class Type
	{
		Start,
		End,
		Pathable,
		Buildable,
		Unassigned
	};

	Tile(Type type, int x, int y, float size);

	void render(float interpolationFactor, sf::RenderWindow& window);

	Type getType() const { return type; }

	std::shared_ptr<Tower> tower;

private:
	Type type;
	int x, y;
	float size;

	sf::RectangleShape shape;
};