#pragma once

#include <Astralbrew>

#include "map.hpp"
#include "credits_bg.maps.h"

using namespace Astralbrew;
using namespace Astralbrew::World;
using namespace Astralbrew::Text;

class CreditsScene : public Scene
{
private:
	VramManager vram_chr_1 = VramManager::from_char_block(1);
	bool title_next;
	MapSource map_source;
	Map* map;
	MapViewer* viewer;
	
	Address ice_tiles;
	
	MapSource ms = credits_bg;
	
	Camera camera;
	sf24 x = 0;
	sf24 y = 80;
	int t=0;
	int sgn = 1;
	
	VwfEngine vwf = VwfEngine(Astralbrew::Resources::Fonts::default_8x16, Astralbrew::Text::ExtendedFontChart::ExtL2);	
	
	const char* msgindex = 0;
	int msgcounter = 0;
	
	static const char* const messages;
public:
	CreditsScene(bool title_next = true);
	void before_frame() override;
	void init() override;		
	void frame() override;
	void on_key_down(int keys) override;
	~CreditsScene();
};