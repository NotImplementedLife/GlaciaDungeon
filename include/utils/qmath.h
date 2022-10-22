#pragma once

// maps internal [0,256] to another interval [0,A] with A integer such that A/256.f approximates PI.
int get_pi_division_units(int f);

bool is_in_circle(int x, int y, int r);


// the following functions consider the circle divided into 512 units 
// e.g sin(256) := sin(pi)
//     atan(1) = 128 := pi/4

// z = fixed signed 15.16
int qatan(int z);

// atan2 but splits the circle into 512 units and returns the closest unit to the angle value
int qatan2(int y, int x);


// sqrt(1+x), x=0..1, mapped 0..8192
// result: 0..sqrt(2), where 0..1 is mapped 0..8192
int qsqrt1_sub(int x);

// alternative for atan2() but returns sin and cos of angle scaled 256
int norm_coords(int x, int y);