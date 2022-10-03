#include <Astralbrew>
#include <gba.h>
#include <stdio.h>

using namespace Astralbrew;
using namespace Astralbrew::World;
using namespace Astralbrew::Drawing;
using namespace Astralbrew::Memory;
using namespace Astralbrew::Video;
using namespace Astralbrew::Objects;
using namespace Astralbrew::Entity;

#include "player.hpp"

class MainScene : public Scene
{	
private:
	VramManager vram_chr_1 = VramManager::from_char_block(1);
	VramManager vram_obj = VramManager::from_char_block(4);
	
	Player* player;	
public:	
	virtual void init() override
	{
		//Video::setMode(0);						
		
		consoleDemoInit();
		printf("\n          Ice Skater\n        position  test\n\n");
		printf("UP  /DOWN  Change Orientation\n");
		printf("LEFT/RIGHT Change Position\n");
				
		objEnable1D();	
		
		player = new Player();
		
		player->set_position(120-32, 80-32);
		player->update_position(nullptr);
		player->update_visual();		
	}
	
	virtual void on_key_down(int keys) override
	{
		int pos_index = player->get_pos_index();
		int orientation = player->get_orientation();
		if(keys & KEY_LEFT)
		{		
			if(pos_index > 0)
			{
				player->set_current_frame(orientation, pos_index-1);
			}			
		}		
		else if(keys & KEY_RIGHT)
		{
			if(pos_index < 6)
			{
				player->set_current_frame(orientation, pos_index+1);
			}
		}
		
		if(keys & KEY_UP)
		{		
			if(orientation > 0)
			{
				player->set_current_frame(orientation-1, pos_index);
			}			
		}		
		else if(keys & KEY_DOWN)
		{
			if(orientation < 3)
			{
				player->set_current_frame(orientation+1, pos_index);
			}
		}
	}
	
	virtual void frame() override
	{
		OamPool::deploy();
	}
};

astralbrew_launch(MainScene)


