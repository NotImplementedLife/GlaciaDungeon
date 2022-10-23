#pragma once

#include <Astralbrew>
#include "mapstats.hpp"

struct SaveData
{
	int header;
	int language;
	unsigned maps_mmss[MAP_STATS_COUNT];
	
};

class SaveFile : public Astralbrew::SaveTool::StructuredSaveFile<SaveData>
{	
public:
	SaveFile();
	void load();		
};

extern SaveFile SAVE_FILE;