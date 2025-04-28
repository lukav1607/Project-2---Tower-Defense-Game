#include "Tile.hpp"
// ================================================================================================
// File: Tile.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// Description: Defines the Tile class, which represents a single tile on the game grid. Each tile
//              holds information about its position, size, and whether it is occupied. It is 
//              visually represented by an SFML RectangleShape for rendering.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

Tile::Tile(Type type, int x, int y, float size) :
	type(type),
	x(x),
	y(y),
	size(size)
{
	shape.setSize({ size, size });
	shape.setPosition({ x * size, y * size });
	shape.setOutlineColor(sf::Color::White);
	shape.setOutlineThickness(0.1f);

	if (type == Type::Pathable)
		shape.setFillColor(sf::Color(225, 25, 25, 100));
	else if (type == Type::Buildable)
		shape.setFillColor(sf::Color(25, 225, 25, 100));
	else if (type == Type::Pathable2)
		shape.setFillColor(sf::Color(25, 25, 225, 100));
	else if (type == Type::Pathable3)
		shape.setFillColor(sf::Color(225, 225, 225, 100));
	else
		shape.setFillColor(sf::Color(255, 255, 255, 100));
}

void Tile::render(float interpolationFactor, sf::RenderWindow& window)
{
	window.draw(shape);
}
