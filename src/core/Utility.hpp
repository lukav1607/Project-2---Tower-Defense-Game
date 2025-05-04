// ================================================================================================
// File: Utility.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// Description: Defines an assortment of utility functions, which are often used throughout the
//              codebase. These functions include mathematical operations, string manipulations, and
//              other general-purpose functions.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <optional>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Color.hpp>

namespace Utility
{
	float randomNumber(float min, float max);
	int randomNumber(int min, int max);

	// Interpolates between two vectors based on a factor (0.0 to 1.0).
	sf::Vector2f interpolate(sf::Vector2f previous, sf::Vector2f current, float factor);

	// Normalizes a vector to a unit length.
	sf::Vector2f normalize(sf::Vector2f vector);

	// Returns the distance between two points in 2D space.
	inline float distance(sf::Vector2f a, sf::Vector2f b) { return std::hypotf(b.x - a.x, b.y - a.y); }

	// Returns the squared distance between two points in 2D space.
	inline float distanceSquared(sf::Vector2f a, sf::Vector2f b) { return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y); }

	// Predicts the intercept point of a target moving with constant velocity and a projectile moving with constant speed.
	// Returns std::nullopt if the target is unreachable.
	std::optional<sf::Vector2f> predictTargetIntercept(
		const sf::Vector2f& shooterPos,
		const sf::Vector2f& targetPos,
		const sf::Vector2f& targetVelocity,
		float projectileSpeed);

	// Blends two colors based on the alpha value of the overlay color.
	sf::Color blendColors(sf::Color base, sf::Color overlay);

	sf::Vector2f tileToPixelPosition(int col, int row, bool getCenterOfTile = true);
	sf::Vector2f tileToPixelPosition(sf::Vector2i, bool getCenterOfTile = true);
	sf::Vector2i pixelToTilePosition(sf::Vector2f pixelPosition);

	bool isMouseButtonReleased(sf::Mouse::Button button);
	bool isKeyReleased(sf::Keyboard::Key key);

	// Removes trailing zeros from a float and returns it as a string.
	std::string removeTrailingZeros(float number);
}