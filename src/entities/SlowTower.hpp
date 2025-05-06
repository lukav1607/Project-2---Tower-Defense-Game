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

	void update(float fixedTimeStep, std::vector<Enemy>& enemies, SoundManager& soundManager) override;
	void render(float interpolationFactor, sf::RenderWindow& window) override;

private:
	void fireAt(sf::Vector2f target) override;

	const float PULSE_DURATION = 0.3f;
	float pulseTimer;
	bool isPulsing;
	sf::CircleShape pulseCircle;

	sf::Color effectOverlayColor;
};