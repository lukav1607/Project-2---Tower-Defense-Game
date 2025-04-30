// ================================================================================================
// File: Grid.cpp
// Author: Luka Vukorepa (https://github.com/lukav1607)
// Created: April 28, 2025
// ================================================================================================
// License: MIT License
// Copyright (c) 2025 Luka Vukorepa
// ================================================================================================

#include <iostream>
#include "Grid.hpp"
#include "Utility.hpp"

const float Grid::TILE_SIZE = 120.f;

Grid::Grid(int cols, int rows) :
	cols(cols),
	rows(rows)
{
	generateNewRandomLevel(cols, rows);
}

void Grid::render(float interpolationFactor, sf::RenderWindow& window)
{
	for (auto& row : tiles)
	{
		for (auto& tile : row)
		{
			tile.render(interpolationFactor, window);
		}
	}
}

void Grid::generateNewRandomLevel(int cols, int rows)
{
	bool doAllRowsHavePathableTiles = false;

	while (!doAllRowsHavePathableTiles)
	{
		// Initialize a grid of tiles with dimensions rows x cols
		std::vector<std::vector<Tile>> level(rows, std::vector<Tile>(cols, Tile(Tile::Type::Unassigned, 0, 0, 0.f)));

		std::vector<bool> rowHasPathableTile(rows, false);
		// Set the first and last rows as pathable tiles because paths will never be generated in the first and last rows
		rowHasPathableTile[0] = true;
		rowHasPathableTile[size_t(rows - 1)] = true;

		int currentRow = Utility::randomNumber(1, rows - 2);
		int currentCol = 0;
		level[currentRow][currentCol] = Tile(Tile::Type::Start, currentCol, currentRow, TILE_SIZE);
		startTileCoordinates = { currentCol, currentRow };

		int previousTileDirection = 0;

		// Fill the grid with pathable tiles randomly from left to right
		// Start from the second column (index 1) to avoid the first column
		for (int col = 1; col < cols; ++col)
		{
			int previousRow = currentRow;
			int previousCol = col - 1;

			// Randomly roll for a new direction
			int roll = Utility::randomNumber(0, 9);
			int verticalSteps = Utility::randomNumber(1, rows);
			int nextTileDirection = 0;

			// Don't move up or down if at the last column
			if (col != cols - 1)
			{
				// Move up (50% chance), but only if last move was not down
				if (roll < 5 && previousTileDirection != 1)
					nextTileDirection = -1;
				// Move down (50% chance), but only if last move was not up
				else if (roll > 4 && previousTileDirection != -1)
					nextTileDirection = 1;
				// Otherwise, stay in the same row (0% chance)
			}

			previousTileDirection = nextTileDirection;

			// Perform the vertical step movement
			for (int i = 0; i < verticalSteps; ++i)
			{
				// Add the direction to the current row to determine the next tile's row.
				// Ensure it stays within the bounds of the grid and doesn't move
				// up if at the second-to-top row or down if at the second-to-bottom row
				currentRow = std::clamp(currentRow + nextTileDirection, 1, rows - 2);
				level[currentRow][col] = Tile(Tile::Type::Pathable, col, currentRow, TILE_SIZE);
				rowHasPathableTile[currentRow] = true;
			}

			// Ensure orthogonal connection if the next tile is not in the same row
			if (currentRow != previousRow)
			{
				int step = (currentRow > previousRow) ? 1 : -1;
				for (int row = previousRow; row != currentRow; row += step)
				{
					level[row][col] = Tile(Tile::Type::Pathable, col, row, TILE_SIZE);
					rowHasPathableTile[currentRow] = true;
				}
			}

			// Fill the current tile with a pathable tile
			if (col != cols - 1)
			{
				// Set the current tile as a pathable tile
				level[currentRow][col] = Tile(Tile::Type::Pathable, col, currentRow, TILE_SIZE);
				rowHasPathableTile[currentRow] = true;
			}
			else
			{
				// Set the last tile as the end tile
				level[currentRow][col] = Tile(Tile::Type::End, col, currentRow, TILE_SIZE);
			}
		}

		// Check if all rows have at least one pathable tile
		if (std::all_of(rowHasPathableTile.begin(), rowHasPathableTile.end(), [](bool hasPathableTile) { return hasPathableTile; }))
			doAllRowsHavePathableTiles = true;

		// Fill the rest of the grid with buildable tiles
		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < cols; ++col)
			{
				if (level[row][col].getType() == Tile::Type::Unassigned)
				{
					level[row][col] = Tile(Tile::Type::Buildable, col, row, TILE_SIZE);
				}
			}
		}

		tiles = std::move(level);
	}
}

void Grid::placeTower(int col, int row, std::shared_ptr<Tower> tower)
{
	if (row < 0 || row >= rows || col < 0 || col >= cols)
	{
		std::cerr << "Invalid Tile coordinates for Tower placement!" << std::endl;
		return;
	}
	if (tiles[row][col].getType() == Tile::Type::Buildable && tiles[row][col].tower == nullptr)
	{
		std::cout << "Placing tower on tile at (" << col << ", " << row << ")" << std::endl;
		tiles[row][col].tower = tower;
	}

	// TODO: Maybe this should instead just return true to say "yes you can place a tower here"
	//       and then something else should handle the tower placement?
	//if (tiles[col][row].getType() == Tile::Type::Buildable &&
	//	tiles[col][row].tower == nullptr)
	//{
	//	tiles[col][row].tower = tower;
	//}
	//else
	//{
	//	// TODO: Handle the case when the tile is not buildable
	//	std::cout << "Cannot place tower on this tile!" << std::endl;
	//}
}

bool Grid::isTowerPlaceableAtTile(int col, int row) const
{
	if (row < 0 || row >= rows || col < 0 || col >= cols)
	{
		std::cerr << "Invalid Tile coordinates for Tower placement!" << std::endl;
		return false;
	}
	if (tiles[row][col].getType() == Tile::Type::Buildable && tiles[row][col].tower == nullptr)
	{
		return true;
	}
	else
	{
		std::cout << "Cannot place tower on this tile!" << std::endl;
		return false;
	}
}

bool Grid::isTowerPlaceableAtTile(sf::Vector2i tilePosition) const
{
	return isTowerPlaceableAtTile(tilePosition.x, tilePosition.y);
}

Tile::Type Grid::getTileType(int col, int row) const
{
	if (row < 0 || row >= rows || col < 0 || col >= cols)
		return Tile::Type::Unassigned;

	return tiles[row][col].getType();
}

Tile::Type Grid::getTileType(sf::Vector2i tilePosition) const
{
	return getTileType(tilePosition.x, tilePosition.y);
}
