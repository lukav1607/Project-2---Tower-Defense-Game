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
		Bullet,
		Count
	};

	Tower(Type type, sf::Vector2i tilePosition);
	Tower(const Tower&) = default;
	Tower& operator=(const Tower&) = default;
	virtual ~Tower() = default;

	void update(float fixedTimeStep, std::vector<Enemy>& enemies);
	void render(float interpolationFactor, sf::RenderWindow& window);

	void markForUpgrade();
	bool tryUpgrade(int gold);
	void markForSale();
	void fireAt(sf::Vector2f target);

	inline void setRangeCircleVisible(bool isVisible) { isRangeCircleVisible = isVisible; }
	inline sf::Vector2i getTilePosition() const { return Utility::pixelToTilePosition(position); }
	inline sf::Vector2f getPixelPosition() const { return position; }
	inline int getLevel() const { return level; }
	inline int getMaxLevel() const { return LEVEL_MAX; }
	inline int getBaseBuyCost() const { return attributes[0].buyCost; }
	std::string getName() const;
	inline bool canFire() const { return timeSinceLastShot >= attributes[level].fireRate; }
	inline bool isMarkedForSale() const { return m_isMarkedForSale; }
	inline bool isMarkedForUpgrade() const { return m_isMarkedForUpgrade; }

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
	{ 20, 5, 1, 200.f, 1.25f },
	{ 50, 15, 2, 250.f, 1.1f },
	{ 90, 35, 3, 300.f, 0.9f }
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

	sf::Color towerColor;
	sf::Color bulletColor;

	sf::Vector2f position;

	Type type;

	int LEVEL_MAX = 2;
	int level;

	bool m_isMarkedForSale;
	bool m_isMarkedForUpgrade;

	float timeSinceLastShot;

	sf::RectangleShape shape;

	sf::CircleShape rangeCircle;
	bool isRangeCircleVisible;
};