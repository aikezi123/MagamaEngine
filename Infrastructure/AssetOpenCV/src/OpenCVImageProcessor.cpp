#include "OpenCVImageProcessor.h"
#include <opencv2/imgproc.hpp> // 解锁重型图像处理算法
#include <vector>
#include <algorithm>

namespace Magma::Infrastructure {

    cv::Mat OpenCVImageProcessor::removeBackgroundColor(const cv::Mat& input, const cv::Scalar& bgrColor, int tolerance) {
        // 安全检查：如果图片是空的，直接原样打回
        if (input.empty()) return cv::Mat();

        // 1. 动态计算容差上下限，并防止数据越界 (0~255)
        cv::Scalar lowerBound(
            std::max(0.0, bgrColor[0] - tolerance),
            std::max(0.0, bgrColor[1] - tolerance),
            std::max(0.0, bgrColor[2] - tolerance)
        );
        cv::Scalar upperBound(
            std::min(255.0, bgrColor[0] + tolerance),
            std::min(255.0, bgrColor[1] + tolerance),
            std::min(255.0, bgrColor[2] + tolerance)
        );

        // 2. 锁定背景：在这个颜色范围内的像素，面具(Mask)上会被涂成白色(255)
        cv::Mat backgroundMask;
        cv::inRange(input, lowerBound, upperBound, backgroundMask);

        // 3. 反转面具：我们要保留的是“非背景”的主体部分 (如宝可梦精灵)
        cv::Mat foregroundMask;
        cv::bitwise_not(backgroundMask, foregroundMask);

        // 4. 暴力升维：给原来的 BGR 图像强制加上一个 Alpha 透明通道
        cv::Mat bgra;
        if (input.channels() == 3) {
            cv::cvtColor(input, bgra, cv::COLOR_BGR2BGRA);
        }
        else {
            input.copyTo(bgra);
        }

        // 5. 核心切割手术：分离出 BGRA 的四个通道
        std::vector<cv::Mat> channels;
        cv::split(bgra, channels);

        // 6. 将前景面具与 Alpha 通道进行物理叠加（与运算）
        // 原理：背景区域的掩膜是 0(黑)，Alpha 碰到 0 就会变成全透明；
        // 主体区域的掩膜是 255(白)，Alpha 保持原有的 255 不变。
        cv::bitwise_and(channels[3], foregroundMask, channels[3]);

        // 7. 将做完手术的通道重新缝合起来
        cv::merge(channels, bgra);

        // 完美返还处理后的透明图像
        return bgra;
    }

}