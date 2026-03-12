#pragma once
#include <opencv2/core.hpp> // 极度克制：只引入基础货币 cv::Mat 和 cv::Scalar

namespace Magma::Application {

    // 图像处理器接口契约
    class IImageProcessor {
    public:
        virtual ~IImageProcessor() = default;

        /**
         * @brief 移除图像中指定的纯色背景（支持容差），转换为透明 PNG
         * @param input 源图像 (BGR 格式)
         * @param bgrColor 要抠除的背景颜色 (如纯绿：0, 255, 0)
         * @param tolerance 容差值 (0-255)。纯像素画设为 0~5。
         * @return 带有 Alpha 透明通道的图像 (BGRA 格式)
         */
        virtual cv::Mat removeBackgroundColor(const cv::Mat& input, const cv::Scalar& bgrColor, int tolerance = 10) = 0;
    };

}