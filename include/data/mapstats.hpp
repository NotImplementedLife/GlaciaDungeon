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

#include "intro1.mapdata.h"
#include "intro2.mapdata.h"
#include "intro3.mapdata.h"

#include "map1.mapdata.h"
#include "map2.mapdata.h"
#include "map3.mapdata.h"
#include "map4.mapdata.h"
#include "map5.mapdata.h"

constexpr MapData MAP_STATS[]
{	
	MAP_DATA_intro1,
	MAP_DATA_intro2,
	MAP_DATA_intro3,
	MAP_DATA_map1,
	MAP_DATA_map2,
	MAP_DATA_map3,
	MAP_DATA_map4,
	MAP_DATA_map5,
};

#define MAP_STATS_COUNT ((int)(sizeof(MAP_STATS)/sizeof(MapData)))


const MapData* random_map();