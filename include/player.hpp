#pragma once
#include <Astralbrew>
#include "map.hpp"

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
	
	static constexpr int frames_standby[] {0, 1};
	static constexpr int frames_start_skate[] {1, 2};
	static constexpr int frames_normal_skate[] {3, 2, 4, 2};
	static constexpr int frames_rage_skate[] {5,6};
	
	const int* crt_frames = frames_standby;
	int crt_frames_len = 2;
	int crt_frame_index = 0;
	int cooldown = 50;
	int crt_cooldown = 0;
	
	sf24 vx = 0, vy = 0;
	sf24 ax = 0, ay = 0;
	
	int bndx=0, bndy=0, bndw=100, bndh=100;
	
	int feetx[2];
	int feety[2];
	
	int falling_scale = 0;	
	int fall_x = 0;
	int fall_y = 0;
	
	bool fell = false;
public:
	Player();
	
	void set_movement_bounds(int x, int y, int w, int h);
	
	void set_current_frame(int orientation, int pos_index);
	
	inline int get_orientation() const { return orientation; }
	inline int get_pos_index() const { return pos_index; }
	
	void move(sf24 dx, sf24 dy);	
	
	void frameset_standby() ;	
	
	void frameset_normal();	
	
	void frameset_start();	
	
	void frameset_rage();	
	
	void update();
	
	bool check_feet(const Map* map);
	
	~Player() = default;
	
	inline int get_px() const {return (s16)px;}
	inline int get_py() const {return (s16)py;}
	
	void enable_falling();
	
	inline bool has_fallen() const {return fell;}
	
};