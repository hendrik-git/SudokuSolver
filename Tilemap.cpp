// Tilemap.cpp

#include "Tilemap.h"
#include <iostream>
#include <set>
#include <algorithm>

constexpr int WINDOW_WITH   {450};
constexpr int WINDOW_HEIGHT {450};

auto Tile::set(const int val) -> void
{
	value    = val;
	is_given = (val != 0)? true : false;
}

Map::Map(unsigned int width, unsigned int height)
	: map_width{width}, map_height{height}
{
	// helper values for dimension assignment
	int x{ 0 };
	int y{ 0 };

	for(int row = 0; row < width; row++)
	{
		std::vector<Tile> new_row;
		for(int col = 0; col < height; col++)
		{
			// initialize values for new tile
			Tile tile;
			tile.width  = WINDOW_WITH   / width;
			tile.height = WINDOW_HEIGHT / height;
			tile.x      = x;
			tile.y      = y;
			tile.value  = 0;

			// adjust x position of tile
			x += tile.width;

			// put this tile into the row
			new_row.push_back(tile);
		}
		// adjust position of tile
		x = 0;
		y += WINDOW_HEIGHT / height;

		// put this row into the map
		Tilemap.push_back(new_row);
	}
}


auto Map::clear(bool all) -> void
{
	for(int x = 0; x < map_width; x++)
	{
		for(int y = 0; y < map_height; y++)
		{
			if(all){
				Tilemap[y][x].value = 0;
				Tilemap[y][x].is_given = false;
			}
			else if(Tilemap[y][x].is_given)
				continue;
			else
				Tilemap[y][x].value = 0;
		}
	}

	stopped = false;
	pos = 0;
	dir = 1;
}


auto Map::set(
		unsigned int m_x,
		unsigned int m_y,
		unsigned int val) -> void
{
	// translate mouse coordinate to tile coordinate
	for(int x = 0; x < map_width; x++){
		for(int y = 0; y < map_height; y++){
			auto& tile = Tilemap[y][x];
    		if( m_x > tile.x && m_x < tile.x + tile.width &&
    			m_y > tile.y && m_y < tile.y + tile.height){
    			tile.set(val);
    		}
    	}
	}
}


auto Map::pos_to_coordinates() -> std::pair<unsigned int,unsigned int>
{
	unsigned int column{ pos / map_height };
	unsigned int row   { pos % map_width  };
	std::pair<unsigned int,unsigned int> result(column,row);
	return result;
}


auto Map::pos_in_map() -> bool
{
	return(pos >= map_width * map_height) ? false : true;
}


auto Map::step() -> void
{
	if(stopped)
		return;

	if(Tilemap[0][0].value != 0){
		// prevent negative index positions
		pos = std::max(0, (int)pos + dir);
	}

	// check if pos is outside tilemap
	if(!pos_in_map())
		return;

	auto& tile = Tilemap[pos/map_height][pos%map_width];

	// never change a given tile
	if(tile.is_given){
		step();
		return;
	}

	dir = 1;

	do {
		tile.value += 1;
		if(tile.value > 15)
		{   // prevent endless loop
			std::cerr << "ERROR: map cannot be solved\n";
			stopped = true;
			break;
		}
	} while( !check_row() || !check_column() || !check_box() );

	if( tile.value > 9){ // invalid value
		tile.value = 0;  // delete and
		dir = -1;        // start backtracking
	}
}

auto Map::check_row() -> bool
{
	// counting all values and look for duplicates
	// --> 
	std::set<int> container;

	for(auto i = 0; i < map_width; i++)
	{
		auto value = Tilemap[pos/map_height][i].value; // [column][row]
		auto search = container.find(value);

		if(value == 0){
			// ignore empty tiles
			continue;
		}

    	else if(search != container.end()) {
    		// duplicate found
        	return false;
    	} 

    	else {
        	// value not found in container
        	container.insert(value);
    	}
	}
	return true;
}

auto Map::check_column() -> bool
{
	// | counting all values and look for duplicates
	// v 
	std::set<int> container;

	for(auto i = 0; i < map_width; i++)
	{
		auto value = Tilemap[i][pos%map_width].value; // [column][row]
		auto search = container.find(value);

		if(value == 0){
			// ignore empty tiles
			continue;
		}

    	else if(search != container.end()) {
    		// duplicate found
        	return false;
    	} 

    	else {
        	// value not found in container
        	container.insert(value);
    	}
	}
	return true;
}

auto Map::check_box() -> bool
{
	// 3x3 box of values
	std::set<int> container;
	auto x_a{ ((pos%map_width) /3)*3 };
	auto x_e{ x_a + 3 };
	auto y_a{ ((pos/map_height)/3)*3 };
	auto y_e{ y_a + 3 };

	for(auto i=x_a; i<x_e; i++)
	{
		for(auto j=y_a; j<y_e; j++)
		{
			auto value = Tilemap[j][i].value;
			auto search = container.find(value);
			if(value == 0){
				// ignore empty tiles
				continue;
			}

    		else if(search != container.end()) {
    			// duplicate found
        		return false;
    		} 

    		else {
        		// value not found in container
        		container.insert(value);
    		}
		} // end j
	} // end i
	return true;
}

auto Map::print_to_console() -> void 
{
	for(auto i : Tilemap){
    	for(auto j : i){
			std::cout << j.value << "\t";
    	}
    std::cout << "\n";
	}
}

auto Map::set_example() -> void
{
	// map[v column][> row]
	Tilemap[0][0].set(5);
	Tilemap[0][1].set(3);
	Tilemap[0][4].set(7);
	Tilemap[1][0].set(6);
	Tilemap[1][3].set(1);
	Tilemap[1][4].set(9);
	Tilemap[1][5].set(5);
	Tilemap[2][1].set(9);
	Tilemap[2][2].set(8);
	Tilemap[2][7].set(6);
	Tilemap[3][0].set(8);
	Tilemap[3][4].set(6);
	Tilemap[3][8].set(3);
	Tilemap[4][0].set(4);
	Tilemap[4][3].set(8);
	Tilemap[4][5].set(3);
	Tilemap[4][8].set(1);
	Tilemap[5][0].set(7);
	Tilemap[5][4].set(2);
	Tilemap[5][8].set(6);
	Tilemap[6][1].set(6);
	Tilemap[6][6].set(2);
	Tilemap[6][7].set(8);
	Tilemap[7][3].set(4);
	Tilemap[7][4].set(1);
	Tilemap[7][5].set(9);
	Tilemap[7][8].set(5);
	Tilemap[8][4].set(8);
	Tilemap[8][7].set(7);
	Tilemap[8][8].set(9);
}