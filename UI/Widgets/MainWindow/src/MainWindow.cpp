#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QSplitter>
#include <QHBoxLayout>
#include <QLabel>
#include "ImageMattingPage.h"



MainWindow::~MainWindow() {
    delete ui;
}

// 1. 构造函数初始化列表里，接管这把武器 m_processor(std::move(processor))
MainWindow::MainWindow(std::shared_ptr<Magma::Application::IImageProcessor> processor, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_processor(std::move(processor))
{
    ui->setupUi(this);
    this->setWindowTitle("Magma Engine");
    this->setFixedSize(1920, 1080);

    initUIStyle();
    connectSignals();
    initPages();
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->setWindowTitle("MagamaEngine");
    this->setFixedSize(1920, 1080); // 锁定工业控制台标准分辨率

    initUIStyle();
    connectSignals();
    initPages();
}

void MainWindow::initUIStyle() {
    // 1. 消除系统自带装饰和虚线框
    ui->treeWidget->setRootIsDecorated(false);     // 隐藏系统左侧展开箭头
    ui->treeWidget->setFocusPolicy(Qt::NoFocus);   //  彻底干掉点击时恶心的虚线框

    // 2. 布局基础设置
    ui->treeWidget->setIndentation(40);            // 子节点向右缩进量（根据之前测试的最佳视觉值）
    ui->treeWidget->setHeaderHidden(true);
    ui->treeWidget->setMinimumWidth(250);

    // 3. 核心 QSS 样式
    ui->treeWidget->setStyleSheet(QString(R"(
        QTreeWidget {
            background-color: #ffffff;
            border: none;
            outline: none;
        }

        /* 抹除系统自带的左侧分支箭头和虚线占用的空间 */
        QTreeWidget::branch {
            image: none;
            border-image: none;
        }

        /* 所有项的基础样式 */
        QTreeWidget::item {
            font-family: 'Source Han Sans SC';
            font-size: 14px;
            font-weight: bold;
            height: 50px;
            color: #333333;
            border: none;
            padding-left: 15px; /* 全局基础左边距 */
        }

        /* 选中状态 */
        QTreeWidget::item:selected {
            background-color: #eef6ff;
            color: #00b8cc;
        }

        /* 悬停状态 */
        QTreeWidget::item:hover {
            background-color: #f5f7fa;
        }
    )"));
}

void MainWindow::initPages() {
    QString defaultIcon = ":/Image/MainWindow/Image/MainWindow/Icon/rootIcon.png";

    // ==========================================
    // 极度干净的路由挂载逻辑
    // ==========================================
    auto toolsCategory = addCategoryNode("资产处理", defaultIcon);

    // 实例化工具页，并将我们保管的武器 (m_processor) 发给它！
    // 注意：这里我们不需要、也不能知道这把武器是不是 OpenCV 造的，我们只管传！
    auto mattingPage = new ImageMattingPage(m_processor, this);

    addBusinessPage(toolsCategory, "智能绿幕抠图", mattingPage);

    // 初始化时展开所有节点
    ui->treeWidget->expandAll();

    if (ui->treeWidget->topLevelItemCount() > 0) {
        ui->treeWidget->setCurrentItem(ui->treeWidget->topLevelItem(0));
    }
}



void MainWindow::connectSignals() {
    //  1. 导航切换逻辑：通过 currentItemChanged 实现路由跳转
    connect(ui->treeWidget, &QTreeWidget::currentItemChanged,
        this, [this](QTreeWidgetItem* current, QTreeWidgetItem* previous) {
            if (!current) return;
            QVariant data = current->data(0, Qt::UserRole);
            if (data.isValid()) {
                ui->stackedWidget->setCurrentIndex(data.toInt());
            }
        });

    //  2. 交互优化：单击父节点即可展开/收缩
    connect(ui->treeWidget, &QTreeWidget::itemClicked, this, [](QTreeWidgetItem* item, int column) {
        if (item->childCount() > 0) {
            item->setExpanded(!item->isExpanded());
        }
        });

    //  3. 动画逻辑：节点展开时 -> 箭头朝上
    connect(ui->treeWidget, &QTreeWidget::itemExpanded, this, [this](QTreeWidgetItem* item) {
        QWidget* container = ui->treeWidget->itemWidget(item, 0);
        if (container) {
            QLabel* arrowLab = container->findChild<QLabel*>("arrowLabel");
            if (arrowLab) {
                arrowLab->setPixmap(QPixmap(":/Image/MainWindow/Image/MainWindow/Icon/arrow-up.png").scaled(12, 12, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
        });

    //  4. 动画逻辑：节点收缩时 -> 箭头朝下
    connect(ui->treeWidget, &QTreeWidget::itemCollapsed, this, [this](QTreeWidgetItem* item) {
        QWidget* container = ui->treeWidget->itemWidget(item, 0);
        if (container) {
            QLabel* arrowLab = container->findChild<QLabel*>("arrowLabel");
            if (arrowLab) {
                arrowLab->setPixmap(QPixmap(":/Image/MainWindow/Image/MainWindow/Icon/arrow-down.png").scaled(12, 12, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
        });
}

// ==============================================================================
// 核心业务函数：极度纯粹，不包含任何 UI 布局代码
// ==============================================================================

// 1. 插入分类根节点（修复重影：只建空节点，文字交给 UI 装饰器绘制）
QTreeWidgetItem* MainWindow::addCategoryNode(const QString& name, const QString& iconPath) {
    QTreeWidgetItem* root = new QTreeWidgetItem(ui->treeWidget);

    // 调用 UI 装饰器进行美化（带展开箭头）
    decorateParentNodeUI(root, name, iconPath, true);
    return root;
}

void MainWindow::addBusinessPage(QTreeWidgetItem* parent, const QString& name, QWidget* page) {
    if (!parent || !page) return;
    int index = ui->stackedWidget->addWidget(page);
    QTreeWidgetItem* child = new QTreeWidgetItem(parent, QStringList() << name);
    // 建立业务路由绑定
    child->setData(0, Qt::UserRole, index);
}

// 2. 插入业务根节点（修复重影：只建空节点，文字交给 UI 装饰器绘制）
void MainWindow::addRootBusinessPage(const QString& name, QWidget* page, const QString& iconPath) {
    if (!page) return;
    int index = ui->stackedWidget->addWidget(page);

    // 去掉了 QStringList() << name
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(ui->treeWidget);
    rootItem->setData(0, Qt::UserRole, index);

    // 调用 UI 装饰器进行美化（首页无展开箭头）
    decorateParentNodeUI(rootItem, name, iconPath, false);
}

// ==============================================================================
// 纯 UI 装饰器：将杂乱的布局代码隔离在这里
// ==============================================================================

void MainWindow::decorateParentNodeUI(QTreeWidgetItem* item, const QString& name, const QString& iconPath, bool hasArrow) {
    QWidget* container = new QWidget();
    // 允许鼠标穿透，保证背后的 QTreeWidget 能响应悬停和选中颜色
    container->setAttribute(Qt::WA_TransparentForMouseEvents);
    container->setStyleSheet("background: transparent;");

    QHBoxLayout* layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 20, 0); // 左侧靠 QTreeWidget::item 的 padding-left 控制

    // 左侧图标
    QLabel* iconLab = new QLabel();
    if (!iconPath.isEmpty()) {
        iconLab->setPixmap(QPixmap(iconPath).scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // 中间文字
    QLabel* textLab = new QLabel(name);
    textLab->setStyleSheet("font-weight: bold; font-family: 'Source Han Sans SC'; font-size: 14px; color: #333333;");

    layout->addWidget(iconLab);
    layout->addSpacing(10);
    layout->addWidget(textLab);
    layout->addStretch();

    // 右侧动态箭头
    if (hasArrow) {
        QLabel* arrowLab = new QLabel();
        arrowLab->setObjectName("arrowLabel"); // 打上标签，供动画函数 findChild 查找
        arrowLab->setPixmap(QPixmap(":/Image/MainWindow/Image/MainWindow/Icon/arrow-down.png").scaled(12, 12, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        layout->addWidget(arrowLab);
    }

    ui->treeWidget->setItemWidget(item, 0, container);
}