#ifndef VIDEOREADER_H
#define VIDEOREADER_H
#pragma once

#include <opencv2/core/core.hpp>

struct fas_context_struct;
typedef struct fas_context_struct* fas_context_ref_type;

class VideoReader {
public:
    VideoReader();
    VideoReader(const char* filename);
    ~VideoReader();

    bool open(const char* filename);

    bool isOpened() const;

    bool seek(int frameIndex);

    bool read(cv::Mat& image);

    bool read(cv::Mat& image, int frameIndex);

    void release();

    int frameCount() const;
private:
    fas_context_ref_type m_ctx;
};


#endif 
