#include "mapstats.hpp"


#include "intro1.mapdata.h"
#include "intro2.mapdata.h"
#include "intro3.mapdata.h"

#include "map1.mapdata.h"
#include "map2.mapdata.h"
#include "map3.mapdata.h"
#include "map4.mapdata.h"
#include "map5.mapdata.h"

const MapData MAP_STATS[]
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


const int MAP_STATS_COUNT = sizeof(MAP_STATS)/sizeof(MapData);