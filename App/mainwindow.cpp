/******************************************************************
 Copyright (C) 2019 - All Rights Reserved by
 文 件 名 : mainwindow.cpp ---
 作 者    : Niyh(lynnhua)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
  Niyh	   2019    	1.0.0 1     文件创建
*******************************************************************/
#include "mainwindow.h"
#include "statusbarwidget.h"
#include "unit.h"
#include "skin.h"
#include "appconfig.h"

#include "musicplayer.h"
#include "camerawidget.h"
#include "carmeterwidget.h"
#include "dhtcollection.h"
#include "filesystemwindow.h"
#include "notepadwidget.h"
#include "settingwidget.h"
#include "infoneswidget.h"

#ifdef BUILD_WIN_NES
#include "nessimulator.h"
#endif

#ifdef BUILD_WITH_WEBVIEW
#include "browserwindow.h"
#endif

#include <QPainter>
#include <QBoxLayout>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTimerEvent>
#include <QRegExp>

#define MOUSE_DEV_PATH       "/dev/input/by-path"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
#ifdef __arm__
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

    // 启动检测鼠标是否接入系统
    AppConfig::m_bMouseInsert = CheckMouseInsert();
    bool bOn = AppConfig::ReadSetting("System", "mouse", true).toBool();
    this->setCursor((bOn || AppConfig::m_bMouseInsert) ? Qt::ArrowCursor : Qt::BlankCursor);
#endif

    m_widgetWorkSpace = NULL;
    m_musicWidget = NULL;
    m_nCurrentIndex = -1;
    m_bStartApp = false;

    InitWidget();
    InitDesktop();
    InitThreads();

    m_mainTimer = new QTimer(this);
    connect(m_mainTimer, SIGNAL(timeout()), this, SLOT(slotTimeoutMainTimer()));  

    /* Start timer */
    m_mainTimer->start(1000);   // 1ms
          
#ifdef CHECK_MOUSE_BY_TIMER
    m_nMouseCheckId = startTimer(3000);
#endif
}

MainWindow::~MainWindow()
{
#ifdef CHECK_MOUSE_BY_TIMER
    killTimer(m_nMouseCheckId);
#endif

    delete m_aboutUs;
    m_aboutUs = NULL;

    delete m_mainTimer;
    m_mainTimer = NULL;    

    delete displayTime;
    
    if (NULL != m_widgetWorkSpace) {
        delete m_widgetWorkSpace;
        m_widgetWorkSpace = NULL;
    }
}

void MainWindow::InitWidget() {
    QFont font(Skin::m_strAppFontBold);
    
    QVBoxLayout *verLayout = new QVBoxLayout(this);
    verLayout->setContentsMargins(0, 0, 0, 0);
    verLayout->setSpacing(0);

    m_launcherWidget = new LauncherWidget(this);
    m_launcherWidget->SetWallpaper(QPixmap(":/images/mainwindow/background.png"));
    m_launcherWidget->setFont(QFont(Skin::m_strAppFontNormal));
    connect(m_launcherWidget, SIGNAL(currentItemClicked(int)), this, SLOT(SltCurrentAppChanged(int)));
    verLayout->addWidget(m_launcherWidget, 1);

    // Advertisement
    m_aboutUs = new AboutUs(this);
    m_aboutUs->hide();
//    connect(m_launcherWidget, SIGNAL(signalAboutClicked()), m_aboutUs, SLOT(SltStartMove()));

    /* Time */
    QTime currentTime = QTime::currentTime();
    displayTime = new QLabel(this);
    displayTime->setGeometry((float)710/800*this->geometry().width(), (float)395/480*this->geometry().height(),
                             (float)280/800*this->geometry().width(), (float)100/480*this->geometry().height());
    displayTime->setStyleSheet("QLabel { color: white;background: transparent}");
    displayTime->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    displayTime->setText(currentTime.toString().mid(0, 8));
    font.setPointSize((float)22/800*this->geometry().width());
    displayTime->setFont(font);
    
#if 0
    if (NULL == m_musicWidget) {
        m_musicWidget = new MusicPlayer(this);
        connect(m_musicWidget, SIGNAL(signalBackHome()), this, SLOT(SltBackHome()));
        connect(m_musicWidget, SIGNAL(signalAnimationFinished()), this, SLOT(SltAppStartOk()));
    }
#endif
}

void MainWindow::InitDesktop()
{
#if 1
    // 第一页
    int nPage = 0;
    m_launchItems.insert(0, new LauncherItem(0, nPage, tr("File Manage"), QPixmap(":/images/mainwindow/ic_file.png")));
    m_launchItems.insert(6, new LauncherItem(6, nPage, tr("Notepad"), QPixmap(":/images/mainwindow/ic_notepad.png")));

    m_launchItems.insert(4, new LauncherItem(4, nPage, tr("Camera"), QPixmap(":/images/mainwindow/ic_camera.png")));
    m_launchItems.insert(9, new LauncherItem(9, nPage, tr("DHT11"), QPixmap(":/images/mainwindow/ic_temp.png")));
    m_launchItems.insert(11, new LauncherItem(11, nPage, tr("Music"), QPixmap(":/images/mainwindow/ic_music.png")));

#ifdef BUILD_WITH_WEBVIEW
    m_launchItems.insert(14, new LauncherItem(14, nPage, tr("Webengine"), QPixmap(":/images/mainwindow/ic_webview.png")));
#endif
    m_launchItems.insert(15, new LauncherItem(15, nPage, tr("Dashboard"), QPixmap(":/images/mainwindow/ic_car.png")));
    m_launchItems.insert(23, new LauncherItem(23, nPage, tr("System"), QPixmap(":/images/mainwindow/ic_setting.png")));
 
    m_launchItems.insert(24, new LauncherItem(24, nPage, tr("InfoNES Simulator"), QPixmap(":/images/mainwindow/ic_game.png")));
        
    // 第二页
    nPage++;


    // 第三页
    nPage++;


    m_launcherWidget->SetItems(m_launchItems);
#endif
}

void MainWindow::InitThreads()
{
    m_threadUsbInsert = new ThreadMouseCheck(this);
    connect(m_threadUsbInsert, SIGNAL(signalMouseInsert(bool)), this, SLOT(SltMouseInsert(bool)));
#ifdef __arm__
    m_threadUsbInsert->start();
#endif

    m_threadPowerKey = new ThreadKey(this);
    m_threadKey = new ThreadKey(this, 1);

    m_threadPowerKey->start();
    m_threadKey->start();
}

/* Update date and time zone */
void MainWindow::slotTimeoutMainTimer(void) {
    QTime currentTime = QTime::currentTime();
    QString time = currentTime.toString().mid(0, 8);

    /* 设置时间 */
    displayTime->setText(time);

}

void MainWindow::SltCurrentAppChanged(int index)
{
    if (m_bStartApp) return;
    m_launcherWidget->setEnabled(false);
    m_bStartApp = true;

    if (NULL != m_widgetWorkSpace ) {
        if (m_nCurrentIndex != index) {
            disconnect(m_widgetWorkSpace, SIGNAL(signalBackHome()), this, SLOT(SltBackHome()));
            delete m_widgetWorkSpace;
            m_widgetWorkSpace = NULL;
        } else {
            m_widgetWorkSpace->setVisible(true);
            m_widgetWorkSpace->StartAnimation(QPoint(this->width(), this->height()), QPoint(0, 0), 300, true);
            return;
        }
    }

    switch (index) {
 
    case 0: {
        m_widgetWorkSpace = new FileSystemWindow(this);
    }
        break;
    case 6: {
        m_widgetWorkSpace = new NotePadWidget(this);
    }
        break;

    case 9: {
        m_widgetWorkSpace = new DhtCollection(this);
    }
        break;

    case 11: {
        //        m_widgetWorkSpace = new MusicPlayer(this);
        m_nCurrentIndex = index;
        if (NULL == m_musicWidget) {
            m_musicWidget = new MusicPlayer(this);
            connect(m_musicWidget, SIGNAL(signalBackHome()), this, SLOT(SltBackHome()));
            connect(m_musicWidget, SIGNAL(signalAnimationFinished()), this, SLOT(SltAppStartOk()));
        }

        m_musicWidget->resize(this->size());
        m_musicWidget->setVisible(true);
        m_musicWidget->StartAnimation(QPoint(this->width(), this->height()), QPoint(0, 0), 300, true);
        return;
    }
        break;

    case 14: {
#ifdef BUILD_WITH_WEBVIEW
        m_widgetWorkSpace = new BrowserWindow(this);
#endif
    }
        break;
    case 15: {
        m_widgetWorkSpace = new CarMeterWidget(this);
    }
        break;

    case 23: {
        m_widgetWorkSpace = new SettingWidget(this);
        connect((SettingWidget *)m_widgetWorkSpace, SIGNAL(signalChangeCursorShape(Qt::CursorShape)),
                this, SLOT(SltChangeCursorShap(Qt::CursorShape)));
    }
        break;
    case 24: {
        // 关闭音乐
        if (NULL != m_musicWidget) {
            m_musicWidget->StopMusic();
        }

#ifdef BUILD_WIN_NES
        m_widgetWorkSpace = new NesSimulator(this);
#else
        m_widgetWorkSpace = new InfoNesWidget(this);
#endif
    }
        break;

    default:
        m_nCurrentIndex = -1;
        break;
    }

    if (NULL != m_widgetWorkSpace) {
        m_widgetWorkSpace->resize(this->size());
        connect(m_widgetWorkSpace, SIGNAL(signalBackHome()), this, SLOT(SltBackHome()));
        connect(m_widgetWorkSpace, SIGNAL(signalAnimationFinished()), this, SLOT(SltAppStartOk()));

        m_nCurrentIndex = index;
        m_widgetWorkSpace->setVisible(true);
        m_widgetWorkSpace->StartAnimation(QPoint(this->width(), this->height()), QPoint(0, 0), 300, true);
    }
}

void MainWindow::SltBackHome()
{
    if (NULL != m_widgetWorkSpace) {
        m_widgetWorkSpace->StartAnimation(QPoint(0, 0), QPoint(-this->width(), -this->height()), 300, false);
    }
    else if (NULL != m_musicWidget) {
        m_musicWidget->StartAnimation(QPoint(0, 0), QPoint(-this->width(), -this->height()), 300, false);
    }
}

void MainWindow::SltChangeCursorShap(Qt::CursorShape shape)
{
    this->setCursor(shape);
}

void MainWindow::SltAppStartOk()
{
    m_bStartApp = false;
    m_launcherWidget->setEnabled(true);
    if (11 == m_nCurrentIndex) {
        if (NULL != m_musicWidget && m_musicWidget->isVisible()) {
            m_musicWidget->InitPlayList();
        }
    }

    if (19 == m_nCurrentIndex && (NULL != m_widgetWorkSpace) && !m_widgetWorkSpace->isVisible()) {
        disconnect(m_widgetWorkSpace, SIGNAL(signalBackHome()), this, SLOT(SltBackHome()));
        delete m_widgetWorkSpace;
        m_widgetWorkSpace = NULL;
    }
}

void MainWindow::SltMouseInsert(bool bOk)
{
    AppConfig::m_bMouseInsert = bOk;
    if (this->cursor().shape() == Qt::BlankCursor &&
            !AppConfig::m_bPlayVideo &&
            AppConfig::m_bMouseInsert)
    {
        this->setCursor(Qt::ArrowCursor);
        return;
    }

    bool bOn = AppConfig::ReadSetting("System", "mouse", true).toBool();
    if (!bOk && !AppConfig::m_bPlayVideo) {
        this->setCursor(bOn ? Qt::ArrowCursor : Qt::BlankCursor);
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    m_aboutUs->resize(this->size());

    if (NULL != m_widgetWorkSpace) {
        m_widgetWorkSpace->resize(this->size());
    }

    if (NULL != m_musicWidget) {
        m_musicWidget->resize(this->size());
    }
    QWidget::resizeEvent(e);
}

#ifdef BUILD_WIN_NES
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (24 == m_nCurrentIndex && NULL != m_widgetWorkSpace) {
        NesSimulator *simulator = (NesSimulator *)m_widgetWorkSpace;
        simulator->sendKeyPressed(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (24 == m_nCurrentIndex && NULL != m_widgetWorkSpace) {
        NesSimulator *simulator = (NesSimulator *)m_widgetWorkSpace;
        simulator->sendKeyReleased(event);
    }
}
#endif


bool MainWindow::CheckMouseInsert()
{
    QDir dir(MOUSE_DEV_PATH);
    if (!dir.exists()) return false;

    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); i++) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.fileName().contains("mouse")) {
            return true;
        }
    }

    return false;
}

#if CHECK_MOUSE_BY_TIMER
void MainWindow::timerEvent(QTimerEvent *e)
{
    if (m_nMouseCheckId == e->timerId()) {
        AppConfig::m_bMouseInsert = CheckMouseInsert();
        if (this->cursor().shape() == Qt::BlankCursor &&
                !AppConfig::m_bPlayVideo &&
                AppConfig::m_bMouseInsert)
        {
            this->setCursor(Qt::ArrowCursor);
        }
    }
}
#endif


