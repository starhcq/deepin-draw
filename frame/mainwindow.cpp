#include "mainwindow.h"

#include <QVBoxLayout>
#include <QCheckBox>
#include <QDebug>
#include <QApplication>

#include "utils/configsettings.h"

#include <DTitlebar>

const QSize WINDOW_MINISIZR = QSize(800, 500);

MainWindow::MainWindow(QWidget *parent)
    :DMainWindow(parent)
{
    int defaultW = ConfigSettings::instance()->value("window", "width").toInt();
    int defaultH = ConfigSettings::instance()->value("window", "height").toInt();

    if (defaultW == 0 || defaultH == 0)
    {
        window()->setWindowState(Qt::WindowMaximized);
    } else
    {
       window()->resize(defaultW, defaultH);
    }

    setMinimumSize(WINDOW_MINISIZR);

    m_topToolbar = new TopToolbar(this);
    QSize canvasSize = qApp->desktop()->size();
    emit m_topToolbar->canvasSizeChanged(canvasSize.width(), canvasSize.height());

    m_titlebarWidth = titlebar()->buttonAreaWidth();
    m_topToolbar->setFixedWidth(width() - m_titlebarWidth);

    titlebar()->setCustomWidget(m_topToolbar, Qt::AlignLeft);
    titlebar()->setMenu(m_topToolbar->mainMenu());

    m_mainWidget = new MainWidget(this);
    m_mainWidget->setFocusPolicy(Qt::StrongFocus);
    setContentsMargins(QMargins(0, 0, 0, 0));
    setCentralWidget(m_mainWidget);

    connect(m_topToolbar, &TopToolbar::drawShapeChanged,
            m_mainWidget, &MainWidget::drawShapeChanged);

    connect(m_topToolbar, &TopToolbar::fillShapeSelectedActive,
            m_mainWidget, &MainWidget::fillShapeSelectedActive);

    connect(m_topToolbar, &TopToolbar::rotateImage,
            m_mainWidget, &MainWidget::rotateImage);

    connect(m_topToolbar, &TopToolbar::mirroredImage,
            m_mainWidget, &MainWidget::mirroredImage);

    connect(m_topToolbar, &TopToolbar::saveImageAction,
            m_mainWidget, &MainWidget::saveImageAction);

    connect(m_mainWidget, &MainWidget::updateMiddleWidget,
            m_topToolbar, &TopToolbar::updateMiddleWidget);
    connect(m_mainWidget, &MainWidget::adjustArtBoardSize,
            m_topToolbar, &TopToolbar::adjustArtBoardSize);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    m_topToolbar->setFixedWidth(this->width() -  m_titlebarWidth);

    int ww = window()->width();
    int wh = window()->height();

    ConfigSettings::instance()->setValue("window", "width", ww);
    ConfigSettings::instance()->setValue("window", "height", wh);

    DMainWindow::resizeEvent(event);
    this->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    DMainWindow::keyPressEvent(event);
}

MainWindow::~MainWindow()
{
}
