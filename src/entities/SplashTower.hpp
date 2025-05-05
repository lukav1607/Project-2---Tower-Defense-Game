// ================================================================================================
// File: SplashTower.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 5, 2025
// Description: Defines the SplashTower class, which represents a tower that deals splash damage to enemies.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include "Tower.hpp"

class SplashTower : public Tower
{
public:
	SplashTower(sf::Vector2i tilePosition);
	SplashTower(const SplashTower&) = default;
	SplashTower& operator=(const SplashTower&) = default;

	void update(float fixedTimeStep, std::vector<Enemy>& enemies) override;
	void render(float interpolationFactor, sf::RenderWindow& window) override;

private:
	void fireAt(sf::Vector2f target) override;
	void explodeAt(sf::Vector2f location, std::vector<Enemy>& enemies);

	//struct Bullet
	//{
	//	float speed = 400.f;
	//	bool hasHitEnemy = false;
	//	sf::Vector2f positionCurrent;
	//	sf::Vector2f positionPrevious;
	//	sf::Vector2f direction;
	//	sf::CircleShape shape;
	//	sf::Color color;
	//};
	//std::vector<Bullet> bullets;
	////sf::Color bulletColor; 
	
	std::vector<sf::CircleShape> explosions;
	std::vector<float> explosionTimers;

	const float EXPLOSION_DURATION = 0.3f;
	sf::Color explosionColor = sf::Color(255, 100, 0, 150); // orange-ish
};