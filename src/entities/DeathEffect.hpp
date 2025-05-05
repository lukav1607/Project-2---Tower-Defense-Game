// ================================================================================================
// File: DeathEffect.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 5, 2025
// Description: Defines the DeathEffect struct, which represents a visual effect that occurs when an enemy dies.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>

struct DeathEffect
{
    sf::CircleShape shape;
	sf::Vector2f positionCurrent, positionPrevious;
    sf::Vector2f velocity;
    float lifetime = 0.5f; // seconds
    float timer = 0.f;

    void update(float fixedTimeStep);
    void render(float interpolationFactor, sf::RenderWindow& window);
    inline bool isExpired() const { return timer >= lifetime; }
};