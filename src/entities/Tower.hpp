// ================================================================================================
// File: Tower.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 29, 2025
// Description: Defines the Tower class, which represents a tower in the game. The class contains 
//              information about the tower's position, range, damage, and other attributes. It also 
//              includes methods for updating the tower's state and rendering it on the screen.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>

class Tower
{
public:
	enum class Type
	{
		Bullet
	};

	Tower(Type type, sf::Vector2i tilePosition);
	Tower(const Tower&) = default;
	Tower& operator=(const Tower&) = default;
	virtual ~Tower() = default;

	void update(float fixedTimeStep);
	void render(float interpolationFactor, sf::RenderWindow& window);

	const sf::RectangleShape& getShape() const { return shape; }

protected:
	struct Bullet
	{
		sf::CircleShape shape;
		sf::Vector2f positionCurrent;
		sf::Vector2f positionPrevious;
		sf::Vector2f direction;
		float speed;
	};
	std::vector<Bullet> bullets;

	sf::Vector2f position;
	sf::Vector2f target;

	Type type;

	int damage;
	float range;
	float bulletSpeed;
	float fireRate;

	int level;
	int cost;

	sf::RectangleShape shape;
};