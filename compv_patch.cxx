/* Copyright (C) 2016 Doubango Telecom <https://www.doubango.org>
*
* This file is part of Open Source ComputerVision (a.k.a CompV) project.
* Source code hosted at https://github.com/DoubangoTelecom/compv
* Website hosted at http://compv.org
*
* CompV is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* CompV is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with CompV.
*/

#include "compv/compv_patch.h"
#include "compv/compv_mem.h"
#include "compv/compv_math_utils.h"

COMPV_NAMESPACE_BEGIN()

static void Moments0110_C(const uint8_t* top, const uint8_t* bottom, const int16_t* x, const int16_t* y, compv_scalar_t count, compv_scalar_t* s10, compv_scalar_t* s01);

CompVPatch::CompVPatch()
: m_pMaxAbscissas(NULL)
, m_pX(NULL)
, m_pY(NULL)
, m_pTop(NULL)
, m_pBottom(NULL)
{
	
}

CompVPatch::~CompVPatch()
{
	CompVMem::free((void**)&m_pMaxAbscissas);
	CompVMem::free((void**)&m_pX);
	CompVMem::free((void**)&m_pY);
	CompVMem::free((void**)&m_pTop);
	CompVMem::free((void**)&m_pBottom);
}

void CompVPatch::moments0110(const uint8_t* ptr, int center_x, int center_y, int img_width, int img_stride, int img_height, int* m01, int* m10)
{
	COMPV_DEBUG_INFO_CODE_NOT_OPTIMIZED(); // TODO(dmi): SIMD
	int s01 = 0, s10 = 0, i, j;
	bool closeToBorder = (center_x < m_nRadius || (center_x + m_nRadius) >= img_width || (center_y < m_nRadius) || (center_y + m_nRadius) >= img_height);

	if (closeToBorder) {
		const uint8_t* img_ptr;
		int img_y, i, j, minI, maxI, minJ, maxJ, dX;
		COMPV_DEBUG_INFO_CODE_FOR_TESTING(); // Code never called be used for interop. against OpenCV
		COMPV_DEBUG_INFO_CODE_NOT_OPTIMIZED(); // It's useless to compute moments for these points because it won't be possible to have a description (partial circle)
		// This code must never be called as we remove elements close to the border before computing the moments
		// Compute minJ and maxJ
		minJ = -m_nRadius;
		if ((center_y + minJ) < 0) {
			minJ = -center_y;
		}
		maxJ = +m_nRadius;
		if ((center_y + maxJ) >= img_height) {
			maxJ = (img_height - center_y - 1);
			maxJ = COMPV_MATH_CLIP3(minJ, m_nRadius, maxJ);
		}

		for (j = minJ, img_y = (center_y + j); j <= maxJ; ++j, ++img_y) {
			// Pythagorean theorem: x = sqrt(r**2 - y**2)
			// dX = ((int)sqrt(patch_radius_pow2 - (j * j)));
			dX = m_pMaxAbscissas[j < 0 ? -j : +j];

			// Compute minI and maxI
			minI = -dX;
			if ((center_x + minI) < 0) {
				minI = -center_x;
			}
			maxI = +dX;
			if ((center_x + maxI) >= img_width) {
				maxI = (img_width - center_x - 1);
				maxI = COMPV_MATH_CLIP3(minI, dX, maxI);
			}

			img_ptr = &ptr[(img_y * img_stride) + (center_x + minI)];
			for (i = minI; i <= maxI; ++i, ++img_ptr) {
				s10 += (i **img_ptr); // i^p * j^q * I(x, y) = i^1 * j^0 * I(x, y) = i * I(x, y)
				s01 += j **img_ptr; // i^p * j^q * I(x, y) = i^0 * j^1 * I(x, y) = j * I(x, y)
			}
		}
	}
	else {
		const uint8_t *img_center, *img_top, *img_bottom, *t, *b;
		uint8_t top, bottom;
		const int16_t *dX;
		int sj;

		img_center = &ptr[(center_y * img_stride) + center_x];

		/* Handle 'j==0' case */
		{
			for (i = -m_nRadius; i <= +m_nRadius; ++i) {
				s10 += (i * img_center[i]);
			}
		}
		/* Handle 'j==patch_radius' case */
		{
			top = *(img_center + (img_stride * m_nRadius));
			bottom = *(img_center - (img_stride * m_nRadius));
			s01 += (m_nRadius * top) - (m_nRadius * bottom);
		}

		img_top = img_center + img_stride;
		img_bottom = img_center - img_stride;

		// Handle j=1... cases
		for (j = 1, dX = &m_pMaxAbscissas[j]; j < m_nRadius; ++j, ++dX) {
			sj = 0;
			for (i = -*dX, t = &img_top[i], b = &img_bottom[i]; i <= *dX; ++i, ++t, ++b) {
				s10 += i * (*t + *b); // (i * t) + (i * b)
				sj += (*t - *b);
			}
			s01 += j * sj; // for SIMD: move inside the loop and s01 = (j * t) - (j * b) = j * (t - b)
			img_top += img_stride;
			img_bottom -= img_stride;
		}
	}
	*m01 = s01;
	*m10 = s10;
}
	
COMPV_ERROR_CODE CompVPatch::newObj(CompVPtr<CompVPatch* >* patch, int diameter)
{
	COMPV_CHECK_EXP_RETURN(!patch || diameter <= 1, COMPV_ERROR_CODE_E_INVALID_PARAMETER);
	COMPV_ERROR_CODE err_ = COMPV_ERROR_CODE_S_OK;
	int radius_ = diameter >> 1, radius2_ = radius_ * radius_;
	int i, j, k;
	const int16_t* dX;
	int16_t* pMaxAbscissas_ = NULL, *pX_ = NULL, *pY_ = NULL;
	uint8_t *pTop_ = NULL, *p_Bottom_ = NULL;
	size_t count_ = 0, stride_;
	CompVPtr<CompVPatch* > patch_;

	pMaxAbscissas_ = (int16_t*)CompVMem::malloc((radius_ + 1) * sizeof(int16_t));
	COMPV_CHECK_EXP_BAIL(!pMaxAbscissas_, (err_ = COMPV_ERROR_CODE_E_OUT_OF_MEMORY));
	for (int i = 0; i <= radius_; ++i) {
		// Pythagorean theorem: x = sqrt(r**2 - y**2)
		pMaxAbscissas_[i] = ((int16_t)sqrt(radius2_ - (i * i)));
	}

	// Count
	for (j = 1, dX = &pMaxAbscissas_[j]; j < radius_; ++j, ++dX) {
		for (i = -*dX; i <= *dX; ++i) {
			++count_;
		}
	}
	stride_ = CompVMem::alignForward(count_, COMPV_SIMD_ALIGNV_DEFAULT);

	pX_ = (int16_t*)CompVMem::calloc(stride_, sizeof(int16_t));
	COMPV_CHECK_EXP_BAIL(!pX_, (err_ = COMPV_ERROR_CODE_E_OUT_OF_MEMORY));
	pY_ = (int16_t*)CompVMem::calloc(stride_, sizeof(int16_t));
	COMPV_CHECK_EXP_BAIL(!pY_, (err_ = COMPV_ERROR_CODE_E_OUT_OF_MEMORY));
	pTop_ = (uint8_t*)CompVMem::calloc(stride_, sizeof(uint8_t));
	COMPV_CHECK_EXP_BAIL(!pTop_, (err_ = COMPV_ERROR_CODE_E_OUT_OF_MEMORY));
	p_Bottom_ = (uint8_t*)CompVMem::calloc(stride_, sizeof(uint8_t));
	COMPV_CHECK_EXP_BAIL(!p_Bottom_, (err_ = COMPV_ERROR_CODE_E_OUT_OF_MEMORY));

	// Set X and Y values
	k = 0;
	for (j = 1, dX = &pMaxAbscissas_[j]; j < radius_; ++j, ++dX) {
		for (i = -*dX; i <= *dX; ++i) {
			pX_[k] = i;
			pY_[k] = j;
			++k;
		}
	}

	patch_ = new CompVPatch();
	COMPV_CHECK_EXP_BAIL(!patch_, (err_ = COMPV_ERROR_CODE_E_OUT_OF_MEMORY));
	patch_->m_nRadius = radius_;
	patch_->m_pMaxAbscissas = pMaxAbscissas_;
	patch_->m_pX = pX_;
	patch_->m_pY = pY_;
	patch_->m_pTop = pTop_;
	patch_->m_pBottom = p_Bottom_;
	patch_->m_nCount = count_;
	patch_->m_nStride = stride_;

	*patch = patch_;

bail:
	if (COMPV_ERROR_CODE_IS_NOK(err_)) {
		CompVMem::free((void**)&pMaxAbscissas_);
		CompVMem::free((void**)&pX_);
		CompVMem::free((void**)&pY_);
		CompVMem::free((void**)&pTop_);
		CompVMem::free((void**)&p_Bottom_);
	}
	return err_;
}

// top, bottom, x, y are allocated with padding which means you can read up to align_fwd(count, alignv)
static void Moments0110_C(const uint8_t* top, const uint8_t* bottom, const int16_t* x, const int16_t* y, compv_scalar_t count, compv_scalar_t* s10, compv_scalar_t* s01)
{
	COMPV_DEBUG_INFO_CODE_NOT_OPTIMIZED();

	compv_scalar_t s10_ = 0;
	compv_scalar_t s01_ = 0;

	for (compv_scalar_t i = 0; i < count; ++i, ++top, ++bottom, ++x, ++y) {
		s10_ += *x * (*top + *bottom);
		s01_ += *y * (*top - *bottom);
	}
	*s10 = s10_;
	*s01 = s01_;
}

COMPV_NAMESPACE_END()
