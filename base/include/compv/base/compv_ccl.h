/* Copyright (C) 2016-2018 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#if !defined(_COMPV_BASE_CCL_H_)
#define _COMPV_BASE_CCL_H_

#include "compv/base/compv_config.h"
#include "compv/base/compv_common.h"
#include "compv/base/compv_obj.h"
#include "compv/base/compv_caps.h"
#include "compv/base/compv_mat.h"

#include <map>
#include <vector>
#include <functional>

COMPV_NAMESPACE_BEGIN()

typedef CompVPoint2DInt16Vector CompVConnectedComponentPoints;
typedef std::vector<CompVConnectedComponentPoints > CompVConnectedComponentPointsVector;

typedef CompVRectInt16 CompVConnectedComponentBoundingBox;
typedef CompVRectInt16Vector CompVConnectedComponentBoundingBoxesVector;

typedef std::function<bool(const int32_t id)> CompVConnectedComponentCallbackRemoveLabel;

typedef double CompVConnectedComponentMoments[6]; // sum(1), sum(x), sum(y), sum(x*x), sum(x*y), sum(y*y)

COMPV_OBJECT_DECLARE_PTRS(ConnectedComponentLabeling)
COMPV_OBJECT_DECLARE_PTRS(ConnectedComponentLabelingResult)
COMPV_OBJECT_DECLARE_PTRS(ConnectedComponentLabelingResultLSL)

struct CompVConnectedComponentLabelingFactory {
	int id;
	const char* name;
	COMPV_ERROR_CODE(*newObj)(CompVConnectedComponentLabelingPtrPtr ccl);
};

enum COMPV_CCL_EXTRACT_TYPE {
	COMPV_CCL_EXTRACT_TYPE_SEGMENT, // external contour only (TODO(dmi): contour extraction using PLSL not correct -> use canny followed by blob extraction)
	COMPV_CCL_EXTRACT_TYPE_BLOB // everything
};

/* Connected component labeling setters and getters */
enum {
	/* Common to all features */

	
	/* Parallel Light Speed Labeling */
	COMPV_PLSL_ID, // Binary image
	COMPV_PLSL_SET_INT_TYPE,
	COMPV_PLSL_TYPE_STD,
	COMPV_PLSL_TYPE_STDZ,
	COMPV_PLSL_TYPE_RLC,
	COMPV_PLSL_TYPE_XRLC,
	COMPV_PLSL_TYPE_RLE,
	COMPV_PLSL_TYPE_XRLE,
	COMPV_PLSL_TYPE_XRLEZ,

	/* Contour Tracing approach by Fu Chang et al */
	COMPV_CT_ID, // Binary image

	/* By Wan-Yu Chang et al.  */
	COMPV_CCIT_ID, // Binary image

	/* By Di Stefano */
	COMPV_DISTEFANO_ID, // Binary image

	/* Block Based with Decision Trees algorithm by Grana et al. */
	COMPV_BBDT_ID, // Binary image

	/* the Scan Array Union Find algorithm by Wu et al. */
	COMPV_SAUF_ID, // Binary image

	/* Configuration-Transition-Based algorithm by He et al. */
	COMPV_CTB_ID, // Binary image

	/* The stripe-based algorithm by Zhao et al. */
	COMPV_SBLA_ID, // Binary image

	/* Optimized Pixel Prediction by Grana et al. */
	COMPV_PRED_ID, // Binary image

	/* Linear Time Maximally Stable Extremal Regions */
	COMPV_LMSER_ID, // Grayscale image
};

// Class: CompVConnectedComponentLabelingResult
class COMPV_BASE_API CompVConnectedComponentLabelingResult : public CompVObj
{
protected:
	CompVConnectedComponentLabelingResult(int id) : m_nId(id) {
	}
public:
	virtual ~CompVConnectedComponentLabelingResult() {
	}
	COMPV_INLINE int id() const {
		return m_nId;
	}
	virtual int32_t backgroundLabelId() const { return 0; }
	virtual size_t labelsCount() const = 0;
	virtual COMPV_ERROR_CODE debugFlatten(CompVMatPtrPtr ptr32sLabels) const {
		COMPV_DEBUG_INFO_CODE_NOT_OPTIMIZED("This function is for visual debugging only. It created an image with the list of the labels");
		COMPV_CHECK_CODE_RETURN(COMPV_ERROR_CODE_E_NOT_IMPLEMENTED);
		return COMPV_ERROR_CODE_S_OK;
	}
	virtual COMPV_ERROR_CODE extract(CompVConnectedComponentPointsVector& points, COMPV_CCL_EXTRACT_TYPE type = COMPV_CCL_EXTRACT_TYPE_BLOB) const {
		COMPV_DEBUG_INFO_CODE_NOT_OPTIMIZED("If you just want to compute the label features (e.g. centroid, bounding boxes, first order moment...) then, you don't need to extract the points");
		COMPV_CHECK_CODE_RETURN(COMPV_ERROR_CODE_E_NOT_IMPLEMENTED);
		return COMPV_ERROR_CODE_S_OK;
	}
private:
	int m_nId;
};

// Class: CompVConnectedComponentLabelingResultLSL
class COMPV_BASE_API CompVConnectedComponentLabelingResultLSL : public CompVConnectedComponentLabelingResult
{
protected:
	CompVConnectedComponentLabelingResultLSL()
		: CompVConnectedComponentLabelingResult(COMPV_PLSL_ID) {
	}
public:
	virtual ~CompVConnectedComponentLabelingResultLSL() {
	}
	
	virtual const std::vector<int32_t>& labelIds() const = 0;
	
	virtual COMPV_ERROR_CODE boundingBoxes(CompVConnectedComponentBoundingBoxesVector& boxes) const = 0;
	virtual COMPV_ERROR_CODE boundingBoxes(const CompVConnectedComponentPointsVector& segments, CompVConnectedComponentBoundingBoxesVector& boxes) const = 0;
	virtual COMPV_ERROR_CODE COMPV_DEPRECATED(remove)(CompVConnectedComponentCallbackRemoveLabel funcPtr, size_t &removedCount) = 0;
};

// Class: CompVConnectedComponentLabelingResultLMSER
class COMPV_BASE_API CompVConnectedComponentLabelingResultLMSER : public CompVConnectedComponentLabelingResult
{
protected:
	CompVConnectedComponentLabelingResultLMSER()
		: CompVConnectedComponentLabelingResult(COMPV_LMSER_ID) {
	}
public:
	virtual ~CompVConnectedComponentLabelingResultLMSER() {
	}
	virtual COMPV_ERROR_CODE boundingBoxes(CompVConnectedComponentBoundingBoxesVector& boxes) const = 0;
	virtual COMPV_ERROR_CODE moments(CompVConnectedComponentMoments& moments) const = 0;
};

// Class: CompVConnectedComponentLabeling
class COMPV_BASE_API CompVConnectedComponentLabeling : public CompVObj, public CompVCaps
{
protected:
	CompVConnectedComponentLabeling(int id);
public:
	virtual ~CompVConnectedComponentLabeling();
	COMPV_INLINE int id() const {
		return m_nId;
	}

	virtual COMPV_ERROR_CODE process(const CompVMatPtr& binar, CompVConnectedComponentLabelingResultPtrPtr result) = 0;
	template<typename T>
	static const T* reinterpret_castr(const CompVConnectedComponentLabelingResultPtr& result) {
		if (result) {
			if (result->id() == COMPV_PLSL_ID && std::is_same<T, CompVConnectedComponentLabelingResultLSL>::value) {
				return reinterpret_cast<const T*>(*result);
			}
			if (result->id() == COMPV_LMSER_ID && std::is_same<T, CompVConnectedComponentLabelingResultLMSER>::value) {
				return reinterpret_cast<const T*>(*result);
			}
		}
		return nullptr;
	}

	static COMPV_ERROR_CODE newObj(CompVConnectedComponentLabelingPtrPtr ccl, int id);
	static COMPV_ERROR_CODE addFactory(const CompVConnectedComponentLabelingFactory* factory);
	static const CompVConnectedComponentLabelingFactory* findFactory(int id);

private:
	COMPV_VS_DISABLE_WARNINGS_BEGIN(4251 4267)
	static std::map<int, const CompVConnectedComponentLabelingFactory*> s_Factories;
	int m_nId;
	COMPV_VS_DISABLE_WARNINGS_END()
};

COMPV_NAMESPACE_END()

#endif /* _COMPV_BASE_CCL_H_ */
