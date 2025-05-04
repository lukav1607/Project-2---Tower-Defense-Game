// ================================================================================================
// File: Tile.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "Tile.hpp"
#include "Utility.hpp"

Tile::Tile(Type type, int x, int y, float size) :
	isSelected(false),
	type(type),
	x(x),
	y(y),
	size(size)
{
	shape.setSize({ size, size });
	shape.setPosition({ x * size, y * size });

	sf::Color pathColor = sf::Color(155, 118, 83);
	sf::Color grassColor = sf::Color
	(
		Utility::randomNumber(157, 165), //161, 
		Utility::randomNumber(217, 229), //223, 
		Utility::randomNumber(77, 83)    //80
	);

	if (type == Type::Start)
		shape.setFillColor(pathColor);
	else if (type == Type::End)
		shape.setFillColor(pathColor);
	else if (type == Type::Pathable)
		shape.setFillColor(pathColor);
	else if (type == Type::Buildable)
		shape.setFillColor(grassColor);
	else
		shape.setFillColor(sf::Color(255, 255, 255, 100));
}

void Tile::render(float interpolationFactor, sf::RenderWindow& window)
{
	if (isSelected)
	{
		shape.setOutlineColor(sf::Color(255, 255, 255, 255));
		shape.setOutlineThickness(4.f);
	}
	else
	{
		shape.setOutlineColor(sf::Color(0, 0, 0, 0));
		shape.setOutlineThickness(0.f);
	}
	window.draw(shape);
}