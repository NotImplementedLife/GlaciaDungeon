#pragma once
#include <Astralbrew>

using namespace Astralbrew;
using namespace Astralbrew::Entity;
using namespace Astralbrew::Objects;
using namespace Astralbrew::Memory;

register_class(player)

extern const int PLAYER_FRONT;
extern const int PLAYER_BACK;
extern const int PLAYER_LEFT;
extern const int PLAYER_RIGHT;

class Player : public Sprite
{
private:
	Address gfx_addr;
	int orientation, pos_index;
	sf24 px=0, py=0;
public:
	Player();
	
	void set_current_frame(int orientation, int pos_index);
	
	inline int get_orientation() const { return orientation; }
	inline int get_pos_index() const { return pos_index; }
	
	void move(sf24 dx, sf24 dy);
	
	void update_movement();
	
	~Player() = default;
	
};