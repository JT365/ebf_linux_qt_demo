/******************************************************************
 Copyright (C) 2025 - All Rights Reserved by
 文 件 名 : fingerpaint.cpp ---
 作 者    : wdz
 编写日期 : 2025
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2025/4/15
*******************************************************************/
#include "fingerpaint.h"
#include "skin.h"
#include "imageviewer.h"
#include "appconfig.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QDir>

FingerPaint::FingerPaint(QWidget *parent) : QtAnimationWidget(parent)
{
    m_strDirPath = AppConfig::ReadSetting("FingerPaint", "location", "/").toString();
    if (m_strDirPath == "/") {
        m_strDirPath = qApp->applicationDirPath() + "/fingerpaint/";
    }

    this->SetBackground(QColor("#ffffff"));

    InitWidget();
}

FingerPaint::~FingerPaint()
{
    AppConfig::SaveSetting("FingerPaint", "location", m_strDirPath);
}

void FingerPaint::InitWidget()
{
    QtWidgetTitleBar *widgetTitle = new QtWidgetTitleBar(this);
    widgetTitle->SetScalSize(Skin::m_nScreenWidth, 60);
    widgetTitle->SetBackground(QColor("#f0f0f0"));
    widgetTitle->SetTitle(tr("FingerPaint"), "#333333", 25);
    connect(widgetTitle, SIGNAL(signalBtnClicked(int)), this, SLOT(SltToolBtnClicked(int)));

    m_btnBack = new QtPixmapButton(BtnBack, QRect(10, 10, 40, 40), QPixmap(":/images/fingerpaint/ic_back.png"), QPixmap(":/images/fingerpaint/ic_back_pre.png"));
    m_btnBack->setVisible(false);

    m_btnClear = new QtPixmapButton(BtnClear, QRect(746, 10, 38, 38), QPixmap(":/images/fingerpaint/413-38.png"), QPixmap(":/images/fingerpaint/8f-38.png"));
//    m_btnClear->setVisible(false);

    QMap<int,QtPixmapButton*> btngroup;
    btngroup.insert(BtnBack, m_btnBack);
    btngroup.insert(BtnClear, m_btnClear);
    widgetTitle->SetToolButtons(btngroup);

    m_scribble = new ScribbleArea(this);
//    m_scribble->SetBackground(QColor("#ffffff"));
//    connect(m_scribble, SIGNAL(currentItemClicked(QtPageListWidgetItem*)), this, SLOT(SltCurrentItemClicked(QtPageListWidgetItem*)));

    QVBoxLayout *verLayout = new QVBoxLayout(this);
    verLayout->setContentsMargins(0, 0, 0, 0);
    verLayout->setSpacing(0);
    verLayout->addWidget(widgetTitle, 1);
    verLayout->addWidget(m_scribble, 7);

}

void FingerPaint::SltToolBtnClicked(int index)
{
    if (BtnHome == index) {
        emit signalBackHome();
    } else if (BtnBack == index) {

    } else if (BtnClear == index) {
//        m_dirDialog->setSaveFileMode(false);
        m_scribble->clearImage();
//        m_dirDialog->StartAnimation(QPoint(this->width(), -this->height()), QPoint(0, 0), 200, true);
    } else if (BtnSetting == index) {

    }
}

void FingerPaint::resizeEvent(QResizeEvent *e)
{
//    m_dirDialog->resize(this->size());
    m_scaleX = (this->width() * 1.0) / m_nBaseWidth;
    m_scaleY = (this->height() * 1.0) / m_nBaseHeight;

    QWidget::resizeEvent(e);
}
