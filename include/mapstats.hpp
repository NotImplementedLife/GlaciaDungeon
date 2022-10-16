#pragma once

#include "map_source.h"

struct MapData
{
	const char* const name;
	const MapSource& source;
	int start_x;
	int start_y;
	int finish_x;
	int finish_y;
};

extern const MapData MAP_STATS[];

extern const int MAP_STATS_COUNT;

#include <stdlib.h>

inline const MapData& random_map()
{
	return MAP_STATS[rand()%MAP_STATS_COUNT];
}