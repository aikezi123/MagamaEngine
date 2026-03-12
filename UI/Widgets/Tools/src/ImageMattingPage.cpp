#include "ImageMattingPage.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QPixmap>
#include "ui_ImageMattingPage.h"

ImageMattingPage::ImageMattingPage(std::shared_ptr<Magma::Application::IImageProcessor> processor, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ImageMattingPageClass) // 🚨【核心修复】：必须在这里给 ui 申请内存！绝对不能漏掉！
    , m_processor(std::move(processor))
{
    // 现在 ui 指针合法了，调用 setupUi 就绝对安全了
    ui->setupUi(this);

    // 初始状态下禁用处理和保存按钮
    ui->btnProcess->setEnabled(false);
    ui->btnSave->setEnabled(false);
}

ImageMattingPage::~ImageMattingPage()
{
    delete ui;
}

void ImageMattingPage::on_btnLoad_clicked() {
    QString path = QFileDialog::getOpenFileName(this, "选择精灵图", "", "Images (*.png *.jpg *.bmp)");
    if (path.isEmpty()) return;

    // 使用 Qt 读取图片，严守防腐墙，不碰 OpenCV 的读写模块！
    QImage img(path);
    if (img.isNull()) {
        QMessageBox::warning(this, "错误", "无法读取图片！");
        return;
    }

    // 界面显示原图
    ui->lblOriginal->setPixmap(QPixmap::fromImage(img).scaled(300, 300, Qt::KeepAspectRatio));

    // 【跨界桥梁】：将 Qt 的 QImage 转换为引擎通用货币 cv::Mat (BGR 格式)
    QImage bgrImg = img.convertToFormat(QImage::Format_BGR888);
    m_currentImage = cv::Mat(bgrImg.height(), bgrImg.width(), CV_8UC3, (void*)bgrImg.bits(), bgrImg.bytesPerLine()).clone();

    // 更新按钮状态
    ui->btnProcess->setEnabled(true);
    ui->lblProcessed->setText("准备就绪，点击处理");
    ui->lblProcessed->setPixmap(QPixmap());
    ui->btnSave->setEnabled(false);
}

void ImageMattingPage::on_btnProcess_clicked() {
    if (m_currentImage.empty() || !m_processor) return;

    // 呼叫底层契约开始干活 (这里默认抠除纯绿色 0,255,0，容差为10)
    m_processedImage = m_processor->removeBackgroundColor(m_currentImage, cv::Scalar(0, 255, 0), 10);

    if (m_processedImage.empty()) {
        QMessageBox::critical(this, "失败", "图像处理异常！");
        return;
    }

    // 【神奇的内存对齐】：OpenCV 的 CV_8UC4 (BGRA) 在内存里的字节序
    // 刚好和 Qt 的 QImage::Format_ARGB32 在 x64 机器上的字节序完全一致！无需转换！
    QImage resultImg(m_processedImage.data, m_processedImage.cols, m_processedImage.rows,
        static_cast<int>(m_processedImage.step), QImage::Format_ARGB32);

    ui->lblProcessed->setPixmap(QPixmap::fromImage(resultImg).scaled(300, 300, Qt::KeepAspectRatio));
    ui->btnSave->setEnabled(true);
}

void ImageMattingPage::on_btnSave_clicked() {
    if (m_processedImage.empty()) return;

    QString savePath = QFileDialog::getSaveFileName(this, "导出透明图块", "PokemonSprite_Transparent.png", "PNG Images (*.png)");
    if (savePath.isEmpty()) return;

    // 再次利用完美的内存对齐机制，使用 Qt 直接保存为带 Alpha 通道的 PNG
    QImage resultImg(m_processedImage.data, m_processedImage.cols, m_processedImage.rows,
        static_cast<int>(m_processedImage.step), QImage::Format_ARGB32);

    if (resultImg.save(savePath)) {
        QMessageBox::information(this, "成功", "透明图片已成功导出！");
    }
    else {
        QMessageBox::critical(this, "错误", "保存失败！");
    }
}