/******************************************************************
 Copyright (C) 2017 - All Rights Reserved by
 文 件 名 : photosview.cpp ---
 作 者    : Niyh  (QQ:393320854)
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
           2019/9/8
*******************************************************************/
#include "photosview.h"
#include "skin.h"
#include "imageviewer.h"
#include "appconfig.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QDir>

PhotosView::PhotosView(QWidget *parent) : QtAnimationWidget(parent)
{
    m_strDirPath = AppConfig::ReadSetting("Photos", "location", "/").toString();
    if (m_strDirPath == "/") {
        m_strDirPath = qApp->applicationDirPath() + "/photos/";
    }

    this->SetBackground(QColor("#ffffff"));

    InitWidget();

    QTimer::singleShot(500, this, SLOT(SltLoadPhotos()));
}

PhotosView::~PhotosView()
{
    AppConfig::SaveSetting("Photos", "location", m_strDirPath);
}

void PhotosView::InitWidget()
{
    QtWidgetTitleBar *widgetTitle = new QtWidgetTitleBar(this);
    widgetTitle->SetScalSize(Skin::m_nScreenWidth, 60);
    widgetTitle->SetBackground(QColor("#f0f0f0"));
    widgetTitle->SetTitle(tr("Photos"), "#333333", 25);
    connect(widgetTitle, SIGNAL(signalBtnClicked(int)), this, SLOT(SltToolBtnClicked(int)));

    m_btnBack = new QtPixmapButton(BtnBack, QRect(10, 10, 40, 40), QPixmap(":/images/ebook/ic_back.png"), QPixmap(":/images/ebook/ic_back.png"));
    m_btnBack->setVisible(false);

    m_btnAdd = new QtPixmapButton(BtnAdd, QRect(746, 0, 54, 54), QPixmap(":/images/photos/menu_icon.png"), QPixmap(":/images/photos/menu_icon_pressed.png"));

    m_btnSetting = new QtPixmapButton(BtnSetting, QRect(750, 10, 40, 40), QPixmap(":/images/ebook/ic_setting.png"), QPixmap(":/images/ebook/ic_setting_Press.png"));
    m_btnSetting->setVisible(false);

    QMap<int,QtPixmapButton*> btngroup;
    btngroup.insert(BtnBack, m_btnBack);
    btngroup.insert(BtnAdd, m_btnAdd);
    btngroup.insert(BtnSetting, m_btnSetting);
    widgetTitle->SetToolButtons(btngroup);

    m_photoListView = new PhotoListView(this);
    m_photoListView->SetBackground(QColor("#ffffff"));
    connect(m_photoListView, SIGNAL(currentItemClicked(QtPageListWidgetItem*)), this, SLOT(SltCurrentItemClicked(QtPageListWidgetItem*)));

    QVBoxLayout *verLayout = new QVBoxLayout(this);
    verLayout->setContentsMargins(0, 0, 0, 0);
    verLayout->setSpacing(0);
    verLayout->addWidget(widgetTitle, 1);
    verLayout->addWidget(m_photoListView, 7);

    m_dirDialog = new QtFileDialog(this);
    m_dirDialog->setRootPath(m_strDirPath);
    m_dirDialog->setLineditVisible(false);
    m_dirDialog->setVisible(false);
    connect(m_dirDialog, SIGNAL(signalBackHome()), this, SLOT(SltDirDialogClose()));
    connect(m_dirDialog, SIGNAL(signalSelected(QString)), this, SLOT(SltDirSelected(QString)));
}

void PhotosView::SltLoadPhotos()
{
    m_listItems.clear();
    QDir dir(m_strDirPath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList(QStringList() << "*.jpg" << "*.png" << "*.bmp");
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QPixmap pixmap(fileInfo.absoluteFilePath());
        if (pixmap.width() > pixmap.height()) {
            pixmap = pixmap.scaledToHeight(200);
        } else {
            pixmap = pixmap.scaledToWidth(200);
        }
        pixmap = pixmap.copy(0, 0, 200, 200);
        m_listItems.insert(i, new QtPageListWidgetItem(i, fileInfo.absoluteFilePath(), pixmap));
    }
    m_photoListView->SetItems(m_listItems);
}

void PhotosView::SltCurrentItemClicked(QtPageListWidgetItem *item)
{
    ImageViewer *imageView = new ImageViewer(this);
    imageView->setGeometry(0, 0, this->width(), this->height());
    imageView->SetPixmap(item->m_nId, m_listItems);
}

void PhotosView::SltDirSelected(const QString &fileName)
{
    m_strDirPath = fileName;
    qDebug() << "selected dir is" << fileName;

    SltDirDialogClose();

    // 重新扫描
    SltLoadPhotos();
}

void PhotosView::SltDirDialogClose()
{
    m_dirDialog->StartAnimation(QPoint(0, 0), QPoint(this->width(), -this->height()), 200, false);
}

void PhotosView::SltToolBtnClicked(int index)
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

void PhotosView::resizeEvent(QResizeEvent *e)
{
    m_dirDialog->resize(this->size());
    m_scaleX = (this->width() * 1.0) / m_nBaseWidth;
    m_scaleY = (this->height() * 1.0) / m_nBaseHeight;

    QWidget::resizeEvent(e);
}
