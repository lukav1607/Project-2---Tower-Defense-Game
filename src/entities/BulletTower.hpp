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

private:
	void fireAt(sf::Vector2f target) override;
};