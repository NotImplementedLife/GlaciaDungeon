#include "fader.h"

#include <fade.h>

u16 SH_PALETTE[512];

u16* SH_BG_PALETTE = SH_PALETTE;
u16* SH_SPRITE_PALETTE = &SH_PALETTE[256];

void shpal_set_black()
{
	for(int i=0;i<512;i++) SH_PALETTE[i] = 0;
}

void shpal_fade()
{
	FadeToPalette(SH_PALETTE, 10);
}