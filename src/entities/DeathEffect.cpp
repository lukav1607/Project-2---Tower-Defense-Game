// ================================================================================================
// File: DeathEffect.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 5, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include "DeathEffect.hpp"
#include "../core/Utility.hpp"

void DeathEffect::update(float fixedTimeStep)
{
    timer += fixedTimeStep;
    positionPrevious = positionCurrent;
    positionCurrent += velocity * fixedTimeStep;

    float alpha = 255.f * (1.f - (timer / lifetime));
    sf::Color c = shape.getFillColor();
    c.a = static_cast<std::uint8_t>(alpha);
    shape.setFillColor(c);
}


void DeathEffect::render(float interpolationFactor, sf::RenderWindow& window)
{
	shape.setPosition(Utility::interpolate(positionPrevious, positionCurrent, interpolationFactor));
	window.draw(shape);
}