#pragma once

#include <Astralbrew>

#include "player.hpp"

using namespace Astralbrew::World;

class TitleScene : public Scene
{
private:
	VramManager vram_chr_1 = VramManager::from_char_block(1);
	Player* player;
	int mvcnt = 0;
	sf24 a = sf24(0,16);
public:
	void init() override;
	
	void frame() override;
	
	void on_key_down(int keys) override;
	
	void launch_map();
	
	~TitleScene();
};