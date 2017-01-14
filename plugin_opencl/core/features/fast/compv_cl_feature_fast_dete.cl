/* Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/

__local static const unsigned short kCompVFast9Flags[16] = { 0x1ff, 0x3fe, 0x7fc, 0xff8, 0x1ff0, 0x3fe0, 0x7fc0, 0xff80, 0xff01, 0xfe03, 0xfc07, 0xf80f, 0xf01f, 0xe03f, 0xc07f, 0x80ff };
__local static const unsigned short kCompVFast12Flags[16] = { 0xfff, 0x1ffe, 0x3ffc, 0x7ff8, 0xfff0, 0xffe1, 0xffc3, 0xff87, 0xff0f, 0xfe1f, 0xfc3f, 0xf87f, 0xf0ff, 0xe1ff, 0xc3ff, 0x87ff };

#define _opencl_fast_check(a, b, c, d) \
		t0 = IP[idx + pixels16[a]], t1 = IP[idx + pixels16[b]]; \
		sd = (t0 < darker) + (t1 < darker), sb = (t0 > brighter) + (t1 > brighter); \
		if (!(sd || sb)) { strengths[idx] = 0; return; } \
		sumd += sd, sumb += sb; \
		t0 = IP[idx + pixels16[c]], t1 = IP[idx + pixels16[d]]; \
		sd = (t0 < darker) + (t1 < darker), sb = (t0 > brighter) + (t1 > brighter); \
		if (!(sd || sb)) { strengths[idx] = 0; return; } \
		sumd += sd, sumb += sb; \
		if (sumd < minsum && sumb < minsum) { strengths[idx] = 0; return; } \

#define _opencl_fast_strenght() \
	unsigned flags = \
		(neighborhoods16[0] ? (1 << 0) : 0) \
		| (neighborhoods16[1] ? (1 << 1) : 0) \
		| (neighborhoods16[2] ? (1 << 2) : 0) \
		| (neighborhoods16[3] ? (1 << 3) : 0) \
		| (neighborhoods16[4] ? (1 << 4) : 0) \
		| (neighborhoods16[5] ? (1 << 5) : 0) \
		| (neighborhoods16[6] ? (1 << 6) : 0) \
		| (neighborhoods16[7] ? (1 << 7) : 0) \
		| (neighborhoods16[8] ? (1 << 8) : 0) \
		| (neighborhoods16[9] ? (1 << 9) : 0) \
		| (neighborhoods16[10] ? (1 << 10) : 0) \
		| (neighborhoods16[11] ? (1 << 11) : 0) \
		| (neighborhoods16[12] ? (1 << 12) : 0) \
		| (neighborhoods16[13] ? (1 << 13) : 0) \
		| (neighborhoods16[14] ? (1 << 14) : 0) \
		| (neighborhoods16[15] ? (1 << 15) : 0); \
		for (unsigned arcStart = 0; arcStart < 16; ++arcStart) { \
			if ((flags & FastXFlags[arcStart]) == FastXFlags[arcStart]) { \
				t0 = 0xff; \
				for (unsigned j = arcStart, k = 0; k < N; ++j, ++k) { \
					t0 = min(neighborhoods16[j & 15], t0); \
				} \
				strength = max(strength, t0); \
			} \
		} \


// Requires OpenCL 1.1
__kernel void clFAST(
	__global const unsigned char* IP,
	unsigned int width,
	unsigned int height,
	unsigned int stride,
	__global const int* pixels16,
	unsigned char N,
	unsigned char threshold,
	__global unsigned char* strengths
)
{
	// FIXME: We can use a vector of 64 (localsize)

#if 0
	int x = get_global_id(0);
	int y = get_global_id(1);
	int idx = x + (y*stride);
	if (x < width) {
		if (x >= 3 && x < width - 3 && y >= 3 && y < height - 3) {
			const unsigned char minsum = (N == 12 ? 3 : 2); // FIXME: make param
			unsigned char strength, sumb, sumd, sb, sd, brighter, darker, t0, t1;
			strength = sumb = sumd = 0;
			brighter = add_sat(IP[idx], threshold);
			darker = sub_sat(IP[idx], threshold);

			_opencl_fast_check(0, 8, 4, 12);
			_opencl_fast_check(1, 9, 5, 13);
			_opencl_fast_check(2, 10, 6, 14);
			_opencl_fast_check(3, 11, 7, 15);

			//_opencl_fast_check(0, 8, 4, 12);
			int ii = pixels16[0];
			t0 = IP[idx + ii], t1 = IP[idx + pixels16[8]];
			sd = ((t0 < darker) ? 1 : 0) + ((t1 < darker) ? 1 : 0);
			sb = ((t0 > brighter) ? 1 : 0) + ((t1 > brighter) ? 1 : 0);
			//if (!(sd || sb)) { strengths[idx] = 0; return; } 
			if (t0 < darker) {
				strengths[idx] = 1;
			}
			else {
				strengths[idx] = 0;
			}
		}
	}
#endif
	
	int x = get_global_id(0);
	if (x < width) {

		int y = get_global_id(1);
		int idx = x + (y*stride);
		strengths[x+(y * stride)] = x;
		if (x >= 3 && x < width - 3 && y >= 3 && y < height - 3) {
			const unsigned char minsum = (N == 12 ? 3 : 2); // FIXME: make param
			unsigned char strength, sumb, sumd, sb, sd, brighter, darker, t0, t1;
			strength = sumb = sumd = 0;
			brighter = add_sat(IP[idx], threshold);
			darker = sub_sat(IP[idx], threshold);

			_opencl_fast_check(0, 8, 4, 12);
			_opencl_fast_check(1, 9, 5, 13);
			_opencl_fast_check(2, 10, 6, 14);
			_opencl_fast_check(3, 11, 7, 15);

			unsigned char neighborhoods16[16];
			const unsigned short *FastXFlags = N == 9 ? kCompVFast9Flags : kCompVFast12Flags;

			if (sumd >= N) {
				neighborhoods16[0] = sub_sat(darker, IP[idx + pixels16[0]]);
				neighborhoods16[1] = sub_sat(darker, IP[idx + pixels16[1]]);
				neighborhoods16[2] = sub_sat(darker, IP[idx + pixels16[2]]);
				neighborhoods16[3] = sub_sat(darker, IP[idx + pixels16[3]]);
				neighborhoods16[4] = sub_sat(darker, IP[idx + pixels16[4]]);
				neighborhoods16[5] = sub_sat(darker, IP[idx + pixels16[5]]);
				neighborhoods16[6] = sub_sat(darker, IP[idx + pixels16[6]]);
				neighborhoods16[7] = sub_sat(darker, IP[idx + pixels16[7]]);
				neighborhoods16[8] = sub_sat(darker, IP[idx + pixels16[8]]);
				neighborhoods16[9] = sub_sat(darker, IP[idx + pixels16[9]]);
				neighborhoods16[10] = sub_sat(darker, IP[idx + pixels16[10]]);
				neighborhoods16[11] = sub_sat(darker, IP[idx + pixels16[11]]);
				neighborhoods16[12] = sub_sat(darker, IP[idx + pixels16[12]]);
				neighborhoods16[13] = sub_sat(darker, IP[idx + pixels16[13]]);
				neighborhoods16[14] = sub_sat(darker, IP[idx + pixels16[14]]);
				neighborhoods16[15] = sub_sat(darker, IP[idx + pixels16[15]]);
				_opencl_fast_strenght();
			}
			else if (sumb >= N) {
				neighborhoods16[0] = sub_sat(IP[idx + pixels16[0]], brighter);
				neighborhoods16[1] = sub_sat(IP[idx + pixels16[1]], brighter);
				neighborhoods16[2] = sub_sat(IP[idx + pixels16[2]], brighter);
				neighborhoods16[3] = sub_sat(IP[idx + pixels16[3]], brighter);
				neighborhoods16[4] = sub_sat(IP[idx + pixels16[4]], brighter);
				neighborhoods16[5] = sub_sat(IP[idx + pixels16[5]], brighter);
				neighborhoods16[6] = sub_sat(IP[idx + pixels16[6]], brighter);
				neighborhoods16[7] = sub_sat(IP[idx + pixels16[7]], brighter);
				neighborhoods16[8] = sub_sat(IP[idx + pixels16[8]], brighter);
				neighborhoods16[9] = sub_sat(IP[idx + pixels16[9]], brighter);
				neighborhoods16[10] = sub_sat(IP[idx + pixels16[10]], brighter);
				neighborhoods16[11] = sub_sat(IP[idx + pixels16[11]], brighter);
				neighborhoods16[12] = sub_sat(IP[idx + pixels16[12]], brighter);
				neighborhoods16[13] = sub_sat(IP[idx + pixels16[13]], brighter);
				neighborhoods16[14] = sub_sat(IP[idx + pixels16[14]], brighter);
				neighborhoods16[15] = sub_sat(IP[idx + pixels16[15]], brighter);
				_opencl_fast_strenght();
			}
			
			strengths[idx] = strength;
		}
		else {
			strengths[idx] = 0;
		}
	}
}