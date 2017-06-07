/* Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#include "compv/core/video/compv_core_video_reader_ffmpeg.h"
#if defined(HAVE_FFMPEG)
#include "compv/base/image/compv_image.h"

COMPV_EXTERNC_BEGIN()
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
COMPV_EXTERNC_END()

COMPV_NAMESPACE_BEGIN()

#define COMPV_THIS_CLASSNAME	"CompVVideoReaderFFmpeg"

static int __open_codec_context(int *stream_idx, AVFormatContext *fmt_ctx, enum AVMediaType type, const char* src_filename);
static COMPV_ERROR_CODE __pixfmt_to_subtype(const AVPixelFormat pixfmt, COMPV_SUBTYPE& subtype);

//
//	CompVVideoReaderFactoryFFmpeg
//

static COMPV_ERROR_CODE CompVVideoReaderFactoryFFmpeg_newObj(CompVVideoReaderPtrPtr reader)
{
	COMPV_CHECK_EXP_RETURN(!reader, COMPV_ERROR_CODE_E_INVALID_PARAMETER);
	CompVVideoReaderFFmpegPtr readerFFmpeg_;
	COMPV_CHECK_CODE_RETURN(CompVVideoReaderFFmpeg::newObj(&readerFFmpeg_));
	*reader = *readerFFmpeg_;
	return COMPV_ERROR_CODE_S_OK;
}

const CompVVideoReaderFactory CompVVideoReaderFactoryFFmpeg = {
	"FFmpeg Video Reader factory",
	CompVVideoReaderFactoryFFmpeg_newObj
};

//
//	CompVVideoReaderFFmpeg
//

CompVVideoReaderFFmpeg::CompVVideoReaderFFmpeg()
	: CompVVideoReader()
	, m_bOpened(false)
	, m_pFmtCtx(nullptr)
	, m_nStreamIdx(-1)
	, m_pStream(nullptr)
	, m_pDecCtx(nullptr)
	, m_pFrame(nullptr)
	, m_ePixFmt(AV_PIX_FMT_NONE)
{

}

CompVVideoReaderFFmpeg::~CompVVideoReaderFFmpeg()
{
	COMPV_CHECK_CODE_NOP(close());
}


COMPV_ERROR_CODE CompVVideoReaderFFmpeg::open(const char* path)
{
	COMPV_CHECK_EXP_RETURN(!path || *path == '\0', COMPV_ERROR_CODE_E_INVALID_PARAMETER);
	if (!m_bOpened) {
		int avret;
		// Open input file
		if ((avret = avformat_open_input(&m_pFmtCtx, path, NULL, NULL)) < 0) {
			COMPV_DEBUG_ERROR_EX(COMPV_THIS_CLASSNAME, "avformat_open_input returned %d", avret);
			return COMPV_ERROR_CODE_E_FFMPEG;
		}
		// Dump input information to stderr
		av_dump_format(m_pFmtCtx, 0, path, 0);
		// Retrieve stream information */
		if ((avret = avformat_find_stream_info(m_pFmtCtx, NULL)) < 0) {
			COMPV_DEBUG_ERROR_EX(COMPV_THIS_CLASSNAME, "avformat_find_stream_info returned %d", avret);
			return COMPV_ERROR_CODE_E_FFMPEG;
		}
		// Retrive the codec context
		if ((avret = __open_codec_context(&m_nStreamIdx, m_pFmtCtx, AVMEDIA_TYPE_VIDEO, path)) < 0) {
			COMPV_DEBUG_ERROR_EX(COMPV_THIS_CLASSNAME, "open_codec_context(AVMEDIA_TYPE_VIDEO) returned %d", avret);
			return COMPV_ERROR_CODE_E_FFMPEG;
		}
		m_pStream = m_pFmtCtx->streams[m_nStreamIdx];
		m_pDecCtx = m_pStream->codec;

		m_bOpened = true;
		return COMPV_ERROR_CODE_S_OK;
	}
	return COMPV_ERROR_CODE_S_OK;
}

bool CompVVideoReaderFFmpeg::isOpen()const
{
	return m_bOpened;
}

COMPV_ERROR_CODE CompVVideoReaderFFmpeg::close()
{
	if (m_pDecCtx) {
		avcodec_close(m_pDecCtx);
		m_pDecCtx = nullptr;
	}
	if (m_pFmtCtx) {
		avformat_close_input(&m_pFmtCtx);
		m_pFmtCtx = nullptr;
	}
	if (m_pFrame) {
		av_frame_free(&m_pFrame);
		m_pFrame = nullptr;
	}
	m_ePixFmt = AV_PIX_FMT_NONE;
	m_bOpened = false;
	return COMPV_ERROR_CODE_S_OK;
}

COMPV_ERROR_CODE CompVVideoReaderFFmpeg::read(CompVMatPtrPtr frame)
{
	COMPV_CHECK_EXP_RETURN(!frame, COMPV_ERROR_CODE_E_INVALID_PARAMETER);
	COMPV_CHECK_EXP_RETURN(!isOpen(),  COMPV_ERROR_CODE_E_INVALID_STATE, "reader not opened");
	CompVMatPtr ptrFrame = *frame ? *frame : nullptr;
	*frame = nullptr;
	int avret, got_frame = 0;
	AVPacket pkt;
	av_init_packet(&pkt);

	// Alloc frame
	if (!m_pFrame) {
		m_pFrame = av_frame_alloc();
		COMPV_CHECK_EXP_RETURN(!m_pFrame, COMPV_ERROR_CODE_E_OUT_OF_MEMORY);
	}

	// Read frame
	pkt.data = NULL;
	pkt.size = 0;
	pkt.stream_index = m_nStreamIdx;
	do {
		if ((avret = av_read_frame(m_pFmtCtx, &pkt)) < 0) {
			COMPV_DEBUG_ERROR_EX(COMPV_THIS_CLASSNAME, "av_read_frame returned %d", avret);
			return COMPV_ERROR_CODE_E_FFMPEG;
		}
	} while ((pkt.stream_index != m_nStreamIdx));

	// Decode frame
	while ((avret = avcodec_decode_video2(m_pDecCtx, m_pFrame, &got_frame, &pkt)) >= 0 && !got_frame);
	if (avret < 0) {
		COMPV_DEBUG_ERROR_EX(COMPV_THIS_CLASSNAME, "avcodec_decode_video2 returned %d", avret);
		return COMPV_ERROR_CODE_E_FFMPEG;
	}
	if (got_frame) {
		if (m_ePixFmt != m_pDecCtx->pix_fmt) {
			COMPV_CHECK_CODE_RETURN(__pixfmt_to_subtype(m_pDecCtx->pix_fmt, m_eSubType));
			m_ePixFmt = m_pDecCtx->pix_fmt;
		}
		COMPV_CHECK_CODE_RETURN(CompVImage::newObj8u(&ptrFrame, m_eSubType, m_pFrame->width, m_pFrame->height));
		// Copy the image
		uint8_t *dst_data[4];
		int dst_linesizes[4];
		for (size_t p = 0; p < ptrFrame->planeCount(); ++p) {
			dst_data[p] = ptrFrame->ptr<uint8_t>(0, 0, static_cast<int>(p));
			dst_linesizes[p] = static_cast<int>(ptrFrame->stride(static_cast<int>(p)));
		}
		av_image_copy(dst_data, dst_linesizes,
				(const uint8_t **)(m_pFrame->data), m_pFrame->linesize,
			m_pDecCtx->pix_fmt, m_pDecCtx->width, m_pDecCtx->height);
		*frame = ptrFrame;
		return COMPV_ERROR_CODE_S_OK;
	}
	else {
		COMPV_DEBUG_INFO_EX(COMPV_THIS_CLASSNAME, "End-Of-File");
		return COMPV_ERROR_CODE_E_END_OF_FILE;
	}
}

COMPV_ERROR_CODE CompVVideoReaderFFmpeg::newObj(CompVVideoReaderFFmpegPtrPtr reader)
{
	COMPV_CHECK_EXP_RETURN(!reader, COMPV_ERROR_CODE_E_INVALID_PARAMETER);
	CompVVideoReaderFFmpegPtr reader_ = new CompVVideoReaderFFmpeg();
	COMPV_CHECK_EXP_RETURN(!reader_, COMPV_ERROR_CODE_E_OUT_OF_MEMORY);

	*reader = reader_;
	return COMPV_ERROR_CODE_S_OK;
}

static int __open_codec_context(int *stream_idx, AVFormatContext *fmt_ctx, enum AVMediaType type, const char* src_filename)
{
	int ret;
	AVStream *st;
	AVCodecContext *dec_ctx = NULL;
	AVCodec *dec = NULL;

	ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
	if (ret < 0) {
		COMPV_DEBUG_ERROR_EX(COMPV_THIS_CLASSNAME, "Could not find %s stream in input file '%s'\n",
			av_get_media_type_string(type),
			src_filename);
		return ret;
	}
	else {
		*stream_idx = ret;
		st = fmt_ctx->streams[*stream_idx];

		/* find decoder for the stream */
		dec_ctx = st->codec;
		dec = avcodec_find_decoder(dec_ctx->codec_id);
		if (!dec) {
			COMPV_DEBUG_ERROR_EX(COMPV_THIS_CLASSNAME, "Failed to find %s codec",
				av_get_media_type_string(type));
			return ret;
		}

		if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0) {
			COMPV_DEBUG_ERROR_EX(COMPV_THIS_CLASSNAME, "Failed to open %s codec",
				av_get_media_type_string(type));
			return ret;
		}
	}
	return 0;
}

static COMPV_ERROR_CODE __pixfmt_to_subtype(const AVPixelFormat pixfmt, COMPV_SUBTYPE& subtype)
{
	switch (pixfmt){
	case AV_PIX_FMT_YUV420P: subtype = COMPV_SUBTYPE_PIXELS_YUV420P; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_NV12: subtype = COMPV_SUBTYPE_PIXELS_NV12; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_NV21: subtype = COMPV_SUBTYPE_PIXELS_NV21; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_BGR24: subtype = COMPV_SUBTYPE_PIXELS_BGR24; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_BGRA: subtype = COMPV_SUBTYPE_PIXELS_BGRA32; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_BGR565LE: subtype = COMPV_SUBTYPE_PIXELS_BGR565LE; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_BGR565BE: subtype = COMPV_SUBTYPE_PIXELS_BGR565BE; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_RGB24: subtype = COMPV_SUBTYPE_PIXELS_RGB24; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_ARGB: subtype = COMPV_SUBTYPE_PIXELS_ARGB32; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_RGBA: subtype = COMPV_SUBTYPE_PIXELS_RGBA32; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_RGB565LE: subtype = COMPV_SUBTYPE_PIXELS_RGB565LE; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_RGB565BE: subtype = COMPV_SUBTYPE_PIXELS_RGB565BE; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_GRAY8: subtype = COMPV_SUBTYPE_PIXELS_Y; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_YUV444P: subtype = COMPV_SUBTYPE_PIXELS_YUV444P; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_YUYV422: subtype = COMPV_SUBTYPE_PIXELS_YUYV422; return COMPV_ERROR_CODE_S_OK;
	case AV_PIX_FMT_UYVY422: subtype = COMPV_SUBTYPE_PIXELS_UYVY422; return COMPV_ERROR_CODE_S_OK;
	default:
		COMPV_DEBUG_ERROR_EX(COMPV_THIS_CLASSNAME, "%d not supported as raw video format", static_cast<int>(pixfmt));
		return COMPV_ERROR_CODE_E_INVALID_IMAGE_FORMAT;
	}
}

COMPV_NAMESPACE_END()

#endif /* HAVE_FFMPEG */
