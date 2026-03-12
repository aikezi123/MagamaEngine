// ==========================================
// Magma Engine - Composition Root (组合根)
// ==========================================

#include <QApplication>
#include <QPushButton> // <--- 新增：包含按钮的完整定义
#include <QMessageBox>
#include <memory>

#include "MainWindow.h"

// 引入我们的业务契约与底层实现
#include "IImageProcessor.h"
#include "OpenCVImageProcessor.h"

int main(int argc, char* argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    // =========================================================
    // 阶段 A：依赖注入中心 
    // 【修复】：乖乖把底层和大脑层的命名空间加回来！
    // =========================================================
    std::shared_ptr<Magma::Application::IImageProcessor> imageProcessor =
        std::make_shared<Magma::Infrastructure::OpenCVImageProcessor>();

    // 实例化主界面 (你的 MainWindow 是全局的，所以不需要加命名空间)
    MainWindow mainWindow;

    // =========================================================
    // 阶段 B：业务接线
    // =========================================================
    // 测试：拦截名为 "btnTest" 的按钮的点击事件
    QObject::connect(mainWindow.findChild<QPushButton*>("btnTest"), &QPushButton::clicked, [&]() {

        // 1. 模拟生成一张纯绿色的图片 (高100, 宽100)
        cv::Mat testImage(100, 100, CV_8UC3, cv::Scalar(0, 255, 0));

        // 2. 调用万能抠图引擎 (抠除绿色)
        cv::Mat resultImage = imageProcessor->removeBackgroundColor(testImage, cv::Scalar(0, 255, 0), 0);

        if (!resultImage.empty() && resultImage.channels() == 4) {
            QMessageBox::information(&mainWindow, "底层响应成功",
                "OpenCV 引擎已成功运行！\n\n图片已增加 Alpha 通道，绿幕已完全移除。");
        }
        else {
            QMessageBox::critical(&mainWindow, "错误", "底层 OpenCV 处理失败！");
        }
        });

    // =========================================================
    // 阶段 C：点火发射
    // =========================================================
    mainWindow.show();

    return app.exec();
}