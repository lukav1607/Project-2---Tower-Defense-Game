// ================================================================================================
// File: Utility.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include <sstream>
#include <random>
#include <map>
#include <cmath>
#include "Utility.hpp"
#include "Grid.hpp"

float Utility::randomNumber(float min, float max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

int Utility::randomNumber(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

sf::Vector2f Utility::interpolate(sf::Vector2f previous, sf::Vector2f current, float factor)
{
	return previous * (1.f - factor) + current * factor;
}

sf::Vector2f Utility::normalize(sf::Vector2f vector)
{
	float length = std::hypotf(vector.x, vector.y);

	if (length != 0.f)
		return vector / length;

	return { 0.f, 0.f };
}

sf::Vector2f Utility::angleToVector(float angleDegrees)
{
	float radians = angleDegrees * (3.14159265f / 180.f);
	return { std::cos(radians), std::sin(radians) };
}

std::optional<sf::Vector2f> Utility::predictTargetIntercept(
	const sf::Vector2f& shooterPosition,
	const sf::Vector2f& targetPosition,
	const sf::Vector2f& targetVelocity,
	float projectileSpeed)
{
	// Calculate the relative position and velocity vectors
	const sf::Vector2f toTarget = targetPosition - shooterPosition;

	// Calculate the coefficients of the quadratic equation and the discriminant
	float a = targetVelocity.x * targetVelocity.x + targetVelocity.y * targetVelocity.y - projectileSpeed * projectileSpeed;
	float b = 2 * (toTarget.x * targetVelocity.x + toTarget.y * targetVelocity.y);
	float c = toTarget.x * toTarget.x + toTarget.y * toTarget.y;
	float discriminant = b * b - 4 * a * c;

	// If there's no solution or the projectile is too slow
	if (discriminant < 0 || std::abs(a) < 1e-6f)
		return std::nullopt;

	// Calculate the two possible times of intercept
	float sqrtDiscriminant = std::sqrt(discriminant);
	float t1 = (-b - sqrtDiscriminant) / (2 * a);
	float t2 = (-b + sqrtDiscriminant) / (2 * a);

	// Choose the earliest positive time
	float t = std::min(t1, t2);	// If both are positive, use the smaller one
	if (t < 0.f)
		t = std::max(t1, t2); // If one is negative, use the other
	if (t < 0.f)
		return std::nullopt; // If both are negative, return nullopt

	// Return the intercept position
	return targetPosition + targetVelocity * t;
}

const Enemy* Utility::getClosestEnemyInRange(const sf::Vector2f& origin, const std::vector<Enemy>& enemies, float range)
{
	const Enemy* closestEnemy = nullptr;
	float closestDistanceSq = range * range;

	for (const auto& enemy : enemies)
	{
		if (enemy.isDead()) continue;

		float distanceSq = Utility::distanceSquared(origin, enemy.getPixelPosition());
		if (distanceSq <= closestDistanceSq)
		{
			closestDistanceSq = distanceSq;
			closestEnemy = &enemy;
		}
	}

	return closestEnemy;
}

sf::Color Utility::blendColors(sf::Color base, sf::Color overlay)
{
	float alpha = overlay.a / 255.f;

	auto blendChannel = [&](std::uint8_t baseChannel, std::uint8_t overlayChannel) 
	{
		return static_cast<std::uint8_t>(baseChannel * (1.f - alpha) + overlayChannel * alpha);
	};

	return sf::Color
	(
		blendChannel(base.r, overlay.r),
		blendChannel(base.g, overlay.g),
		blendChannel(base.b, overlay.b),
		255
	);
}

sf::Vector2f Utility::tileToPixelPosition(int col, int row, bool getCenterOfTile)
{
	if (getCenterOfTile)
		return { col * Grid::TILE_SIZE + Grid::TILE_SIZE / 2.f, row * Grid::TILE_SIZE + Grid::TILE_SIZE / 2.f };
	else
		return { col * Grid::TILE_SIZE, row * Grid::TILE_SIZE };
}

sf::Vector2f Utility::tileToPixelPosition(sf::Vector2i tilePosition, bool getCenterOfTile)
{
	return tileToPixelPosition(tilePosition.x, tilePosition.y, getCenterOfTile);
}

sf::Vector2i Utility::pixelToTilePosition(sf::Vector2f pixelPosition)
{
	return { static_cast<int>(pixelPosition.x / Grid::TILE_SIZE), static_cast<int>(pixelPosition.y / Grid::TILE_SIZE) };
}

bool Utility::isMouseButtonReleased(sf::Mouse::Button button)
{
	static std::map<sf::Mouse::Button, bool> buttonStates;
	if (buttonStates.find(button) == buttonStates.end())
		buttonStates[button] = false;
	bool isPressedNow = sf::Mouse::isButtonPressed(button);
	bool wasPressedLastFrame = buttonStates[button];
	buttonStates[button] = isPressedNow;
	return !isPressedNow && wasPressedLastFrame;
}

bool Utility::isKeyReleased(sf::Keyboard::Key key)
{
	static std::map<sf::Keyboard::Key, bool> keyStates;
	if (keyStates.find(key) == keyStates.end())
		keyStates[key] = false;
	bool isPressedNow = sf::Keyboard::isKeyPressed(key);
	bool wasPressedLastFrame = keyStates[key];
	keyStates[key] = isPressedNow;
	return !isPressedNow && wasPressedLastFrame;
}

std::string Utility::removeTrailingZeros(float number)
{
	std::ostringstream ss;
	ss.precision(2);
	ss << std::fixed << number;
	std::string str = ss.str();
	// Remove trailing zeros
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	if (str.back() == '.')
		str.pop_back();
	return str;
}
