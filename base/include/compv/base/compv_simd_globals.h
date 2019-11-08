/* Copyright (C) 2016-2019 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#if !defined(_COMPV_BASE_SIMD_GLOBALS_H_)
#define _COMPV_BASE_SIMD_GLOBALS_H_

#include "compv/base/compv_config.h"
#include "compv/base/compv_common.h"

#if defined(_COMPV_API_H_)
#error("This is a private file and must not be part of the API")
#endif


COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint8_t k0_8u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint8_t k_0_0_0_255_8u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int8_t k1_8s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int8_t k2_8s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int8_t k3_8s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int8_t k5_8s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int8_t k15_8s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int8_t k16_8s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int8_t k85_8s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int8_t k127_8s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint8_t k128_8u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint8_t k171_8u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint8_t k254_8u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint8_t k255_8u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k13_26_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k16_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k37_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k51_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k65_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k127_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k128_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k255_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k7120_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k8912_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int16_t k4400_16s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t k_0_2_4_6_0_2_4_6_32s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t kAtan2Eps_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t kAtan2P1_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t kAtan2P3_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t kAtan2P5_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t kAtan2P7_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t k43_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t k90_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t k180_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t k255_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float32_t k360_32f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float64_t ksqrt2_64f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float64_t k1_64f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float64_t km1_64f[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() COMPV_NAMESPACE::compv_float64_t km1_0_64f[];

COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_Popcnt_32s[];

COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_Deinterleave8uL2_32s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_Deinterleave16uL2_32s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_Deinterleave8uL3_32s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_Interleave8uL3_Step0_s32[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_Interleave8uL3_Step1_s32[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_Interleave8uL3_Step2_s32[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kShuffleEpi8_Deinterleave8uL4_32s[];

#if COMPV_ARCH_X86

COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint64_t kAVXMaskstore_0_64u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint64_t kAVXMaskstore_0_1_64u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint32_t kAVXMaskstore_0_32u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint64_t kAVXMaskzero_3_64u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint64_t kAVXMaskzero_2_3_64u[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint64_t kAVXMaskzero_1_2_3_64u[];

COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kAVXPermutevar8x32_AEBFCGDH_32s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kAVXPermutevar8x32_ABCDDEFG_32s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kAVXPermutevar8x32_CDEFFGHX_32s[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() int32_t kAVXPermutevar8x32_XXABBCDE_32s[];

COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint32_t kAVXFloat64MaskAbs[];
COMPV_EXTERNC COMPV_GCC_VISIBILITY_HIDDEN COMPV_BASE_API COMPV_ALIGN_DEFAULT() uint32_t kAVXFloat64MaskNegate[];

#endif /* COMPV_ARCH_X86 */

#endif /* _COMPV_BASE_SIMD_GLOBALS_H_ */
