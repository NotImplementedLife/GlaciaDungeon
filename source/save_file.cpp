#include "save_file.hpp"
#include "langs.h"

SaveFile::SaveFile()
{
	
}

static const int HEADER = 925972534;

void SaveFile::load()
{	
	int header = read<int>(0);		
	if(header!=HEADER)
	{
		init_data();
		return;
	}	
	StructuredSaveFile<SaveData>::load();
}

void SaveFile::init_data()
{
	data().header = HEADER;
	data().language = -1;
	for(int i=0;i<MAP_STATS_COUNT;i++)
		data().maps_mmss[i]=0xFFFFFFFF;
	data().current_level = -1;
	save();
}

__attribute__((section(".ewram.savefile"))) SaveFile SAVE_FILE;