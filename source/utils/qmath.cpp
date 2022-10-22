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

__attribute__((target("arm"), section(".iwram.qmath.norm_coords"))) 
int norm_coords(int x, int y)
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
