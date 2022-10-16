#include "mapstats.hpp"


#include "map1.mapdata.h"
#include "map2.mapdata.h"
#include "map3.mapdata.h"
#include "map4.mapdata.h"

#define cr_map(mapx, sx, sy, fx, fy) { #mapx, mapx, sx, sy, fx, fy }

const MapData MAP_STATS[]
{	
	MAP_DATA_map1,
	MAP_DATA_map2,	
	MAP_DATA_map3,		
	MAP_DATA_map4,
};


const int MAP_STATS_COUNT = sizeof(MAP_STATS)/sizeof(MapData);