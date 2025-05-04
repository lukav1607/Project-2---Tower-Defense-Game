// ================================================================================================
// File: BulletTower.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 4, 2025
// Description: Defines the BulletTower class, which represents a tower that shoots bullets at enemies.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include "Tower.hpp"

class BulletTower : public Tower
{
public:
	BulletTower(sf::Vector2i tilePosition);
	BulletTower(const BulletTower&) = default;
	BulletTower& operator=(const BulletTower&) = default;

	void update(float fixedTimeStep, std::vector<Enemy>& enemies) override;
	void render(float interpolationFactor, sf::RenderWindow& window) override;

	bool tryUpgrade(int gold) override;
	void fireAt(sf::Vector2f target) override;

	inline std::string getName() const override { return "Bullet Tower"; }
	/*inline int getDamage(int towerLevel) const override { return attributes[towerLevel].damage; }
	inline float getFireRate(int towerLevel) const override { return attributes[towerLevel].fireRate; }
	inline float getRange(int towerLevel) const override { return attributes[towerLevel].range; }
	inline int getBuyCost(int towerLevel) const override { return attributes[towerLevel].buyCost; }
	inline int getSellCost() const override { return attributes[level].sellCost; }*/

protected:
	inline int getBaseBuyCost() const override { return attributes[0].buyCost; }

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
};