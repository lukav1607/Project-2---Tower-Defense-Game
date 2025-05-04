// ================================================================================================
// File: HasTooltip.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: May 3, 2025
// Description: Defines the HasTooltip class, which provides a tooltip functionality for UI elements,
//              allowing UI elements to display additional information when hovered over.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <SFML/Graphics.hpp>

class HasTooltip
{
protected:
	float hoverTimer; // The time the mouse has been hovering over the element
	float timeToDisplayTooltip; // The time the mouse needs to be hovering over the element to display the tooltip
	sf::RectangleShape tooltipBackground;
	sf::Text tooltipText;
};