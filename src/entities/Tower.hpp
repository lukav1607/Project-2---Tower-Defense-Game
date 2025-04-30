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
#include "../core/Utility.hpp"
#include "Enemy.hpp"

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

	void update(float fixedTimeStep, std::vector<Enemy>& enemies);
	void render(float interpolationFactor, sf::RenderWindow& window);

	void fireAt(sf::Vector2f target);

	inline sf::Vector2i getTilePosition() const { return Utility::pixelToTilePosition(position); }
	inline sf::Vector2f getPixelPosition() const { return position; }
	inline int getLevel() const { return level; }
	inline bool canFire() const { return timeSinceLastShot >= attributes[level].fireRate; }

	const struct AttributesPerLevel
	{
		int buyCost;
		int sellCost;
		int damage;
		float range;
		float fireRate;
	}
	attributes[3] =
	{
	{ 20, 5, 1, 200.f, 0.77f },
	{ 45, 15, 2, 250.f, 0.66f },
	{ 100, 35, 3, 300.f, 0.55f }
	};

protected:
	struct Bullet
	{
		Bullet();
		sf::CircleShape shape;
		sf::Vector2f positionCurrent;
		sf::Vector2f positionPrevious;
		sf::Vector2f direction;
		float speed;
		bool hasHitEnemy;
	};
	std::vector<Bullet> bullets;

	sf::Vector2f position;

	Type type;

	int LEVEL_MAX = 3;
	int level;

	float timeSinceLastShot;

	sf::RectangleShape shape;
};