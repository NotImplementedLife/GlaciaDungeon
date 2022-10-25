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

#define cr_map(mapx, sx, sy, fx, fy) { #mapx, mapx, sx, sy, fx, fy }

extern const MapData MAP_STATS[];

//#define MAP_STATS_COUNT ((int)(sizeof(MAP_STATS)/sizeof(MapData)))
#define MAP_STATS_COUNT 23


const MapData* random_map();