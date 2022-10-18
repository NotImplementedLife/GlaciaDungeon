#pragma once

class GhostAI
{
protected:
	int cx, cy; // chunk
	int gx, gy; // ghost pos
	int px, py; // player pos
public:
	GhostAI();
	virtual void set_chunk(int cx, int cy);
	void ghost_pos(int gx, int gy);
	void player_pos(int px, int py);
	virtual void execute_step() = 0;
	int get_x() const;
	int get_y() const;	
	virtual ~GhostAI() = default;
};


class SquareAI : public GhostAI
{
private:
	int dir = 0;
	int len = 0;
	int crtlen = 0;
public:
	SquareAI();
	void set_chunk(int cx, int cy) override;
	void execute_step() override;
};