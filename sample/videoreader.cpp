#include <opencv2/highgui/highgui_c.h>
#include "ffmpeg_fas.h"
#include "videoreader.h"

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
        int currFrameIndex = currentFrameIndex();
        if (currFrameIndex != frameIndex) {
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
    return grab() && retrieve(image);
}

bool VideoReader::read(cv::Mat& image, int frameIndex)
{
    return seek(frameIndex) && retrieve(image);
}

int VideoReader::frameCount() const
{
    if (isOpened()) {
        return fas_get_frame_count(m_ctx);
    }
    return 0;
}

bool VideoReader::grab()
{
    if (m_ctx) {
        fas_error_type err = fas_step_forward(m_ctx);
        return err == FAS_SUCCESS;
    }
    return false;
}

bool VideoReader::retrieve(cv::Mat& image)
{
    fas_raw_image_type fas_image;
    fas_error_type err = fas_get_frame(m_ctx, &fas_image);
    if (err != FAS_SUCCESS) {
        return false;
    }
    image = cv::Mat(fas_image.height, fas_image.width, CV_8UC3, fas_image.data, fas_image.bytes_per_line).clone();
    fas_free_frame(fas_image);
    return true;
}

int VideoReader::currentFrameIndex() const
{
    if (m_ctx){
        return fas_get_frame_index(m_ctx);
    }
    return -1;
}

double VideoReader::get(int propId)
{
    if (m_ctx) {
        switch (propId)
        {
        case CV_CAP_PROP_POS_FRAMES:
            return (double)currentFrameIndex();
        case CV_CAP_PROP_FRAME_COUNT:
            return (double)frameCount();
        case CV_CAP_PROP_FPS:
            return fps();
        default:
            return 0.0;
        }
    }
    return 0.0;
}

double VideoReader::fps() const
{
    if (m_ctx) {
        int num, den;
        fas_get_fps(m_ctx, &num, &den);
        return double(num) / den;
    }
    return 0.0;
}

