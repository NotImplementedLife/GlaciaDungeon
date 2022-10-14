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
public:
	Player();
	
	void set_current_frame(int orientation, int pos_index);
	
	inline int get_orientation() const { return orientation; }
	inline int get_pos_index() const { return pos_index; }
	
	void move(sf24 dx, sf24 dy);
	
	sf24 to_0(sf24 val, sf24 amount) {
		if(amount<0) amount=-amount;
		if(val<0) {
			val+=amount;
			if(val>0) val=0;
		}
		else if(val>0){
			val-=amount;
			if(val<0) val=0;
		}
		return val;
	}
	
	void update()
	{	
		vx+=ax;
		vy+=ay;
		px+=vx;
		py+=vy;	
		
		vx = to_0(vx, sf24(0,5));
		vy = to_0(vy, sf24(0,5));
		
		ax = to_0(ax, ax*sf24(0,128));
		ay = to_0(ay, ay*sf24(0,128));
		
				
		set_position(px,py);
	
		if(crt_cooldown>0) {
			crt_cooldown++;
			if(crt_cooldown>=cooldown) 
				crt_cooldown=0;
			return;
		}
		crt_cooldown++;
		set_current_frame(orientation, crt_frames[crt_frame_index]);
		
		crt_frame_index++;
		if(crt_frame_index>=crt_frames_len) 
			crt_frame_index = 0;		
	}
	
	~Player() = default;
	
};