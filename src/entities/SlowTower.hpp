// ================================================================================================
// File: SlowTower.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 4, 2025
// Description: Defines the SlowTower class, which represents a tower that slows down enemies.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include "Tower.hpp"

class SlowTower : public Tower
{
public:
	SlowTower(sf::Vector2i tilePosition);
	SlowTower(const SlowTower&) = default;
	SlowTower& operator=(const SlowTower&) = default;

	void update(float fixedTimeStep, std::vector<Enemy>& enemies) override;
	void render(float interpolationFactor, sf::RenderWindow& window) override;

	void fireAt(sf::Vector2f target) override;

	inline std::string getName() const override { return "Slow Tower"; }

private:
	inline int getBaseBuyCost() const override { return attributes[0].buyCost; }

	const float PULSE_DURATION = 0.3f;
	float pulseTimer;
	bool isPulsing;
	sf::CircleShape pulseCircle;

	sf::Color effectOverlayColor;
};