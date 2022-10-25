#include "mapstats.hpp"

#include "intro1.mapdata.h"
#include "intro2.mapdata.h"
#include "intro3.mapdata.h"

#include "map1.mapdata.h"
#include "map2.mapdata.h"
#include "map3.mapdata.h"
#include "map4.mapdata.h"
#include "map5.mapdata.h"
#include "map6.mapdata.h"
#include "map7.mapdata.h"
#include "map8.mapdata.h"
#include "map9.mapdata.h"
#include "map10.mapdata.h"
#include "map11.mapdata.h"
#include "map12.mapdata.h"
#include "map13.mapdata.h"
#include "map14.mapdata.h"
#include "map15.mapdata.h"
#include "map16.mapdata.h"
#include "map17.mapdata.h"
#include "map18.mapdata.h"
#include "map19.mapdata.h"
#include "map20.mapdata.h"


#include <stdlib.h>

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
	MAP_DATA_map6,
	MAP_DATA_map7,
	MAP_DATA_map8,
	MAP_DATA_map9,
	MAP_DATA_map10,
	MAP_DATA_map11,
	MAP_DATA_map12,
	MAP_DATA_map13,
	MAP_DATA_map14,
	MAP_DATA_map15,
	MAP_DATA_map16,
	MAP_DATA_map17,
	MAP_DATA_map18,
	MAP_DATA_map19,
	MAP_DATA_map20,
	
};

const MapData* random_map()
{
	return &MAP_STATS[rand()%MAP_STATS_COUNT];
}