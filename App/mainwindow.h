/******************************************************************
 Copyright (C) 2019 - All Rights Reserved by
 文 件 名 : mainwindow.h ---
 作 者    : Niyh(lynnhua)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
  Niyh	   2019    	1.0.0 1     文件创建
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "launcherwidget.h"
#include "qtwidgetbase.h"
#include "aboutus.h"
#include "threadkey.h"
#include "threadmousecheck.h"

// 声卡
#define VOICE_DEV       "/dev/snd/pcmC0D0p"

// IIC
#define IIC_DEV         "/sys/bus/iio/devices"
// 背光
#define BKLIGHT_DEV     "/sys/class/backlight/backlight/brightness"

// keyboard
#define KEY_DEV         "/dev/input/by-path/platform-sgpio-keys-event"
// switch
#define POWER_DEV       "/dev/input/by-path/platform-20cc000.snvs:snvs-powerkey-event"

class MusicPlayer;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    LauncherWidget      *m_launcherWidget;
    QMap<int, LauncherItem*> m_launchItems;

    AboutUs             *m_aboutUs;
    QtAnimationWidget   *m_widgetWorkSpace;
    MusicPlayer         *m_musicWidget;

    int                  m_nCurrentIndex;
    bool                 m_bStartApp;

    ThreadMouseCheck    *m_threadUsbInsert;
    ThreadKey           *m_threadPowerKey;
    ThreadKey           *m_threadKey;

#ifdef CHECK_MOUSE_BY_TIMER
    int              m_nMouseCheckId;
#endif
private:
    void InitWidget();
    void InitDesktop();
    void InitThreads();

    bool CheckDevice(QString device);
    QString CheckMPU6050Device();
private slots:
    void SltCurrentAppChanged(int index);

    void SltBackHome();
    void SltChangeCursorShap(Qt::CursorShape shape);
    void SltAppStartOk();

    void SltMouseInsert(bool bOk);

protected:
    void resizeEvent(QResizeEvent *e);
#ifdef BUILD_WIN_NES
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
#endif

    // 新增鼠标检测
    bool CheckMouseInsert();

#if CHECK_MOUSE_BY_TIMER
    void timerEvent(QTimerEvent *e);
#endif
};

#endif // MAINWINDOW_H
