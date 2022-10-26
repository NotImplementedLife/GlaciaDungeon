#include "qmath.h"

__attribute__((section(".iwram.qmath"))) int get_pi_division_units(int f)
{	
	return 355*f/113;
}

__attribute__((target("arm"), section(".iwram.qmath"))) 
bool is_in_circle(int x, int y, int r)
{
	return x*x+y*y<r*r;
}


// z = [0..1) scaled to 16-bit int
// return: [0..pi/4) scaled to [0..65536)
int qatan_sub(int z)
{
    long long r = (5653921792LL - 22272LL*z)/1012;
    r*=1LL*z;
	r/=65536;
	r/=65536/1024;
	return (int)r;
}


// z = fixed signed 15.16
int qatan(int z)
{
	bool neg = false;
	if(z<0)
	{
		z=-z;	
		neg = true;
	}	
	
	if(z<65536)
	{
		int at = qatan_sub(z); // 0..65536 mapped to 0..pi/4 <=> 0..64
		at /= 1024; 
		return neg ? -at : at;
	}
	
	// z>0 => z=fixed unsigned 14.16		
	z = (signed)((unsigned)(((unsigned)(1<<31)-1) / (unsigned)z)); // 1/z scaled to int
	
	z*=2;
	int at = qatan_sub(z); 
	at /= 1024; 
	at = 128 - at;
	return neg ? -at : at;
}


int qatan2(int y, int x)
{
	if(x==0)
	{
		if(y==0) return 0;
		if(y>0) return 128;
		if(y<0) return -128;		
	}
	
	int z = y*65536/x;	
	int at = qatan(z);
	
	if(x<0)
	{
		if(y<0) return at-256;		
		else return at+256;
	}
	
	return at;
}


// sqrt(1+x), x=0..1
__attribute__((target("arm"), section(".iwram.qmath.qsqrt1_sub"))) 
int qsqrt1_sub(int x)
{
	static const int W = 8192;
	int y=x;
	int res = W;	
	res+=y/2;       y = y*x/W;
	res-=y/8;       y = y*x/W;
	res+=y/16;      y = y*x/W;
	res-=(5*y)/128;
	return res;
}

struct precalc_norms
{
	int a, x, y;	
};

static const precalc_norms NORMS[] = 
{
	{44, 255, 22},
	{46, 255, 23},
	{64, 254, 32},
	{78, 253, 39},
	{91, 252, 45},
	{101, 251, 50},
	{112, 250, 55},
	{138, 247, 67},
	{147, 246, 71},
	{154, 245, 74},
	{189, 240, 89},
	{197, 239, 92},
	{209, 237, 97},
	{214, 236, 99},
	{227, 234, 104},
	{232, 233, 106},
	{238, 232, 108},
	{277, 225, 122},
	{283, 224, 124},
	{304, 220, 131},
	{320, 217, 136},
	{331, 215, 139},
	{341, 213, 142},
	{351, 211, 145},
	{362, 209, 148},
	{377, 206, 152},
	{393, 203, 156},
	{414, 199, 161},
	{435, 195, 166},
	{440, 194, 167},
	{484, 186, 176},
	{489, 185, 177},
	{495, 184, 178},
	{500, 183, 179},
	{506, 182, 180},
	{512, 181, 181},
	{517, 180, 182},
	{523, 179, 183},
	{529, 178, 184},
	{535, 177, 185},
	{541, 176, 186},
	{594, 167, 194},
	{601, 166, 195},
	{632, 161, 199},
	{666, 156, 203},
	{693, 152, 206},
	{723, 148, 209},
	{745, 145, 211},
	{768, 142, 213},
	{791, 139, 215},
	{816, 136, 217},
	{859, 131, 220},
	{924, 124, 224},
	{944, 122, 225},
	{1099, 108, 232},
	{1125, 106, 233},
	{1152, 104, 234},
	{1220, 99, 236},
	{1250, 97, 237},
	{1330, 92, 239},
	{1380, 89, 240},
	{1695, 74, 245},
	{1773, 71, 246},
	{1887, 67, 247},
	{2327, 55, 250},
	{2570, 50, 251},
	{2867, 45, 252},
	{3321, 39, 253},
	{4064, 32, 254},
	{5934, 22, 255},
	{5676, 23, 255},
};

__attribute__((target("arm"), section(".iwram.qmath.norm_coords"))) 
int norm_coords_sm(int x, int y)
{
	int nx, ny;
	
	int sx = x*x;
	int sy = y*y;

	if(sx==0)
	{
		if(sy==0) return 0;
		nx = 0;
		ny = y>0 ? 256:-256;
	}
	else if(sy==0)
	{
		ny = 0;
		nx = x>0 ? 256:-256;
	}	
	else if(sx == sy)
	{
		int a = qsqrt1_sub(8192);
		nx = ny = 8192*256/a;
		if(x<0) nx=-nx;
		if(y<0) ny=-ny;
	}
	else if(sx < sy)
	{
	    int a = sx*8192/sy;
		int s = qsqrt1_sub(a);
		nx = x*8192 * 256 /(y*s);
		ny = 256 * 8192/s;
		if(y<0) ny=-ny, nx=-nx;
	}
	else
	{
		int a = sy*8192/sx;
		int s = qsqrt1_sub(a);
		ny = y*8192 *256 /(x*s);
		nx = 256 * 8192/s;
		if(x<0) ny=-ny, nx=-nx;
	}	
	return ((ny&0xFFFF)<<16)|(nx&0xFFFF);
}


#define abs(x) ((x)<0?-(x):(x))

__attribute__((target("arm"), section(".iwram.qmath.norm_coords"))) 
int norm_coords(int x, int y)
{
	if(abs(x)<256 && abs(y)<256)
		return norm_coords_sm(x,y);
	int nx = (x<0 ? -x:x), ny = (y<0?-y:y);
	
	if(ny==0)
	{
		nx = 256;
	}
	else if(nx==0)
	{
		ny = 256;
	}
	else
	{
		int a = nx==0 ? 16000 : ny*512/nx;
		
		const int n = sizeof(NORMS)/sizeof(precalc_norms);
		int l = 0, r=n;
		
		while(l<r)
		{
			int m = (l+r)/2;
			if(NORMS[m].a == a) break;
			if(NORMS[m].a < a)
				l=m+1;
			else
				r=m;
		}
		
		if(l<n)
		{
			if(abs(NORMS[r].a-a)<abs(NORMS[l].a-a))
			{
				nx = NORMS[r].x;
				ny = NORMS[r].y;
			}
			else
			{
				nx = NORMS[l].x;
				ny = NORMS[l].y;
			}
		}
		else
		{
			nx = NORMS[r].x;
			ny = NORMS[r].y;
		}
	}
	
	if(x<0) nx=-nx;
	if(y<0) ny=-ny;
	
	
	return ((ny&0xFFFF)<<16)|(nx&0xFFFF);
}
