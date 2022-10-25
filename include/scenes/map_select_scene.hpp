#pragma once

#include <Astralbrew>

using namespace Astralbrew::Entity;
using namespace Astralbrew::World;

class MapSelectScene : public Scene
{
private:	
	static const int half_w = 80;
	static const int half_h = 50;
	void set_tile(int x, int y, int id);
	int index = 0;
	void generate_map();
	Sprite* a_select;
	int cnt = 0;
	int sgn = 1;
public:
	virtual void init() override;
	virtual void frame() override;
	virtual void on_key_down(int keys) override;
	~MapSelectScene();
};