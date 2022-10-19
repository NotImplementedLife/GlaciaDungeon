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
	len = 10+rand()%90;
}

void SquareAI::set_chunk(int cx, int cy) 
{
	GhostAI::set_chunk(cx,cy);
	gx = 9+this->cx+rand()%(110-len);
	gy = 9+this->cy+rand()%(110-len);
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


CircleAI::CircleAI() : GhostAI() 
{
	r = 30+rand()%32;	
	t = 0;
}

typedef unsigned short u16;
typedef short s16;

extern const int cos_sin[];
const s16* trig_table = (s16*)cos_sin;

void CircleAI::set_chunk(int cx, int cy)
{
	GhostAI::set_chunk(cx,cy);
	ox = r+this->cx+rand()%(128-2*r);
	oy = r+this->cy+rand()%(128-2*r);
}

void CircleAI::execute_step()
{
	cooldown++;
	if(cooldown==2) cooldown=0;
	if(cooldown) return;	
	int x = trig_table[2*t];
	int y = trig_table[2*t+1];
	gx = ox + ((r*x)>>14);
	gy = oy + ((r*y)>>14);
	t+=dir, t&=0xFF;
}

__attribute__((section(".ewram"))) const int cos_sin[256] = 
{
	(((u16)16384<<16)|(u16)0), (((u16)16379<<16)|(u16)402), (((u16)16364<<16)|(u16)803), (((u16)16339<<16)|(u16)1205), 
	(((u16)16069<<16)|(u16)3196), (((u16)15985<<16)|(u16)3589), (((u16)15892<<16)|(u16)3980), (((u16)15790<<16)|(u16)4369), 
	(((u16)15136<<16)|(u16)6269), (((u16)14978<<16)|(u16)6639), (((u16)14810<<16)|(u16)7005), (((u16)14634<<16)|(u16)7366), 
	(((u16)13622<<16)|(u16)9102), (((u16)13395<<16)|(u16)9434), (((u16)13159<<16)|(u16)9759), (((u16)12916<<16)|(u16)10079), 
	(((u16)11585<<16)|(u16)11585), (((u16)11297<<16)|(u16)11866), (((u16)11002<<16)|(u16)12139), (((u16)10701<<16)|(u16)12406), 
	(((u16)9102<<16)|(u16)13622), (((u16)8765<<16)|(u16)13842), (((u16)8423<<16)|(u16)14053), (((u16)8075<<16)|(u16)14255), 
	(((u16)6269<<16)|(u16)15136), (((u16)5896<<16)|(u16)15286), (((u16)5519<<16)|(u16)15426), (((u16)5139<<16)|(u16)15557), 
	(((u16)3196<<16)|(u16)16069), (((u16)2801<<16)|(u16)16142), (((u16)2404<<16)|(u16)16206), (((u16)2005<<16)|(u16)16260), 
	(((u16)0<<16)|(u16)16384), (((u16)-402<<16)|(u16)16379), (((u16)-803<<16)|(u16)16364), (((u16)-1205<<16)|(u16)16339), 
	(((u16)-3196<<16)|(u16)16069), (((u16)-3589<<16)|(u16)15985), (((u16)-3980<<16)|(u16)15892), (((u16)-4369<<16)|(u16)15790), 
	(((u16)-6269<<16)|(u16)15136), (((u16)-6639<<16)|(u16)14978), (((u16)-7005<<16)|(u16)14810), (((u16)-7366<<16)|(u16)14634), 
	(((u16)-9102<<16)|(u16)13622), (((u16)-9434<<16)|(u16)13395), (((u16)-9759<<16)|(u16)13159), (((u16)-10079<<16)|(u16)12916), 
	(((u16)-11585<<16)|(u16)11585), (((u16)-11866<<16)|(u16)11297), (((u16)-12139<<16)|(u16)11002), (((u16)-12406<<16)|(u16)10701), 
	(((u16)-13622<<16)|(u16)9102), (((u16)-13842<<16)|(u16)8765), (((u16)-14053<<16)|(u16)8423), (((u16)-14255<<16)|(u16)8075), 
	(((u16)-15136<<16)|(u16)6269), (((u16)-15286<<16)|(u16)5896), (((u16)-15426<<16)|(u16)5519), (((u16)-15557<<16)|(u16)5139), 
	(((u16)-16069<<16)|(u16)3196), (((u16)-16142<<16)|(u16)2801), (((u16)-16206<<16)|(u16)2404), (((u16)-16260<<16)|(u16)2005), 
	(((u16)-16384<<16)|(u16)0), (((u16)-16379<<16)|(u16)-402), (((u16)-16364<<16)|(u16)-803), (((u16)-16339<<16)|(u16)-1205), 
	(((u16)-16069<<16)|(u16)-3196), (((u16)-15985<<16)|(u16)-3589), (((u16)-15892<<16)|(u16)-3980), (((u16)-15790<<16)|(u16)-4369), 
	(((u16)-15136<<16)|(u16)-6269), (((u16)-14978<<16)|(u16)-6639), (((u16)-14810<<16)|(u16)-7005), (((u16)-14634<<16)|(u16)-7366), 
	(((u16)-13622<<16)|(u16)-9102), (((u16)-13395<<16)|(u16)-9434), (((u16)-13159<<16)|(u16)-9759), (((u16)-12916<<16)|(u16)-10079), 
	(((u16)-11585<<16)|(u16)-11585), (((u16)-11297<<16)|(u16)-11866), (((u16)-11002<<16)|(u16)-12139), (((u16)-10701<<16)|(u16)-12406), 
	(((u16)-9102<<16)|(u16)-13622), (((u16)-8765<<16)|(u16)-13842), (((u16)-8423<<16)|(u16)-14053), (((u16)-8075<<16)|(u16)-14255), 
	(((u16)-6269<<16)|(u16)-15136), (((u16)-5896<<16)|(u16)-15286), (((u16)-5519<<16)|(u16)-15426), (((u16)-5139<<16)|(u16)-15557), 
	(((u16)-3196<<16)|(u16)-16069), (((u16)-2801<<16)|(u16)-16142), (((u16)-2404<<16)|(u16)-16206), (((u16)-2005<<16)|(u16)-16260), 
	(((u16)0<<16)|(u16)-16384), (((u16)402<<16)|(u16)-16379), (((u16)803<<16)|(u16)-16364), (((u16)1205<<16)|(u16)-16339), 
	(((u16)3196<<16)|(u16)-16069), (((u16)3589<<16)|(u16)-15985), (((u16)3980<<16)|(u16)-15892), (((u16)4369<<16)|(u16)-15790), 
	(((u16)6269<<16)|(u16)-15136), (((u16)6639<<16)|(u16)-14978), (((u16)7005<<16)|(u16)-14810), (((u16)7366<<16)|(u16)-14634), 
	(((u16)9102<<16)|(u16)-13622), (((u16)9434<<16)|(u16)-13395), (((u16)9759<<16)|(u16)-13159), (((u16)10079<<16)|(u16)-12916), 
	(((u16)11585<<16)|(u16)-11585), (((u16)11866<<16)|(u16)-11297), (((u16)12139<<16)|(u16)-11002), (((u16)12406<<16)|(u16)-10701), 
	(((u16)13622<<16)|(u16)-9102), (((u16)13842<<16)|(u16)-8765), (((u16)14053<<16)|(u16)-8423), (((u16)14255<<16)|(u16)-8075), 
	(((u16)15136<<16)|(u16)-6269), (((u16)15286<<16)|(u16)-5896), (((u16)15426<<16)|(u16)-5519), (((u16)15557<<16)|(u16)-5139), 
	(((u16)16069<<16)|(u16)-3196), (((u16)16142<<16)|(u16)-2801), (((u16)16206<<16)|(u16)-2404), (((u16)16260<<16)|(u16)-2005), 
	(((u16)16384<<16)|(u16)0), (((u16)16379<<16)|(u16)402), (((u16)16364<<16)|(u16)803), (((u16)16339<<16)|(u16)1205), 
	(((u16)16069<<16)|(u16)3196), (((u16)15985<<16)|(u16)3589), (((u16)15892<<16)|(u16)3980), (((u16)15790<<16)|(u16)4369), 
	(((u16)15136<<16)|(u16)6269), (((u16)14978<<16)|(u16)6639), (((u16)14810<<16)|(u16)7005), (((u16)14634<<16)|(u16)7366), 
	(((u16)13622<<16)|(u16)9102), (((u16)13395<<16)|(u16)9434), (((u16)13159<<16)|(u16)9759), (((u16)12916<<16)|(u16)10079), 
	(((u16)11585<<16)|(u16)11585), (((u16)11297<<16)|(u16)11866), (((u16)11002<<16)|(u16)12139), (((u16)10701<<16)|(u16)12406), 
	(((u16)9102<<16)|(u16)13622), (((u16)8765<<16)|(u16)13842), (((u16)8423<<16)|(u16)14053), (((u16)8075<<16)|(u16)14255), 
	(((u16)6269<<16)|(u16)15136), (((u16)5896<<16)|(u16)15286), (((u16)5519<<16)|(u16)15426), (((u16)5139<<16)|(u16)15557), 
	(((u16)3196<<16)|(u16)16069), (((u16)2801<<16)|(u16)16142), (((u16)2404<<16)|(u16)16206), (((u16)2005<<16)|(u16)16260), 
	(((u16)0<<16)|(u16)16384), (((u16)-402<<16)|(u16)16379), (((u16)-803<<16)|(u16)16364), (((u16)-1205<<16)|(u16)16339), 
	(((u16)-3196<<16)|(u16)16069), (((u16)-3589<<16)|(u16)15985), (((u16)-3980<<16)|(u16)15892), (((u16)-4369<<16)|(u16)15790), 
	(((u16)-6269<<16)|(u16)15136), (((u16)-6639<<16)|(u16)14978), (((u16)-7005<<16)|(u16)14810), (((u16)-7366<<16)|(u16)14634), 
	(((u16)-9102<<16)|(u16)13622), (((u16)-9434<<16)|(u16)13395), (((u16)-9759<<16)|(u16)13159), (((u16)-10079<<16)|(u16)12916), 
	(((u16)-11585<<16)|(u16)11585), (((u16)-11866<<16)|(u16)11297), (((u16)-12139<<16)|(u16)11002), (((u16)-12406<<16)|(u16)10701), 
	(((u16)-13622<<16)|(u16)9102), (((u16)-13842<<16)|(u16)8765), (((u16)-14053<<16)|(u16)8423), (((u16)-14255<<16)|(u16)8075), 
	(((u16)-15136<<16)|(u16)6269), (((u16)-15286<<16)|(u16)5896), (((u16)-15426<<16)|(u16)5519), (((u16)-15557<<16)|(u16)5139), 
	(((u16)-16069<<16)|(u16)3196), (((u16)-16142<<16)|(u16)2801), (((u16)-16206<<16)|(u16)2404), (((u16)-16260<<16)|(u16)2005), 
	(((u16)-16384<<16)|(u16)0), (((u16)-16379<<16)|(u16)-402), (((u16)-16364<<16)|(u16)-803), (((u16)-16339<<16)|(u16)-1205), 
	(((u16)-16069<<16)|(u16)-3196), (((u16)-15985<<16)|(u16)-3589), (((u16)-15892<<16)|(u16)-3980), (((u16)-15790<<16)|(u16)-4369), 
	(((u16)-15136<<16)|(u16)-6269), (((u16)-14978<<16)|(u16)-6639), (((u16)-14810<<16)|(u16)-7005), (((u16)-14634<<16)|(u16)-7366), 
	(((u16)-13622<<16)|(u16)-9102), (((u16)-13395<<16)|(u16)-9434), (((u16)-13159<<16)|(u16)-9759), (((u16)-12916<<16)|(u16)-10079), 
	(((u16)-11585<<16)|(u16)-11585), (((u16)-11297<<16)|(u16)-11866), (((u16)-11002<<16)|(u16)-12139), (((u16)-10701<<16)|(u16)-12406), 
	(((u16)-9102<<16)|(u16)-13622), (((u16)-8765<<16)|(u16)-13842), (((u16)-8423<<16)|(u16)-14053), (((u16)-8075<<16)|(u16)-14255), 
	(((u16)-6269<<16)|(u16)-15136), (((u16)-5896<<16)|(u16)-15286), (((u16)-5519<<16)|(u16)-15426), (((u16)-5139<<16)|(u16)-15557), 
	(((u16)-3196<<16)|(u16)-16069), (((u16)-2801<<16)|(u16)-16142), (((u16)-2404<<16)|(u16)-16206), (((u16)-2005<<16)|(u16)-16260), 
	(((u16)0<<16)|(u16)-16384), (((u16)402<<16)|(u16)-16379), (((u16)803<<16)|(u16)-16364), (((u16)1205<<16)|(u16)-16339), 
	(((u16)3196<<16)|(u16)-16069), (((u16)3589<<16)|(u16)-15985), (((u16)3980<<16)|(u16)-15892), (((u16)4369<<16)|(u16)-15790), 
	(((u16)6269<<16)|(u16)-15136), (((u16)6639<<16)|(u16)-14978), (((u16)7005<<16)|(u16)-14810), (((u16)7366<<16)|(u16)-14634), 
	(((u16)9102<<16)|(u16)-13622), (((u16)9434<<16)|(u16)-13395), (((u16)9759<<16)|(u16)-13159), (((u16)10079<<16)|(u16)-12916), 
	(((u16)11585<<16)|(u16)-11585), (((u16)11866<<16)|(u16)-11297), (((u16)12139<<16)|(u16)-11002), (((u16)12406<<16)|(u16)-10701), 
	(((u16)13622<<16)|(u16)-9102), (((u16)13842<<16)|(u16)-8765), (((u16)14053<<16)|(u16)-8423), (((u16)14255<<16)|(u16)-8075), 
	(((u16)15136<<16)|(u16)-6269), (((u16)15286<<16)|(u16)-5896), (((u16)15426<<16)|(u16)-5519), (((u16)15557<<16)|(u16)-5139), 
	(((u16)16069<<16)|(u16)-3196), (((u16)16142<<16)|(u16)-2801), (((u16)16206<<16)|(u16)-2404), (((u16)16260<<16)|(u16)-2005), 
};