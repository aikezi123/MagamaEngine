#pragma once
#include "IImageProcessor.h" // 包含内城的契约头文件 (依赖倒置的体现)

namespace Magma::Infrastructure {

    // 继承并实现契约
    class OpenCVImageProcessor : public Magma::Application::IImageProcessor {
    public:
        cv::Mat removeBackgroundColor(const cv::Mat& input, const cv::Scalar& bgrColor, int tolerance = 10) override;
    };

}