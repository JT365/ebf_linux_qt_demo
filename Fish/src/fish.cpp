/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : videoplayer.cpp ---
 作 者    : Niyh  (QQ:393320854)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2019/9/7
*******************************************************************/
#include "skin.h"
#include "appconfig.h"
#include "fish.h"

#include <QTapAndHoldGesture>
#include <QGestureEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDir>
#include <QFileInfo>
#include <QTimer>
#include <QDebug>

Fish::Fish(QWidget *parent) : QtAnimationWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents);
    grabGesture(Qt::TapAndHoldGesture);

    m_strDirPath = AppConfig::ReadSetting("Fish", "location", "/").toString();
    if (m_strDirPath == "/") {
        m_strDirPath = qApp->applicationDirPath() + "/fish/";
    }

    this->SetBackground(QPixmap(":/images/fish/poolb.png"));
    InitWidget();
}

Fish::~Fish()
{
    AppConfig::SaveSetting("Fish", "location", m_strDirPath);

    if (m_timerShow->isActive()) {
        m_timerShow->stop();
    }

}

void Fish::InitWidget()
{
    m_magicPool = new MagicPool(this);

    m_titleBar = new FishTitleBarWidget(this);
    m_titleBar->SetText("Fish");
    connect(m_titleBar, SIGNAL(signalBack()), this, SLOT(SltBackClicked()));

    m_timerShow = new QTimer(this);
    m_timerShow->setSingleShot(true);
    m_timerShow->setInterval(5000);
    connect(m_timerShow, SIGNAL(timeout()), this, SLOT(SltAutoCloseToolBar()));

}

void Fish::ShowToolBar()
{
//    m_bToolBarShow = !m_bToolBarShow;
    if (m_bToolBarShow) {
        m_titleBar->SetAnimation(QPoint(0, -m_titleBar->height()), QPoint(0, 0));
    } else {
        m_titleBar->SetAnimation(QPoint(0, 0), QPoint(0, -m_titleBar->height()));
    }

    this->setCursor(m_bToolBarShow ? Qt::ArrowCursor : Qt::BlankCursor);
}

void Fish::SltAutoCloseToolBar()
{
    if (m_bToolBarShow) {
        m_bToolBarShow = false;
        ShowToolBar();
    }
}

void Fish::SltBackClicked()
{
    emit signalBackHome();
}

void Fish::resizeEvent(QResizeEvent *e)
{
    SetScaleValue();
    m_scaleX = (this->width() * 1.0) / m_nBaseWidth;
    m_scaleY = (this->height() * 1.0) / m_nBaseHeight;

    m_titleBar->resize(this->width(), 50 * m_scaleY);
    m_titleBar->move(0, -m_titleBar->height());

    setGeometry(0, 0, this->width(), this->height());

    QWidget::resizeEvent(e);
}

bool Fish::event(QEvent *event) {
    if (event->type() == QEvent::Gesture) {
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    return QWidget::event(event);
}

bool Fish::gestureEvent(QGestureEvent *event) {
    if (QGesture *tapAndHold = event->gesture(Qt::TapAndHoldGesture)) {
        if (tapAndHold->state() == Qt::GestureFinished) {
            qDebug() << "Tap and hold gesture detected!";

            m_bToolBarShow = true;
            ShowToolBar();
            if (m_timerShow->isActive()) {
                m_timerShow->stop();
            }

            // 有点击重新响应
            m_timerShow->start();
        } else if (tapAndHold->state() == Qt::GestureStarted){
             qDebug() << "Tap and hold gesture started";
        }
        return true;
    }
    return false;
}
