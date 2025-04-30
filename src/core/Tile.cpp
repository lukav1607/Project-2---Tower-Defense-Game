// ================================================================================================
// File: Tile.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Tile.hpp"

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

	if (type == Type::Start)
		shape.setFillColor(sf::Color(225, 25, 25, 100));
	else if (type == Type::End)
		shape.setFillColor(sf::Color(25, 225, 25, 100));
	else if (type == Type::Pathable)
		shape.setFillColor(sf::Color(25, 225, 225, 100));
	else if (type == Type::Buildable)
		shape.setFillColor(sf::Color(225, 225, 25, 100));
	else
		shape.setFillColor(sf::Color(255, 255, 255, 100));
}

void Tile::render(float interpolationFactor, sf::RenderWindow& window)
{
	window.draw(shape);
	if (tower != nullptr)
		window.draw(tower->getShape());
}
