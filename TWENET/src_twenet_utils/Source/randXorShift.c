/*
 * randXorShift.c
 *
 *
 */

#include <jendefs.h>

static uint32 u32xyzw[4];

/**
 *
 * @param s
 */
void XORSRND_vInitSeed(uint32 s) {
	int i;
	u32xyzw[0] = 123456789;
	u32xyzw[1] = 362436069;
	u32xyzw[2] = 521288629;
	u32xyzw[3] = 88675123;

	for (i = 0; i < 4; ++i) {
		u32xyzw[i] = s = 1812433253UL * (s ^ (s >> 30)) + i;
	}
}

uint32 XORSRND_u32GetRand() {
	uint32 u32t;

	u32t = (u32xyzw[0] ^ (u32xyzw[0] << 11));
	u32xyzw[0] = u32xyzw[1];
	u32xyzw[1] = u32xyzw[2];
	u32xyzw[2] = u32xyzw[3];
	u32xyzw[3] = (u32xyzw[3] ^ (u32xyzw[3] >> 19)) ^ (u32t ^ (u32t >> 8));

	return u32xyzw[3];
}

