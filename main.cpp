// ==========================================
// Magma Engine - Composition Root (组合根)
// ==========================================

#include <QApplication>
#include <QPushButton> 
#include <QMessageBox>
#include <memory>

#include "MainWindow.h"

// 引入我们的业务契约与底层实现
#include "IImageProcessor.h"
#include "OpenCVImageProcessor.h"

// 在 main.cpp 头部引入你的新页面
#include "ImageMattingPage.h"

// ... 前面的代码不变 ...

int main(int argc, char* argv[])
{
    // ...
    QApplication app(argc, argv);

    // 1. 实例化 OpenCV 重型武器
    std::shared_ptr<Magma::Application::IImageProcessor> imageProcessor =
        std::make_shared<Magma::Infrastructure::OpenCVImageProcessor>();

    // 2. 实例化主界面
    MainWindow mainWindow;

    // 3. 实例化你的 UI 工具页，把 OpenCV 武器塞给它
    auto mattingPage = new ImageMattingPage(imageProcessor);



    // 5. 显示窗口
    mainWindow.show();

    return app.exec();
}