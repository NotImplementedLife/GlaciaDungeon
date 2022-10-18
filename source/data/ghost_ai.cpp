#include "ghost_ai.hpp"

GhostAI::GhostAI()
{
	
}

void GhostAI::set_chunk(int cx, int cy)
{
	this->cx = 128*cx;
	this->cy = 128*cy;
}

void GhostAI::ghost_pos(int gx, int gy)
{
	this->gx = gx;
	this->gy = gy;
}

void GhostAI::player_pos(int px, int py)
{
	this->px = px;
	this->py = py;
}

int GhostAI::get_x() const
{
	return gx;
}

int GhostAI::get_y() const
{
	return gy;
}

#include <stdlib.h>

SquareAI::SquareAI() : GhostAI()
{
	len = 32+rand()%90;
}

void SquareAI::set_chunk(int cx, int cy) 
{
	GhostAI::set_chunk(cx,cy);
	gx = this->cx+rand()%(110-len);
	gy = this->cy+rand()%(110-len);
}

void SquareAI::execute_step()
{	
	switch(dir&3)
	{
		case 0: gx++; break;
		case 1: gy++; break;
		case 2: gx--; break;
		case 3: gy--; break;
	}
	
	crtlen++;
	if(crtlen==len) 
	{
		dir++, dir&=3;	
		crtlen = 0;
	}		
}