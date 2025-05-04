// ================================================================================================
// File: BulletTower.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 4, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include "BulletTower.hpp"
#include "../core/Utility.hpp"

BulletTower::BulletTower(sf::Vector2i tilePosition) :
	Tower(TowerRegistry::Type::Bullet, tilePosition),
	towerColor(sf::Color(10, 92, 54)),
	bulletColor(sf::Color(25, 17, 6))
{
	shape.setSize({ 60.f, 60.f });
	shape.setFillColor(towerColor);
	shape.setOrigin({ shape.getSize().x / 2.f, shape.getSize().y / 2.f });
	shape.setPosition(position);

	rangeCircle.setRadius(attributes.at(level).range);
	rangeCircle.setOrigin({ rangeCircle.getRadius(), rangeCircle.getRadius() });
	rangeCircle.setFillColor(sf::Color(0, 0, 0, 25));
	rangeCircle.setOutlineColor(sf::Color(0, 0, 0, 75));
	rangeCircle.setOutlineThickness(2.f);
	rangeCircle.setPosition(position);
	rangeCircle.setPointCount(100);

	std::cout << "BulletTower attributes size: " << attributes.size() << std::endl;
}

void BulletTower::update(float fixedTimeStep, std::vector<Enemy>& enemies)
{
	timeSinceLastShot += fixedTimeStep;

	// Erase bullets that have hit an enemy only at the start of the next update.
	// This is done to avoid visual issues where the bullet's interpolated position is not
	// the same as the position it was at when it logically hit the enemy. This gives the
	// rendering interpolation enough time to catch up with the logic.
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
		[](const Bullet& bullet) { return bullet.hasHitEnemy; }), bullets.end());

	for (auto& bullet : bullets)
	{
		bullet.positionPrevious = bullet.positionCurrent;
		bullet.positionCurrent += bullet.direction * bullet.speed * fixedTimeStep;

		for (auto& enemy : enemies)
		{
			if (Utility::distance(bullet.positionCurrent, enemy.getPixelPosition()) <= enemy.getSize())
			{
				enemy.takeDamage(attributes.at(level).damage);
				bullet.hasHitEnemy = true;
				break;
			}
		}
	}
}

void BulletTower::render(float interpolationFactor, sf::RenderWindow& window)
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

	if (isRangeCircleVisible)
		window.draw(rangeCircle);

	window.draw(shape);

	for (auto& bullet : bullets)
	{
		bullet.shape.setPosition(Utility::interpolate(bullet.positionPrevious, bullet.positionCurrent, interpolationFactor));
		window.draw(bullet.shape);
	}
}

bool BulletTower::tryUpgrade(int gold)
{
	if (level >= getMaxLevel())
		return false;
	
	if (gold >= attributes.at(level + 1).buyCost)
	{
		level++;
		rangeCircle.setRadius(attributes.at(level).range);
		rangeCircle.setOrigin({ rangeCircle.getRadius(), rangeCircle.getRadius() });
		rangeCircle.setPosition(position);
		m_isMarkedForUpgrade = false;
		return true;
	}
	return false;
}

void BulletTower::fireAt(sf::Vector2f target)
{
	if (target.x <= 0)
		return;

	Bullet bullet;
	bullet.positionCurrent = position;
	bullet.positionPrevious = position;

	sf::Vector2f direction = Utility::normalize(target - position);
	bullet.direction = direction;
	bullet.speed = 1000.f;

	bullet.shape.setRadius(5.f);
	bullet.shape.setOrigin({ bullet.shape.getRadius(), bullet.shape.getRadius() });
	bullet.shape.setFillColor(bulletColor);

	bullets.push_back(bullet);
	timeSinceLastShot = 0.f;
}

BulletTower::Bullet::Bullet() :
	hasHitEnemy(false),
	speed(1000.f)
{}