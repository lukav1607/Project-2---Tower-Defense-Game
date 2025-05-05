// ================================================================================================
// File: Tower.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 29, 2025
// Description: Defines the Tower class, which is a base class for all tower types in the game.
//              It contains common attributes and methods for all tower types.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>
#include "../core/Utility.hpp"
#include "TowerRegistry.hpp"
#include "Enemy.hpp"

class Tower
{
public:
	Tower(TowerRegistry::Type type, sf::Color color, sf::Vector2i tilePosition);
	Tower(const Tower&) = default;
	Tower& operator=(const Tower&) = default;
	virtual ~Tower() = default;

	virtual void update(float fixedTimeStep, std::vector<Enemy>& enemies) = 0;
	virtual void render(float interpolationFactor, sf::RenderWindow& window) = 0;

	bool tryUpgrade(int gold);
	inline void markForUpgrade() { m_isMarkedForUpgrade = true; }
	inline void markForSale() { m_isMarkedForSale = true; }
	inline void setRangeCircleVisible(bool isVisible) { isRangeCircleVisible = isVisible; }

	inline const std::string& getName() const {	return TowerRegistry::getTowerMetadataRegistry()[static_cast<int>(type)].name; }
	const std::vector<TowerRegistry::AttributesPerLevel>& getAttributes() const { return attributes; }

    inline sf::Vector2i getTilePosition() const { return Utility::pixelToTilePosition(position); }
    inline sf::Vector2f getPixelPosition() const { return position; }
    inline int getLevel() const { return level; }
	inline int getMaxLevel() const { return static_cast<int>(attributes.size() - 1); }
    inline bool isMarkedForUpgrade() const { return m_isMarkedForUpgrade; }
	inline bool isMarkedForSale() const { return m_isMarkedForSale; }

	bool isSelected;

protected:
	virtual void fireAt(sf::Vector2f target) = 0;
	void updateSelectionOutline();

	inline bool canFire() const { return timeSinceLastShot >= attributes.at(level).fireRate; }

	struct Bullet
	{
		bool hasHitEnemy = false;
		sf::Vector2f positionCurrent;
		sf::Vector2f positionPrevious;
		sf::Vector2f direction;
		sf::CircleShape shape;
	};
	std::vector<Bullet> bullets;
	sf::Color bulletColor;
	float bulletSpeed;

	std::vector<TowerRegistry::AttributesPerLevel> attributes;

	sf::Vector2f position;

	TowerRegistry::Type type;
	float timeSinceLastShot;

	int level;

	sf::RectangleShape shape;
	sf::CircleShape rangeCircle;
	bool isRangeCircleVisible;
	sf::Color towerColor;

	bool m_isMarkedForSale;
	bool m_isMarkedForUpgrade;
};