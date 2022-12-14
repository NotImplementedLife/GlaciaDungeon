#pragma once

#include <Astralbrew>
#include "mapstats.hpp"

struct SaveData
{
	int header;
	int language;
	unsigned maps_mmss[MAP_STATS_COUNT];
	int current_level;
};

class SaveFile : public Astralbrew::SaveTool::StructuredSaveFile<SaveData>
{	
public:
	SaveFile();
	void load();		
	void init_data();
};

extern SaveFile SAVE_FILE;