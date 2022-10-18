#pragma once

#include <Astralbrew>
#include "player.hpp"
#include "map.hpp"
#include "mapstats.hpp"
#include "chunk_provider.hpp"
#include "chunk_entity.hpp"


using namespace Astralbrew::World;
using namespace Astralbrew::Entity;

class MosaicIncreaser : public ScheduledTask
{
private:
	int value = 1;
public:
	MosaicIncreaser();
	void action() override;	
};

class PortalUpdater;

class MapScene : public Scene
{	
private:
	VramManager vram_chr_1 = VramManager::from_char_block(1);
	VramManager vram_obj = VramManager(0x06011000, 0x7000, 200);
	
	Player* player;			
	Sprite* finish_portal;
	Sprite* arrow;
	
	MapSource map_source;
	Map* map;
	MapViewer* viewer;
	
	Address ice_tiles;
	Address portal_tiles;
	Address arrow_tiles;
	
	Camera camera;
	
	bool falling = false;	
	
	MosaicIncreaser mosaic_increaser;
	PortalUpdater* portal_updater;
	
	int finish_x, finish_y;
	const MapData* mapdata = nullptr;
	
	ChunkProvider chunk_provider;
	Vector<ChunkEntity*> chunk_entities;
public:	
	MapScene(const MapData* md = nullptr);	
	void load_mapstat(const MapData* md);		
	void next_map();	
	virtual void init() override;		
	
	void update_arrow();		
	
	virtual void on_key_held(int keys) override;
	
	virtual void on_key_down(int keys) override;
	
	void restart();
	
	bool player_near_portal();	
	
	virtual void frame() override;
	
	~MapScene();	
};