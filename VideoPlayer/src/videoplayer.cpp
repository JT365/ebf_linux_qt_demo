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
#include "videoplayer.h"
#include "skin.h"
#include "appconfig.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDir>
#include <QFileInfo>
#include <QTimer>

#ifndef FLASH_FILE_EMMC
#define FLASH_FILE_EMMC     "/sys/class/mmc_host/mmc1/mmc1:0001/block/mmcblk1/size"
#endif

VideoPlayer::VideoPlayer(QWidget *parent) : QtAnimationWidget(parent)
{
    m_strDirPath = AppConfig::ReadSetting("Video", "location", "/").toString();
    if (m_strDirPath == "/") {
        m_strDirPath = qApp->applicationDirPath() + "/video/";
    }

    this->SetBackground(QPixmap(":/images/video/ic_background.png"));

    InitWidget();
    QTimer::singleShot(500, this, SLOT(LoadMedias()));
}

VideoPlayer::~VideoPlayer()
{
    AppConfig::SaveSetting("Video", "location", m_strDirPath);
}

void VideoPlayer::InitWidget()
{
    QtWidgetTitleBar *m_widgetTitle = new QtWidgetTitleBar(this);
    m_widgetTitle->SetScalSize(Skin::m_nScreenWidth, 80);
    m_widgetTitle->SetBackground(Qt::transparent);
    m_widgetTitle->setFont(QFont(Skin::m_strAppFontNormal));
    m_widgetTitle->SetTitle(tr("Video Player"), "#ffffff", 32);
    m_widgetTitle->SetBtnHomePixmap(QPixmap(":/images/browser/ic_prev_pre.png"), QPixmap(":/images/browser/ic_prev.png"));
    connect(m_widgetTitle, SIGNAL(signalBtnClicked(int)), this, SLOT(SltToolBtnClicked(int)));

    m_btnBack = new QtPixmapButton(BtnBack, QRect(10, 10, 40, 40), QPixmap(":/images/ebook/ic_back.png"), QPixmap(":/images/ebook/ic_back.png"));
    m_btnBack->setVisible(false);

    m_btnAdd = new QtPixmapButton(BtnAdd, QRect(746, 0, 54, 54), QPixmap(":/images/video/menu_icon.png"), QPixmap(":/images/video/menu_icon_pressed.png"));

    m_btnSetting = new QtPixmapButton(BtnSetting, QRect(750, 10, 40, 40), QPixmap(":/images/ebook/ic_setting.png"), QPixmap(":/images/ebook/ic_setting_Press.png"));
    m_btnSetting->setVisible(false);

    QMap<int,QtPixmapButton*> btngroup;
    btngroup.insert(BtnBack, m_btnBack);
    btngroup.insert(BtnAdd, m_btnAdd);
    btngroup.insert(BtnSetting, m_btnSetting);
    m_widgetTitle->SetToolButtons(btngroup);

    QVBoxLayout *verLayoutCentor = new QVBoxLayout(this);
    verLayoutCentor->setContentsMargins(0, 0, 0, 0);
    verLayoutCentor->setSpacing(0);
    verLayoutCentor->addWidget(m_widgetTitle, 1);

    m_videosList = new VideoListViewer(this);
    m_videosList->SetBackground(Qt::transparent);
    connect(m_videosList, SIGNAL(currentItemClicked(QtPageListWidgetItem*)), this, SLOT(SltItemClicked(QtPageListWidgetItem *)));
    verLayoutCentor->addWidget(m_videosList, 5);

    m_videoWidget = new QtVideoWidget(this);
    m_videoWidget->hide();

    m_dirDialog = new QtFileDialog(this);
    m_dirDialog->setRootPath(m_strDirPath);
    m_dirDialog->setLineditVisible(false);
    m_dirDialog->setVisible(false);
    connect(m_dirDialog, SIGNAL(signalBackHome()), this, SLOT(SltDirDialogClose()));
    connect(m_dirDialog, SIGNAL(signalSelected(QString)), this, SLOT(SltDirSelected(QString)));
}

void VideoPlayer::LoadMedias()
{
    m_videoWidget->m_playList->LoadLocalFiles(m_strDirPath);
    QMap<int, QtPageListWidgetItem *> items;
    foreach (QtListWidgetItem *item, m_videoWidget->m_playList->items()) {
        items.insert(item->m_nId, new QtPageListWidgetItem(item->m_nId, item->m_strPath, item->m_strBaseName, QPixmap(":/images/video/ic_video_preview.png")));
    }
    m_videosList->SetItems(items);
}

void VideoPlayer::SltItemClicked(QtPageListWidgetItem *item)
{
    QFileInfo fileInfo(item->m_strPath);
    m_videoWidget->setMedia(fileInfo.fileName(), item->m_nId);
    m_videoWidget->setGeometry(0, 0, this->width(), this->height());
    m_videoWidget->show();
}

void VideoPlayer::SltDirSelected(const QString &fileName)
{
    m_strDirPath = fileName;
    qDebug() << "selected dir is" << fileName;

    SltDirDialogClose();

    // 重新扫描
    LoadMedias();
}

void VideoPlayer::SltDirDialogClose()
{
    m_dirDialog->StartAnimation(QPoint(0, 0), QPoint(this->width(), -this->height()), 200, false);
}

void VideoPlayer::SltToolBtnClicked(int index)
{
    if (BtnHome == index) {
        emit signalBackHome();
    } else if (BtnBack == index) {

    } else if (BtnAdd == index) {
        m_dirDialog->setSaveFileMode(false);
        m_dirDialog->StartAnimation(QPoint(this->width(), -this->height()), QPoint(0, 0), 200, true);
    } else if (BtnSetting == index) {

    }
}

void VideoPlayer::resizeEvent(QResizeEvent *e)
{
    m_dirDialog->resize(this->size());
    m_scaleX = (this->width() * 1.0) / m_nBaseWidth;
    m_scaleY = (this->height() * 1.0) / m_nBaseHeight;

    if (NULL != m_videoWidget) {
        m_videoWidget->setGeometry(0, 0, this->width(), this->height());
    }
    QWidget::resizeEvent(e);
}
