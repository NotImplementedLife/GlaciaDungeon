#include "mapstats.hpp"

#include <stdlib.h>

const MapData* random_map()
{
	return &MAP_STATS[rand()%MAP_STATS_COUNT];
}