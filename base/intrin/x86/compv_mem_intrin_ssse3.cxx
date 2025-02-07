/* Copyright (C) 2011-2020 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#include "compv/base/intrin/x86/compv_mem_intrin_ssse3.h"

#if COMPV_ARCH_X86 && COMPV_INTRINSIC
#include "compv/base/compv_simd_globals.h"
#include "compv/base/intrin/x86/compv_intrin_sse.h"
#include "compv/base/compv_debug.h"

COMPV_NAMESPACE_BEGIN()

void CompVMemUnpack4_Intrin_SSSE3(
	COMPV_ALIGNED(SSE) uint8_t* dstPt0, COMPV_ALIGNED(SSE) uint8_t* dstPt1, COMPV_ALIGNED(SSE) uint8_t* dstPt2, COMPV_ALIGNED(SSE) uint8_t* dstPt3,
	COMPV_ALIGNED(SSE) const compv_uint8x4_t* srcPtr,
	compv_uscalar_t width, compv_uscalar_t height, COMPV_ALIGNED(SSE) compv_uscalar_t stride)
{
	COMPV_DEBUG_INFO_CHECK_SSSE3();
	__m128i vecLane0, vecLane1, vecLane2, vecLane3, vectmp0, vectmp1;

	for (compv_uscalar_t j = 0; j < height; ++j) {
		for (compv_uscalar_t i = 0; i < width; i += 16) { // strided/SSE-aligned -> can write beyond width
			COMPV_VLD4_U8_SSSE3(&srcPtr[i], vecLane0, vecLane1, vecLane2, vecLane3, vectmp0, vectmp1);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt0[i]), vecLane0);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt1[i]), vecLane1);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt2[i]), vecLane2);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt3[i]), vecLane3);
		}
		dstPt0 += stride;
		dstPt1 += stride;
		dstPt2 += stride;
		dstPt3 += stride;
		srcPtr += stride;
	}
}

void CompVMemUnpack3_Intrin_SSSE3(
	COMPV_ALIGNED(SSE) uint8_t* dstPt0, COMPV_ALIGNED(SSE) uint8_t* dstPt1, COMPV_ALIGNED(SSE) uint8_t* dstPt2,
	COMPV_ALIGNED(SSE) const compv_uint8x3_t* srcPtr,
	compv_uscalar_t width, compv_uscalar_t height, COMPV_ALIGNED(SSE) compv_uscalar_t stride)
{
    COMPV_DEBUG_INFO_CHECK_SSSE3();
	__m128i vecLane0, vecLane1, vecLane2, vectmp0, vectmp1;
    
	for (compv_uscalar_t j = 0; j < height; ++j) {
		for (compv_uscalar_t i = 0; i < width; i += 16) { // strided/SSE-aligned -> can write beyond width
			COMPV_VLD3_U8_SSSE3(&srcPtr[i], vecLane0, vecLane1, vecLane2, vectmp0, vectmp1);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt0[i]), vecLane0);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt1[i]), vecLane1);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt2[i]), vecLane2);
		}
		dstPt0 += stride;
		dstPt1 += stride;
		dstPt2 += stride;
		srcPtr += stride;
	}
}

// Same as CompVMemUnpack3_Intrin_SSSE3 but "srcPtr" to aligned
// Happens frequently when the image is from a region (plate, car...) with left not aligned on 16-bytes
void CompVMemUnpack3_SrcPtrNotAligned_Intrin_SSSE3(
	COMPV_ALIGNED(SSE) uint8_t* dstPt0, COMPV_ALIGNED(SSE) uint8_t* dstPt1, COMPV_ALIGNED(SSE) uint8_t* dstPt2,
	const compv_uint8x3_t* srcPtr,
	compv_uscalar_t width, compv_uscalar_t height, COMPV_ALIGNED(SSE) compv_uscalar_t stride)
{
	COMPV_DEBUG_INFO_CHECK_SSSE3();
	__m128i vecLane0, vecLane1, vecLane2, vectmp0, vectmp1;

	for (compv_uscalar_t j = 0; j < height; ++j) {
		for (compv_uscalar_t i = 0; i < width; i += 16) { // strided/SSE-aligned -> can write beyond width
			COMPV_VLD3_U8_UNALIGNED_SSSE3(&srcPtr[i], vecLane0, vecLane1, vecLane2, vectmp0, vectmp1);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt0[i]), vecLane0);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt1[i]), vecLane1);
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt2[i]), vecLane2);
		}
		dstPt0 += stride;
		dstPt1 += stride;
		dstPt2 += stride;
		srcPtr += stride;
	}
}

void CompVMemUnpack2_Intrin_SSSE3(
	COMPV_ALIGNED(SSE) uint8_t* dstPt0, COMPV_ALIGNED(SSE) uint8_t* dstPt1, COMPV_ALIGNED(SSE) const compv_uint8x2_t* srcPtr,
	compv_uscalar_t width, compv_uscalar_t height, COMPV_ALIGNED(SSE) compv_uscalar_t stride)
{
	COMPV_DEBUG_INFO_CHECK_SSSE3();
	const __m128i vecDeinterleaveUV = _mm_load_si128(reinterpret_cast<const __m128i*>(kShuffleEpi8_Deinterleave8uL2_32s));

	for (compv_uscalar_t j = 0; j < height; ++j) {
		for (compv_uscalar_t i = 0; i < width; i += 16) { // strided/SSE-aligned -> can write beyond width
			__m128i vec0 = _mm_load_si128(reinterpret_cast<const __m128i*>(&srcPtr[i])); // UVUVUVUVUVUVUVUV
			__m128i vec1 = _mm_load_si128(reinterpret_cast<const __m128i*>(&srcPtr[i + 8])); // UVUVUVUVUVUVUVUV
			vec0 = _mm_shuffle_epi8(vec0, vecDeinterleaveUV); // UUUUUUUUVVVVVVVV
			vec1 = _mm_shuffle_epi8(vec1, vecDeinterleaveUV); // UUUUUUUUVVVVVVVV
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt0[i]), _mm_unpacklo_epi64(vec0, vec1)); // UUUUUUUUUUUUUUUU
			_mm_store_si128(reinterpret_cast<__m128i*>(&dstPt1[i]), _mm_unpackhi_epi64(vec0, vec1)); // VVVVVVVVVVVVVVVV
		}
		dstPt0 += stride;
		dstPt1 += stride;
		srcPtr += stride;
	}
}

void CompVMemPack3_Intrin_SSSE3(
	COMPV_ALIGNED(SSE) compv_uint8x3_t* dstPtr,
	COMPV_ALIGNED(SSE) const uint8_t* srcPt0, COMPV_ALIGNED(SSE) const uint8_t* srcPt1, COMPV_ALIGNED(SSE) const uint8_t* srcPt2,
	compv_uscalar_t width, compv_uscalar_t height, COMPV_ALIGNED(SSE) compv_uscalar_t stride)
{
	COMPV_DEBUG_INFO_CHECK_SSSE3();
	__m128i vecLane0, vecLane1, vecLane2, vectmp0, vectmp1;
	for (compv_uscalar_t j = 0; j < height; ++j) {
		for (compv_uscalar_t i = 0; i < width; i += 16) { // strided/SSE-aligned -> can write beyond width
			vecLane0 = _mm_load_si128(reinterpret_cast<const __m128i*>(&srcPt0[i]));
			vecLane1 = _mm_load_si128(reinterpret_cast<const __m128i*>(&srcPt1[i]));
			vecLane2 = _mm_load_si128(reinterpret_cast<const __m128i*>(&srcPt2[i]));
			COMPV_VST3_U8_SSSE3(&dstPtr[i], vecLane0, vecLane1, vecLane2, vectmp0, vectmp1);
		}
		dstPtr += stride;
		srcPt0 += stride;
		srcPt1 += stride;
		srcPt2 += stride;
	}
}

COMPV_NAMESPACE_END()

#endif /* COMPV_ARCH_X86 && COMPV_INTRINSIC */
