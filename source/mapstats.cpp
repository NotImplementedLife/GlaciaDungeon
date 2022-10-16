#include "mapstats.hpp"


#include "map0.maps.h"
#include "map3.maps.h"

#include "map1.mapdata.h"
#include "map2.mapdata.h"
#include "map4.mapdata.h"

#define cr_map(mapx, sx, sy, fx, fy) { #mapx, mapx, sx, sy, fx, fy }

const MapData MAP_STATS[]
{
	cr_map( map0, 150,  80, 0, 0 ),
	MAP_DATA_map1,
	MAP_DATA_map2,	
	cr_map( map3, 120,  80, 0, 0 ),
	MAP_DATA_map4,
};


const int MAP_STATS_COUNT = sizeof(MAP_STATS)/sizeof(MapData);