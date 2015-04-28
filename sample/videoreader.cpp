#include "videoreader.h"
#include "ffmpeg_fas.h"

struct FasInitializer
{
    FasInitializer() {
#ifdef _DEBUG
        fas_initialize(FAS_TRUE, FAS_BGR24);
#else
        fas_initialize(FAS_FALSE, FAS_BGR24);
#endif
    }
};

static FasInitializer fasInitializer;

VideoReader::VideoReader() : m_ctx(0)
{

}

VideoReader::VideoReader(const char* filename) : m_ctx(0)
{
    open(filename);
}

VideoReader::~VideoReader()
{
    release();
}

void VideoReader::release()
{
    if (m_ctx) {
        fas_close_video(m_ctx);
    }
}


bool VideoReader::open(const char* filename)
{
    fas_error_type err = fas_open_video(&m_ctx, filename);
    return err == FAS_SUCCESS;
}

bool VideoReader::isOpened() const
{
    return m_ctx != 0;
}

bool VideoReader::seek(int frameIndex)
{
    if (m_ctx) {
        int currentFrameIndex = fas_get_frame_index(m_ctx);
        if (currentFrameIndex != frameIndex) {
            fas_error_type err = fas_seek_to_frame(m_ctx, frameIndex);
            if (err != FAS_SUCCESS) {
                return false;
            }
        }
        return true;
    }
    return false;
    
}

bool VideoReader::read(cv::Mat& image)
{
    fas_raw_image_type fas_image;
    fas_error_type err = fas_get_frame(m_ctx, &fas_image);
    if (err != FAS_SUCCESS) {
        return false;
    }
    image = cv::Mat(fas_image.height, fas_image.width, CV_8UC3, fas_image.data, fas_image.bytes_per_line).clone();
    return true;
}

bool VideoReader::read(cv::Mat& image, int frameIndex)
{
    return seek(frameIndex) && read(image);
}

int VideoReader::frameCount() const
{
    if (isOpened()) {
        return fas_get_frame_count(m_ctx);
    }
    return 0;
}

