// ==========================================
// Magma Engine - Composition Root (组合根)
// 职责：依赖注入、组装系统、启动引擎
// ==========================================

#include <QApplication>
#include <QMainWindow>
#include <QLabel>

int main(int argc, char* argv[])
{
    // 1. 初始化 Qt 的全局应用对象 (接管操作系统底层的事件循环)
    QApplication app(argc, argv);

    // =========================================================
    // 阶段 A：依赖注入与装配 (Dependency Injection)
    // 现在的架子已经搭好，未来我们会在这里 new 出各个底层的实现
    // =========================================================

    // 2. 创建最简单的 UI 表现层 (临时验证架构是否跑通)
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Magma Engine v0.1.0 - Clean Architecture");
    mainWindow.resize(1280, 720); // 设定一个标准的 720p 宽屏视窗

    // 塞一个居中的文本作为我们成功跑通工具链的证明
    QLabel* label = new QLabel("欢迎来到 Magma Engine!\n5+1 层顶级架构已完美点亮。", &mainWindow);
    label->setAlignment(Qt::AlignCenter);
    QFont font = label->font();
    font.setPointSize(16);
    label->setFont(font);

    mainWindow.setCentralWidget(label);

    // 3. 将界面呈现给用户
    mainWindow.show();

    // 4. 将控制权彻底交给 Qt 的事件循环
    return app.exec();
}