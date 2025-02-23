/******************************************************************
 Copyright (C) 2019 - All Rights Reserved by
 文 件 名 : aboutboard.cpp --- AboutBoard
 作 者    : Niyh(lynnhua)
 论 坛    : http://www.firebbs.cn
 编写日期 : 2019
 说 明    :
 历史纪录 :
 <作者>    <日期>        <版本>        <内容>
  Niyh	   2019    	1.0.0 1     文件创建
*******************************************************************/
#include "aboutboard.h"
#include "unit.h"
#include "skin.h"
#include "HAL.h"
#include "clickedwidget.h"

#include <QLabel>
#include <QBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QFile>

#define FLASH_FILE_PART     4
#define FLASH_FILE_INFO     "/sys/class/mtd/mtd%1/size"
#ifndef FLASH_FILE_EMMC
#define FLASH_FILE_EMMC     "/sys/class/mmc_host/mmc1/mmc1:0001/block/mmcblk1/size"
#endif

AboutBoard::AboutBoard(QWidget *parent) : QtListWidget(parent)
{
    m_backgroundColor = Qt::transparent;
    m_nItemSize = 45;
    m_bHorizontal = false;
    m_nBaseWidth = Skin::m_nScreenWidth;
    m_nBaseHeight = 400;

    m_strModel = "BeadaPanel 7";
    m_strPlatform = "NXP i.mx6ull";
    m_strNandSize = "32GB TF";
    m_strResolutions = "800x480";
    m_strFirmwareRev = "V820";

    InitBoardInfo();
    InitWidget();
    connect(this, SIGNAL(currentIndexClicked(int)), this, SLOT(SltCurrentIndexClicked(int)));
}

AboutBoard::~AboutBoard()
{

}

// 512MB NAND / 8GB eMMC
void AboutBoard::InitBoardInfo()
{
#ifdef __arm__
    quint16 rev;
    quint8 plat, os;
    quint32 usize;
    qint32 px, py;

    getHalVer(&rev, &plat, &os);
    m_strFirmwareRev = QString("%1").arg(rev);

    if (plat==4) {
        m_strPlatform = "ALLWinner V3S";
    }
    else if (plat==5) {
        m_strPlatform = "ALLWinner T113";
    }
    else if (plat==1) {
        m_strPlatform = "NXP i.mx6ul";
    }

    if (os==20) {
        m_strModel = "BeadaPanel 5S";
    }

    // nand容量读取
    usize = getStorageSize();
    usize /= 1024;
    m_strNandSize = QString("%1MB").arg(usize);

    getFBResolution(&px, &py, NULL);
    m_strResolutions = QString("%1x%2").arg(px).arg(py);
#endif
}

void AboutBoard::InitWidget()
{
    int index = 0;

    m_listItems.insert(index, new QtListWidgetItem(index, tr("Model"),  m_strModel, QPixmap())); index++;
    m_listItems.insert(index, new QtListWidgetItem(index, tr("Platform"),  m_strPlatform, QPixmap())); index++;
    m_listItems.insert(index, new QtListWidgetItem(index, tr("Storage space"),  m_strNandSize, QPixmap())); index++;
    m_listItems.insert(index, new QtListWidgetItem(index, tr("Resolutions"),  m_strResolutions, QPixmap())); index++;
    m_listItems.insert(index, new QtListWidgetItem(index, tr("Libpld rev."),  m_strFirmwareRev, QPixmap())); index++;
    m_listItems.insert(index, new QtListWidgetItem(index, tr("Update"),  tr(""), QPixmap(":/images/setting/ic_next.png")));
}

void AboutBoard::SltCurrentIndexClicked(int index)
{
    if (5 == index) {
        emit signalChangePage(6);
    }
}

void AboutBoard::drawItemInfo(QPainter *painter, QtListWidgetItem *item)
{
    painter->save();
    painter->setPen(QColor("#797979"));
    painter->drawLine(QPoint(item->m_rect.left() - m_nMargin, item->m_rect.bottom()),
                      QPoint(item->m_rect.right() + m_nMargin, item->m_rect.bottom()));

    QRect rect(item->m_rect.left() + 20, item->m_rect.top(), item->m_rect.width() - 40, item->m_rect.height());
    QFont font(Skin::m_strAppFontNormal);
    font.setPixelSize(24);
    painter->setFont(font);
    painter->setPen(QColor("#ffffff"));
    painter->drawText(rect, Qt::AlignVCenter, item->m_strText);

    QPixmap pixmap = item->m_pixmapIcon;
    if (pixmap.isNull()) {
        int nW = painter->fontMetrics().width(item->m_strBaseName);
        painter->drawText(QRect(m_nBaseWidth - nW - 20, rect.top(), nW, rect.height()), Qt::AlignCenter, item->m_strBaseName);
    } else {
        painter->drawPixmap(m_nBaseWidth - pixmap.width() - 20, rect.top() + (rect.height() - pixmap.height()) / 2, pixmap);
    }
    painter->restore();
}
