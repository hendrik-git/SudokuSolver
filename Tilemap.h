// Tilemap.h

#pragma once
#include<vector>
#include<utility> // pair

struct Tile 
{
	unsigned int width;
	unsigned int height;
	unsigned int x;
	unsigned int y;
	unsigned int value;
	bool         is_given;

	auto set(const int val) -> void;
};


class Map 
{
	unsigned int map_width  = 0;
	unsigned int map_height = 0;
	unsigned int pos        = 0;
	int          dir        = 1;
	bool         stopped    = false;

	auto pos_to_coordinates() -> std::pair<unsigned int,unsigned int>; // column, row
	auto pos_in_map()         -> bool;
	auto check_row()          -> bool;
	auto check_column()       -> bool;
	auto check_box()          -> bool;
	
public:
	std::vector< std::vector<Tile> > Tilemap;

	Map(unsigned int width, unsigned int height);

	auto clear(bool all=true) -> void;
	auto step()               -> void;
	auto set_example()        -> void;
	auto print_to_console()   -> void;
	auto set(
		unsigned int m_x,
		unsigned int m_y,
		unsigned int val)     -> void;
};