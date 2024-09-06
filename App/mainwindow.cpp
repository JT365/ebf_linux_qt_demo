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
#include "backlightwidget.h"
#include "calendarwidget.h"
#include "calculator.h"
#include "ebookwidget.h"
#include "photosview.h"
//#include "recorderwidget.h"
#include "settingwidget.h"
#include "videoplayer.h"
#include "infoneswidget.h"

#include "qtmessage.h"

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

    if (NULL != m_widgetWorkSpace) {
        delete m_widgetWorkSpace;
        m_widgetWorkSpace = NULL;
    }
}

void MainWindow::InitWidget() {
    QVBoxLayout *verLayout = new QVBoxLayout(this);
    verLayout->setContentsMargins(0, 0, 0, 0);
    verLayout->setSpacing(0);

    m_launcherWidget = new LauncherWidget(this);
    m_launcherWidget->SetWallpaper(QPixmap(":/images/mainwindow/background.png"));
    connect(m_launcherWidget, SIGNAL(currentItemClicked(int)), this, SLOT(SltCurrentAppChanged(int)));
    verLayout->addWidget(m_launcherWidget, 1);

    // 广告
    m_aboutUs = new AboutUs(this);
    m_aboutUs->hide();
    connect(m_launcherWidget, SIGNAL(signalAboutClicked()), m_aboutUs, SLOT(SltStartMove()));

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
    bool bMiniBoard = AppConfig::ReadSetting("System", "mini", false).toBool();
#if 1
    // 第一页
    int nPage = 0;
    m_launchItems.insert(3, new LauncherItem(3, nPage, tr("相册"), QPixmap(":/images/mainwindow/ic_photos.png")));
    m_launchItems.insert(7, new LauncherItem(7, nPage, tr("时钟"), QPixmap(":/images/mainwindow/ic_clock.png")));
    m_launchItems.insert(8, new LauncherItem(8, nPage, tr("电子书"), QPixmap(":/images/mainwindow/ic_ebook.png")));
    m_launchItems.insert(10, new LauncherItem(10, nPage, tr("计算器"), QPixmap(":/images/mainwindow/ic_calc.png")));

    // mini板卡取消功能
    if (!bMiniBoard) {
        m_launchItems.insert(1, new LauncherItem(1, nPage, tr("视频播放"), QPixmap(":/images/mainwindow/ic_video.png")));
        m_launchItems.insert(11, new LauncherItem(11, nPage, tr("音乐播放"), QPixmap(":/images/mainwindow/ic_music.png")));
    }

    // 第二页
    nPage++;
#ifdef BUILD_WITH_WEBVIEW
    m_launchItems.insert(14, new LauncherItem(14, nPage, tr("网络浏览器"), QPixmap(":/images/mainwindow/ic_webview.png")));
#endif
    m_launchItems.insert(16, new LauncherItem(16, nPage, tr("背光调节"), QPixmap(":/images/mainwindow/ic_backlight.png")));
    m_launchItems.insert(23, new LauncherItem(23, nPage, tr("系统设置"), QPixmap(":/images/mainwindow/ic_setting.png")));

    // 第三页
    nPage++;
    m_launchItems.insert(24, new LauncherItem(24, nPage, tr("InfoNES模拟器"), QPixmap(":/images/mainwindow/ic_game.png")));

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
    case 1: {
        if(CheckDevice("声卡"))
        {
            if (NULL != m_musicWidget) {
                m_musicWidget->StopMusic();
            }
            m_widgetWorkSpace = new VideoPlayer(this);
        }
    }
        break;
    case 3: {
        m_widgetWorkSpace = new PhotosView(this);
    }
        break;
    case 7: {
        m_widgetWorkSpace = new CalendarWidget(this);
    }
        break;
    case 8: {
        m_widgetWorkSpace = new EBookWidget(this);
    }
        break;
    case 10: {
        m_widgetWorkSpace = new Calculator(this);
    }
        break;
    case 11: {
        //m_widgetWorkSpace = new MusicPlayer(this);
        if(CheckDevice("声卡"))
        {
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

    }
        break;
    case 14: {
#ifdef BUILD_WITH_WEBVIEW
        m_widgetWorkSpace = new BrowserWindow(this);
#endif
    }
        break;
    case 16: {
        if(CheckDevice("背光调节"))
            m_widgetWorkSpace = new BackLightWidget(this);
    }
        break;
    case 18: {
//        m_widgetWorkSpace = new RecorderWidget(this);
    }
        break;
    case 23: {
        m_widgetWorkSpace = new SettingWidget(this);
        connect((SettingWidget *)m_widgetWorkSpace, SIGNAL(signalChangeCursorShape(Qt::CursorShape)),
                this, SLOT(SltChangeCursorShap(Qt::CursorShape)));
    }
        break;
    case 24: {

        if(CheckDevice("NES声卡"))
        {
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


bool MainWindow::CheckDevice(QString device)
{
    QFileInfo info;

    if(device=="声卡")
        info.setFile(VOICE_DEV);
    if(device=="NES声卡")
        info.setFile(VOICE_DEV);
    else if(device=="背光调节")
        info.setFile(BKLIGHT_DEV);

    if(!info.exists())
    {
        qDebug()<<"device not found";
        QtMessage *msg=new QtMessage(this);
        msg->setMessage(QString("没有检测到%1，\r\n请确认板卡支持该功能或已接入该硬件").arg(device));

        if(msg->exec()==QDialog::Accepted)
        {
            //NES 直接不打开
            if(device=="NES声卡")
            {
                m_launcherWidget->setEnabled(true);
                m_bStartApp = false;
                return false;
            }

            m_launcherWidget->setEnabled(false);
            m_bStartApp = true;
            return true;
        }
        else
        {
            m_launcherWidget->setEnabled(true);
            m_bStartApp = false;
            return false;
        }
    }
    m_launcherWidget->setEnabled(false);
    m_bStartApp = true;
    return true;
}

QString MainWindow::CheckMPU6050Device()
{
    QDir devDir(IIC_DEV);
    if(!devDir.exists())
        return "";

    QString mpuDevice="/sys/bus/iio/devices/iio\:device%1";

    QFileInfoList list = devDir.entryInfoList();
    for(int i=0 ; i<list.size(); i++)
    {
        QFile iioDeviceName(list.at(i).filePath()+"/name");
        if (!iioDeviceName.open(QIODevice::ReadOnly))
            continue;
        QString name = iioDeviceName.readAll();
        iioDeviceName.close();

        if(name == "mpu6050\n")
            return list.at(i).filePath();
    }

    return "";
}
