// ================================================================================================
// File: Grid.hpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// Description: Defines the Grid class, which contains a collection of Tile objects representing 
//              the game grid. It is responsible for initializing the grid and rendering it to the 
//              window.
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#pragma once

#include <vector>
#include "Tile.hpp"

class Grid
{
public:
	Grid(int cols, int rows);

	void render(float interpolationFactor, sf::RenderWindow& window);

	void generateNewRandomLevel(int cols, int rows);

	void selectTile(sf::Vector2i tilePosition);
	void deselectAllTiles();
	void markTileAsTower(sf::Vector2i tilePosition);

	inline sf::Vector2u getSize() const { return sf::Vector2u(cols, rows); }
	inline sf::Vector2i getStartTile() const {	return startTileCoordinates; }
	Tile::Type getTileType(int col, int row) const;
	Tile::Type getTileType(sf::Vector2i tilePosition) const;

	static const float TILE_SIZE;

private:
	int cols, rows;
	std::vector<std::vector<Tile>> tiles;
	sf::Vector2i startTileCoordinates;
};