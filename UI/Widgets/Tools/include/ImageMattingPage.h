#pragma once

#include <QWidget>
#include <memory>
#include <opencv2/core.hpp> 
#include "IImageProcessor.h"

// 【修复 1】：删掉 #include "ui_ImageMattingPage.h"
// 【修复 2】：使用前置声明，告诉编译器未来会有这个类
QT_BEGIN_NAMESPACE
namespace Ui { class ImageMattingPageClass; }
QT_END_NAMESPACE

class ImageMattingPage : public QWidget
{
    Q_OBJECT

public:
    explicit ImageMattingPage(std::shared_ptr<Magma::Application::IImageProcessor> processor, QWidget* parent = nullptr);
    ~ImageMattingPage();

private slots:
    void on_btnLoad_clicked();
    void on_btnProcess_clicked();
    void on_btnSave_clicked();

private:
    // 这里改成指针形式 (Qt 的标准做法)
    Ui::ImageMattingPageClass* ui;

    std::shared_ptr<Magma::Application::IImageProcessor> m_processor;
    cv::Mat m_currentImage;
    cv::Mat m_processedImage;
};