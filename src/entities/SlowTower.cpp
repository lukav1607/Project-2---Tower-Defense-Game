// ================================================================================================
// File: SlowTower.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 4, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "SlowTower.hpp"

SlowTower::SlowTower(sf::Vector2i tilePosition) :
	Tower(TowerRegistry::Type::Slow, sf::Color(8, 60, 86), sf::Color(9, 66, 96), sf::Color(10, 73, 106), tilePosition),
	effectOverlayColor(sf::Color(54, 139, 193, 123)),
	isPulsing(false),
	pulseTimer(0.f)
{}

void SlowTower::update(float fixedTimeStep, std::vector<Enemy>& enemies)
{
	timeSinceLastShot += fixedTimeStep;

	updateSelectionOutline();

	// Check if there are any enemies in range
	bool enemiesInRange = false;
	for (const auto& enemy : enemies)
	{
		if (Utility::distance(enemy.getPixelPosition(), position) <= attributes.at(level).range)
		{
			enemiesInRange = true;
			break; // No need to check further if at least one enemy is in range
		}
	}

	if (enemiesInRange && timeSinceLastShot >= attributes.at(level).fireRate)
	{
		timeSinceLastShot = 0.f;
		
		isPulsing = true;
		pulseTimer = 0.f;
		pulseCircle.setRadius(0.f);
		pulseCircle.setFillColor(effectOverlayColor);
		pulseCircle.setOrigin({ pulseCircle.getRadius(), pulseCircle.getRadius() });
		pulseCircle.setPosition(position);

		for (auto& enemy : enemies)
		{
			if (Utility::distance(enemy.getPixelPosition(), position) <= attributes.at(level).range)
			{
				enemy.applyStatusEffect(
					{
						Enemy::StatusEffect::Type::Slow,
						effectOverlayColor,
						attributes.at(level).slowAmount,
						attributes.at(level).slowDuration
					});
			}
		}
	}

	if (isPulsing)
	{
		pulseTimer += fixedTimeStep;
		float t = pulseTimer / PULSE_DURATION;
		float radius = attributes.at(level).range * t;
		pulseCircle.setRadius(radius);
		pulseCircle.setOrigin({ radius, radius });
		pulseCircle.setPosition(position);

		int alpha = static_cast<int>((1.f - t) * effectOverlayColor.a); // Fade out
		pulseCircle.setFillColor(sf::Color(effectOverlayColor.r, effectOverlayColor.g, effectOverlayColor.b, alpha));

		if (pulseTimer >= PULSE_DURATION)
			isPulsing = false;
	}
}

void SlowTower::render(float interpolationFactor, sf::RenderWindow& window)
{
	if (isPulsing)
		window.draw(pulseCircle);

	if (isRangeCircleVisible)
		window.draw(rangeCircle);

	window.draw(shape);
	if (level > 0)
		window.draw(shape2);
	if (level > 1)
		window.draw(shape3);
}

void SlowTower::fireAt(sf::Vector2f target)
{
	/*isPulsing = true;
	pulseTimer = 0.f;
	pulseCircle.setRadius(0.f);
	pulseCircle.setFillColor(effectOverlayColor);
	pulseCircle.setOrigin({ pulseCircle.getRadius(), pulseCircle.getRadius() });
	pulseCircle.setPosition(position);*/
}